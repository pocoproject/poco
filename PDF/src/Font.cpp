//
// Font.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  Font
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/Font.h"

#include <hpdf.h>

namespace Poco {
namespace PDF {


Font::Font(HPDF_Doc* pPDF, HPDF_Font font):
	Resource<HPDF_Font>(pPDF, font, HPDF_Font_GetFontName(font))
{
}

Font::~Font() = default;

std::string Font::encodingName() const
{
	return HPDF_Font_GetEncodingName(handle());
}


int Font::unicodeWidth(Poco::UInt16 ch) const
{
	return HPDF_Font_GetUnicodeWidth(handle(), ch);
}


Rectangle Font::boundingBox() const
{
	return HPDF_Font_GetBBox(handle());
}


int Font::ascent() const
{
	return HPDF_Font_GetAscent(handle());
}


int Font::descent() const
{
	return HPDF_Font_GetDescent(handle());
}


int Font::lowerHeight() const
{
	return static_cast<int>(HPDF_Font_GetXHeight(handle()));
}


int Font::upperHeight() const
{
	return static_cast<int>(HPDF_Font_GetCapHeight(handle()));
}


TextWidth Font::textWidth(const std::string& text)
{
	return HPDF_Font_TextWidth(handle(),
							   reinterpret_cast<const HPDF_BYTE*>(text.data()),
							   static_cast<HPDF_UINT>(text.size()));
}


int Font::measureText(const std::string& text,
							 float width,
							 float fontSize,
							 float charSpace,
							 float wordSpace,
							 bool wordWrap)
{
	return static_cast<int>(HPDF_Font_MeasureText(handle(),
												  reinterpret_cast<const HPDF_BYTE*>(text.data()),
												  static_cast<HPDF_UINT>(text.size()),
												  width,
												  fontSize,
												  charSpace,
												  wordSpace,
												  wordWrap,
												  nullptr));
}


} } // namespace Poco::PDF
