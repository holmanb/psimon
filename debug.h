#ifndef _H_DEBUG
#define _H_DEBUG

#define DEBUG 1

#define debug(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)
#endif
