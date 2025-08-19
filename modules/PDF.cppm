module;

#define ENABLE_PDF

#ifdef ENABLE_PDF
#include "Poco/PDF/AttributedString.h"
#include "Poco/PDF/Cell.h"
#include "Poco/PDF/Destination.h"
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

export module Poco.PDF;

export namespace Poco::PDF {
	#ifdef ENABLE_PDF
	using Poco::PDF::AttributedString;
	using Poco::PDF::Cell;
	using Poco::PDF::Destination;
	using Poco::PDF::Document;
	using Poco::PDF::Encoder;
	using Poco::PDF::Font;
	using Poco::PDF::Image;
	using Poco::PDF::LinkAnnotation;
	using Poco::PDF::Outline;
	using Poco::PDF::Page;
	using Poco::PDF::PDFCreateException;
	using Poco::PDF::PDFException;
	using Poco::PDF::Resource;
	using Poco::PDF::Table;
	using Poco::PDF::TextAnnotation;
	using Poco::PDF::XMLTemplate;
	#endif
}
