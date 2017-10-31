#ifndef PDJSON_H
#define PDJSON_H

#include <stdio.h>

#if defined(__cplusplus) && !(defined(_WIN32_WCE) || defined(_WIN32) || defined(_WIN64))
extern "C" {
#endif

enum json_type {
    JSON_ERROR = 1, JSON_DONE,
    JSON_OBJECT, JSON_OBJECT_END, JSON_ARRAY, JSON_ARRAY_END,
    JSON_STRING, JSON_NUMBER, JSON_TRUE, JSON_FALSE, JSON_NULL
};

struct json_allocator {
    void *(*malloc)(size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
};

#include "pd_json_private.h"

typedef struct json_stream json_stream;
typedef struct json_allocator json_allocator;

void json_open_buffer(json_stream *json, const void *buffer, size_t size);
void json_open_string(json_stream *json, const char *string);
void json_open_stream(json_stream *json, FILE *stream);
void json_close(json_stream *json);

void json_set_allocator(json_stream *json, json_allocator *a);
void json_set_streaming(json_stream *json, bool strict);

enum json_type json_next(json_stream *json);
enum json_type json_peek(json_stream *json);
void json_reset(json_stream *json);
const char *json_get_string(json_stream *json, size_t *length);
double json_get_number(json_stream *json);

size_t json_get_lineno(json_stream *json);
size_t json_get_position(json_stream *json);
size_t json_get_depth(json_stream *json);
const char *json_get_error(json_stream *json);

#if defined(__cplusplus) && !(defined(_WIN32_WCE) || defined(_WIN32) || defined(_WIN64))
}
#endif

#endif
