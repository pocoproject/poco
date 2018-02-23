/******************************************************************************
* libbmpread - tiny, fast bitmap (.bmp) image file loader                     *
*              <https://github.com/chazomaticus/libbmpread>                   *
* Copyright (C) 2005, 2012, 2016, 2018 Charles Lindsay <chaz@chazomatic.us>   *
*                                                                             *
*  This software is provided 'as-is', without any express or implied          *
*  warranty.  In no event will the authors be held liable for any damages     *
*  arising from the use of this software.                                     *
*                                                                             *
*  Permission is granted to anyone to use this software for any purpose,      *
*  including commercial applications, and to alter it and redistribute it     *
*  freely, subject to the following restrictions:                             *
*                                                                             *
*  1. The origin of this software must not be misrepresented; you must not    *
*     claim that you wrote the original software. If you use this software    *
*     in a product, an acknowledgment in the product documentation would be   *
*     appreciated but is not required.                                        *
*  2. Altered source versions must be plainly marked as such, and must not be *
*     misrepresented as being the original software.                          *
*  3. This notice may not be removed or altered from any source distribution. *
******************************************************************************/


/* bmpread.c
 * version 3.0
 * 2018-02-02
 */


#include "bmpread.h"

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* If your compiler doesn't come with stdint.h, which is technically a C99
 * feature, see <http://stackoverflow.com/q/126279>.  There are 3rd party
 * solutions to this problem, which you should be able to find with a little
 * searching.  Alternately, just define the following types yourself: uint8_t,
 * uint16_t, uint32_t, and int32_t.
 */
#include <stdint.h>

/* This code makes a number of assumptions about a byte being 8 bits, which is
 * technically not required by the C spec(s).  It's likely that not a whole lot
 * here would need to change if CHAR_BIT != 8, but I haven't taken the time to
 * figure out exactly what those changes would be.
 */
#if CHAR_BIT != 8
#error "libbmpread requires CHAR_BIT == 8"
#endif


/* Default value for alpha when none is present in the file. */
#define BMPREAD_DEFAULT_ALPHA 255

/* I've tried to make every effort to remove the possibility of undefined
 * behavior and prevent related errors where maliciously crafted files could
 * lead to buffer overflows or the like.  To that end, we'll start with some
 * functions that check various operations for behaving as expected.  This one
 * returns nonzero if the two size_ts can be added without wrapping, or 0 if
 * the result would wrap.
 */
static int CanAdd(size_t a, size_t b)
{
    return a <= SIZE_MAX - b;
}

/* Returns nonzero if the two size_ts can be multiplied without wrapping, or 0
 * if the result would wrap.  b must not be 0 (we don't even check here since
 * everything we pass in will have been checked before).
 */
static int CanMultiply(size_t a, size_t b)
{
    return a <= SIZE_MAX / b;
}

/* Returns nonzero if the uint32_t can be converted to a size_t without losing
 * data, which is always the case on 32-bit systems and higher, or 0 if such a
 * conversion would lose data, as could happen on 16-bit systems.
 */
static int CanMakeSizeT(uint32_t x)
{
    /* The preprocessor guard is there to prevent a warning about the condition
     * inside being true by definition on systems where size_t is at least 32
     * bits.  I'm relying on C's integer promotion rules to make this all safe.
     * I *think* it works as intended here (either way, typecasts don't really
     * help clarify things, so I've gone without).
     */
#if UINT32_MAX > SIZE_MAX
    if(x > SIZE_MAX) return 0;
#endif

    (void)x; /* Sometimes unused; this prevents a pedantic warning. */
    return 1;
}

/* Returns nonzero if the uint32_t can be converted to a long without losing
 * data, or 0 if the conversion would lose data.
 */
static int CanMakeLong(uint32_t x)
{
#if UINT32_MAX > LONG_MAX
    if(x > LONG_MAX) return 0;
#endif

    (void)x; /* Sometimes unused. */
    return 1;
}

/* Returns nonzero if the int32_t can be negated properly.  INT32_MIN doesn't
 * work because its positive value isn't representable inside an int32_t (given
 * two's complement).
 */
static int CanNegate(int32_t x)
{
    return x != INT32_MIN;
}

/* Reads up to 4 little-endian bytes from fp and stores the result in the
 * uint32_t pointed to by dest in the host's byte order.  Returns 0 on EOF or
 * nonzero on success.
 */
static int ReadLittleBytes(uint32_t * dest, int bytes, FILE * fp)
{
    uint32_t shift = 0;

    *dest = 0;

    while(bytes--)
    {
        int byte;
        if((byte = fgetc(fp)) == EOF) return 0;

        *dest += (uint32_t)byte << shift;
        shift += 8;
    }

    return 1;
}

/* Reads a little-endian uint32_t from fp and stores the result in *dest in the
 * host's byte order.  Returns 0 on EOF or nonzero on success.
 */
#define ReadLittleUint32(dest, fp) ReadLittleBytes(dest, 4, fp)

/* Reads a little-endian int32_t from fp and stores the result in *dest in the
 * host's byte order.  Returns 0 on EOF or nonzero on success.
 */
static int ReadLittleInt32(int32_t * dest, FILE * fp)
{
    /* I *believe* casting unsigned -> signed is implementation-defined when
     * the unsigned value is out of range for the signed type, which would be
     * the case for any negative number we've just read out of the file into a
     * uint.  This is a portable way to "reinterpret" the bits as signed
     * without running into undefined/implementation-defined behavior.  I
     * think.
     */
    union int32_signedness_swap
    {
        uint32_t uint32;
        int32_t  int32;

    } t;

    if(!ReadLittleBytes(&t.uint32, 4, fp)) return 0;
    *dest = t.int32;
    return 1;
}

/* Reads a little-endian uint16_t from fp and stores the result in *dest in the
 * host's byte order.  Returns 0 on EOF or nonzero n success.
 */
static int ReadLittleUint16(uint16_t * dest, FILE * fp)
{
    uint32_t t;
    if(!ReadLittleBytes(&t, 2, fp)) return 0;
    *dest = (uint16_t)t;
    return 1;
}

/* Reads a uint8_t from fp and stores the result in *dest.  Returns 0 on EOF or
 * nonzero on success.
 */
static int ReadUint8(uint8_t * dest, FILE * fp)
{
    int byte;
    if((byte = fgetc(fp)) == EOF) return 0;
    *dest = (uint8_t)byte;
    return 1;
}

/* Bitmap file header, including magic bytes.
 */
typedef struct bmp_header
{
    uint8_t  magic[2];    /* Magic bytes 'B' and 'M'. */
    uint32_t file_size;   /* Size of whole file. */
    uint32_t unused;      /* Should be 0. */
    uint32_t data_offset; /* Offset from beginning of file to bitmap data. */

} bmp_header;

/* Reads a bitmap header from fp into header.  Returns 0 on EOF or invalid
 * header, or nonzero on success.
 */
static int ReadHeader(bmp_header * header, FILE * fp)
{
    if(!ReadUint8(&header->magic[0], fp)) return 0;
    if(!ReadUint8(&header->magic[1], fp)) return 0;

    /* If it doesn't look like a bitmap header, don't even bother. */
    if(header->magic[0] != 0x42 /* 'B' */) return 0;
    if(header->magic[1] != 0x4d /* 'M' */) return 0;

    if(!ReadLittleUint32(&header->file_size,   fp)) return 0;
    if(!ReadLittleUint32(&header->unused,      fp)) return 0;
    if(!ReadLittleUint32(&header->data_offset, fp)) return 0;

    return 1;
}

/* How many bytes in the file are occupied by a header, by definition in the
 * spec.  Note that even though our definition logically matches the spec's, C
 * struct padding/packing rules mean it might not be the same as
 * sizeof(bmp_header).
 */
#define BMP_HEADER_SIZE 14

/* Bitmap info: comes immediately after the header and describes the image.
 */
typedef struct bmp_info
{
    uint32_t info_size;   /* Size of info struct (> sizeof(bmp_info)). */
    int32_t  width;       /* Width of image. */
    int32_t  height;      /* Height (< 0 means right-side up). */
    uint16_t planes;      /* Planes (should be 1). */
    uint16_t bits;        /* Number of bits (1, 4, 8, 16, 24, or 32). */
    uint32_t compression; /* See COMPRESSION_* values below. */
    uint32_t unused0[3];  /* We don't care about these fields. */
    uint32_t colors;      /* How many colors in the palette, 0 = 1<<bits. */
    uint32_t unused1;     /* Another field we don't care about. */
    uint32_t masks[4];    /* Bitmasks for 16- and 32-bit images. */

    /* There can be additional later fields in the actual file info, but we
     * don't need them here.
     */

} bmp_info;

/* We don't support files in bitmap formats older than Windows 3, due to
 * incompatibilities I didn't want to bother coding around.  info_size is
 * defined as 40 for both Windows 3 and NT bitmap formats (together "bitmap
 * version 3" format), and gets larger in later incarnations.  We *don't*
 * support Windows NT format, which is just to say we don't support 16- or
 * 32-bit depths before "bitmap version 4", because their data is in an awkward
 * format.
 */
#define BMP3_INFO_SIZE 40
#define MIN_INFO_SIZE BMP3_INFO_SIZE

/* Values for the compression field.  We only support COMPRESSION_NONE and
 * COMPRESSION_BITFIELDS so far.
 */
#define COMPRESSION_NONE      0
#define COMPRESSION_RLE8      1
#define COMPRESSION_RLE4      2
#define COMPRESSION_BITFIELDS 3

/* Reads bitmap metadata from fp into info.  Returns 0 on EOF or invalid info,
 * or nonzero on success.  info is assumed to be initialized to 0 already.
 */
static int ReadInfo(bmp_info * info, FILE * fp)
{
    if(!ReadLittleUint32(&info->info_size, fp)) return 0;

    /* Older formats might not have all the fields we require, so this check
     * comes first.
     */
    if(info->info_size < MIN_INFO_SIZE) return 0;

    if(!ReadLittleInt32( &info->width,       fp)) return 0;
    if(!ReadLittleInt32( &info->height,      fp)) return 0;
    if(!ReadLittleUint16(&info->planes,      fp)) return 0;
    if(!ReadLittleUint16(&info->bits,        fp)) return 0;
    if(!ReadLittleUint32(&info->compression, fp)) return 0;
    if(!ReadLittleUint32(&info->unused0[0],  fp)) return 0;
    if(!ReadLittleUint32(&info->unused0[1],  fp)) return 0;
    if(!ReadLittleUint32(&info->unused0[2],  fp)) return 0;
    if(!ReadLittleUint32(&info->colors,      fp)) return 0;
    if(!ReadLittleUint32(&info->unused1,     fp)) return 0;

    /* We don't bother to even try to read bitmasks if they aren't needed,
     * since they won't be present in Windows 3 format bitmap files.
     */
    if(info->compression == COMPRESSION_BITFIELDS)
    {
        /* Reject Windows NT format files with bitfields, since we don't
         * support them, and their masks aren't part of the info header anyway.
         */
        if(info->info_size == BMP3_INFO_SIZE) return 0;

        if(!ReadLittleUint32(&info->masks[0], fp)) return 0;
        if(!ReadLittleUint32(&info->masks[1], fp)) return 0;
        if(!ReadLittleUint32(&info->masks[2], fp)) return 0;
        if(!ReadLittleUint32(&info->masks[3], fp)) return 0;
    }

    return 1;
}

/* Bitfields for 16- and 32-bit files.  We track the first set bit (rightmost
 * being 0) and how many bits it spans.
 */
typedef struct bitfield
{
    uint32_t start;
    uint32_t span;

} bitfield;

/* Applies a bitfield mask to a value, x.
 */
#define ApplyBitfield(x, bitfield) \
        (((x) >> (bitfield).start) & ((UINT32_C(1) << (bitfield).span) - 1))

/* Turns a single mask component into a bitfield.  Returns 0 if the bitmask was
 * invalid, or nonzero if it's ok.  Span of 0 means the bitmask was absent.
 */
static int ParseBitfield(bitfield * field, uint32_t mask)
{
    uint32_t bit;
    for(bit = 0; bit < 32 && !(mask & (UINT32_C(1) << bit)); bit++)
        ;

    if(bit >= 32)
    {
        /* Absent bitmasks are valid. */
        field->start = field->span = 0;
        return 1;
    }

    field->start = bit;
    for(; bit < 32 && (mask & (UINT32_C(1) << bit)); bit++)
        ;
    field->span = bit - field->start;

    /* If there are more set bits, there was a gap, which is invalid. */
    if(bit < 32 && (mask & ~((UINT32_C(1) << bit) - 1))) return 0;

    return 1;
}

/* A single color entry in the palette, in file order (BGR + one unused byte).
 */
typedef struct bmp_color
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t unused;

} bmp_color;

/* How many bytes in the file are occupied by a palette entry, by definition in
 * the spec (and again note that it might not be the same as
 * sizeof(bmp_color), even if we match).
 */
#define BMP_COLOR_SIZE 4

/* Reads the given number of colors from fp into the palette array.  Returns 0
 * on EOF or nonzero on success.
 */
static int ReadPalette(bmp_color * palette, int colors, FILE * fp)
{
    /* This isn't the guaranteed-fastest way to implement this, but it should
     * perform quite well in practice due to compiler optimization and stdio
     * input buffering.  It's implemented this way because of how simple the
     * code is, while avoiding undefined and implementation-defined behavior or
     * allocating any memory.  If you aren't averse to an extra allocation (or
     * using a chunk of the stack), it might be made faster while still
     * avoiding implementation-defined behavior by reading the entire palette
     * into one big buffer up front, then copying bytes into place.
     */
    int i;
    for(i = 0; i < colors; i++)
    {
        uint8_t components[BMP_COLOR_SIZE];
        if(fread(components, 1, sizeof(components), fp) != sizeof(components))
            return 0;

        palette[i].blue   = components[0];
        palette[i].green  = components[1];
        palette[i].red    = components[2];
        palette[i].unused = components[3];
    }
    return 1;
}

/* Context shared between the below functions.
 */
typedef struct read_context
{
    unsigned int   flags;         /* Flags passed to bmpread. */
    FILE         * fp;            /* File pointer. */
    bmp_header     header;        /* Bitmap file header. */
    bmp_info       info;          /* Bitmap file info. */
    uint32_t       headers_size;  /* Total size of header + info. */
    uint32_t       after_headers; /* Size of space for palette. */
    int32_t        lines;         /* How many scan lines (abs(height)). */
    size_t         file_line_len; /* How many bytes each scan line is. */
    size_t         out_channels;  /* Output color channels (3, or 4=alpha). */
    size_t         out_line_len;  /* Bytes in each output line. */
    bitfield       bitfields[4];  /* How to decode 16- and 32-bits. */
    bmp_color    * palette;       /* Enough entries for our bit depth. */
    uint8_t      * file_data;     /* A line of data in the file. */
    uint8_t      * data_out;      /* RGB(A) data output buffer. */

} read_context;

/* A sub-function to Validate() that handles the bitfields.  Returns 0 on
 * invalid bitfields or nonzero on success.  Note that we don't treat odd
 * bitmasks such as R8G8 or A1G1B1 as invalid, even though they may not load in
 * most other loaders.
 */
static int ValidateBitfields(read_context * p_ctx)
{
    bitfield * bf = p_ctx->bitfields;

    uint32_t total_mask = 0;
    bitfield total_field;

    int i;

    if(p_ctx->info.compression != COMPRESSION_BITFIELDS)
        return 1;

    for(i = 0; i < 4; i++)
    {
        /* No overlapping masks. */
        if(total_mask & p_ctx->info.masks[i]) return 0;
        total_mask |= p_ctx->info.masks[i];

        if(!ParseBitfield(&bf[i], p_ctx->info.masks[i])) return 0;

        /* Make sure we fit in our bit size. */
        if(bf[i].start + bf[i].span > p_ctx->info.bits) return 0;
    }

    if(!total_mask) return 0;

    /* Check for contiguous-ity between fields, too. */
    if(!ParseBitfield(&total_field, total_mask)) return 0;

    return 1;
}

/* A sub-function to Validate() that handles the palette.  Returns 0 on EOF or
 * invalid palette, or nonzero on success.
 */
static int ValidateAndReadPalette(read_context * p_ctx)
{
    uint32_t colors = UINT32_C(1) << p_ctx->info.bits;
    uint32_t file_colors = p_ctx->info.colors;

    if(p_ctx->info.bits > 8)
        return 1;

    if(file_colors > colors) return 0;
    if(!file_colors)
        file_colors = colors;

    /* Make sure we actually have space in the file for all the colors. */
    if(p_ctx->after_headers / BMP_COLOR_SIZE < file_colors) return 0;

    /* We always allocate a full palette even if the file only claims to
     * contain a smaller number, so we don't have to check for out of bound
     * color lookups.  Not sure what the desired behavior is, but loading the
     * image anyway and treating OOB colors as black seems ok to me.  0-fill so
     * lookups beyond the file's palette get set to black.
     */
    if(!(p_ctx->palette = (bmp_color *)
         calloc(colors, sizeof(p_ctx->palette[0])))) return 0;

    if(!CanMakeLong(p_ctx->headers_size))                    return 0;
    if(fseek(p_ctx->fp, p_ctx->headers_size, SEEK_SET))      return 0;
    if(!ReadPalette(p_ctx->palette, file_colors, p_ctx->fp)) return 0;

    return 1;
}

/* Returns whether a non-negative integer is a power of 2.
 */
static int IsPowerOf2(uint32_t x)
{
    while(x)
    {
        /* When we find a bit, return whether no other bits are set. */
        if(x & 1)
            return !(x & ~UINT32_C(1));
        x = x >> 1;
    }

    /* 0, the only value for x which lands us here, isn't a power of 2. */
    return 0;
}

/* Returns the byte length of a scan line padded as necessary to be divisible
 * by four.  For example, 3 pixels wide at 24 bpp would yield 12 (3 pixels * 3
 * bytes each = 9 bytes, padded by 3 to the next multiple of 4).  bpp is *bits*
 * per pixel, not bytes.  Returns 0 in case of overflow.
 */
static size_t GetLineLength(size_t width, size_t bpp)
{
    size_t bits = width * bpp;
    size_t pad_bits = (32 - (bits & 0x1f)) & 0x1f; /* x & 0x1f == x % 32 */

    /* Check for overflow, in both the above multiplication and the below
     * addition.  It's well defined to do this in any order relative to the
     * operations themselves (since size_t is unsigned), so we combine the
     * checks into one if.  bpp has been checked for being nonzero elsewhere.
     */
    if(!CanMultiply(width, bpp) || !CanAdd(bits, pad_bits)) return 0;

    /* Convert to bytes. */
    return (bits + pad_bits) / 8;
}

/* Reads and validates the bitmap header metadata from the context's file
 * object.  Assumes the file pointer is at the start of the file.  Returns 1 if
 * ok or 0 if error or invalid file.
 */
static int Validate(read_context * p_ctx)
{
    if(!ReadHeader(&p_ctx->header, p_ctx->fp)) return 0;
    if(!ReadInfo(  &p_ctx->info,   p_ctx->fp)) return 0;

    if(p_ctx->info.info_size > UINT32_MAX - BMP_HEADER_SIZE) return 0;
    p_ctx->headers_size = BMP_HEADER_SIZE + p_ctx->info.info_size;

    if(p_ctx->header.data_offset < p_ctx->headers_size) return 0;
    p_ctx->after_headers = p_ctx->header.data_offset - p_ctx->headers_size;

    if(p_ctx->info.width <= 0 || p_ctx->info.height == 0) return 0;

    if(!CanMakeSizeT(p_ctx->info.width))  return 0;
    if(!CanNegate(   p_ctx->info.height)) return 0;
    p_ctx->lines = ((p_ctx->info.height < 0) ?
                    -p_ctx->info.height :
                     p_ctx->info.height);

    if(!(p_ctx->flags & BMPREAD_ANY_SIZE))
    {
        /* Both of these values have just been checked against being negative,
         * and thus it's safe to pass them on as uint32_t.
         */
        if(!IsPowerOf2(p_ctx->info.width)) return 0;
        if(!IsPowerOf2(p_ctx->lines))      return 0;
    }

    switch(p_ctx->info.compression)
    {
        case COMPRESSION_NONE:
            if(p_ctx->info.bits != 1 && p_ctx->info.bits != 4 &&
               p_ctx->info.bits != 8 && p_ctx->info.bits != 24) return 0;
            break;

        case COMPRESSION_BITFIELDS:
            if(p_ctx->info.bits != 16 && p_ctx->info.bits != 32) return 0;
            break;

        default: /* No compression supported yet (TODO: handle RLE). */
            return 0;
    }

    p_ctx->file_line_len = GetLineLength(p_ctx->info.width, p_ctx->info.bits);
    if(p_ctx->file_line_len == 0) return 0;

    p_ctx->out_channels = ((p_ctx->flags & BMPREAD_ALPHA) ? 4 : 3);

    /* This check happens outside the following if, where it would seem to
     * belong, because we make the same computation again in the future.
     */
    if(!CanMultiply(p_ctx->info.width, p_ctx->out_channels)) return 0;

    if(p_ctx->flags & BMPREAD_BYTE_ALIGN)
        p_ctx->out_line_len = (size_t)p_ctx->info.width * p_ctx->out_channels;
    else
    {
        p_ctx->out_line_len = GetLineLength(p_ctx->info.width,
                                            p_ctx->out_channels * 8);
        if(p_ctx->out_line_len == 0) return 0;
    }

    if(!ValidateBitfields(p_ctx))      return 0;
    if(!ValidateAndReadPalette(p_ctx)) return 0;

    /* Set things up for decoding. */
    if(!(p_ctx->file_data = (uint8_t *)malloc(p_ctx->file_line_len))) return 0;

    if(!CanMakeSizeT(p_ctx->lines))                           return 0;
    if(!CanMultiply( p_ctx->lines, p_ctx->out_line_len))      return 0;
    if(!(p_ctx->data_out = (uint8_t *)
         malloc((size_t)p_ctx->lines * p_ctx->out_line_len))) return 0;

    return 1;
}

/* Evenly distribute a value that spans a given number of bits into 8 bits.
 */
static uint32_t Make8Bits(uint32_t value, uint32_t bitspan)
{
    uint32_t output = 0;

    if(bitspan == 8)
        return value;
    if(bitspan > 8)
        return value >> (bitspan - 8);

    value <<= (8 - bitspan); /* Shift it up into the most significant bits. */
    while(value)
    {
        /* Repeat the bit pattern down into the least significant bits.  This
         * gives an even distribution when extrapolating from [0, 2^bitspan-1]
         * into [0, 2^8-1], and avoids both floating point and awkward integer
         * multiplication.  Unfortunately, because we don't enforce a whitelist
         * of bit patterns we support and can hard-code for, it necessitates a
         * loop.  I believe this is a fairly efficient way to express the idea,
         * but it'd still be nice if the compiler could optimize this whole
         * function heavily, since it's called in a tight decode loop.
         */
        output |= value;
        value >>= bitspan;
    }

    return output;
}

/* Reads four bytes out of a memory buffer and converts it to a uint32_t.
 */
#define LoadLittleUint32(buf) (((uint32_t)(buf)[0]      ) + \
                               ((uint32_t)(buf)[1] <<  8) + \
                               ((uint32_t)(buf)[2] << 16) + \
                               ((uint32_t)(buf)[3] << 24))

/* Decodes 32-bit bitmap data by applying bitmasks.  The 16- and 32-bit
 * decoders could be made more efficient by whitelisting supported bit patterns
 * ahead of time and special-casing their decoding here, but this allows us to
 * support more bitmask patterns, and shouldn't be *too* inefficient in any
 * case.
 *
 * Takes a pointer to an output buffer scan line (p_out), a pointer to the end
 * of the *pixel data* of this scan line (p_out_end), a pointer to the source
 * scan line of file data (p_file), and our context.
 */
static void Decode32(uint8_t * p_out,
                     const uint8_t * p_out_end,
                     const uint8_t * p_file,
                     const read_context * p_ctx)
{
    const bitfield * bf = p_ctx->bitfields;

    while(p_out < p_out_end)
    {
        uint32_t value = LoadLittleUint32(p_file);

        *p_out++ = Make8Bits(ApplyBitfield(value, bf[0]), bf[0].span);
        *p_out++ = Make8Bits(ApplyBitfield(value, bf[1]), bf[1].span);
        *p_out++ = Make8Bits(ApplyBitfield(value, bf[2]), bf[2].span);
        if(p_ctx->out_channels == 4)
        {
            if(bf[3].span)
                *p_out++ = Make8Bits(ApplyBitfield(value, bf[3]), bf[3].span);
            else
                *p_out++ = BMPREAD_DEFAULT_ALPHA;
        }

        p_file += 4;
    }
}

/* Decodes 24-bit bitmap data--basically just swaps the order of color
 * components.
 */
static void Decode24(uint8_t * p_out,
                     const uint8_t * p_out_end,
                     const uint8_t * p_file,
                     const read_context * p_ctx)
{
    while(p_out < p_out_end)
    {
        *p_out++ = *(p_file + 2);
        *p_out++ = *(p_file + 1);
        *p_out++ = *(p_file    );
        if(p_ctx->out_channels == 4)
            *p_out++ = BMPREAD_DEFAULT_ALPHA;

        p_file += 3;
    }
}

/* Reads two bytes out of a memory buffer and converts it to a uint16_t.
 */
#define LoadLittleUint16(buf) (((uint16_t)(buf)[0]     ) + \
                               ((uint16_t)(buf)[1] << 8))

/* Decodes 16-bit bitmap data by applying bitmasks.
 */
static void Decode16(uint8_t * p_out,
                     const uint8_t * p_out_end,
                     const uint8_t * p_file,
                     const read_context * p_ctx)
{
    const bitfield * bf = p_ctx->bitfields;

    while(p_out < p_out_end)
    {
        uint16_t value = LoadLittleUint16(p_file);

        *p_out++ = Make8Bits(ApplyBitfield(value, bf[0]), bf[0].span);
        *p_out++ = Make8Bits(ApplyBitfield(value, bf[1]), bf[1].span);
        *p_out++ = Make8Bits(ApplyBitfield(value, bf[2]), bf[2].span);
        if(p_ctx->out_channels == 4)
        {
            if(bf[3].span)
                *p_out++ = Make8Bits(ApplyBitfield(value, bf[3]), bf[3].span);
            else
                *p_out++ = BMPREAD_DEFAULT_ALPHA;
        }

        p_file += 2;
    }
}

/* Decodes 8-bit bitmap data by looking colors up in the palette.
 */
static void Decode8(uint8_t * p_out,
                    const uint8_t * p_out_end,
                    const uint8_t * p_file,
                    const read_context * p_ctx)
{
    while(p_out < p_out_end) {
        *p_out++ = p_ctx->palette[*p_file].red;
        *p_out++ = p_ctx->palette[*p_file].green;
        *p_out++ = p_ctx->palette[*p_file].blue;
        if(p_ctx->out_channels == 4)
            *p_out++ = BMPREAD_DEFAULT_ALPHA;

        p_file++;
    }
}

/* Decodes 4-bit bitmap data by looking colors up in the palette.
 */
static void Decode4(uint8_t * p_out,
                    const uint8_t * p_out_end,
                    const uint8_t * p_file,
                    const read_context * p_ctx)
{
    while(p_out < p_out_end)
    {
        unsigned int lookup = (*p_file & 0xf0U) >> 4;

        *p_out++ = p_ctx->palette[lookup].red;
        *p_out++ = p_ctx->palette[lookup].green;
        *p_out++ = p_ctx->palette[lookup].blue;
        if(p_ctx->out_channels == 4)
            *p_out++ = BMPREAD_DEFAULT_ALPHA;

        if(p_out < p_out_end)
        {
            lookup = *p_file++ & 0x0fU;

            *p_out++ = p_ctx->palette[lookup].red;
            *p_out++ = p_ctx->palette[lookup].green;
            *p_out++ = p_ctx->palette[lookup].blue;
            if(p_ctx->out_channels == 4)
                *p_out++ = BMPREAD_DEFAULT_ALPHA;
        }
    }
}

/* Decodes 1-bit bitmap data by looking colors up in the two-color palette.
 */
static void Decode1(uint8_t * p_out,
                    const uint8_t * p_out_end,
                    const uint8_t * p_file,
                    const read_context * p_ctx)
{
    while(p_out < p_out_end)
    {
        unsigned int bit;
        for(bit = 0; bit < 8 && p_out < p_out_end; bit++)
        {
            unsigned int lookup = (*p_file >> (7 - bit)) & 1;

            *p_out++ = p_ctx->palette[lookup].red;
            *p_out++ = p_ctx->palette[lookup].green;
            *p_out++ = p_ctx->palette[lookup].blue;
            if(p_ctx->out_channels == 4)
                *p_out++ = BMPREAD_DEFAULT_ALPHA;
        }

        p_file++;
    }
}

/* Selects an above decoder and runs it for each scan line of the file.
 * Returns 0 if there's an error or 1 if it's gravy.
 */
static int Decode(read_context * p_ctx)
{
    void (* decoder)(uint8_t *, const uint8_t *, const uint8_t *,
                     const read_context *);

    uint8_t * p_out;      /* Pointer to current scan line in output buffer. */
    uint8_t * p_out_end;  /* End marker for output buffer. */
    uint8_t * p_line_end; /* Pointer to end of current scan line in output. */

    /* out_inc is an incrementor for p_out to advance it one scan line.  I'm
     * not exactly sure what the correct type for it would be, perhaps ssize_t,
     * but that's not C standard.  I went with ptrdiff_t because its value
     * will be equivalent to the difference between two pointers, whether it
     * was derived that way or not.
     */
    ptrdiff_t out_inc;

    /* Double check this won't overflow.  Who knows, man. */
#if SIZE_MAX > PTRDIFF_MAX
    if(p_ctx->out_line_len > PTRDIFF_MAX) return 0;
#endif
    out_inc = p_ctx->out_line_len;

    if(!(p_ctx->info.height < 0) == !(p_ctx->flags & BMPREAD_TOP_DOWN))
    {
        /* We're keeping scan lines in order.  These and subsequent operations
         * have all been checked earlier.
         */
        p_out     = p_ctx->data_out;
        p_out_end = p_ctx->data_out +
                    ((size_t)p_ctx->lines * p_ctx->out_line_len);
    }
    else /* We're reversing scan lines. */
    {
        /* TODO: I'm not 100% sure about the legality, purely C spec-wise, of
         * this subtraction.
         */
        p_out_end = p_ctx->data_out - p_ctx->out_line_len;
        p_out     = p_ctx->data_out +
                    (((size_t)p_ctx->lines - 1) * p_ctx->out_line_len);

        /* Always safe, given two's complement, since it was positive. */
        out_inc = -out_inc;
    }

    p_line_end = p_out + (size_t)p_ctx->info.width * p_ctx->out_channels;

    switch(p_ctx->info.bits)
    {
        case 32: decoder = Decode32; break;
        case 24: decoder = Decode24; break;
        case 16: decoder = Decode16; break;
        case 8:  decoder = Decode8;  break;
        case 4:  decoder = Decode4;  break;
        case 1:  decoder = Decode1;  break;
        default: return 0;
    }

    if(!CanMakeLong(p_ctx->header.data_offset))               return 0;
    if(fseek(p_ctx->fp, p_ctx->header.data_offset, SEEK_SET)) return 0;

    while(p_out != p_out_end &&
          fread(p_ctx->file_data, 1, p_ctx->file_line_len, p_ctx->fp) ==
          p_ctx->file_line_len)
    {
        decoder(p_out, p_line_end, p_ctx->file_data, p_ctx);

        p_out      += out_inc;
        p_line_end += out_inc;
    }

    return (p_out == p_out_end);
}

/* Frees resources allocated by various functions along the way.  Only frees
 * data_out if !leave_data_out (if the bitmap loads successfully, you want the
 * data to remain until THEY free it).
 */
static void FreeContext(read_context * p_ctx, int leave_data_out)
{
    if(p_ctx->fp)
        fclose(p_ctx->fp);
    if(p_ctx->palette)
        free(p_ctx->palette);
    if(p_ctx->file_data)
        free(p_ctx->file_data);

    if(!leave_data_out && p_ctx->data_out)
        free(p_ctx->data_out);
}

int bmpread(const char * bmp_file, unsigned int flags, bmpread_t * p_bmp_out)
{
    int success = 0;

    read_context ctx;
    memset(&ctx, 0, sizeof(ctx));

    do
    {
        if(!bmp_file)  break;
        if(!p_bmp_out) break;
        memset(p_bmp_out, 0, sizeof(*p_bmp_out));

        ctx.flags = flags;

        if(!(ctx.fp = fopen(bmp_file, "rb"))) break;
        if(!Validate(&ctx))                   break;
        if(!Decode(&ctx))                     break;

        /* Finally, make sure we can stuff these into ints.  I feel like this
         * is slightly justified by how it keeps the header definition dead
         * simple (including, well, no #includes).  I suppose this could also
         * be done way earlier and maybe save some disk reads, but I like
         * keeping the check with the code it's checking.
         */
#if INT32_MAX > INT_MAX
        if(ctx.info.width > INT_MAX) break;
        if(ctx.lines      > INT_MAX) break;
#endif

        p_bmp_out->width  = ctx.info.width;
        p_bmp_out->height = ctx.lines;
        p_bmp_out->flags  = ctx.flags;
        p_bmp_out->data   = ctx.data_out;

        success = 1;
    } while(0);

    FreeContext(&ctx, success);

    return success;
}

void bmpread_free(bmpread_t * p_bmp)
{
    if(p_bmp)
    {
        if(p_bmp->data)
            free(p_bmp->data);

        memset(p_bmp, 0, sizeof(*p_bmp));
    }
}
