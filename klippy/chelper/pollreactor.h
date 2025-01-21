#ifndef POLLREACTOR_H
#define POLLREACTOR_H

#define PR_NOW   0.
#define PR_NEVER 9999999999999999.

struct pollreactor *pollreactor_alloc(int num_fds, int num_timers
                                      , void *callback_data);
void pollreactor_free(struct pollreactor *pr);
void pollreactor_add_fd(struct pollreactor *pr, int pos, int fd, void *callback
                        , int write_only);
void pollreactor_add_timer(struct pollreactor *pr, int pos, void *callback);
double pollreactor_get_timer(struct pollreactor *pr, int pos);
void pollreactor_update_timer(struct pollreactor *pr, int pos, double waketime);
void pollreactor_run(struct pollreactor *pr);
void pollreactor_do_exit(struct pollreactor *pr);
int pollreactor_is_exit(struct pollreactor *pr);
int fd_set_non_blocking(int fd);

#endif // pollreactor.h
