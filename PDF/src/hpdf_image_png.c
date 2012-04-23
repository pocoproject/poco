/*
 * << Haru Free PDF Library 2.0.3 >> -- hpdf_image.c
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#include "hpdf_conf.h"
#include "hpdf_utils.h"
#include "hpdf_image.h"

#ifndef HPDF_NOPNGLIB
#include <png.h>

static void
PngErrorFunc  (png_structp       png_ptr,
               const char  *msg);


static void
PngReadFunc  (png_structp   png_ptr,
              png_bytep     data,
              png_uint_32   length)
{
    HPDF_UINT len = length;
    HPDF_Stream stream = (HPDF_Stream)png_get_io_ptr (png_ptr);

    HPDF_Stream_Read (stream, data, &len);
}


static HPDF_STATUS
LoadPngData  (HPDF_Dict     image,
              HPDF_Stream   png_data,
              HPDF_BOOL     delayed_loading);


static void
PngErrorFunc  (png_structp       png_ptr,
               const char  *msg);


static HPDF_STATUS
ReadPngData_Interlaced  (HPDF_Dict    image,
                         png_structp  png_ptr,
                         png_infop    info_ptr);


static HPDF_STATUS
ReadPngData  (HPDF_Dict    image,
              png_structp  png_ptr,
              png_infop    info_ptr);


static HPDF_STATUS
CreatePallet (HPDF_Dict    image,
              png_structp  png_ptr,
              png_infop    info_ptr);


static HPDF_STATUS
PngBeforeWrite  (HPDF_Dict obj);


static HPDF_STATUS
PngAfterWrite  (HPDF_Dict obj);


/*---------------------------------------------------------------------------*/

static void
PngErrorFunc  (png_structp       png_ptr,
               const char  *msg)
{
    char error_number[16];
    HPDF_UINT i;
    HPDF_STATUS detail_no;
    HPDF_Error error;

    /* pick out error-number from error message */
    HPDF_MemSet (error_number, 0, 16);

     for (i = 0; i < 15; i++) {
         error_number[i] = *(msg + i);
         if (*(msg + i + 1) == ' ')
             break;
     }

     error = (HPDF_Error)png_get_error_ptr (png_ptr);
     detail_no = (HPDF_STATUS)HPDF_AToI (error_number);
     HPDF_SetError (error, HPDF_LIBPNG_ERROR, detail_no);
}


static HPDF_STATUS
ReadPngData_Interlaced  (HPDF_Dict    image,
                         png_structp  png_ptr,
                         png_infop    info_ptr)
{
    png_uint_32 len = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep* row_pointers = HPDF_GetMem (image->mmgr,
                info_ptr->height * sizeof (png_bytep));

    if (row_pointers) {
        HPDF_UINT i;

        HPDF_MemSet (row_pointers, 0, info_ptr->height * sizeof (png_bytep));
        for (i = 0; i < (HPDF_UINT)info_ptr->height; i++) {
            row_pointers[i] = HPDF_GetMem (image->mmgr, len);

            if (image->error->error_no != HPDF_OK)
                break;
        }

        if (image->error->error_no == HPDF_OK) {
/*      } else {                                              remove this line */
            png_read_image(png_ptr, row_pointers);
/*            if (image->error->error_no != HPDF_OK) {        remove this line */
            if (image->error->error_no == HPDF_OK) {       /* add this line */
                for (i = 0; i < (HPDF_UINT)info_ptr->height; i++) {
                    if (HPDF_Stream_Write (image->stream, row_pointers[i], len) !=
                            HPDF_OK)
                        break;
                }
            }
        }

        /* clean up */
        for (i = 0; i < (HPDF_UINT)info_ptr->height; i++) {
            HPDF_FreeMem (image->mmgr, row_pointers[i]);
/*          i--                                               remove this line */
        }

        HPDF_FreeMem (image->mmgr, row_pointers);
    }

    return image->error->error_no;
}

static HPDF_STATUS
ReadPngData  (HPDF_Dict    image,
              png_structp  png_ptr,
              png_infop    info_ptr)
{
    png_uint_32 len = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep buf_ptr = HPDF_GetMem (image->mmgr, len);

    if (buf_ptr) {
        HPDF_UINT i;

        for (i = 0; i < (HPDF_UINT)info_ptr->height; i++) {
            png_read_rows(png_ptr, (png_byte**)&buf_ptr, NULL, 1);
            if (image->error->error_no != HPDF_OK)
                break;

            if (HPDF_Stream_Write (image->stream, buf_ptr, len) != HPDF_OK)
                break;
        }

        HPDF_FreeMem (image->mmgr, buf_ptr);
    }

    return image->error->error_no;
}


static HPDF_STATUS
CreatePallet (HPDF_Dict    image,
              png_structp  png_ptr,
              png_infop    info_ptr)
{
    HPDF_INT num_pl = 0;
    png_color *src_pl = NULL;
    HPDF_BYTE *ppallet;
    HPDF_BYTE *p;
    HPDF_UINT i;
    HPDF_Array array;

    /* png_get_PLTE does not call PngErrorFunc even if it failed.
     * so we call HPDF_Set_Error to set error-code.
     */
    if (png_get_PLTE(png_ptr, info_ptr, (png_color**)&src_pl, &num_pl) !=
            PNG_INFO_PLTE)
        return HPDF_SetError (image->error, HPDF_LIBPNG_ERROR,
                    HPDF_CANNOT_GET_PALLET);


    /* make a pallet array for indexed image. */
    ppallet = HPDF_GetMem (image->mmgr, num_pl * 3);
    if (!ppallet)
        return image->error->error_no;

    p = ppallet;
    for (i = 0; i < num_pl; i++, src_pl++) {
        *p++ = src_pl->red;
        *p++ = src_pl->green;
        *p++ = src_pl->blue;
    }

    array = HPDF_Array_New (image->mmgr);
    if (array) {
        HPDF_Binary b;

        HPDF_Dict_Add (image, "ColorSpace", array);

        HPDF_Array_AddName (array, "Indexed");
        HPDF_Array_AddName (array, "DeviceRGB");
        HPDF_Array_AddNumber (array, num_pl - 1);

        b = HPDF_Binary_New (image->mmgr, ppallet, num_pl * 3);
        if (b)
            HPDF_Array_Add (array, b);
    }

    HPDF_FreeMem (image->mmgr, ppallet);

    return image->error->error_no;
}

#define HPDF_PNG_BYTES_TO_CHECK 8

HPDF_Image
HPDF_Image_LoadPngImage  (HPDF_MMgr        mmgr,
                          HPDF_Stream      png_data,
                          HPDF_Xref        xref,
                          HPDF_BOOL        delayed_loading)
{
    HPDF_STATUS ret;
    HPDF_Dict image;
    png_byte header[HPDF_PNG_BYTES_TO_CHECK];
    HPDF_UINT len = HPDF_PNG_BYTES_TO_CHECK;

    HPDF_PTRACE ((" HPDF_Image_LoadPngImage\n"));

    HPDF_MemSet (header, 0x00, HPDF_PNG_BYTES_TO_CHECK);
    ret = HPDF_Stream_Read (png_data, header, &len);
    if (ret != HPDF_OK ||
            png_sig_cmp (header, (png_size_t)0, HPDF_PNG_BYTES_TO_CHECK)) {
        HPDF_SetError (mmgr->error, HPDF_INVALID_PNG_IMAGE, 0);
        return NULL;
    }

    image = HPDF_DictStream_New (mmgr, xref);
    if (!image)
        return NULL;

    image->header.obj_class |= HPDF_OSUBCLASS_XOBJECT;
    ret += HPDF_Dict_AddName (image, "Type", "XObject");
    ret += HPDF_Dict_AddName (image, "Subtype", "Image");
    if (ret != HPDF_OK)
        return NULL;

    if (LoadPngData (image, png_data, delayed_loading) != HPDF_OK)
        return NULL;

    return image;
}


static HPDF_STATUS
LoadPngData  (HPDF_Dict     image,
              HPDF_Stream   png_data,
              HPDF_BOOL     delayed_loading)

{
    HPDF_STATUS ret;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    HPDF_PTRACE ((" HPDF_Image_LoadPngImage\n"));

    /* create read_struct. */
    png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING,
        image->error, PngErrorFunc, PngErrorFunc);

    if (png_ptr == NULL) {
        HPDF_SetError (image->error, HPDF_FAILD_TO_ALLOC_MEM, 0);
        return HPDF_FAILD_TO_ALLOC_MEM;
    }

    /* create info-struct */
    info_ptr = png_create_info_struct (png_ptr);

    if (info_ptr == NULL) {
        HPDF_SetError (image->error, HPDF_FAILD_TO_ALLOC_MEM, 0);
        goto Exit;
    }

    png_set_sig_bytes (png_ptr, HPDF_PNG_BYTES_TO_CHECK);
    png_set_read_fn (png_ptr, (void *)png_data, (png_rw_ptr)&PngReadFunc);

    /* reading info structure. */
    png_read_info(png_ptr, info_ptr);
    if (image->error->error_no != HPDF_OK)
        goto Exit;

    /* 16bit image and alpha color type are not supported. */
    if (info_ptr->bit_depth == 16)
        png_set_strip_16(png_ptr);

    if (PNG_COLOR_MASK_ALPHA & info_ptr->color_type) {
        /* png image with alpha chanel is not supported. */
        png_set_strip_alpha(png_ptr);
    }

    png_read_update_info(png_ptr, info_ptr);
    if (image->error->error_no  != HPDF_OK)
        goto Exit;

    /* if the image has color palette, copy the pallet of the image to
     * create color map.
     */
    if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
        ret = CreatePallet(image, png_ptr, info_ptr);
    else if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY)
        ret = HPDF_Dict_AddName (image, "ColorSpace", "DeviceGray");
    else
        ret = HPDF_Dict_AddName (image, "ColorSpace", "DeviceRGB");

    if (ret != HPDF_OK)
        goto Exit;

    /* read image-data
     * if the image is interlaced, read whole image at once.
     * if delayed_loading is HPDF_TRUE, the data does not load this phase.
     */
    if (delayed_loading) {
        image->before_write_fn = PngBeforeWrite;
        image->after_write_fn = PngAfterWrite;
    } else {
        if (png_get_interlace_type(png_ptr, info_ptr) != PNG_INTERLACE_NONE)
            ret = ReadPngData_Interlaced(image, png_ptr, info_ptr);
        else
            ret = ReadPngData(image, png_ptr, info_ptr);

        if (ret != HPDF_OK)
            goto Exit;
    }

    /* setting the info of the image. */
    if (HPDF_Dict_AddNumber (image, "Width", (HPDF_UINT)info_ptr->width)
        != HPDF_OK)
        goto Exit;

    if (HPDF_Dict_AddNumber (image, "Height", (HPDF_UINT)info_ptr->height)
        != HPDF_OK)
        goto Exit;

    if (HPDF_Dict_AddNumber (image, "BitsPerComponent",
                (HPDF_UINT)info_ptr->bit_depth) != HPDF_OK)
        goto Exit;

    /* clean up */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return HPDF_OK;

Exit:
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return image->error->error_no;
}


static HPDF_STATUS
PngBeforeWrite  (HPDF_Dict obj)
{
    HPDF_STATUS ret;
    png_byte header[HPDF_PNG_BYTES_TO_CHECK];
    HPDF_UINT len = HPDF_PNG_BYTES_TO_CHECK;
    HPDF_Stream png_data;
    HPDF_String s;

    HPDF_PTRACE ((" PngBeforeWrite\n"));

    HPDF_MemStream_FreeData(obj->stream);

    s = HPDF_Dict_GetItem (obj, "_FILE_NAME", HPDF_OCLASS_STRING);
    if (!s)
        return HPDF_SetError (obj->error, HPDF_MISSING_FILE_NAME_ENTRY, 0);

    png_data = HPDF_FileReader_New (obj->mmgr, s->value);
    if (!HPDF_Stream_Validate (png_data))
        return obj->error->error_no;

    HPDF_MemSet (header, 0x00, HPDF_PNG_BYTES_TO_CHECK);
    ret = HPDF_Stream_Read (png_data, header, &len);
    if (ret != HPDF_OK ||
            png_sig_cmp (header, (png_size_t)0, HPDF_PNG_BYTES_TO_CHECK)) {
        HPDF_Stream_Free(png_data);
        return HPDF_SetError (obj->error, HPDF_INVALID_PNG_IMAGE, 0);
    }

    if ((ret = LoadPngData (obj, png_data, HPDF_FALSE)) != HPDF_OK) {
        HPDF_Stream_Free(png_data);
        return ret;
    }

    HPDF_Stream_Free(png_data);

    return HPDF_OK;
}


static HPDF_STATUS
PngAfterWrite  (HPDF_Dict obj)
{
   HPDF_PTRACE ((" PngAfterWrite\n"));

   HPDF_MemStream_FreeData(obj->stream);

   return HPDF_OK;
}


#endif /* HPDF_NOPNGLIB */

