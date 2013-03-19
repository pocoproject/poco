#ifndef JSON_BASE64_H
#define JSON_BASE64_H

#include "JSONDebug.h"
#if defined(JSON_BINARY) || defined(JSON_EXPOSE_BASE64)  //if this is not needed, don't waste space compiling it

#include <string>


class JSONBase64 {
public:
    static json_string json_encode64(const unsigned char * binary, size_t bytes) json_nothrow json_cold;
    static std::string json_decode64(const json_string & encoded) json_nothrow json_cold;
};

#endif
#endif
