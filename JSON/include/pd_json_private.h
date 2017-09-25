#ifndef PDJSON_PRIVATE_H
#define PDJSON_PRIVATE_H

#if defined(__STDC_VERSION__) || (__STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#endif // __STDC_VERSION__
#include <stdio.h>

struct json_source {
    int (*get) (struct json_source *);
    int (*peek) (struct json_source *);
    size_t position;
    union {
        struct {
            FILE *stream;
        } stream;
        struct {
            const char *buffer;
            size_t length;
        } buffer;
    } source;
};

struct json_stack {
    enum json_type type;
    long count;
};

struct json_stream {
    size_t lineno;

    struct json_stack *stack;
    size_t stack_top;
    size_t stack_size;
    enum json_type next;
    int error : 31;
    bool streaming : 1;

    struct {
        char *string;
        size_t string_fill;
        size_t string_size;
    } data;

    size_t ntokens;

    struct json_source source;
    struct json_allocator alloc;
    char errmsg[128];
};

#endif
