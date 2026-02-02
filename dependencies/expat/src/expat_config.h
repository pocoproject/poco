/*
 * expat_config.h
 *
 * Configuration for bundled Expat.
 * Platform-specific entropy defines are set via compiler flags.
 */

#ifndef EXPAT_CONFIG_H
#define EXPAT_CONFIG_H

/* Byte order detection using compiler predefined macros */
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
    #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        #define BYTEORDER 1234
    #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        #define BYTEORDER 4321
    #endif
#elif defined(_WIN32) || defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || \
      defined(__arm__) || defined(__aarch64__) || defined(__ARMEL__) || defined(__AARCH64EL__) || \
      defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64)
    #define BYTEORDER 1234
#elif defined(__sparc__) || defined(__sparc) || defined(__hppa__) || defined(__hppa) || \
      defined(__ARMEB__) || defined(__AARCH64EB__) || defined(__MIPSEB__) || defined(__s390__)
    #define BYTEORDER 4321
#else
    #define BYTEORDER 1234
#endif

#define XML_CONTEXT_BYTES 1024
#define XML_GE 1

#endif /* EXPAT_CONFIG_H */
