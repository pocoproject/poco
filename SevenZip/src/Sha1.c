/* Sha1.c -- SHA-1 Hash
2016-05-20 : Igor Pavlov : Public domain
This code is based on public domain code of Steve Reid from Wei Dai's Crypto++ library. */

#include "Precomp.h"

#include <string.h>

#include "CpuArch.h"
#include "RotateDefs.h"
#include "Sha1.h"

// define it for speed optimization
// #define _SHA1_UNROLL

#ifdef _SHA1_UNROLL
  #define kNumW 16
  #define WW(i) W[(i)&15]
#else
  #define kNumW 80
  #define WW(i) W[i]
#endif

#define w0(i) (W[i] = data[i])

#define w1(i) (WW(i) = rotlFixed(WW((i)-3) ^ WW((i)-8) ^ WW((i)-14) ^ WW((i)-16), 1))

#define f1(x,y,z)  (z^(x&(y^z)))
#define f2(x,y,z)  (x^y^z)
#define f3(x,y,z)  ((x&y)|(z&(x|y)))
#define f4(x,y,z)  (x^y^z)

#define RK(a,b,c,d,e, fx, w, k)  e += fx(b,c,d) + w + k + rotlFixed(a,5); b = rotlFixed(b,30);

#define R0(a,b,c,d,e, i)  RK(a,b,c,d,e, f1, w0(i), 0x5A827999)
#define R1(a,b,c,d,e, i)  RK(a,b,c,d,e, f1, w1(i), 0x5A827999)
#define R2(a,b,c,d,e, i)  RK(a,b,c,d,e, f2, w1(i), 0x6ED9EBA1)
#define R3(a,b,c,d,e, i)  RK(a,b,c,d,e, f3, w1(i), 0x8F1BBCDC)
#define R4(a,b,c,d,e, i)  RK(a,b,c,d,e, f4, w1(i), 0xCA62C1D6)

#define RX_1_4(rx1, rx4, i) \
    rx1(a,b,c,d,e, i); \
    rx4(e,a,b,c,d, i+1); \
    rx4(d,e,a,b,c, i+2); \
    rx4(c,d,e,a,b, i+3); \
    rx4(b,c,d,e,a, i+4); \

#define RX_5(rx, i)  RX_1_4(rx, rx, i);

#ifdef _SHA1_UNROLL

  #define RX_15 \
      RX_5(R0, 0); \
      RX_5(R0, 5); \
      RX_5(R0, 10);
  
  #define RX_20(rx, i) \
      RX_5(rx, i); \
      RX_5(rx, i + 5); \
      RX_5(rx, i + 10); \
      RX_5(rx, i + 15);

#else
  
#define RX_15  { unsigned i; for (i = 0; i < 15; i += 5) { RX_5(R0, i); } }
#define RX_20(rx, ii)  { unsigned i; i = ii; for (; i < ii + 20; i += 5) { RX_5(rx, i); } }

#endif


void Sha1_Init(CSha1 *p)
{
  p->state[0] = 0x67452301;
  p->state[1] = 0xEFCDAB89;
  p->state[2] = 0x98BADCFE;
  p->state[3] = 0x10325476;
  p->state[4] = 0xC3D2E1F0;
  p->count = 0;
}

void Sha1_GetBlockDigest(CSha1 *p, const UInt32 *data, UInt32 *destDigest)
{
  UInt32 a, b, c, d, e;
  UInt32 W[kNumW];

  a = p->state[0];
  b = p->state[1];
  c = p->state[2];
  d = p->state[3];
  e = p->state[4];
  
  RX_15

  RX_1_4(R0, R1, 15);

  RX_20(R2, 20);
  RX_20(R3, 40);
  RX_20(R4, 60);

  destDigest[0] = p->state[0] + a;
  destDigest[1] = p->state[1] + b;
  destDigest[2] = p->state[2] + c;
  destDigest[3] = p->state[3] + d;
  destDigest[4] = p->state[4] + e;
}

void Sha1_UpdateBlock_Rar(CSha1 *p, UInt32 *data, int returnRes)
{
  UInt32 a, b, c, d, e;
  UInt32 W[kNumW];

  a = p->state[0];
  b = p->state[1];
  c = p->state[2];
  d = p->state[3];
  e = p->state[4];
  
  RX_15

  RX_1_4(R0, R1, 15);

  RX_20(R2, 20);
  RX_20(R3, 40);
  RX_20(R4, 60);

  p->state[0] += a;
  p->state[1] += b;
  p->state[2] += c;
  p->state[3] += d;
  p->state[4] += e;

  if (returnRes)
  {
    unsigned i;
    for (i = 0 ; i < SHA1_NUM_BLOCK_WORDS; i++)
      data[i] = W[kNumW - SHA1_NUM_BLOCK_WORDS + i];
  }
}

#define Sha1_UpdateBlock(p) Sha1_GetBlockDigest(p, p->buffer, p->state)

void Sha1_Update(CSha1 *p, const Byte *data, size_t size)
{
  unsigned pos, pos2;
  if (size == 0)
    return;
  pos = (unsigned)p->count & 0x3F;
  p->count += size;
  pos2 = pos & 3;
  pos >>= 2;
  
  if (pos2 != 0)
  {
    UInt32 w;
    pos2 = (3 - pos2) * 8;
    w = ((UInt32)*data++) << pos2;
    if (--size && pos2)
    {
      pos2 -= 8;
      w |= ((UInt32)*data++) << pos2;
      if (--size && pos2)
      {
        pos2 -= 8;
        w |= ((UInt32)*data++) << pos2;
        size--;
      }
    }
    p->buffer[pos] |= w;
    if (pos2 == 0)
      pos++;
  }

  for (;;)
  {
    if (pos == SHA1_NUM_BLOCK_WORDS)
    {
      for (;;)
      {
        unsigned i;
        Sha1_UpdateBlock(p);
        if (size < SHA1_BLOCK_SIZE)
          break;
        size -= SHA1_BLOCK_SIZE;
        for (i = 0; i < SHA1_NUM_BLOCK_WORDS; i += 2)
        {
          p->buffer[i    ] = GetBe32(data);
          p->buffer[i + 1] = GetBe32(data + 4);
          data += 8;
        }
      }
      pos = 0;
    }
    if (size < 4)
      break;

    p->buffer[pos] = GetBe32(data);
    data += 4;
    size -= 4;
    pos++;
  }

  if (size != 0)
  {
    UInt32 w = ((UInt32)data[0]) << 24;
    if (size > 1)
    {
      w |= ((UInt32)data[1]) << 16;
      if (size > 2)
        w |= ((UInt32)data[2]) << 8;
    }
    p->buffer[pos] = w;
  }
}

void Sha1_Update_Rar(CSha1 *p, Byte *data, size_t size /* , int rar350Mode */)
{
  int returnRes = False;
  
  unsigned pos = (unsigned)p->count & 0x3F;
  p->count += size;

  while (size--)
  {
    unsigned pos2 = (pos & 3);
    UInt32 v = ((UInt32)*data++) << (8 * (3 - pos2));
    UInt32 *ref = &(p->buffer[pos >> 2]);
    pos++;
    if (pos2 == 0)
    {
      *ref = v;
      continue;
    }
    *ref |= v;
    
    if (pos == SHA1_BLOCK_SIZE)
    {
      pos = 0;
      Sha1_UpdateBlock_Rar(p, p->buffer, returnRes);
      if (returnRes)
      {
        unsigned i;
        for (i = 0; i < SHA1_NUM_BLOCK_WORDS; i++)
        {
          UInt32 d = p->buffer[i];
          Byte *prev = data + i * 4 - SHA1_BLOCK_SIZE;
          SetUi32(prev, d);
        }
      }
      // returnRes = rar350Mode;
      returnRes = True;
    }
  }
}

void Sha1_Final(CSha1 *p, Byte *digest)
{
  unsigned pos = (unsigned)p->count & 0x3F;
  unsigned pos2 = (pos & 3);
  UInt64 numBits;
  UInt32 w;
  unsigned i;
  
  pos >>= 2;
  
  w = 0;
  if (pos2 != 0)
    w = p->buffer[pos];
  p->buffer[pos++] = w | (((UInt32)0x80000000) >> (8 * pos2));

  while (pos != (SHA1_NUM_BLOCK_WORDS - 2))
  {
    pos &= 0xF;
    if (pos == 0)
      Sha1_UpdateBlock(p);
    p->buffer[pos++] = 0;
  }
  
  numBits = (p->count << 3);
  p->buffer[SHA1_NUM_BLOCK_WORDS - 2] = (UInt32)(numBits >> 32);
  p->buffer[SHA1_NUM_BLOCK_WORDS - 1] = (UInt32)(numBits);
  Sha1_UpdateBlock(p);

  for (i = 0; i < SHA1_NUM_DIGEST_WORDS; i++)
  {
    UInt32 v = p->state[i];
    SetBe32(digest, v);
    digest += 4;
  }

  Sha1_Init(p);
}


void Sha1_32_PrepareBlock(const CSha1 *p, UInt32 *block, unsigned size)
{
  const UInt64 numBits = (p->count + size) << 5;
  block[SHA1_NUM_BLOCK_WORDS - 2] = (UInt32)(numBits >> 32);
  block[SHA1_NUM_BLOCK_WORDS - 1] = (UInt32)(numBits);
  block[size++] = 0x80000000;
  while (size != (SHA1_NUM_BLOCK_WORDS - 2))
    block[size++] = 0;
}

void Sha1_32_Update(CSha1 *p, const UInt32 *data, size_t size)
{
  unsigned pos = (unsigned)p->count & 0xF;
  p->count += size;
  while (size--)
  {
    p->buffer[pos++] = *data++;
    if (pos == SHA1_NUM_BLOCK_WORDS)
    {
      pos = 0;
      Sha1_UpdateBlock(p);
    }
  }
}

void Sha1_32_Final(CSha1 *p, UInt32 *digest)
{
  UInt64 numBits;
  unsigned pos = (unsigned)p->count & 0xF;
  p->buffer[pos++] = 0x80000000;

  while (pos != (SHA1_NUM_BLOCK_WORDS - 2))
  {
    pos &= 0xF;
    if (pos == 0)
      Sha1_UpdateBlock(p);
    p->buffer[pos++] = 0;
  }
  
  numBits = (p->count << 5);
  p->buffer[SHA1_NUM_BLOCK_WORDS - 2] = (UInt32)(numBits >> 32);
  p->buffer[SHA1_NUM_BLOCK_WORDS - 1] = (UInt32)(numBits);

  Sha1_GetBlockDigest(p, p->buffer, digest);
  
  Sha1_Init(p);
}
