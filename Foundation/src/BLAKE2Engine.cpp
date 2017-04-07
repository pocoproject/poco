//
// BLAKE2Engine.cpp
//
// Library: Foundation
// Package: Crypt
// Module:  BLAKE2Engine
//
// Code of class BLAKE2Engine.
//
// This class implements the BLAKE2 hashing algorithm.
// (RFC 7693, see https://tools.ietf.org/html/rfc7693)
//
// Based on the BLAKE2 reference implementation (CC0, OpenSSL or Apache 2.0)
// http://creativecommons.org/publicdomain/zero/1.0
// https://www.openssl.org/source/license.html
// http://www.apache.org/licenses/LICENSE-2.0
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/BLAKE2Engine.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#if defined(_MSC_VER)
#define BLAKE2_PACKED(x) __pragma(pack(push, 1)) x __pragma(pack(pop))
#else
#define BLAKE2_PACKED(x) x __attribute__((packed))
#endif
#if !defined(__cplusplus) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L)
#if   defined(_MSC_VER)
#define BLAKE2_INLINE __inline
#elif defined(__GNUC__)
#define BLAKE2_INLINE __inline__
#else
#define BLAKE2_INLINE
#endif
#else
#define BLAKE2_INLINE inline
#endif

namespace Poco {
static BLAKE2_INLINE uint32_t load32(const void *src)
{
#if defined(POCO_ARCH_LITTLE_ENDIAN)
	uint32_t w;
	memcpy(&w, src, sizeof w);
	return w;
#else
	const uint8_t *p = (const uint8_t *)src;
	return ((uint32_t)(p[0]) << 0) |
		((uint32_t)(p[1]) << 8) |
		((uint32_t)(p[2]) << 16) |
		((uint32_t)(p[3]) << 24);
#endif
}

static BLAKE2_INLINE uint64_t load64(const void *src)
{
#if defined(POCO_ARCH_LITTLE_ENDIAN)
	uint64_t w;
	memcpy(&w, src, sizeof w);
	return w;
#else
	const uint8_t *p = (const uint8_t *)src;
	return ((uint64_t)(p[0]) << 0) |
		((uint64_t)(p[1]) << 8) |
		((uint64_t)(p[2]) << 16) |
		((uint64_t)(p[3]) << 24) |
		((uint64_t)(p[4]) << 32) |
		((uint64_t)(p[5]) << 40) |
		((uint64_t)(p[6]) << 48) |
		((uint64_t)(p[7]) << 56);
#endif
}

static BLAKE2_INLINE uint16_t load16(const void *src)
{
#if defined(POCO_ARCH_LITTLE_ENDIAN)
	uint16_t w;
	memcpy(&w, src, sizeof w);
	return w;
#else
	const uint8_t *p = (const uint8_t *)src;
	return ((uint16_t)(p[0]) << 0) |
		((uint16_t)(p[1]) << 8);
#endif
}

static BLAKE2_INLINE void store16(void *dst, uint16_t w)
{
#if defined(POCO_ARCH_LITTLE_ENDIAN)
	memcpy(dst, &w, sizeof w);
#else
	uint8_t *p = (uint8_t *)dst;
	*p++ = (uint8_t)w; w >>= 8;
	*p++ = (uint8_t)w;
#endif
}

static BLAKE2_INLINE void store32(void *dst, uint32_t w)
{
#if defined(POCO_ARCH_LITTLE_ENDIAN)
	memcpy(dst, &w, sizeof w);
#else
	uint8_t *p = (uint8_t *)dst;
	p[0] = (uint8_t)(w >> 0);
	p[1] = (uint8_t)(w >> 8);
	p[2] = (uint8_t)(w >> 16);
	p[3] = (uint8_t)(w >> 24);
#endif
}

static BLAKE2_INLINE void store64(void *dst, uint64_t w)
{
#if defined(POCO_ARCH_LITTLE_ENDIAN)
	memcpy(dst, &w, sizeof w);
#else
	uint8_t *p = (uint8_t *)dst;
	p[0] = (uint8_t)(w >> 0);
	p[1] = (uint8_t)(w >> 8);
	p[2] = (uint8_t)(w >> 16);
	p[3] = (uint8_t)(w >> 24);
	p[4] = (uint8_t)(w >> 32);
	p[5] = (uint8_t)(w >> 40);
	p[6] = (uint8_t)(w >> 48);
	p[7] = (uint8_t)(w >> 56);
#endif
}

static BLAKE2_INLINE uint64_t load48(const void *src)
{
	const uint8_t *p = (const uint8_t *)src;
	return ((uint64_t)(p[0]) << 0) |
		((uint64_t)(p[1]) << 8) |
		((uint64_t)(p[2]) << 16) |
		((uint64_t)(p[3]) << 24) |
		((uint64_t)(p[4]) << 32) |
		((uint64_t)(p[5]) << 40);
}

static BLAKE2_INLINE void store48(void *dst, uint64_t w)
{
	uint8_t *p = (uint8_t *)dst;
	p[0] = (uint8_t)(w >> 0);
	p[1] = (uint8_t)(w >> 8);
	p[2] = (uint8_t)(w >> 16);
	p[3] = (uint8_t)(w >> 24);
	p[4] = (uint8_t)(w >> 32);
	p[5] = (uint8_t)(w >> 40);
}

static BLAKE2_INLINE uint32_t rotr32(const uint32_t w, const unsigned c)
{
	return (w >> c) | (w << (32 - c));
}

static BLAKE2_INLINE uint64_t rotr64(const uint64_t w, const unsigned c)
{
	return (w >> c) | (w << (64 - c));
}

#define BLAKE2B_BLOCKBYTES 128
#define BLAKE2B_OUTBYTES 64
#define BLAKE2B_KEYBYTES 64
#define BLAKE2B_SALTBYTES 16
#define BLAKE2B_PERSONALBYTES 16

static const uint64_t blake2b_IV[8] =
{
	0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
	0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
	0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
	0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
};

static const uint8_t blake2b_sigma[12][16] =
{
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
	{ 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
	{ 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
	{ 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
	{ 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
	{ 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
	{ 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
	{ 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
	{ 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }
};

typedef struct
{
	uint64_t h[8];
	uint64_t t[2];
	uint64_t f[2];
	uint8_t  buf[BLAKE2B_BLOCKBYTES];
	size_t   buflen;
	size_t   outlen;
	uint8_t  last_node;
} blake2b_state;

BLAKE2_PACKED(struct blake2b_param__
{
	uint8_t  digest_length; /* 1 */
	uint8_t  key_length;    /* 2 */
	uint8_t  fanout;        /* 3 */
	uint8_t  depth;         /* 4 */
	uint32_t leaf_length;   /* 8 */
	uint32_t node_offset;   /* 12 */
	uint32_t xof_length;    /* 16 */
	uint8_t  node_depth;    /* 17 */
	uint8_t  inner_length;  /* 18 */
	uint8_t  reserved[14];  /* 32 */
	uint8_t  salt[BLAKE2B_SALTBYTES]; /* 48 */
	uint8_t  personal[BLAKE2B_PERSONALBYTES];  /* 64 */
});

typedef struct blake2b_param__ blake2b_param;

#define G(r,i,a,b,c,d)                      \
  do {                                      \
	a = a + b + m[blake2b_sigma[r][2*i+0]]; \
	d = rotr64(d ^ a, 32);                  \
	c = c + d;                              \
	b = rotr64(b ^ c, 24);                  \
	a = a + b + m[blake2b_sigma[r][2*i+1]]; \
	d = rotr64(d ^ a, 16);                  \
	c = c + d;                              \
	b = rotr64(b ^ c, 63);                  \
	} while(0)

#define ROUND(r)                    \
  do {                              \
	G(r,0,v[ 0],v[ 4],v[ 8],v[12]); \
	G(r,1,v[ 1],v[ 5],v[ 9],v[13]); \
	G(r,2,v[ 2],v[ 6],v[10],v[14]); \
	G(r,3,v[ 3],v[ 7],v[11],v[15]); \
	G(r,4,v[ 0],v[ 5],v[10],v[15]); \
	G(r,5,v[ 1],v[ 6],v[11],v[12]); \
	G(r,6,v[ 2],v[ 7],v[ 8],v[13]); \
	G(r,7,v[ 3],v[ 4],v[ 9],v[14]); \
	} while(0)

static void blake2b_compress(blake2b_state *S, const uint8_t block[BLAKE2B_BLOCKBYTES])
{
	uint64_t m[16];
	uint64_t v[16];
	size_t i;

	for (i = 0; i < 16; ++i) {
		m[i] = load64(block + i * sizeof(m[i]));
	}

	for (i = 0; i < 8; ++i) {
		v[i] = S->h[i];
	}

	v[8] = blake2b_IV[0];
	v[9] = blake2b_IV[1];
	v[10] = blake2b_IV[2];
	v[11] = blake2b_IV[3];
	v[12] = blake2b_IV[4] ^ S->t[0];
	v[13] = blake2b_IV[5] ^ S->t[1];
	v[14] = blake2b_IV[6] ^ S->f[0];
	v[15] = blake2b_IV[7] ^ S->f[1];

	ROUND(0);
	ROUND(1);
	ROUND(2);
	ROUND(3);
	ROUND(4);
	ROUND(5);
	ROUND(6);
	ROUND(7);
	ROUND(8);
	ROUND(9);
	ROUND(10);
	ROUND(11);

	for (i = 0; i < 8; ++i) {
		S->h[i] = S->h[i] ^ v[i] ^ v[i + 8];
	}
}

BLAKE2Engine::BLAKE2Engine(ALGORITHM algorithm)
	: _algorithm(algorithm)
	, _context(NULL)
{
	_digest.reserve(digestLength());
	reset();
}

BLAKE2Engine::~BLAKE2Engine()
{
	reset();
	free(_context);
}

void BLAKE2Engine::updateImpl(const void* buffer_, std::size_t count)
{
	if (_context == NULL || buffer_ == NULL || count == 0) return;
	const unsigned char * in = (const unsigned char *)buffer_;
	blake2b_state* S = (blake2b_state*)_context;
	size_t left = S->buflen;
	size_t fill = BLAKE2B_BLOCKBYTES - left;
	if (count > fill)
	{
		S->buflen = 0;
		memcpy(S->buf + left, in, fill); /* Fill buffer */
		S->t[0] += BLAKE2B_BLOCKBYTES;
		S->t[1] += (S->t[0] < BLAKE2B_BLOCKBYTES);
		blake2b_compress(S, S->buf); /* Compress */
		in += fill; count -= fill;
		while (count > BLAKE2B_BLOCKBYTES)
		{
			S->t[0] += BLAKE2B_BLOCKBYTES;
			S->t[1] += (S->t[0] < BLAKE2B_BLOCKBYTES);
			blake2b_compress(S, in);
			in += BLAKE2B_BLOCKBYTES;
			count -= BLAKE2B_BLOCKBYTES;
		}
	}
	memcpy(S->buf + S->buflen, in, count);
	S->buflen += count;
}

std::size_t BLAKE2Engine::digestLength() const
{
	return (size_t)((int)_algorithm / 8);
}

void BLAKE2Engine::reset()
{
	if (_context != NULL) free(_context);
	_context = calloc(1, sizeof(blake2b_state));
	blake2b_param P[1];
	uint8_t outlen = (uint8_t)(_algorithm / 8);
	if ((!outlen) || (outlen > BLAKE2B_OUTBYTES)) return;
	P->digest_length = outlen;
	P->key_length = 0;
	P->fanout = 1;
	P->depth = 1;
	store32(&P->leaf_length, 0);
	store32(&P->node_offset, 0);
	store32(&P->xof_length, 0);
	P->node_depth = 0;
	P->inner_length = 0;
	memset(P->reserved, 0, sizeof(P->reserved));
	memset(P->salt, 0, sizeof(P->salt));
	memset(P->personal, 0, sizeof(P->personal));
	blake2b_state* S = (blake2b_state*)_context;
	for (size_t i = 0; i < 8; ++i) S->h[i] = blake2b_IV[i];
	const uint8_t *p = (const uint8_t *)(P);
	for (size_t i = 0; i < 8; ++i) S->h[i] ^= load64(p + sizeof(S->h[i]) * i);
	S->outlen = P->digest_length;
	_digest.resize(S->outlen);
}

const DigestEngine::Digest& BLAKE2Engine::digest()
{
	_digest.clear();
	if (_context == NULL) return _digest;
	blake2b_state* S = (blake2b_state*)_context;
	uint8_t buffer[BLAKE2B_OUTBYTES] = { 0 };
	if (S->f[0] != 0) return _digest;
	S->t[0] += S->buflen;
	S->t[1] += (S->t[0] < S->buflen);
	if (S->last_node) S->f[1] = (uint64_t)-1;
	S->f[0] = (uint64_t)-1;
	memset(S->buf + S->buflen, 0, BLAKE2B_BLOCKBYTES - S->buflen);
	blake2b_compress(S, S->buf);
	for (size_t i = 0; i < 8; ++i) store64(buffer + sizeof(S->h[i]) * i, S->h[i]);
	_digest.insert(_digest.begin(), buffer, buffer + digestLength());
	memset(buffer, 0, BLAKE2B_OUTBYTES);
	reset();
	return _digest;
}

} // namespace Poco
