#ifndef __RUN_TIME_H
#define __RUN_TIME_H

#include <time.h>

#define connect(a, b) a##b

#define macro_start_name(name) connect(start_, name)
#define macro_end_name(name) connect(end_, name)
#define macro_cpu_time_used(name) connect(cpu_time_used_, name)

#define macro_timer_start(name)  \
    struct timespec macro_start_name(name); \
    struct timespec macro_end_name(name); \
    double macro_cpu_time_used(name); \
    clock_gettime(CLOCK_MONOTONIC, &macro_start_name(name));


#define macro_timer_end(name)  \
    clock_gettime(CLOCK_MONOTONIC, &macro_end_name(name)); \
    macro_cpu_time_used(name) = (macro_end_name(name).tv_sec - macro_start_name(name).tv_sec) + (macro_end_name(name).tv_nsec - macro_start_name(name).tv_nsec) / 1000000000.0; \
    printf("Time taken by %s:%f\n", #name, macro_cpu_time_used(name));

#endif