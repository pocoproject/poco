//
// Page.cpp
//
// Library: PDF
// Package: PDFCore
// Module:  Page
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/PDF/Page.h"
#include "Poco/Exception.h"
#include "Poco/PDF/Document.h"
#include <hpdf.h>
#undef min
#undef max
#include <limits>


namespace Poco {
namespace PDF {

static_assert(static_cast<int>(Page::Size::PAGE_SIZE_LETTER) == HPDF_PAGE_SIZE_LETTER, "Page size value mismatch");
static_assert(static_cast<int>(Page::Size::PAGE_SIZE_COMM10) == HPDF_PAGE_SIZE_COMM10, "Page size value mismatch");

static_assert(static_cast<int>(Page::Orientation::ORIENTATION_PORTRAIT) == HPDF_PAGE_PORTRAIT, "Page orientation value mismatch");
static_assert(static_cast<int>(Page::Orientation::ORIENTATION_LANDSCAPE) == HPDF_PAGE_LANDSCAPE, "Page orientation value mismatch");

static_assert(static_cast<int>(Page::RenderMode::RENDER_FILL) == HPDF_FILL, "Render mode value mismatch");
static_assert(static_cast<int>(Page::RenderMode::RENDER_RENDERING_MODE_EOF) == HPDF_RENDERING_MODE_EOF, "render mode value mismatch");

static_assert(static_cast<int>(Page::ColorSpace::CS_DEVICE_GRAY) == HPDF_CS_DEVICE_GRAY, "Page color space value mismatch");
static_assert(static_cast<int>(Page::ColorSpace::CS_EOF) == HPDF_CS_EOF, "Page color space value mismatch");

static_assert(static_cast<int>(Page::TransitionStyle::TS_WIPE_RIGHT) == HPDF_TS_WIPE_RIGHT, "Transition style value mismatch");
static_assert(static_cast<int>(Page::TransitionStyle::TS_REPLACE) == HPDF_TS_REPLACE, "Transition style value mismatch");

static_assert(static_cast<int>(Page::TextAlignment::TEXT_ALIGN_LEFT) == HPDF_TALIGN_LEFT, "Text alignment value mismatch");
static_assert(static_cast<int>(Page::TextAlignment::TEXT_ALIGN_JUSTIFY) == HPDF_TALIGN_JUSTIFY, "Text alignment value mismatch");

Page::Page(Document* pDocument, const HPDF_Page& page, Size pageSize, Orientation orientation)
	: _pDocument(pDocument)
	, _page(page)
	, _size(pageSize)
	, _orientation(orientation)
	, _pCurrentFont(nullptr)
{
}

Page::Page(const Page& other)
	: _pDocument(other._pDocument)
	, _page(other._page)
	, _size(other._size)
	, _orientation(other._orientation)
	, _pCurrentFont(other._pCurrentFont ? new Font(*other._pCurrentFont) : (Font*) nullptr)
{
}


Page::~Page()
{
	delete _pCurrentFont;
}


Page& Page::operator = (const Page& page)
{
	Page tmp(page);
	swap(tmp);
	return *this;
}


bool Page::operator == (const Page& other) const
{
	return &_pDocument->handle() == &other._pDocument->handle() && _page == other._page;
}


void Page::swap(Page& other) noexcept
{
	using std::swap;

	swap(_pDocument, other._pDocument);
	swap(_page, other._page);
	swap(_size, other._size);
	swap(_orientation, other._orientation);
	swap(_pCurrentFont, other._pCurrentFont);
}


Page::operator const Page::Type& () const
{
	return _page;
}


void Page::setWidth(float value)
{
	HPDF_Page_SetWidth(_page, value);
}


float Page::getWidth() const
{
	return HPDF_Page_GetWidth(_page);
}


void Page::setHeight(float value)
{
	HPDF_Page_SetHeight(_page, value);
}


float Page::getHeight() const
{
	return HPDF_Page_GetHeight(_page);
}


void Page::setSizeAndOrientation(Size size, Orientation orientation)
{
	_size = size;
	_orientation = orientation;
	HPDF_Page_SetSize(_page, static_cast<HPDF_PageSizes>(size), static_cast<HPDF_PageDirection>(orientation));
}


void Page::setSize(Size size)
{
	_size = size;
	HPDF_Page_SetSize(_page, static_cast<HPDF_PageSizes>(size), static_cast<HPDF_PageDirection>(_orientation));
}


void Page::setOrientation(Orientation orientation)
{
	_orientation = orientation;
	HPDF_Page_SetSize(_page, static_cast<HPDF_PageSizes>(_size), static_cast<HPDF_PageDirection>(orientation));
}


Page::Size Page::getSize() const
{
	return _size;
}


Page::Orientation Page::getOrientation() const
{
	return _orientation;
}


void Page::setFont(const Font& font, float size)
{
	HPDF_Page_SetFontAndSize(_page, font, size);
}


int Page::getGraphicsMode() const
{
	return HPDF_Page_GetGMode(_page);
}


int Page::getGraphicStateDepth() const
{
	return HPDF_Page_GetGStateDepth(_page);
}


void Page::setExtGraphicsState(ExtGraphicsState state)
{
	HPDF_Page_SetExtGState(_page, state);
}


void Page::saveGraphics()
{
	HPDF_Page_GSave(_page);
}


void Page::restoreGraphics()
{
	HPDF_Page_GRestore(_page);
}


void Page::concatenate(const std::vector<float>& values)
{
	if (values.size() < 6)
		throw InvalidArgumentException("Needs six values");

	HPDF_Page_Concat(_page,
					 values[0],
					 values[1],
					 values[2],
					 values[3],
					 values[4],
					 values[5]);
}


void Page::moveTo(float x, float y)
{
	HPDF_Page_MoveTo(_page, x, y);
}


void Page::lineTo(float x, float y)
{
	HPDF_Page_LineTo(_page, x, y);
}


void Page::curveTo(const std::vector<float>& values)
{
	if (values.size() < 6)
		throw InvalidArgumentException("Needs six values");

	HPDF_Page_CurveTo(_page,
					  values[0],
					  values[1],
					  values[2],
					  values[3],
					  values[4],
					  values[5]);
}


void Page::curveToRight(float x2, float y2, float x3, float y3)
{
	HPDF_Page_CurveTo2(_page, x2, y2, x3, y3);
}


void Page::curveToLeft(float x2, float y2, float x3, float y3)
{
	HPDF_Page_CurveTo3(_page, x2, y2, x3, y3);
}


void Page::closePath()
{
	HPDF_Page_ClosePath(_page);
}


void Page::rectangle(float x, float y, float width, float height)
{
	HPDF_Page_Rectangle(_page, x, y, width, height);
}


void Page::stroke()
{
	HPDF_Page_Stroke(_page);
}


void Page::closeAndStroke()
{
	HPDF_Page_ClosePathStroke(_page);
}


void Page::fill()
{
	HPDF_Page_Fill(_page);
}


void Page::EOFill()
{
	HPDF_Page_Eofill(_page);
}


void Page::fillStroke()
{
	HPDF_Page_FillStroke(_page);
}


void Page::EOFillStroke()
{
	HPDF_Page_EofillStroke(_page);
}


void Page::closeFillAndStroke()
{
	HPDF_Page_ClosePathFillStroke(_page);
}


void Page::closeFillAndEOStroke()
{
	HPDF_Page_ClosePathEofillStroke(_page);
}


void Page::endPath()
{
	HPDF_Page_EndPath(_page);
}


void Page::clip()
{
	HPDF_Page_Clip(_page);
}


void Page::eoClip()
{
	HPDF_Page_Eoclip(_page);
}


Point Page::getPos() const
{
	return HPDF_Page_GetCurrentPos(_page);
}


Point Page::getTextPos() const
{
	return HPDF_Page_GetCurrentTextPos(_page);
}


void Page::moveTextPos(float x, float y)
{
	HPDF_Page_MoveTextPos(_page, x, y);
}


void Page::moveTextNextLine(float x, float y)
{
	HPDF_Page_MoveTextPos2(_page, x, y);
}


void Page::moveTextNextLine()
{
	HPDF_Page_MoveToNextLine(_page);
}


float Page::getFontSize() const
{
	return HPDF_Page_GetCurrentFontSize(_page);
}


TransMatrix Page::getTransMatrix() const
{
	return HPDF_Page_GetTransMatrix(_page);
}


TransMatrix Page::getTextMatrix() const
{
	return HPDF_Page_GetTextMatrix(_page);
}


float Page::getLineWidth() const
{
	return HPDF_Page_GetLineWidth(_page);
}


void Page::setLineWidth(float width)
{
	HPDF_Page_SetLineWidth(_page, width);
}


LineCap Page::getLineCap() const
{
	return static_cast<LineCap>(HPDF_Page_GetLineCap(_page));
}


void Page::setLineCap(LineCap cap) const
{
	HPDF_Page_SetLineCap(_page, static_cast<HPDF_LineCap>(cap));
}


LineJoin Page::getLineJoin() const
{
	return static_cast<LineJoin>(HPDF_Page_GetLineJoin(_page));
}


void Page::setLineJoin(LineJoin join) const
{
	HPDF_Page_SetLineJoin(_page, static_cast<HPDF_LineJoin>(join));
}


float Page::getMiterLimit() const
{
	return HPDF_Page_GetMiterLimit(_page);
}


void Page::setMiterLimit(float limit) const
{
	HPDF_Page_SetMiterLimit(_page, limit);
}


DashMode Page::getDashMode() const
{
	return HPDF_Page_GetDash(_page);
}


void Page::setDashMode(const PatternVec& pattern, int paramNo, int phase) const
{
	HPDF_Page_SetDash(_page, &pattern[0],
					  static_cast<HPDF_UINT>(paramNo),
					  static_cast<HPDF_UINT>(phase));
}


float Page::getFlatness() const
{
	return HPDF_Page_GetFlat(_page);
}


float Page::getCharSpace() const
{
	return HPDF_Page_GetCharSpace(_page);
}


void Page::setCharSpace(float value)
{
	HPDF_Page_SetCharSpace(_page, value);
}


float Page::getWordSpace() const
{
	return HPDF_Page_GetWordSpace(_page);
}


void Page::setWordSpace(float value)
{
	HPDF_Page_SetWordSpace(_page, value);
}


float Page::getHorizontalScale() const
{
	return HPDF_Page_GetHorizontalScalling(_page);
}


void Page::setHorizontalScale(float value)
{
	HPDF_Page_SetHorizontalScalling(_page, value);
}


float Page::getTextLead() const
{
	return HPDF_Page_GetTextLeading(_page);
}


void Page::setTextLead(float value)
{
	HPDF_Page_SetTextLeading(_page, value);
}


Page::RenderMode Page::getTextRenderMode() const
{
	return static_cast<RenderMode>(HPDF_Page_GetTextRenderingMode(_page));
}


void Page::setTextRenderMode(RenderMode value)
{
	HPDF_Page_SetTextRenderingMode(_page, static_cast<HPDF_TextRenderingMode>(value));
}


float Page::getTextRise() const
{
	return HPDF_Page_GetTextRise(_page);
}


void Page::setTextRise(float value)
{
	HPDF_Page_SetTextRise(_page, value);
}


RGBColor Page::getRGBFill() const
{
	return HPDF_Page_GetRGBFill(_page);
}


void Page::setRGBFill(RGBColor value)
{
	HPDF_Page_SetRGBFill(_page, value.r, value.g, value.b);
}


RGBColor Page::getRGBStroke() const
{
	return HPDF_Page_GetRGBStroke(_page);
}


void Page::setRGBStroke(RGBColor value)
{
	HPDF_Page_SetRGBStroke(_page, value.r, value.g, value.b);
}


CMYKColor Page::getCMYKFill() const
{
	return HPDF_Page_GetCMYKFill(_page);
}


void Page::setCMYKFill(CMYKColor value)
{
	HPDF_Page_SetCMYKFill(_page, value.c, value.m, value.y, value.k);
}


CMYKColor Page::getCMYKStroke() const
{
	return HPDF_Page_GetCMYKStroke(_page);
}


void Page::setCMYKStroke(CMYKColor value)
{
	HPDF_Page_SetCMYKStroke(_page, value.c, value.m, value.y, value.k);
}


float Page::getGreyFill() const
{
	return HPDF_Page_GetGrayFill(_page);
}


void Page::setGreyFill(float value)
{
	HPDF_Page_SetGrayFill(_page, value);
}


float Page::getGreyStroke() const
{
	return HPDF_Page_GetGrayStroke(_page);
}


void Page::setGreyStroke(float value)
{
	HPDF_Page_SetGrayStroke(_page, value);
}


Page::ColorSpace Page::getStrokeColorSpace() const
{
	return static_cast<ColorSpace>(HPDF_Page_GetStrokingColorSpace(_page));
}


Page::ColorSpace Page::getFillColorSpace() const
{
	return static_cast<ColorSpace>(HPDF_Page_GetFillingColorSpace(_page));
}


void Page::setSlideShow(TransitionStyle type, float displayTime, float transitionTime)
{
	HPDF_Page_SetSlideShow(_page,
						   static_cast<HPDF_TransitionStyle>(type),
						   displayTime,
						   transitionTime);
}


void Page::beginText()
{
	HPDF_Page_BeginText(_page);
}

void Page::endText()
{
	HPDF_Page_EndText(_page);
}


void Page::write(float xPos, float yPos, const std::string& text)
{
	HPDF_Page_TextOut(_page, xPos, yPos, text.c_str());
}


void Page::write(const std::string& text)
{
	HPDF_Page_ShowText(_page, text.c_str());
}


void Page::writeNextLine(const std::string& text)
{
	HPDF_Page_ShowTextNextLine(_page, text.c_str());
}


void Page::writeNextLineEx(float wordSpace, float charSpace, const std::string& text)
{
	HPDF_Page_ShowTextNextLineEx(_page, wordSpace, charSpace, text.c_str());
}


int Page::writeInRectangle(float left,
								  float top,
								  float right,
								  float bottom,
								  const std::string& text,
								  TextAlignment align)
{
	HPDF_UINT ret = 0;
	HPDF_Page_TextRect(_page,
					   left,
					   top,
					   right,
					   bottom,
					   text.c_str(),
					   static_cast<HPDF_TextAlignment>(align),
					   &ret);

	return static_cast<int>(ret);
}


void Page::drawImage(Image image, float x, float y, float width, float height)
{
	HPDF_Page_DrawImage(_page, image, x, y, width, height);
}


void Page::circle(float x, float y, float radius)
{
	HPDF_Page_Circle(_page, x, y, radius);
}


void Page::arc(float x, float y, float radius, float beginAngle, float endAngle)
{
	HPDF_Page_Arc(_page, x, y, radius, beginAngle, endAngle);
}


void Page::ellipse(float x, float y, float xRadius, float yRadius)
{
	HPDF_Page_Ellipse(_page, x, y, xRadius, yRadius);
}

void Page::writeOnce(float xPos, float yPos, const std::string& text)
{
	beginText();
	write(xPos, yPos, text);
	endText();
}


int Page::writeOnceInRectangle(float left,
		float top,
		float right,
		float bottom,
		const std::string& text,
		TextAlignment align)
{
	beginText();
	int ret = writeInRectangle(left, top, right, bottom, text, align);
	endText();
	return ret;
}


float Page::textWidth(const std::string& text)
{
	return HPDF_Page_TextWidth(_page, text.c_str());
}


void Page::setFont(const std::string& name, float size, const std::string& encoding)
{
	setFont(_pDocument->font(name, encoding), size);
}


void Page::setTTFont(const std::string& name, float size, const std::string& encoding, bool embed)
{
	setFont(_pDocument->font(_pDocument->loadTTFont(name, embed), encoding), size);
}


const Font& Page::getFont() const
{
	delete _pCurrentFont;
	_pCurrentFont = nullptr;
	HPDF_Font pCurFont = HPDF_Page_GetCurrentFont(_page);
	if (!pCurFont) throw Poco::NullPointerException("PDF::Page has no font set.");
	_pCurrentFont = new Font(&_pDocument->handle(), pCurFont);
	return *_pCurrentFont;
}


void Page::setRotation(int angle)
{
	if (0 != angle % 90 || angle > std::numeric_limits<HPDF_UINT16>::max())
		throw InvalidArgumentException("Invalid angle value.");

	HPDF_Page_SetRotate(_page, static_cast<HPDF_UINT16>(angle));
}


const Destination& Page::createDestination(const std::string& name)
{
	auto it = _destinations.find(name);
	if (_destinations.end() != it)
		throw InvalidArgumentException("Destination already exists.");

	Destination dest(&_pDocument->handle(), HPDF_Page_CreateDestination(_page), name);
	std::pair<DestinationContainer::iterator, bool> ret =
		_destinations.insert(DestinationContainer::value_type(name, dest));

	if (ret.second) return ret.first->second;

	throw IllegalStateException("Could not create destination.");
}


const TextAnnotation& Page::createTextAnnotation(const std::string& name,
	const Rectangle& rect,
	const std::string& text,
	const Encoder& encoder)
{
	auto it = _textAnnotations.find(name);
	if (_textAnnotations.end() != it)
		throw InvalidArgumentException("Annotation already exists.");

	TextAnnotation ann(&_pDocument->handle(),
		HPDF_Page_CreateTextAnnot(_page, rect, text.c_str(), encoder),
		name);

	std::pair<TextAnnotationContainer::iterator, bool> ret =
		_textAnnotations.insert(TextAnnotationContainer::value_type(name, ann));

	if (ret.second) return ret.first->second;

	throw IllegalStateException("Could not create annotation.");
}


const LinkAnnotation& Page::createLinkAnnotation(const std::string& name,
	const Rectangle& rect,
	const Destination& dest)
{
	auto it = _linkAnnotations.find(name);
	if (_linkAnnotations.end() != it)
		throw InvalidArgumentException("Annotation already exists.");

	LinkAnnotation ann(&_pDocument->handle(),
		HPDF_Page_CreateLinkAnnot(_page, rect, dest),
		name);
	std::pair<LinkAnnotationContainer::iterator, bool> ret =
		_linkAnnotations.insert(LinkAnnotationContainer::value_type(name, ann));

	if (ret.second) return ret.first->second;

	throw IllegalStateException("Could not create annotation.");
}


const LinkAnnotation& Page::createURILinkAnnotation(const std::string& name,
	const Rectangle& rect,
	const std::string& uri)
{
	auto it = _linkAnnotations.find(name);
	if (_linkAnnotations.end() != it)
		throw InvalidArgumentException("Annotation already exists.");

	LinkAnnotation ann(&_pDocument->handle(),
		HPDF_Page_CreateURILinkAnnot(_page, rect, uri.c_str()),
		name);
	std::pair<LinkAnnotationContainer::iterator, bool> ret =
		_linkAnnotations.insert(LinkAnnotationContainer::value_type(name, ann));

	if (ret.second) return ret.first->second;

	throw IllegalStateException("Could not create annotation.");
}


} } // namespace Poco::PDF
