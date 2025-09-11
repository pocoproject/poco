//
// TextAnnotation.h
//
// Library: PDF
// Package: PDFCore
// Module:  TextAnnotation
//
// Definition of the TextAnnotation class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDF_TextAnnotation_INCLUDED
#define PDF_TextAnnotation_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/PDF/Resource.h"
#include "Poco/PDF/Declarations.h"


namespace Poco {
namespace PDF {


class PDF_API TextAnnotation: public Resource<HPDF_Annotation>
	/// A TextAnnotation represents a PDF annotation resource.
{
public:
	enum IconType
	{
		ANNOTATION_ICON_COMMENT = 0,
		ANNOTATION_ICON_KEY,
		ANNOTATION_ICON_NOTE,
		ANNOTATION_ICON_HELP,
		ANNOTATION_ICON_NEW_PARAGRAPH,
		ANNOTATION_ICON_PARAGRAPH,
		ANNOTATION_ICON_INSERT
	};	

	TextAnnotation(HPDF_Doc* pPDF,
		const HPDF_Annotation& annotation,
		const std::string& name = "");
		/// Creates the annotation.

	~TextAnnotation() override;
		/// Destroys the annotation.

	void open();
		/// Opens the annotation.

	void close();
		/// Closes the annotation.

	void icon(IconType iconType);
};


} } // namespace Poco::PDF


#endif // PDF_TextAnnotation_INCLUDED
