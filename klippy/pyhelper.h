#ifndef PYHELPER_H
#define PYHELPER_H

double get_time(void);
struct timespec fill_time(double time);
void report_errno(char *where, int rc);
char *dump_string(char *outbuf, int outbuf_size, char *inbuf, int inbuf_size);

#endif // pyhelper.h
