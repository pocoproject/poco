/* Sha1.h -- SHA-1 Hash
2016-05-20 : Igor Pavlov : Public domain */

#ifndef __7Z_SHA1_H
#define __7Z_SHA1_H

#include "7zTypes.h"

EXTERN_C_BEGIN

#define SHA1_NUM_BLOCK_WORDS  16
#define SHA1_NUM_DIGEST_WORDS  5

#define SHA1_BLOCK_SIZE   (SHA1_NUM_BLOCK_WORDS * 4)
#define SHA1_DIGEST_SIZE  (SHA1_NUM_DIGEST_WORDS * 4)

typedef struct
{
  UInt32 state[SHA1_NUM_DIGEST_WORDS];
  UInt64 count;
  UInt32 buffer[SHA1_NUM_BLOCK_WORDS];
} CSha1;

void Sha1_Init(CSha1 *p);

void Sha1_GetBlockDigest(CSha1 *p, const UInt32 *data, UInt32 *destDigest);
void Sha1_Update(CSha1 *p, const Byte *data, size_t size);
void Sha1_Final(CSha1 *p, Byte *digest);

void Sha1_Update_Rar(CSha1 *p, Byte *data, size_t size /* , int rar350Mode */);

void Sha1_32_PrepareBlock(const CSha1 *p, UInt32 *block, unsigned size);
void Sha1_32_Update(CSha1 *p, const UInt32 *data, size_t size);
void Sha1_32_Final(CSha1 *p, UInt32 *digest);

EXTERN_C_END

#endif
