// TTY based IO
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#define _GNU_SOURCE
#include <errno.h> // errno
#include <fcntl.h> // fcntl
#include <poll.h> // ppoll
#include <pty.h> // openpty
#include <stdio.h> // fprintf
#include <string.h> // memmove
#include <sys/stat.h> // chmod
#include <time.h> // struct timespec
#include <unistd.h> // ttyname
#include <pthread.h> // pthread_create
#include "board/irq.h" // irq_wait
#include "board/misc.h" // console_sendf
#include "command.h" // command_find_block
#include "internal.h" // console_setup
#include "sched.h" // sched_wake_task
#include "ringbuf.h" // SPSC ring buf

static struct pollfd main_pfd[1];
static struct ring_buf outputq;
static struct ring_buf inputq;
static pthread_t main;
static pthread_t reader;
static pthread_t writer;
#define MP_TTY_IDX   0

// Report 'errno' in a message written to stderr
void
report_errno(char *where, int rc)
{
    int e = errno;
    fprintf(stderr, "Got error %d in %s: (%d)%s\n", rc, where, e, strerror(e));
}

/****************************************************************
 * Threaded IO
 ****************************************************************/

static void *
tty_reader(void *_unused)
{
    static uint8_t buf[128];
    while (1) {
        int ret = read(main_pfd[MP_TTY_IDX].fd, buf, sizeof(buf));
        if (ret < 0) {
            report_errno("read", ret);
            continue;
        }
        while (ring_buffer_available_to_write(&inputq) < ret)
            nanosleep(&(struct timespec){.tv_nsec = 10000}, NULL);
        ring_buffer_write(&inputq, buf, ret);
    }

    return NULL;
}

static void *
tty_writer(void *_unused)
{
    static uint8_t buf[128];
    while (1) {
        int len = ring_buffer_read(&outputq, buf, sizeof(buf));
        if (len == 0) {
            nanosleep(&(struct timespec){.tv_nsec = 100000}, NULL);
            continue;
        }

        int ret = write(main_pfd[MP_TTY_IDX].fd, buf, len);
        if (ret < 0)
            report_errno("write", ret);
    }

    return NULL;
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

    ring_buffer_init(&inputq);
    ring_buffer_init(&outputq);

    main = pthread_self();
    pthread_create(&reader, NULL, tty_reader, NULL);
    pthread_setschedparam(reader, SCHED_OTHER,
                          &(struct sched_param){.sched_priority = 0});
    pthread_create(&writer, NULL, tty_writer, NULL);
    pthread_setschedparam(writer, SCHED_OTHER,
                          &(struct sched_param){.sched_priority = 0});

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
    int ret = ring_buffer_read(&inputq, &receive_buf[receive_pos],
                                sizeof(receive_buf) - receive_pos);
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
    while (ring_buffer_available_to_write(&outputq) < msglen)
        nanosleep(&(struct timespec){.tv_nsec = 1000}, NULL);

    // Transmit message
    ring_buffer_write(&outputq, buf, msglen);
}

// Sleep for the specified time or until a signal interrupts
void
console_sleep(void)
{
    if (ring_buffer_available_to_read(&inputq) > 0) {
        sched_wake_task(&console_wake);
        return;
    }
    nanosleep(&(struct timespec){.tv_nsec = 500000}, NULL);
}
