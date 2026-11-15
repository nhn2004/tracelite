#ifndef TRACE_COMMON_H
#define TRACE_COMMON_H

#include <time.h>

#define ID_LEN 32
#define NAME_LEN 32
#define STATUS_LEN 16

typedef struct {
    char trace_id[ID_LEN];
    char span_id[ID_LEN];
    char parent_span_id[ID_LEN];
    char service[NAME_LEN];
    char operation[NAME_LEN];
    double start_ms;
    double end_ms;
    double duration_ms;
    char status[STATUS_LEN];
} Span;

static inline double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1000000.0);
}

#endif
