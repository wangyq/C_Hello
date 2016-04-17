/*
 * debug.c
 *
 *  Created on: 2012-6-5
 *      Author: pmserver
 */

#include <stdio.h>
#include <stdarg.h>

#include "util.h"


/**
 * output debug information to stderr
 */
void dprintf_impl(const char* file, int line, int enable, const char* fmt, ...)
{
    va_list ap;
    if (enable) {
        fprintf(stderr, "%s (%d): ", file, (int)line);
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        fprintf(stderr, "\n");
        fflush(stderr);
    }
}

