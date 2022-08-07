#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "logger.h"

void error(const char *fstr, ...) {
    va_list args;
    va_start(args, fstr);

    printf("\n" RED("  Err") ": ");
    vfprintf(stdout, fstr, args);
    // printf(RESET);

    va_end(args);
}

void info(const char *fstr, ...) {
    va_list args;
    va_start(args, fstr);

    printf("\n" GREEN("  Ok") ": ");
    vfprintf(stdout, fstr, args);
    // printf(RESET);

    va_end(args);
}