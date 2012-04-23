/*
 * << Haru Free PDF Library 2.0.3 >> -- hpdf_doc_png.c
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
#include "hpdf.h"
#include "hpdf_image.h"


static HPDF_Image
LoadPngImageFromStream (HPDF_Doc      pdf,
                        HPDF_Stream   imagedata,
                        HPDF_BOOL     delayed_loading);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadPngImageFromFile  (HPDF_Doc     pdf,
                            const char  *filename)
{
    HPDF_Stream imagedata;
    HPDF_Image image;

    HPDF_PTRACE ((" HPDF_LoadPngImageFromFile\n"));

    if (!HPDF_HasDoc (pdf))
        return NULL;

    /* create file stream */
    imagedata = HPDF_FileReader_New (pdf->mmgr, filename);

    if (HPDF_Stream_Validate (imagedata))
        image = LoadPngImageFromStream (pdf, imagedata, HPDF_FALSE);
    else
        image = NULL;

    /* destroy file stream */
    if (imagedata)
        HPDF_Stream_Free (imagedata);

    if (!image)
        HPDF_CheckError (&pdf->error);

    return image;
}

/* delaied loading version of HPDF_LoadPngImageFromFile */
HPDF_EXPORT(HPDF_Image)
HPDF_LoadPngImageFromFile2  (HPDF_Doc     pdf,
                             const char  *filename)
{
    HPDF_Stream imagedata;
    HPDF_Image image;
    HPDF_String fname;

    HPDF_PTRACE ((" HPDF_LoadPngImageFromFile\n"));

    if (!HPDF_HasDoc (pdf))
        return NULL;

    /* check whether file name is valid or not. */
    imagedata = HPDF_FileReader_New (pdf->mmgr, filename);

    if (HPDF_Stream_Validate (imagedata))
        image = LoadPngImageFromStream (pdf, imagedata, HPDF_TRUE);
    else
        image = NULL;

    /* destroy file stream */
    if (imagedata)
        HPDF_Stream_Free (imagedata);

    if (!image)
        HPDF_CheckError (&pdf->error);

    /* add file-name to image dictionary as a hidden entry.
     * it is used when the image data is needed.
     */
    fname = HPDF_String_New (pdf->mmgr, filename, NULL);
    if (!fname) {
        HPDF_CheckError (&pdf->error);
        return NULL;
    }

    fname->header.obj_id |= HPDF_OTYPE_HIDDEN;

    if ((HPDF_Dict_Add (image, "_FILE_NAME", fname)) != HPDF_OK) {
        HPDF_CheckError (&pdf->error);
        return NULL;
    }

    return image;
}

#ifndef HPDF_NOPNGLIB
static HPDF_Image
LoadPngImageFromStream (HPDF_Doc      pdf,
                        HPDF_Stream   imagedata,
                        HPDF_BOOL     delayed_loading)
{
    HPDF_Image image;

    HPDF_PTRACE ((" HPDF_LoadPngImageFromStream\n"));

    image = HPDF_Image_LoadPngImage (pdf->mmgr, imagedata, pdf->xref,
                delayed_loading);

    if (image && (pdf->compression_mode & HPDF_COMP_IMAGE))
        image->filter = HPDF_STREAM_FILTER_FLATE_DECODE;

    return image;
}

#else
static HPDF_Image
LoadPngImageFromStream (HPDF_Doc      pdf,
                        HPDF_Stream   imagedata,
                        HPDF_BOOL     delayed_loading)
{
    HPDF_SetError (&pdf->error, HPDF_UNSUPPORTED_FUNC, 0);

    return NULL;
}

#endif /* HPDF_NOPNGLIB */

