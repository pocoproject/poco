//
// TextAnnotation.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  TextAnnotation
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/TextAnnotation.h"

#include <hpdf.h>

namespace Poco {
namespace PDF {


TextAnnotation::TextAnnotation(HPDF_Doc* pPDF,
	const HPDF_Annotation& annotation,
	const std::string& name):
	Resource<HPDF_Annotation>(pPDF, annotation, name)
{
	open();
}


TextAnnotation::~TextAnnotation()
{
	close();
}


void TextAnnotation::open()
{
	HPDF_TextAnnot_SetOpened(handle(), HPDF_TRUE);
}


void TextAnnotation::close()
{
	HPDF_TextAnnot_SetOpened(handle(), HPDF_FALSE);
}


void TextAnnotation::icon(IconType iconType)
{
	HPDF_TextAnnot_SetIcon(handle(), static_cast<HPDF_AnnotIcon>(iconType));
}


} } // namespace Poco::PDF
