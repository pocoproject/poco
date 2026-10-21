//
// PDF.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_PDF
#include "Poco/PDF/AttributedString.h"
#include "Poco/PDF/Cell.h"
#include "Poco/PDF/Destination.h"
#include "Poco/PDF/Document.h"
#include "Poco/PDF/Encoder.h"
#include "Poco/PDF/Font.h"
#include "Poco/PDF/Image.h"
#include "Poco/PDF/LinkAnnotation.h"
#include "Poco/PDF/Outline.h"
#include "Poco/PDF/Page.h"
#include "Poco/PDF/PDF.h"
#include "Poco/PDF/PDFException.h"
#include "Poco/PDF/Resource.h"
#include "Poco/PDF/Table.h"
#include "Poco/PDF/TextAnnotation.h"
#include "Poco/PDF/XMLTemplate.h"
#endif

export module Poco:PDF;

export namespace Poco::PDF {
	#ifdef ENABLE_PDF
	using Poco::PDF::AttributedString;
	using Poco::PDF::Cell;
	using Poco::PDF::Destination;
	using Poco::PDF::Document;
	using Poco::PDF::Encoder;
	using Poco::PDF::Font;
	using Poco::PDF::Image;
	using Poco::PDF::LineCap;
	using Poco::PDF::LineJoin;
	using Poco::PDF::LinkAnnotation;
	using Poco::PDF::Outline;
	using Poco::PDF::Page;
	using Poco::PDF::PDFCreateException;
	using Poco::PDF::PDFException;
	using Poco::PDF::Resource;
	using Poco::PDF::Table;
	using Poco::PDF::TextAnnotation;
	using Poco::PDF::XMLTemplate;

	using Poco::PDF::CMYKColor;
	using Poco::PDF::DashMode;
	using Poco::PDF::ExtGraphicsState;
	using Poco::PDF::PatternVec;
	using Poco::PDF::Point;
	using Poco::PDF::RGBColor;
	using Poco::PDF::Rectangle;
	using Poco::PDF::TableRow;
	using Poco::PDF::TextWidth;
	using Poco::PDF::TransMatrix;

	using Poco::PDF::HPDF_BEVEL_JOIN;
	using Poco::PDF::HPDF_BUTT_END;
	using Poco::PDF::HPDF_COMP_ALL;
	using Poco::PDF::HPDF_COMP_IMAGE;
	using Poco::PDF::HPDF_COMP_METADATA;
	using Poco::PDF::HPDF_COMP_NONE;
	using Poco::PDF::HPDF_COMP_TEXT;
	using Poco::PDF::HPDF_ENABLE_COPY;
	using Poco::PDF::HPDF_ENABLE_EDIT;
	using Poco::PDF::HPDF_ENABLE_EDIT_ALL;
	using Poco::PDF::HPDF_ENABLE_PRINT;
	using Poco::PDF::HPDF_ENABLE_READ;
	using Poco::PDF::HPDF_LINECAP_EOF;
	using Poco::PDF::HPDF_LINEJOIN_EOF;
	using Poco::PDF::HPDF_MITER_JOIN;
	using Poco::PDF::HPDF_PROJECTING_SCUARE_END;
	using Poco::PDF::HPDF_ROUND_END;
	using Poco::PDF::HPDF_ROUND_JOIN;

	using Poco::PDF::HPDF_Error_Handler;
	#endif
}
