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


/* bmpread.h
 * version 3.0
 * 2018-02-02
 */


#ifndef __bmpread_h__
#define __bmpread_h__

#ifdef __cplusplus
extern "C"
{
#endif


/* Flags for bmpread() and bmpread_t (see below).  Combine with bitwise OR.
 */

/* Output data as top line first (default is bottom line first). */
#define BMPREAD_TOP_DOWN 1u

/* Don't pad lines to span a multiple of four bytes (default does pad). */
#define BMPREAD_BYTE_ALIGN 2u

/* Allow loading of any size bitmap (default is bitmaps must be 2^n x 2^m). */
#define BMPREAD_ANY_SIZE 4u

/* Load and output an alpha channel (default is just color channels). */
#define BMPREAD_ALPHA 8u


/* The struct filled by bmpread().  Holds information about the image's pixels.
 */
typedef struct bmpread_t
{
    int width;  /* Width in pixels. */
    int height; /* Height in pixels. */

    /* BMPREAD_* flags, defined above, combined with bitwise OR, that affect
     * the format of data.  These are set to the flags passed to bmpread().
     */
    unsigned int flags;

    /* A buffer holding the pixel data of the image.
     *
     * By default, each pixel spans three bytes: the red, green, and blue color
     * components in that order.  However, with BMPREAD_ALPHA set in flags,
     * each pixel spans four bytes: the red, green, blue, and alpha components
     * in that order.
     *
     * Pixels are ordered left to right sequentially.  By default, the bottom
     * line comes first, proceeding upward.  However, with BMPREAD_TOP_DOWN set
     * in flags, the top line comes first, proceeding downward instead.
     *
     * Lines by default must span a multiple of four bytes.  If the image width
     * and pixel span don't yield a multiple of four (a non-issue for
     * BMPREAD_ALPHA with four bytes per pixel), the end of each line is padded
     * with up to three unused bytes to meet the requirement.  For example,
     * each line of an image three pixels wide, loaded without BMPREAD_ALPHA,
     * will span 12 bytes (3 pixels * 3 (RGB) channels per pixel = 9, padded
     * with 3 bytes up to the next multiple of 4).  However, this behavior is
     * disabled with BMPREAD_BYTE_ALIGN set in flags, in which case all lines
     * span exactly width * pixel_span bytes.
     */
    unsigned char * data;

} bmpread_t;


/* Loads the specified bitmap file from disk and fills out a bmpread_t struct
 * with data about it.
 *
 * Inputs:
 * bmp_file - The filename of the bitmap file to load.
 * flags - Any BMPREAD_* flags, defined above, combined with bitwise OR.
 *         Specify 0 (or BMPREAD_ALPHA if you want an alpha channel) for
 *         standard, OpenGL compliant behavior.
 * p_bmp_out - Pointer to a bmpread_t struct to fill with information.  Its
 *             contents on input are ignored.  Must be freed with
 *             bmpread_free() when no longer needed.
 *
 * Returns:
 * 0 if there's an error (file doesn't exist or is invalid, i/o error, etc.),
 * or nonzero if the file loaded ok.
 *
 * Notes:
 * The file must be a Windows 3 (not NT) or higher format bitmap file with any
 * valid bit depth (1, 4, 8, 16, 24, or 32), and must not be compressed (no
 * RLE).
 *
 * Default behavior is for bmpread() to return data in a format directly usable
 * by OpenGL texture functions, e.g. glTexImage2D, format GL_RGB (or GL_RGBA if
 * BMPREAD_ALPHA is in flags), type GL_UNSIGNED_BYTE.  This implies a few
 * oddities:
 *  - Lines are ordered bottom-first.  To return data starting with the top
 *    line like you might otherwise expect, pass BMPREAD_TOP_DOWN in flags.
 *  - Lines are padded to span a multiple of four bytes.  To return data with
 *    no padding, pass BMPREAD_BYTE_ALIGN in flags.
 *  - Images with a width or height that isn't a power of 2 will fail to load.
 *    To allow loading images of any size, pass BMPREAD_ANY_SIZE in flags.
 * Note that passing any of these flags may cause the output to be unusable as
 * an OpenGL texture, which may or may not matter to you.
 *
 * Most bitmap files can't include an alpha channel, so the default behavior is
 * to ignore any alpha values present in the file.  Pass BMPREAD_ALPHA in flags
 * to capture alpha values from the file; in case of an absent alpha channel,
 * alpha values are output as 255 (this can be changed by redefining
 * BMPREAD_DEFAULT_ALPHA in bmpread.c).  This allows fully loading 16- and
 * 32-bit bitmaps, which *can* include an alpha channel.
 */
int bmpread(const char * bmp_file, unsigned int flags, bmpread_t * p_bmp_out);


/* Frees memory allocated during bmpread().  Call bmpread_free() when you are
 * done using the bmpread_t struct (e.g. after you have passed the data on to
 * OpenGL).
 *
 * Inputs:
 * p_bmp - The pointer you previously passed to bmpread().
 *
 * Returns:
 * void
 */
void bmpread_free(bmpread_t * p_bmp);


#ifdef __cplusplus
}
#endif

#endif
