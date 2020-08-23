/* timestamp.c: PQSH Timestamp */

#include "pqsh/timestamp.h"

#include <time.h>
#include <sys/time.h>

/**
 * Return current timestamp as a double.
 *
 * Utilizes gettimeofday for precision (accounting for seconds and
 * microseconds) and falls back to time (only account for seconds) if that
 * fails.
 *
 * @return  Double representing the current time.
 **/
double timestamp() {
    /* TODO: Implement */
    struct timeval tv;
    double seconds;

    if (!gettimeofday(&tv, NULL))
        seconds = time(NULL);
    else
        seconds = tv.tv_sec + (tv.tv_usec/1000000.0);
    
    return seconds;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
