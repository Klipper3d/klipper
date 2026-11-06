// TTY based IO
//
// Copyright (C) 2017-2021  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2024 Liam Powell <klipper@liampwll.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h> // errno
#include <fcntl.h> // fcntl
#include <poll.h> // ppoll
#include <pty.h> // openpty
#include <stdio.h> // fprintf
#include <string.h> // memmove
#include <sys/stat.h> // chmod
#include <time.h> // struct timespec
#include <unistd.h> // ttyname
#include "board/irq.h" // irq_wait
#include "board/misc.h" // console_sendf
#include "command.h" // command_find_block
#include "internal.h" // console_setup
#include "sched.h" // sched_wake_task
#include "ringbuf/ringbuf.h"

static ringbuf_t *ringbuf_to_mcu;
static uint8_t ringbuf_to_mcu_data[1024 * 64];
static ringbuf_t *ringbuf_from_mcu;
static uint8_t ringbuf_from_mcu_data[1024 * 64];

static struct pollfd main_pfd[1];
#define MP_TTY_IDX   0

// Report 'errno' in a message written to stderr
void
report_errno(char *where, int rc)
{
    int e = errno;
    fprintf(stderr, "Got error %d in %s: (%d)%s\n", rc, where, e, strerror(e));
}

static void *
ringbuf_to_mcu_worker(void *_unused)
{
    ringbuf_worker_t *ringbuf_worker;
    if ((ringbuf_worker = ringbuf_register(ringbuf_to_mcu, 0)) == NULL) {
        fprintf(stderr, "ringbuf_register failed in %s\n", __func__);
        exit(1);
    }
    static uint8_t local_receive_buf[128];

    for (;;) {
        int ret = read(main_pfd[MP_TTY_IDX].fd, local_receive_buf, sizeof(local_receive_buf));
        if (ret < 0) {
            report_errno("read", ret);
            continue;
        }

        ssize_t off;

        while ((off = ringbuf_acquire(ringbuf_to_mcu, ringbuf_worker, ret)) == -1) {
            nanosleep(&(struct timespec){ .tv_nsec = 500000 }, NULL);
        }

        memcpy(ringbuf_to_mcu_data + off, local_receive_buf, ret);
        ringbuf_produce(ringbuf_to_mcu, ringbuf_worker);
    }

    return NULL;
}

static void *
ringbuf_from_mcu_worker(void *_unused)
{
    for (;;) {
        size_t len, off;

        while ((len = ringbuf_consume(ringbuf_from_mcu, &off)) == 0) {
            nanosleep(&(struct timespec){ .tv_nsec = 500000 }, NULL);
        }

        int ret = write(main_pfd[MP_TTY_IDX].fd, ringbuf_from_mcu_data + off, len);
        if (ret < 0)
            report_errno("write", ret);

        ringbuf_release(ringbuf_from_mcu, len);
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

    size_t ringbuf_size;
    ringbuf_get_sizes(1, &ringbuf_size, NULL);
    ringbuf_from_mcu = malloc(ringbuf_size);
    ringbuf_to_mcu = malloc(ringbuf_size);
    ret = ringbuf_setup(ringbuf_from_mcu, 1, sizeof(ringbuf_from_mcu_data));
    if (ret)
        return -1;
    ret = ringbuf_setup(ringbuf_to_mcu, 1, sizeof(ringbuf_to_mcu_data));
    if (ret)
        return -1;

    static pthread_t worker1;
    pthread_create(&worker1, NULL, ringbuf_to_mcu_worker, NULL);
    static pthread_t worker2;
    pthread_create(&worker2, NULL, ringbuf_from_mcu_worker, NULL);

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
    size_t off;
    int ret = (int)ringbuf_consume(ringbuf_to_mcu, &off);
    if (ret > sizeof(receive_buf) - receive_pos) {
        ret = sizeof(receive_buf) - receive_pos;
    }
    if (ret < 0) {
        if (errno == EWOULDBLOCK) {
            ret = 0;
        } else {
            report_errno("read", ret);
            return;
        }
    }
    if (ret != 0) {
        memcpy(&receive_buf[receive_pos], &ringbuf_to_mcu_data[off], ret);
        ringbuf_release(ringbuf_to_mcu, ret);
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
    static ringbuf_worker_t *ringbuf_worker = NULL;
    if (ringbuf_worker == NULL) {
        if ((ringbuf_worker = ringbuf_register(ringbuf_to_mcu, 0)) == NULL) {
            fprintf(stderr, "ringbuf_register failed in %s\n", __func__);
            exit(1);
        }
    }

    // Generate message
    uint8_t buf[MESSAGE_MAX];
    uint_fast8_t msglen = command_encode_and_frame(buf, ce, args);

    // Transmit message
    ssize_t off;
    if ((off = ringbuf_acquire(ringbuf_from_mcu, ringbuf_worker, msglen)) == -1) {
        fprintf(stderr, "ringbuf_acquire failed in %s\n", __func__);
        return;
    }

    memcpy(ringbuf_from_mcu_data + off, buf, msglen);
    ringbuf_produce(ringbuf_from_mcu, ringbuf_worker);
}

void
console_sleep(void)
{
    // Return immediately instead of actually sleeping as we are dedicating a
    // core to the part of the code that calls this.
    size_t off;
    if (ringbuf_consume(ringbuf_to_mcu, &off) != 0) {
        sched_wake_task(&console_wake);
    }
}
