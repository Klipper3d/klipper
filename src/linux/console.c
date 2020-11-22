// TTY based IO
//
// Copyright (C) 2017  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include <errno.h> // errno
#include <fcntl.h> // fcntl
#include <poll.h> // poll
#include <pty.h> // openpty
#include <stdio.h> // fprintf
#include <string.h> // memmove
#include <sys/stat.h> // chmod
#include <sys/timerfd.h> // timerfd_create
#include <time.h> // struct timespec
#include <unistd.h> // ttyname
#include "board/irq.h" // irq_poll
#include "board/misc.h" // console_sendf
#include "command.h" // command_find_block
#include "internal.h" // console_setup
#include "sched.h" // sched_wake_task

static struct pollfd main_pfd[2];
#define MP_TIMER_IDX 0
#define MP_TTY_IDX   1

// Report 'errno' in a message written to stderr
void
report_errno(char *where, int rc)
{
    int e = errno;
    fprintf(stderr, "Got error %d in %s: (%d)%s\n", rc, where, e, strerror(e));
}


/****************************************************************
 * Setup
 ****************************************************************/

int
set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        report_errno("fcntl getfl", flags);
        return -1;
    }
    int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    if (ret < 0) {
        report_errno("fcntl setfl", flags);
        return -1;
    }
    return 0;
}

int
set_close_on_exec(int fd)
{
    int ret = fcntl(fd, F_SETFD, FD_CLOEXEC);
    if (ret < 0) {
        report_errno("fcntl set cloexec", ret);
        return -1;
    }
    return 0;
}

int
console_setup(char *name)
{
    // Open pseudo-tty
    struct termios ti;
    memset(&ti, 0, sizeof(ti));
    int mfd, sfd, ret = openpty(&mfd, &sfd, NULL, &ti, NULL);
    if (ret) {
        report_errno("openpty", ret);
        return -1;
    }
    ret = set_non_blocking(mfd);
    if (ret)
        return -1;
    ret = set_close_on_exec(mfd);
    if (ret)
        return -1;
    ret = set_close_on_exec(sfd);
    if (ret)
        return -1;
    main_pfd[MP_TTY_IDX].fd = mfd;
    main_pfd[MP_TTY_IDX].events = POLLIN;

    // Create symlink to tty
    unlink(name);
    char *tname = ttyname(sfd);
    if (!tname) {
        report_errno("ttyname", 0);
        return -1;
    }
    ret = symlink(tname, name);
    if (ret) {
        report_errno("symlink", ret);
        return -1;
    }
    ret = chmod(tname, 0660);
    if (ret) {
        report_errno("chmod", ret);
        return -1;
    }

    // Make sure stderr is non-blocking
    ret = set_non_blocking(STDERR_FILENO);
    if (ret)
        return -1;

    // Create sleep wakeup timer fd
    ret = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC|TFD_NONBLOCK);
    if (ret < 0) {
        report_errno("timerfd_create", ret);
        return -1;
    }
    main_pfd[MP_TIMER_IDX].fd = ret;
    main_pfd[MP_TIMER_IDX].events = POLLIN;

    return 0;
}


/****************************************************************
 * Console handling
 ****************************************************************/

static struct task_wake console_wake;
static uint8_t receive_buf[4096];
static int receive_pos;

void *
console_receive_buffer(void)
{
    return receive_buf;
}

// Process any incoming commands
void
console_task(void)
{
    if (!sched_check_wake(&console_wake))
        return;

    // Read data
    int ret = read(main_pfd[MP_TTY_IDX].fd, &receive_buf[receive_pos]
                   , sizeof(receive_buf) - receive_pos);
    if (ret < 0) {
        if (errno == EWOULDBLOCK) {
            ret = 0;
        } else {
            report_errno("read", ret);
            return;
        }
    }
    if (ret == 15 && receive_buf[receive_pos+14] == '\n'
        && memcmp(&receive_buf[receive_pos], "FORCE_SHUTDOWN\n", 15) == 0)
        shutdown("Force shutdown command");

    // Find and dispatch message blocks in the input
    int len = receive_pos + ret;
    uint_fast8_t pop_count, msglen = len > MESSAGE_MAX ? MESSAGE_MAX : len;
    ret = command_find_and_dispatch(receive_buf, msglen, &pop_count);
    if (ret) {
        len -= pop_count;
        if (len) {
            memmove(receive_buf, &receive_buf[pop_count], len);
            sched_wake_task(&console_wake);
        }
    }
    receive_pos = len;
}
DECL_TASK(console_task);

// Encode and transmit a "response" message
void
console_sendf(const struct command_encoder *ce, va_list args)
{
    // Generate message
    uint8_t buf[MESSAGE_MAX];
    uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

    // Transmit message
    int ret = write(main_pfd[MP_TTY_IDX].fd, buf, msglen);
    if (ret < 0)
        report_errno("write", ret);
}

// Sleep until the specified time (waking early for console input if needed)
void
console_sleep(struct timespec ts)
{
    struct itimerspec its;
    its.it_interval = (struct timespec){0, 0};
    its.it_value = ts;
    int ret = timerfd_settime(main_pfd[MP_TIMER_IDX].fd, TFD_TIMER_ABSTIME
                              , &its, NULL);
    if (ret < 0) {
        report_errno("timerfd_settime", ret);
        return;
    }
    ret = poll(main_pfd, ARRAY_SIZE(main_pfd), -1);
    if (ret <= 0) {
        report_errno("poll main_pfd", ret);
        return;
    }
    if (main_pfd[MP_TTY_IDX].revents)
        sched_wake_task(&console_wake);
    if (main_pfd[MP_TIMER_IDX].revents)
        irq_poll();
}
