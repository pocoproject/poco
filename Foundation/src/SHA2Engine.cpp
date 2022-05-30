//
// SHA2Engine.cpp
//
// Library: Foundation
// Package: Crypt
// Module:  SHA2Engine
//
// Code of class SHA2Engine.
//
// Secure Hash Standard SHA-2 algorithm
// (FIPS 180-4, see http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
//
// Based on the implementation of mbed TLS (Apache 2.0)
// http://www.apache.org/licenses/LICENSE-2.0
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SHA2Engine.h"
#include <string.h>


namespace Poco {


typedef struct
{
	union
	{
		Poco::UInt32 total32[4];
		Poco::UInt64 total64[2];
	} total;

	union
	{
		Poco::UInt32 state32[16];
		Poco::UInt64 state64[8];
	} state;

	unsigned char buffer[128];
} HASHCONTEXT;


static const Poco::UInt32 K32[] =
{
	0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
	0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
	0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
	0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
	0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
	0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
	0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
	0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
	0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
	0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
	0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
	0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
	0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
	0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
	0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
	0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2,
};


#if defined(_MSC_VER) || defined(__WATCOMC__)
#define UL64(x) x##ui64
#else
#define UL64(x) x##ULL
#endif


static const Poco::UInt64 K64[80] =
{
	UL64(0x428A2F98D728AE22), UL64(0x7137449123EF65CD),
	UL64(0xB5C0FBCFEC4D3B2F), UL64(0xE9B5DBA58189DBBC),
	UL64(0x3956C25BF348B538), UL64(0x59F111F1B605D019),
	UL64(0x923F82A4AF194F9B), UL64(0xAB1C5ED5DA6D8118),
	UL64(0xD807AA98A3030242), UL64(0x12835B0145706FBE),
	UL64(0x243185BE4EE4B28C), UL64(0x550C7DC3D5FFB4E2),
	UL64(0x72BE5D74F27B896F), UL64(0x80DEB1FE3B1696B1),
	UL64(0x9BDC06A725C71235), UL64(0xC19BF174CF692694),
	UL64(0xE49B69C19EF14AD2), UL64(0xEFBE4786384F25E3),
	UL64(0x0FC19DC68B8CD5B5), UL64(0x240CA1CC77AC9C65),
	UL64(0x2DE92C6F592B0275), UL64(0x4A7484AA6EA6E483),
	UL64(0x5CB0A9DCBD41FBD4), UL64(0x76F988DA831153B5),
	UL64(0x983E5152EE66DFAB), UL64(0xA831C66D2DB43210),
	UL64(0xB00327C898FB213F), UL64(0xBF597FC7BEEF0EE4),
	UL64(0xC6E00BF33DA88FC2), UL64(0xD5A79147930AA725),
	UL64(0x06CA6351E003826F), UL64(0x142929670A0E6E70),
	UL64(0x27B70A8546D22FFC), UL64(0x2E1B21385C26C926),
	UL64(0x4D2C6DFC5AC42AED), UL64(0x53380D139D95B3DF),
	UL64(0x650A73548BAF63DE), UL64(0x766A0ABB3C77B2A8),
	UL64(0x81C2C92E47EDAEE6), UL64(0x92722C851482353B),
	UL64(0xA2BFE8A14CF10364), UL64(0xA81A664BBC423001),
	UL64(0xC24B8B70D0F89791), UL64(0xC76C51A30654BE30),
	UL64(0xD192E819D6EF5218), UL64(0xD69906245565A910),
	UL64(0xF40E35855771202A), UL64(0x106AA07032BBD1B8),
	UL64(0x19A4C116B8D2D0C8), UL64(0x1E376C085141AB53),
	UL64(0x2748774CDF8EEB99), UL64(0x34B0BCB5E19B48A8),
	UL64(0x391C0CB3C5C95A63), UL64(0x4ED8AA4AE3418ACB),
	UL64(0x5B9CCA4F7763E373), UL64(0x682E6FF3D6B2B8A3),
	UL64(0x748F82EE5DEFB2FC), UL64(0x78A5636F43172F60),
	UL64(0x84C87814A1F0AB72), UL64(0x8CC702081A6439EC),
	UL64(0x90BEFFFA23631E28), UL64(0xA4506CEBDE82BDE9),
	UL64(0xBEF9A3F7B2C67915), UL64(0xC67178F2E372532B),
	UL64(0xCA273ECEEA26619C), UL64(0xD186B8C721C0C207),
	UL64(0xEADA7DD6CDE0EB1E), UL64(0xF57D4F7FEE6ED178),
	UL64(0x06F067AA72176FBA), UL64(0x0A637DC5A2C898A6),
	UL64(0x113F9804BEF90DAE), UL64(0x1B710B35131C471B),
	UL64(0x28DB77F523047D84), UL64(0x32CAAB7B40C72493),
	UL64(0x3C9EBE0A15C9BEBC), UL64(0x431D67C49C100D4C),
	UL64(0x4CC5D4BECB3E42B6), UL64(0x597F299CFC657E2A),
	UL64(0x5FCB6FAB3AD6FAEC), UL64(0x6C44198C4A475817)
};


static const unsigned char padding[128] = { 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


#define SHR32(x,n) ((x & 0xFFFFFFFF) >> n)
#define SHR64(x,n) (x >> n)
#define ROTR32(x,n) (SHR32(x,n) | (x << (32 - n)))
#define ROTR64(x,n) (SHR64(x,n) | (x << (64 - n)))
#define S320(x) (ROTR32(x, 7) ^ ROTR32(x,18) ^ SHR32(x, 3))
#define S321(x) (ROTR32(x,17) ^ ROTR32(x,19) ^ SHR32(x,10))
#define S322(x) (ROTR32(x, 2) ^ ROTR32(x,13) ^ ROTR32(x,22))
#define S323(x) (ROTR32(x, 6) ^ ROTR32(x,11) ^ ROTR32(x,25))
#define S640(x) (ROTR64(x, 1) ^ ROTR64(x, 8) ^ SHR64(x, 7))
#define S641(x) (ROTR64(x,19) ^ ROTR64(x,61) ^ SHR64(x, 6))
#define S642(x) (ROTR64(x,28) ^ ROTR64(x,34) ^ ROTR64(x,39))
#define S643(x) (ROTR64(x,14) ^ ROTR64(x,18) ^ ROTR64(x,41))
#define F320(x,y,z) ((x & y) | (z & (x | y)))
#define F321(x,y,z) (z ^ (x & (y ^ z)))
#define F640(x,y,z) ((x & y) | (z & (x | y)))
#define F641(x,y,z) (z ^ (x & (y ^ z)))
#define R32(t) (W[t] = S321(W[t -  2]) + W[t -  7] + S320(W[t - 15]) + W[t - 16])
#define P32(a,b,c,d,e,f,g,h,x,K)               \
{                                              \
	temp1 = h + S323(e) + F321(e,f,g) + K + x; \
	temp2 = S322(a) + F320(a,b,c);             \
	d += temp1; h = temp1 + temp2;             \
}
#define P64(a,b,c,d,e,f,g,h,x,K)               \
{                                              \
	temp1 = h + S643(e) + F641(e,f,g) + K + x; \
	temp2 = S642(a) + F640(a,b,c);             \
	d += temp1; h = temp1 + temp2;             \
}
#ifndef GET_UINT32
#define GET_UINT32(n,b,i)                        \
do {                                             \
	(n) = ( (Poco::UInt32) (b)[(i)    ] << 24 )  \
		| ( (Poco::UInt32) (b)[(i) + 1] << 16 )  \
		| ( (Poco::UInt32) (b)[(i) + 2] <<  8 )  \
		| ( (Poco::UInt32) (b)[(i) + 3]       ); \
} while( 0 )
#endif
#ifndef PUT_UINT32
#define PUT_UINT32(n,b,i)                         \
do {                                              \
	(b)[(i)    ] = (unsigned char) ( (n) >> 24 ); \
	(b)[(i) + 1] = (unsigned char) ( (n) >> 16 ); \
	(b)[(i) + 2] = (unsigned char) ( (n) >>  8 ); \
	(b)[(i) + 3] = (unsigned char) ( (n)       ); \
} while( 0 )
#endif
#ifndef GET_UINT64
#define GET_UINT64(n,b,i)                        \
{                                                \
	(n) = ( (Poco::UInt64) (b)[(i)    ] << 56 )  \
		| ( (Poco::UInt64) (b)[(i) + 1] << 48 )  \
		| ( (Poco::UInt64) (b)[(i) + 2] << 40 )  \
		| ( (Poco::UInt64) (b)[(i) + 3] << 32 )  \
		| ( (Poco::UInt64) (b)[(i) + 4] << 24 )  \
		| ( (Poco::UInt64) (b)[(i) + 5] << 16 )  \
		| ( (Poco::UInt64) (b)[(i) + 6] <<  8 )  \
		| ( (Poco::UInt64) (b)[(i) + 7]       ); \
}
#endif
#ifndef PUT_UINT64
#define PUT_UINT64(n,b,i)                         \
{                                                 \
	(b)[(i)    ] = (unsigned char) ( (n) >> 56 ); \
	(b)[(i) + 1] = (unsigned char) ( (n) >> 48 ); \
	(b)[(i) + 2] = (unsigned char) ( (n) >> 40 ); \
	(b)[(i) + 3] = (unsigned char) ( (n) >> 32 ); \
	(b)[(i) + 4] = (unsigned char) ( (n) >> 24 ); \
	(b)[(i) + 5] = (unsigned char) ( (n) >> 16 ); \
	(b)[(i) + 6] = (unsigned char) ( (n) >>  8 ); \
	(b)[(i) + 7] = (unsigned char) ( (n)       ); \
}
#endif


SHA2Engine::SHA2Engine(ALGORITHM algorithm):
	_context(NULL),
	_algorithm(algorithm)
{
	_digest.reserve(digestLength());
	reset();
}


SHA2Engine::~SHA2Engine()
{
	reset();
	free(_context);
}


void _sha256_process(HASHCONTEXT* pContext, const unsigned char data[64])
{
	unsigned int i;
	Poco::UInt32 temp1, temp2, temp3[8], W[64];
	for (i = 0; i < 8; i++) temp3[i] = pContext->state.state32[i];
	for (i = 0; i < 16; i++) { GET_UINT32(W[i], data, 4 * i); }
	for (i = 0; i < 16; i += 8)
	{
		P32(temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], W[i + 0], K32[i + 0]);
		P32(temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], W[i + 1], K32[i + 1]);
		P32(temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], W[i + 2], K32[i + 2]);
		P32(temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], W[i + 3], K32[i + 3]);
		P32(temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], W[i + 4], K32[i + 4]);
		P32(temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], W[i + 5], K32[i + 5]);
		P32(temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], W[i + 6], K32[i + 6]);
		P32(temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], W[i + 7], K32[i + 7]);
	}
	for (i = 16; i < 64; i += 8)
	{
		P32(temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], R32(i + 0), K32[i + 0]);
		P32(temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], R32(i + 1), K32[i + 1]);
		P32(temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], R32(i + 2), K32[i + 2]);
		P32(temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], R32(i + 3), K32[i + 3]);
		P32(temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], R32(i + 4), K32[i + 4]);
		P32(temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], R32(i + 5), K32[i + 5]);
		P32(temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], R32(i + 6), K32[i + 6]);
		P32(temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], R32(i + 7), K32[i + 7]);
	}
	for (i = 0; i < 8; i++) pContext->state.state32[i] += temp3[i];
}


void _sha512_process(HASHCONTEXT* pContext, const unsigned char data[128])
{
	int i;
	Poco::UInt64 temp1, temp2, temp3[8], W[80];
	for (i = 0; i < 16; i++) { GET_UINT64(W[i], data, i << 3); }
	for (; i < 80; i++) { W[i] = S641(W[i - 2]) + W[i - 7] + S640(W[i - 15]) + W[i - 16]; }
	for (i = 0; i < 8; i++) temp3[i] = pContext->state.state64[i];
	i = 0;
	do
	{
		P64(temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], W[i], K64[i]); i++;
		P64(temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], W[i], K64[i]); i++;
		P64(temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], W[i], K64[i]); i++;
		P64(temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], temp3[4], W[i], K64[i]); i++;
		P64(temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], temp3[3], W[i], K64[i]); i++;
		P64(temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], temp3[2], W[i], K64[i]); i++;
		P64(temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], temp3[1], W[i], K64[i]); i++;
		P64(temp3[1], temp3[2], temp3[3], temp3[4], temp3[5], temp3[6], temp3[7], temp3[0], W[i], K64[i]); i++;
	} while (i < 80);
	for (i = 0; i < 8; i++) pContext->state.state64[i] += temp3[i];
}


void SHA2Engine::updateImpl(const void* buffer_, std::size_t count)
{
	if (_context == NULL || buffer_ == NULL || count == 0) return;
	Poco::UInt32 left = 0;
	HASHCONTEXT* pContext = (HASHCONTEXT*)_context;
	unsigned char* data = (unsigned char*)buffer_;
	if (_algorithm > SHA_256)
	{
		left = (Poco::UInt32)(pContext->total.total64[0] & 0x7F);
		size_t fill = 128 - left;
		pContext->total.total64[0] += (Poco::UInt64)count;
		if (pContext->total.total64[0] < (Poco::UInt64)count)	pContext->total.total64[1]++;
		if (left && count >= fill)
		{
			memcpy((void *)(pContext->buffer + left), data, fill);
			_sha512_process(pContext, pContext->buffer);
			data += fill;
			count -= fill;
			left = 0;
		}
		while (count >= 128)
		{
			_sha512_process(pContext, data);
			data += 128;
			count -= 128;
		}
	}
	else
	{
		left = (Poco::UInt32)(pContext->total.total32[0] & 0x3F);
		size_t fill = 64 - left;
		pContext->total.total32[0] += (Poco::UInt32)count;
		pContext->total.total32[0] &= 0xFFFFFFFF;
		if (pContext->total.total32[0] < (Poco::UInt32)count) pContext->total.total32[1]++;
		if (left && count >= fill)
		{
			memcpy((void *)(pContext->buffer + left), data, fill);
			_sha256_process(pContext, pContext->buffer);
			data += fill;
			count -= fill;
			left = 0;
		}
		while (count >= 64)
		{
			_sha256_process(pContext, data);
			data += 64;
			count -= 64;
		}
	}
	if (count > 0) memcpy((void *)(pContext->buffer + left), data, count);
}


std::size_t SHA2Engine::digestLength() const
{
	size_t result = 0;

	switch (_algorithm)
	{
	case SHA_224:
		result = 224;
		break;
	case SHA_256:
		result = 256;
		break;
	case SHA_384:
		result = 384;
		break;
	case SHA_512:
		result = 512;
		break;
	case SHA_512_224:
		result = 224;
		break;
	case SHA_512_256:
		result = 256;
		break;
	}

	return result / 8;
}


void SHA2Engine::reset()
{
	if (_context != NULL) free(_context);
	_context = calloc(1, sizeof(HASHCONTEXT));
	HASHCONTEXT* pContext = (HASHCONTEXT*)_context;
	if (_algorithm == SHA_224)
	{
		pContext->state.state32[0] = 0xC1059ED8;
		pContext->state.state32[1] = 0x367CD507;
		pContext->state.state32[2] = 0x3070DD17;
		pContext->state.state32[3] = 0xF70E5939;
		pContext->state.state32[4] = 0xFFC00B31;
		pContext->state.state32[5] = 0x68581511;
		pContext->state.state32[6] = 0x64F98FA7;
		pContext->state.state32[7] = 0xBEFA4FA4;
	}
	else if (_algorithm == SHA_256)
	{
		pContext->state.state32[0] = 0x6A09E667;
		pContext->state.state32[1] = 0xBB67AE85;
		pContext->state.state32[2] = 0x3C6EF372;
		pContext->state.state32[3] = 0xA54FF53A;
		pContext->state.state32[4] = 0x510E527F;
		pContext->state.state32[5] = 0x9B05688C;
		pContext->state.state32[6] = 0x1F83D9AB;
		pContext->state.state32[7] = 0x5BE0CD19;
	}
	else if (_algorithm == SHA_384)
	{
		pContext->state.state64[0] = UL64(0xCBBB9D5DC1059ED8);
		pContext->state.state64[1] = UL64(0x629A292A367CD507);
		pContext->state.state64[2] = UL64(0x9159015A3070DD17);
		pContext->state.state64[3] = UL64(0x152FECD8F70E5939);
		pContext->state.state64[4] = UL64(0x67332667FFC00B31);
		pContext->state.state64[5] = UL64(0x8EB44A8768581511);
		pContext->state.state64[6] = UL64(0xDB0C2E0D64F98FA7);
		pContext->state.state64[7] = UL64(0x47B5481DBEFA4FA4);
	}
	else if (_algorithm == SHA_512)
	{
		pContext->state.state64[0] = UL64(0x6A09E667F3BCC908);
		pContext->state.state64[1] = UL64(0xBB67AE8584CAA73B);
		pContext->state.state64[2] = UL64(0x3C6EF372FE94F82B);
		pContext->state.state64[3] = UL64(0xA54FF53A5F1D36F1);
		pContext->state.state64[4] = UL64(0x510E527FADE682D1);
		pContext->state.state64[5] = UL64(0x9B05688C2B3E6C1F);
		pContext->state.state64[6] = UL64(0x1F83D9ABFB41BD6B);
		pContext->state.state64[7] = UL64(0x5BE0CD19137E2179);
	}
	else if (_algorithm == SHA_512_224)
	{
		pContext->state.state64[0] = UL64(0x8C3D37C819544DA2);
		pContext->state.state64[1] = UL64(0x73E1996689DCD4D6);
		pContext->state.state64[2] = UL64(0x1DFAB7AE32FF9C82);
		pContext->state.state64[3] = UL64(0x679DD514582F9FCF);
		pContext->state.state64[4] = UL64(0x0F6D2B697BD44DA8);
		pContext->state.state64[5] = UL64(0x77E36F7304C48942);
		pContext->state.state64[6] = UL64(0x3F9D85A86A1D36C8);
		pContext->state.state64[7] = UL64(0x1112E6AD91D692A1);
	} else
	{
		pContext->state.state64[0] = UL64(0x22312194FC2BF72C);
		pContext->state.state64[1] = UL64(0x9F555FA3C84C64C2);
		pContext->state.state64[2] = UL64(0x2393B86B6F53B151);
		pContext->state.state64[3] = UL64(0x963877195940EABD);
		pContext->state.state64[4] = UL64(0x96283EE2A88EFFE3);
		pContext->state.state64[5] = UL64(0xBE5E1E2553863992);
		pContext->state.state64[6] = UL64(0x2B0199FC2C85B8AA);
		pContext->state.state64[7] = UL64(0x0EB72DDC81C52CA2);
	}
}


const DigestEngine::Digest& SHA2Engine::digest()
{
	_digest.clear();
	if (_context == NULL) return _digest;
	HASHCONTEXT* pContext = (HASHCONTEXT*)_context;
	size_t last, padn;
	unsigned char hash[64];
	memset(hash, 0, 64);

	if (_algorithm > SHA_256)
	{
		unsigned char msglen[16];
		Poco::UInt64 high = (pContext->total.total64[0] >> 61) | (pContext->total.total64[1] << 3);
		Poco::UInt64 low = (pContext->total.total64[0] << 3);
		PUT_UINT64(high, msglen, 0);
		PUT_UINT64(low, msglen, 8);
		last = (size_t)(pContext->total.total64[0] & 0x7F);
		padn = (last < 112) ? (112 - last) : (240 - last);
		updateImpl(padding, padn);
		updateImpl(msglen, 16);
		PUT_UINT64(pContext->state.state64[0], hash, 0);
		PUT_UINT64(pContext->state.state64[1], hash, 8);
		PUT_UINT64(pContext->state.state64[2], hash, 16);
		PUT_UINT64(pContext->state.state64[3], hash, 24);
		PUT_UINT64(pContext->state.state64[4], hash, 32);
		PUT_UINT64(pContext->state.state64[5], hash, 40);
		if (_algorithm > SHA_384)
		{
			PUT_UINT64(pContext->state.state64[6], hash, 48);
			PUT_UINT64(pContext->state.state64[7], hash, 56);
		}
	}
	else
	{
		unsigned char msglen[8];
		Poco::UInt32 high = (pContext->total.total32[0] >> 29) | (pContext->total.total32[1] << 3);
		Poco::UInt32 low = (pContext->total.total32[0] << 3);
		PUT_UINT32(high, msglen, 0);
		PUT_UINT32(low, msglen, 4);
		last = pContext->total.total32[0] & 0x3F;
		padn = (last < 56) ? (56 - last) : (120 - last);
		updateImpl(padding, padn);
		updateImpl(msglen, 8);
		PUT_UINT32(pContext->state.state32[0], hash, 0);
		PUT_UINT32(pContext->state.state32[1], hash, 4);
		PUT_UINT32(pContext->state.state32[2], hash, 8);
		PUT_UINT32(pContext->state.state32[3], hash, 12);
		PUT_UINT32(pContext->state.state32[4], hash, 16);
		PUT_UINT32(pContext->state.state32[5], hash, 20);
		PUT_UINT32(pContext->state.state32[6], hash, 24);
		if (_algorithm > SHA_224) PUT_UINT32(pContext->state.state32[7], hash, 28);
	}
	_digest.insert(_digest.begin(), hash, hash + digestLength());
	reset();
	return _digest;
}


} // namespace Poco
