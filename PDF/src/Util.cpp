//
// Util.cpp
//
// Library: PDF 
// Package: PDFCore 
// Module:  Util 
// 
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH. 
// and Contributors. 
// 
// SPDX-License-Identifier:	BSL-1.0 
// 

#include "Poco/PDF/Util.h"
#include "bmpread.h"
#include "hpdf.h"
#include "hpdf_utils.h"


namespace Poco {
namespace PDF {


HPDF_Image LoadBMPImageFromFile(HPDF_Doc pdf, const char* filename)
{
	HPDF_Stream imagedata;
	HPDF_Image image;

	HPDF_PTRACE((" LoadBMPImageFromFile\n"));

	if (!HPDF_HasDoc(pdf))
		return NULL;

	/* create file stream */
	imagedata = HPDF_FileReader_New(pdf->mmgr, filename);

	if (HPDF_Stream_Validate(imagedata))
	{
		bmpread_t bmp;
		unsigned int flags = BMPREAD_ANY_SIZE | BMPREAD_TOP_DOWN;
		if (!bmpread(filename, flags, &bmp)) {
			image = NULL;
	}
	else
	{
		//BMP Loaded
		HPDF_UINT          width = bmp.width;
		HPDF_UINT          height = bmp.height;
		HPDF_ColorSpace    color_space = HPDF_CS_DEVICE_RGB;//HPDF_CS_CAL_RGB
		HPDF_UINT          bits_per_component = 8;
		image = HPDF_LoadRawImageFromMem(pdf, bmp.data, width, height, color_space, bits_per_component);
		bmpread_free(&bmp);
	}
	}
	else
	{
		image = NULL;
	}

	/* destroy file stream */
	HPDF_Stream_Free(imagedata);

	if (!image) HPDF_CheckError(&pdf->error);

	return image;
}


} } // namespace Poco::PDF
