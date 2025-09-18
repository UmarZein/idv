#ifndef SCANITERC
#define SCANITERC
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

typedef struct {
    const char *cur;  // current position in buffer
    const char *reset_point;
} ScanIter;

void scanIterInit(ScanIter *it, const char *buf) {
    it->cur = buf;
    it->reset_point = buf;
}

void resetScanIter(ScanIter *it){
    it->cur = it->reset_point;
}

int scanIter(ScanIter *it, const char *fmt, ...) {
    va_list args;
    int n;

    // skip leading whitespace/newlines
    while (*it->cur && (*it->cur == '\n' || *it->cur == '\r' || *it->cur == ' '))
        it->cur++;

    if (*it->cur == '\0') {
        return 0; // end of buffer
    }

    va_start(args, fmt);
    n = vsscanf(it->cur, fmt, args);
    va_end(args);

    if (n > 0) {
        // advance pointer past what was just parsed
        // find the end of the current line
        const char *nl = strchr(it->cur, '\n');
        if (nl) {
            it->cur = nl + 1;
        } else {
            it->cur += strlen(it->cur); // move to end
        }
    }
    return n;
}

#endif
