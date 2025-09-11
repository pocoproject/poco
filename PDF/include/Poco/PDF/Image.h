//
// Image.h
//
// Library: PDF
// Package: PDFCore
// Module:  Image
//
// Definition of the Image class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDF_Image_INCLUDED
#define PDF_Image_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/PDF/Resource.h"


namespace Poco {
namespace PDF {


class PDF_API Image: public Resource<HPDF_Image>
	/// Image class represents image resource.
{
public:
	Image(HPDF_Doc* pPDF, const HPDF_Image& resource, const std::string& name = "");
		/// Creates the image.

	~Image() override;
		/// Destroys the image.

	Point size() const;
		/// Returns the size of the image.
	float width() const;
		/// Returns the width of the image.

	float height() const;
		/// Returns the height of the image.

	Poco::UInt32 bitsPerColor() const;
		/// Returns the number of bits per color.

	std::string colorSpace() const;
		/// Returns the name of the image's color space.

	void colorMask(Poco::UInt32 redMin,
		Poco::UInt32 redMax,
		Poco::UInt32 greenMin,
		Poco::UInt32 greenMax,
		Poco::UInt32 blueMin,
		Poco::UInt32 blueMax);
		/// Sets the transparent color of the image by the RGB range values.
		/// The color within the range is displayed as a transparent color.
		/// The Image must be of the RGB color space.
};



} } // namespace Poco::PDF


#endif // PDF_Image_INCLUDED
