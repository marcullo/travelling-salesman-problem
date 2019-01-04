#include "exception.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

static const char* tags[] = {
        [EX_MALLOC] = "EX_MALLOC",
        [EX_FILE_NOT_OPENED] = "EX_FILE_NOT_OPENED",
        [EX_ARGUMENT] = "EX_ARGUMENT",
        [EX_THREAD] = "EX_THREAD",
        [EX_NULL] = "EX_NULL",
};

static char buf[EXCEPTION_MESSAGE_LEN];

void exception_throw(exception_reason_t reason, const char* msg, ...)
{
    if (reason >= EX_TOP) {
        exit(1);
    }

    va_list args;
    int len = sprintf(buf, "[%s] ", tags[reason]);

    va_start(args, msg);
    vsprintf(buf + len, msg, args);
    va_end(args);

    printf("%s\n", buf);

    exit(1);
}
