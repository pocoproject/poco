//
// Image.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  Image
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/Image.h"

#include <hpdf.h>

namespace Poco {
namespace PDF {


Image::Image(HPDF_Doc* pPDF, const HPDF_Image& resource, const std::string& name):
	Resource<HPDF_Image>(pPDF, resource, name)
{
}

Image::~Image() = default;

Point Image::size() const
{
	return HPDF_Image_GetSize(handle());
}


float Image::width() const
{
	return static_cast<float>(HPDF_Image_GetWidth(handle()));
}


float Image::height() const
{
	return static_cast<float>(HPDF_Image_GetHeight(handle()));
}


Poco::UInt32 Image::bitsPerColor() const
{
	return HPDF_Image_GetBitsPerComponent(handle());
}


std::string Image::colorSpace() const
{
	return HPDF_Image_GetColorSpace(handle());
}


void Image::colorMask(Poco::UInt32 redMin,
							 Poco::UInt32 redMax,
							 Poco::UInt32 greenMin,
							 Poco::UInt32 greenMax,
							 Poco::UInt32 blueMin,
							 Poco::UInt32 blueMax)
{
	HPDF_Image_SetColorMask(handle(),
							redMin,
							redMax,
							greenMin,
							greenMax,
							blueMin,
							blueMax);
}

} } // namespace Poco::PDF
