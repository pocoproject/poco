//
// SHA3Engine.cpp
//
// Library: Foundation
// Package: Crypt
// Module:  SHA3Engine
//
// Code of class SHA3Engine.
//
/// This class implements the SHA-3 message digest algorithm.
/// (FIPS 202, see http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.202.pdf)
//
// Based on the Keccak Code Package (public domain)
// https://github.com/gvanas/KeccakCodePackage
//
// Copyright (c) 2017, Applied Informatics Software Engineering GmbH
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/SHA3Engine.h"
#include <string.h>
#include <assert.h>

namespace Poco {

#ifdef _MSC_VER
#pragma intrinsic(memcpy)
#endif

	#if defined(__GNUC__)
#define ALIGN(x) __attribute__ ((aligned(x)))
#elif defined(_MSC_VER)
#define ALIGN(x) __declspec(align(x))
#elif defined(__ARMCC_VERSION)
#define ALIGN(x) __align(x)
#else
#define ALIGN(x)
#endif

#ifdef POCO_PTR_IS_64_BIT
static const UInt64 KeccakRoundConstants[24] =
{
	0x0000000000000001,
	0x0000000000008082,
	0x800000000000808a,
	0x8000000080008000,
	0x000000000000808b,
	0x0000000080000001,
	0x8000000080008081,
	0x8000000000008009,
	0x000000000000008a,
	0x0000000000000088,
	0x0000000080008009,
	0x000000008000000a,
	0x000000008000808b,
	0x800000000000008b,
	0x8000000000008089,
	0x8000000000008003,
	0x8000000000008002,
	0x8000000000000080,
	0x000000000000800a,
	0x800000008000000a,
	0x8000000080008081,
	0x8000000000008080,
	0x0000000080000001,
	0x8000000080008008,
};
static const unsigned int KeccakRhoOffsets[25] =
{
	0, 1, 62, 28, 27, 36, 44, 6, 55, 20, 3, 10, 43, 25, 39, 41, 45, 15, 21, 8, 18, 2, 61, 56, 14
};
#define KeccakP1600_stateAlignment      8
#else
static const UInt32 KeccakRoundConstants[24][2] =
{
	0x00000001, 0x00000000,
	0x00000000, 0x00000089,
	0x00000000, 0x8000008B,
	0x00000000, 0x80008080,
	0x00000001, 0x0000008B,
	0x00000001, 0x00008000,
	0x00000001, 0x80008088,
	0x00000001, 0x80000082,
	0x00000000, 0x0000000B,
	0x00000000, 0x0000000A,
	0x00000001, 0x00008082,
	0x00000000, 0x00008003,
	0x00000001, 0x0000808B,
	0x00000001, 0x8000000B,
	0x00000001, 0x8000008A,
	0x00000001, 0x80000081,
	0x00000000, 0x80000081,
	0x00000000, 0x80000008,
	0x00000000, 0x00000083,
	0x00000000, 0x80008003,
	0x00000001, 0x80008088,
	0x00000000, 0x80000088,
	0x00000001, 0x00008000,
	0x00000000, 0x80008082
};

static const unsigned int KeccakRhoOffsets[25] =
{
	0, 1, 62, 28, 27, 36, 44, 6, 55, 20, 3, 10, 43, 25, 39, 41, 45, 15, 21, 8, 18, 2, 61, 56, 14
};
#define KeccakP1600_stateAlignment      4
#endif

ALIGN(KeccakP1600_stateAlignment) typedef struct KeccakWidth1600_SpongeInstanceStruct
{
	unsigned char state[200];
	unsigned int rate;
	unsigned int byteIOIndex;
	int squeezing;
} KeccakWidth1600_SpongeInstance;

typedef struct
{
	KeccakWidth1600_SpongeInstance sponge;
	unsigned int fixedOutputLength;
	unsigned char delimitedSuffix;
} HASHCONTEXT;

SHA3Engine::SHA3Engine(ALGORITHM algorithm)
	: _algorithm(algorithm)
	, _context(NULL)
{
	_digest.reserve(digestLength());
	reset();
}

SHA3Engine::~SHA3Engine()
{
	reset();
	free(_context);
}

#ifdef POCO_PTR_IS_64_BIT
void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
	assert(offset < 200);
	assert(offset + length <= 200);
	for (unsigned int i = 0; i<length; i++) ((unsigned char *)state)[offset + i] ^= data[i];
}

void KeccakP1600_AddByte(void *state, unsigned char byte, unsigned int offset)
{
	assert(offset < 200);
	((unsigned char *)state)[offset] ^= byte;
}

#define ROL64(a, offset) ((offset != 0) ? ((((UInt64)a) << offset) ^ (((UInt64)a) >> (64-offset))) : a)
#define index(x, y) (((x)%5)+5*((y)%5))

static void theta(UInt64 *A)
{
	unsigned int x, y;
	UInt64 C[5], D[5];

	for (x = 0; x<5; x++) {
		C[x] = 0;
		for (y = 0; y<5; y++)
			C[x] ^= A[index(x, y)];
	}
	for (x = 0; x<5; x++)
		D[x] = ROL64(C[(x + 1) % 5], 1) ^ C[(x + 4) % 5];
	for (x = 0; x<5; x++)
		for (y = 0; y<5; y++)
			A[index(x, y)] ^= D[x];
}

static void rho(UInt64 *A)
{
	unsigned int x, y;

	for (x = 0; x<5; x++) for (y = 0; y<5; y++)
		A[index(x, y)] = ROL64(A[index(x, y)], KeccakRhoOffsets[index(x, y)]);
}

static void pi(UInt64 *A)
{
	unsigned int x, y;
	UInt64 tempA[25];

	for (x = 0; x<5; x++) for (y = 0; y<5; y++)
		tempA[index(x, y)] = A[index(x, y)];
	for (x = 0; x<5; x++) for (y = 0; y<5; y++)
		A[index(0 * x + 1 * y, 2 * x + 3 * y)] = tempA[index(x, y)];
}

static void chi(UInt64 *A)
{
	unsigned int x, y;
	UInt64 C[5];

	for (y = 0; y<5; y++) {
		for (x = 0; x<5; x++)
			C[x] = A[index(x, y)] ^ ((~A[index(x + 1, y)]) & A[index(x + 2, y)]);
		for (x = 0; x<5; x++)
			A[index(x, y)] = C[x];
	}
}

static void iota(UInt64 *A, unsigned int indexRound)
{
	A[index(0, 0)] ^= KeccakRoundConstants[indexRound];
}

void KeccakP1600Round(UInt64 *state, unsigned int indexRound)
{
	theta(state);
	rho(state);
	pi(state);
	chi(state);
	iota(state, indexRound);
}

void KeccakP1600OnWords(UInt64 *state, unsigned int nrRounds)
{
	for (unsigned int i = (24 - nrRounds); i < 24; i++) KeccakP1600Round(state, i);
}

void KeccakP1600_Permute_24rounds(void *state)
{
#ifndef POCO_ARCH_LITTLE_ENDIAN
	UInt64 stateAsWords[1600 / 64];
#endif
#ifdef POCO_ARCH_LITTLE_ENDIAN
	KeccakP1600OnWords((UInt64*)state, 24);
#else
	fromBytesToWords(stateAsWords, (const unsigned char *)state);
	KeccakP1600OnWords(stateAsWords, 24);
	fromWordsToBytes((unsigned char *)state, stateAsWords);
#endif
}

void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
	assert(offset < 200);
	assert(offset + length <= 200);
	memcpy(data, (unsigned char*)state + offset, length);
}
#else
void toBitInterleaving(UInt32 low, UInt32 high, UInt32 *even, UInt32 *odd)
{
	unsigned int i;

	*even = 0;
	*odd = 0;
	for (i = 0; i<64; i++) {
		unsigned int inBit;
		if (i < 32)
			inBit = (low >> i) & 1;
		else
			inBit = (high >> (i - 32)) & 1;
		if ((i % 2) == 0)
			*even |= inBit << (i / 2);
		else
			*odd |= inBit << ((i - 1) / 2);
	}
}

void fromBitInterleaving(UInt32 even, UInt32 odd, UInt32 *low, UInt32 *high)
{
	unsigned int i;

	*low = 0;
	*high = 0;
	for (i = 0; i<64; i++) {
		unsigned int inBit;
		if ((i % 2) == 0)
			inBit = (even >> (i / 2)) & 1;
		else
			inBit = (odd >> ((i - 1) / 2)) & 1;
		if (i < 32)
			*low |= inBit << i;
		else
			*high |= inBit << (i - 32);
	}
}

void KeccakP1600_AddBytesInLane(void *state, unsigned int lanePosition, const unsigned char *data, unsigned int offset, unsigned int length)
{
	if ((lanePosition < 25) && (offset < 8) && (offset + length <= 8)) {
		UInt8 laneAsBytes[8];
		UInt32 low, high;
		UInt32 lane[2];
		UInt32 *stateAsHalfLanes;

		memset(laneAsBytes, 0, 8);
		memcpy(laneAsBytes + offset, data, length);
		low = laneAsBytes[0]
			| ((UInt32)(laneAsBytes[1]) << 8)
			| ((UInt32)(laneAsBytes[2]) << 16)
			| ((UInt32)(laneAsBytes[3]) << 24);
		high = laneAsBytes[4]
			| ((UInt32)(laneAsBytes[5]) << 8)
			| ((UInt32)(laneAsBytes[6]) << 16)
			| ((UInt32)(laneAsBytes[7]) << 24);
		toBitInterleaving(low, high, lane, lane + 1);
		stateAsHalfLanes = (UInt32*)state;
		stateAsHalfLanes[lanePosition * 2 + 0] ^= lane[0];
		stateAsHalfLanes[lanePosition * 2 + 1] ^= lane[1];
	}
}

void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length)
{
	unsigned int lanePosition = offset / 8;
	unsigned int offsetInLane = offset % 8;

	assert(offset < 200);
	assert(offset + length <= 200);
	while (length > 0) {
		unsigned int bytesInLane = 8 - offsetInLane;
		if (bytesInLane > length)
			bytesInLane = length;
		KeccakP1600_AddBytesInLane(state, lanePosition, data, offsetInLane, bytesInLane);
		length -= bytesInLane;
		lanePosition++;
		offsetInLane = 0;
		data += bytesInLane;
	}
}

void KeccakP1600_AddByte(void *state, unsigned char byte, unsigned int offset)
{
	unsigned char data[1];
	assert(offset < 200);
	data[0] = byte;
	KeccakP1600_AddBytes(state, data, offset, 1);
}

#define ROL32(a, offset) ((offset != 0) ? ((((UInt32)a) << offset) ^ (((UInt32)a) >> (32-offset))) : a)
#define index(x, y,z) ((((x)%5)+5*((y)%5))*2 + z)
void ROL64(UInt32 inEven, UInt32 inOdd, UInt32 *outEven, UInt32 *outOdd, unsigned int offset)
{
	if ((offset % 2) == 0) {
		*outEven = ROL32(inEven, offset / 2);
		*outOdd = ROL32(inOdd, offset / 2);
	}
	else {
		*outEven = ROL32(inOdd, (offset + 1) / 2);
		*outOdd = ROL32(inEven, (offset - 1) / 2);
	}
}

static void theta(UInt32 *A)
{
	unsigned int x, y, z;
	UInt32 C[5][2], D[5][2];
	for (x = 0; x<5; x++) {
		for (z = 0; z<2; z++) {
			C[x][z] = 0;
			for (y = 0; y<5; y++)
				C[x][z] ^= A[index(x, y, z)];
		}
	}
	for (x = 0; x<5; x++) {
		ROL64(C[(x + 1) % 5][0], C[(x + 1) % 5][1], &(D[x][0]), &(D[x][1]), 1);
		for (z = 0; z<2; z++)
			D[x][z] ^= C[(x + 4) % 5][z];
	}
	for (x = 0; x<5; x++)
		for (y = 0; y<5; y++)
			for (z = 0; z<2; z++)
				A[index(x, y, z)] ^= D[x][z];
}

static void rho(UInt32 *A)
{
	unsigned int x, y;
	for (x = 0; x<5; x++) for (y = 0; y<5; y++)
		ROL64(A[index(x, y, 0)], A[index(x, y, 1)], &(A[index(x, y, 0)]), &(A[index(x, y, 1)]), KeccakRhoOffsets[5 * y + x]);
}

static void pi(UInt32 *A)
{
	unsigned int x, y, z;
	UInt32 tempA[50];
	for (x = 0; x<5; x++) for (y = 0; y<5; y++) for (z = 0; z<2; z++)
		tempA[index(x, y, z)] = A[index(x, y, z)];
	for (x = 0; x<5; x++) for (y = 0; y<5; y++) for (z = 0; z<2; z++)
		A[index(0 * x + 1 * y, 2 * x + 3 * y, z)] = tempA[index(x, y, z)];
}

static void chi(UInt32 *A)
{
	unsigned int x, y, z;
	UInt32 C[5][2];
	for (y = 0; y<5; y++) {
		for (x = 0; x<5; x++)
			for (z = 0; z<2; z++)
				C[x][z] = A[index(x, y, z)] ^ ((~A[index(x + 1, y, z)]) & A[index(x + 2, y, z)]);
		for (x = 0; x<5; x++)
			for (z = 0; z<2; z++)
				A[index(x, y, z)] = C[x][z];
	}
}

static void iota(UInt32 *A, unsigned int indexRound)
{
	A[index(0, 0, 0)] ^= KeccakRoundConstants[indexRound][0];
	A[index(0, 0, 1)] ^= KeccakRoundConstants[indexRound][1];
}

void KeccakP1600_PermutationOnWords(UInt32 *state, unsigned int nrRounds)
{
	for (unsigned int i = (24 - nrRounds); i < 24; i++)
	{
		theta(state);
		rho(state);
		pi(state);
		chi(state);
		iota(state, i);
	}
}

void KeccakP1600_ExtractBytesInLane(const void *state, unsigned int lanePosition, unsigned char *data, unsigned int offset, unsigned int length)
{
	if ((lanePosition < 25) && (offset < 8) && (offset + length <= 8)) {
		UInt32 *stateAsHalfLanes = (UInt32*)state;
		UInt32 lane[2];
		UInt8 laneAsBytes[8];
		fromBitInterleaving(stateAsHalfLanes[lanePosition * 2], stateAsHalfLanes[lanePosition * 2 + 1], lane, lane + 1);
		laneAsBytes[0] = lane[0] & 0xFF;
		laneAsBytes[1] = (lane[0] >> 8) & 0xFF;
		laneAsBytes[2] = (lane[0] >> 16) & 0xFF;
		laneAsBytes[3] = (lane[0] >> 24) & 0xFF;
		laneAsBytes[4] = lane[1] & 0xFF;
		laneAsBytes[5] = (lane[1] >> 8) & 0xFF;
		laneAsBytes[6] = (lane[1] >> 16) & 0xFF;
		laneAsBytes[7] = (lane[1] >> 24) & 0xFF;
		memcpy(data, laneAsBytes + offset, length);
	}
}

void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length)
{
	unsigned int lanePosition = offset / 8;
	unsigned int offsetInLane = offset % 8;

	assert(offset < 200);
	assert(offset + length <= 200);
	while (length > 0) {
		unsigned int bytesInLane = 8 - offsetInLane;
		if (bytesInLane > length)
			bytesInLane = length;
		KeccakP1600_ExtractBytesInLane(state, lanePosition, data, offsetInLane, bytesInLane);
		length -= bytesInLane;
		lanePosition++;
		offsetInLane = 0;
		data += bytesInLane;
	}
}

void KeccakP1600_Permute_24rounds(void *state)
{
	UInt32 *stateAsHalfLanes = (UInt32*)state;
	{
		UInt8 stateAsBytes[1600 / 8];
		KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600 / 8);
	}
	KeccakP1600_PermutationOnWords(stateAsHalfLanes, 24);
	{
		UInt8 stateAsBytes[1600 / 8];
		KeccakP1600_ExtractBytes(state, stateAsBytes, 0, 1600 / 8);
	}
}
#endif

void SHA3Engine::updateImpl(const void* buffer_, std::size_t count)
{
	if (_context == NULL || buffer_ == NULL || count == 0) return;
	HASHCONTEXT* pContext = (HASHCONTEXT*)_context;
	size_t j;
	unsigned int partialBlock;
	unsigned int rateInBytes = (&pContext->sponge)->rate / 8;
	if ((&pContext->sponge)->squeezing) return;
	size_t i = 0;
	const unsigned char *curData = (const unsigned char *)buffer_;
	while (i < count) {
		if (((&pContext->sponge)->byteIOIndex == 0) && (count >= (i + rateInBytes))) {
			for (j = count - i; j >= rateInBytes; j -= rateInBytes) {
				KeccakP1600_AddBytes((&pContext->sponge)->state, curData, 0, rateInBytes);
				KeccakP1600_Permute_24rounds((&pContext->sponge)->state);
				curData += rateInBytes;
			}
			i = count - j;
		}
		else {
			partialBlock = (unsigned int)(count - i);
			if (partialBlock + (&pContext->sponge)->byteIOIndex > rateInBytes)
				partialBlock = rateInBytes - (&pContext->sponge)->byteIOIndex;
			i += partialBlock;

			KeccakP1600_AddBytes((&pContext->sponge)->state, curData, (&pContext->sponge)->byteIOIndex, partialBlock);
			curData += partialBlock;
			(&pContext->sponge)->byteIOIndex += partialBlock;
			if ((&pContext->sponge)->byteIOIndex == rateInBytes) {
				KeccakP1600_Permute_24rounds((&pContext->sponge)->state);
				(&pContext->sponge)->byteIOIndex = 0;
			}
		}
	}
}

std::size_t SHA3Engine::digestLength() const
{
	return (size_t)((int)_algorithm / 8);
}

int KeccakWidth1600_SpongeInitialize(KeccakWidth1600_SpongeInstance *instance, unsigned int rate, unsigned int capacity)
{
	if (rate + capacity != 1600) return 1;
	if ((rate <= 0) || (rate > 1600) || ((rate % 8) != 0)) return 1;
	memset(instance->state, 0, 1600 / 8);
	instance->rate = rate;
	instance->byteIOIndex = 0;
	instance->squeezing = 0;
	return 0;
}

void SHA3Engine::reset()
{
	if (_context != NULL) free(_context);
	_context = calloc(1, sizeof(HASHCONTEXT));
	HASHCONTEXT* pContext = (HASHCONTEXT*)_context;
	switch (_algorithm)
	{
	case SHA3_224:
		KeccakWidth1600_SpongeInitialize(&pContext->sponge, 1152, 448);
		pContext->fixedOutputLength = 224;
		pContext->delimitedSuffix = 0x06;
		break;
	case SHA3_256:
		KeccakWidth1600_SpongeInitialize(&pContext->sponge, 1088, 512);
		pContext->fixedOutputLength = 256;
		pContext->delimitedSuffix = 0x06;
		break;
	case SHA3_384:
		KeccakWidth1600_SpongeInitialize(&pContext->sponge, 832, 768);
		pContext->fixedOutputLength = 384;
		pContext->delimitedSuffix = 0x06;
		break;
	default:
	case SHA3_512:
		KeccakWidth1600_SpongeInitialize(&pContext->sponge, 576, 1024);
		pContext->fixedOutputLength = 512;
		pContext->delimitedSuffix = 0x06;
		break;
	}
}

int KeccakWidth1600_SpongeAbsorbLastFewBits(KeccakWidth1600_SpongeInstance *instance, unsigned char delimitedData)
{
	unsigned int rateInBytes = instance->rate / 8;

	if (delimitedData == 0)
		return 1;
	if (instance->squeezing)
		return 1; /* Too late for additional input */

	/* Last few bits, whose delimiter coincides with first bit of padding */
	KeccakP1600_AddByte(instance->state, delimitedData, instance->byteIOIndex);
	/* If the first bit of padding is at position rate-1, we need a whole new block for the second bit of padding */
	if ((delimitedData >= 0x80) && (instance->byteIOIndex == (rateInBytes - 1)))
		KeccakP1600_Permute_24rounds(instance->state);
	/* Second bit of padding */
	KeccakP1600_AddByte(instance->state, 0x80, rateInBytes - 1);
	KeccakP1600_Permute_24rounds(instance->state);
	instance->byteIOIndex = 0;
	instance->squeezing = 1;
	return 0;
}

int SpongeAbsorbLastFewBits(KeccakWidth1600_SpongeInstance *instance, unsigned char delimitedData)
{
	unsigned int rateInBytes = instance->rate / 8;
	if (delimitedData == 0) return 1;
	if (instance->squeezing) return 1; /* Too late for additional input */
	/* Last few bits, whose delimiter coincides with first bit of padding */
	KeccakP1600_AddByte(instance->state, delimitedData, instance->byteIOIndex);
	/* If the first bit of padding is at position rate-1, we need a whole new block for the second bit of padding */
	if ((delimitedData >= 0x80) && (instance->byteIOIndex == (rateInBytes - 1)))
		KeccakP1600_Permute_24rounds(instance->state);
	/* Second bit of padding */
	KeccakP1600_AddByte(instance->state, 0x80, rateInBytes - 1);
	KeccakP1600_Permute_24rounds(instance->state);
	instance->byteIOIndex = 0;
	instance->squeezing = 1;
	return 0;
}

int KeccakWidth1600_SpongeSqueeze(KeccakWidth1600_SpongeInstance *instance, unsigned char *data, size_t dataByteLen)
{
	size_t i, j;
	unsigned int partialBlock;
	unsigned int rateInBytes = instance->rate / 8;
	unsigned char *curData;

	if (!instance->squeezing)
		SpongeAbsorbLastFewBits(instance, 0x01);

	i = 0;
	curData = data;
	while (i < dataByteLen) {
		if ((instance->byteIOIndex == rateInBytes) && (dataByteLen >= (i + rateInBytes))) {
			for (j = dataByteLen - i; j >= rateInBytes; j -= rateInBytes) {
				KeccakP1600_Permute_24rounds(instance->state);
				KeccakP1600_ExtractBytes(instance->state, curData, 0, rateInBytes);
				curData += rateInBytes;
			}
			i = dataByteLen - j;
		}
		else {
			/* normal lane: using the message queue */
			if (instance->byteIOIndex == rateInBytes) {
				KeccakP1600_Permute_24rounds(instance->state);
				instance->byteIOIndex = 0;
			}
			partialBlock = (unsigned int)(dataByteLen - i);
			if (partialBlock + instance->byteIOIndex > rateInBytes)
				partialBlock = rateInBytes - instance->byteIOIndex;
			i += partialBlock;

			KeccakP1600_ExtractBytes(instance->state, curData, instance->byteIOIndex, partialBlock);
			curData += partialBlock;
			instance->byteIOIndex += partialBlock;
		}
	}
	return 0;
}

const DigestEngine::Digest& SHA3Engine::digest()
{
	_digest.clear();
	HASHCONTEXT* pContext = (HASHCONTEXT*)_context;
	unsigned char hash[64];
	memset(hash, 0, 64);
	if (KeccakWidth1600_SpongeAbsorbLastFewBits(&pContext->sponge, pContext->delimitedSuffix) == 0)
		KeccakWidth1600_SpongeSqueeze(&pContext->sponge, hash, pContext->fixedOutputLength / 8);
	_digest.insert(_digest.begin(), hash, hash + digestLength());
	reset();
	return _digest;
}

} // namespace Poco
