//
// LinkAnnotation.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  LinkAnnotation
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/LinkAnnotation.h"
#include <hpdf.h>

namespace Poco {
namespace PDF {


LinkAnnotation::LinkAnnotation(HPDF_Doc* pPDF,
	const HPDF_Annotation& annotation,
	const std::string& name):
	Resource<HPDF_Annotation>(pPDF, annotation, name)
{
}

LinkAnnotation::~LinkAnnotation() = default;

void LinkAnnotation::setHighlight(Highlight mode)
{
	HPDF_LinkAnnot_SetHighlightMode(handle(),
									static_cast<HPDF_AnnotHighlightMode>(mode));
}

void LinkAnnotation::setBorderStyle(float width, Poco::UInt32 dashOn, Poco::UInt32 dashOff)
{
	HPDF_LinkAnnot_SetBorderStyle(handle(), width, dashOn, dashOff);
}



} } // namespace Poco::PDF
