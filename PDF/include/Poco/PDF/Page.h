//
// Page.h
//
// Library: PDF
// Package: PDFCore
// Module:  Page
//
// Definition of the Page class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDF_Page_INCLUDED
#define PDF_Page_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/PDF/Resource.h"
#include "Poco/PDF/Font.h"
#include "Poco/PDF/Image.h"
#include "Poco/PDF/Encoder.h"
#include "Poco/PDF/Destination.h"
#include "Poco/PDF/TextAnnotation.h"
#include "Poco/PDF/LinkAnnotation.h"
#include <map>


namespace Poco {
namespace PDF {


class Document;


class PDF_API Page
	/// A Page represents a PDF page object.
{
public:
	using Type = HPDF_Page;
	using DestinationContainer = std::map<std::string, Destination>;
	using TextAnnotationContainer = std::map<std::string, TextAnnotation>;
	using LinkAnnotationContainer = std::map<std::string, LinkAnnotation>;

	enum Size
	{
		PAGE_SIZE_LETTER = 0,
			/// 8½ x 11 (Inches), 612 x 792 px
		PAGE_SIZE_LEGAL,
			/// 8½ x 14 (Inches), 612 x 1008 px
		PAGE_SIZE_A3,
			/// 297 × 420 (mm), 841.89 x 1199.551 px
		PAGE_SIZE_A4,
			/// 210 × 297 (mm), 595.276 x 841.89 px
		PAGE_SIZE_A5,
			/// 148 × 210 (mm), 419.528 x 595.276 px
		PAGE_SIZE_B4,
			/// 250 × 353 (mm), 708.661 x 1000.63 px
		PAGE_SIZE_B5,
			/// 176 × 250 (mm), 498.898 x 708.661 px
		PAGE_SIZE_EXECUTIVE,
			/// 7½ x 10½ (Inches), 522 x 756 px
		PAGE_SIZE_US4x6,
			/// 4 x 6 (Inches), 288 x 432 px
		PAGE_SIZE_US4x8,
			/// 4 x 8 (Inches), 288 x 576 px
		PAGE_SIZE_US5x7,
			/// 5 x 7 (Inches), 360 x 504 px
		PAGE_SIZE_COMM10
			/// 4.125 x 9.5 (Inches) 297x 684 px
	};	

	enum Orientation
	{
		ORIENTATION_PORTRAIT = 0,
			// Portrait orientation.
		ORIENTATION_LANDSCAPE
			// Landscape orientation.
	};

	enum RenderMode
	{
		RENDER_FILL = 0,
		RENDER_STROKE,
		RENDER_FILL_THEN_STROKE,
		RENDER_INVISIBLE,
		RENDER_FILL_CLIPPING,
		RENDER_STROKE_CLIPPING,
		RENDER_FILL_STROKE_CLIPPING,
		RENDER_CLIPPING,
		RENDER_RENDERING_MODE_EOF
	};	

	enum ColorSpace
	{
		CS_DEVICE_GRAY = 0,
		CS_DEVICE_RGB,
		CS_DEVICE_CMYK,
		CS_CAL_GRAY,
		CS_CAL_RGB,
		CS_LAB,
		CS_ICC_BASED,
		CS_SEPARATION,
		CS_DEVICE_N,
		CS_INDEXED,
		CS_PATTERN,
		CS_EOF
	};

	enum TransitionStyle
	{
		TS_WIPE_RIGHT = 0,
		TS_WIPE_UP,
		TS_WIPE_LEFT,
		TS_WIPE_DOWN,
		TS_BARN_DOORS_HORIZONTAL_OUT,
		TS_BARN_DOORS_HORIZONTAL_IN,
		TS_BARN_DOORS_VERTICAL_OUT,
		TS_BARN_DOORS_VERTICAL_IN,
		TS_BOX_OUT,
		TS_BOX_IN,
		TS_BLINDS_HORIZONTAL,
		TS_BLINDS_VERTICAL,
		TS_DISSOLVE,
		TS_GLITTER_RIGHT,
		TS_GLITTER_DOWN,
		TS_GLITTER_TOP_LEFT_TO_BOTTOM_RIGHT,
		TS_REPLACE
	};

	enum TextAlignment
	{
		TEXT_ALIGN_LEFT = 0,
			/// The text is aligned to left.
		TEXT_ALIGN_RIGHT,
			/// The text is aligned to right.
		TEXT_ALIGN_CENTER,
			/// The text is aligned to center.
		TEXT_ALIGN_JUSTIFY
			/// Add spaces between the words to justify both left and right side.
	};	

	Page(Document* pDocument,
		const HPDF_Page& page,
		Size pageSize = PAGE_SIZE_LETTER,
		Orientation orientation = ORIENTATION_PORTRAIT);
		/// Creates the page

	Page(const Page& other);
		/// Copy creates the page.

	virtual ~Page();
		/// Destroys the page.

	Page& operator = (const Page& page);
		/// Assignment operator.

	operator const Type& () const;
		/// Const conversion operator into reference to native type.

	bool operator == (const Page& other) const;
		/// Equality operator.

	void swap(Page& other) noexcept;
		/// Swaps this page with another.

	void setWidth(float value);
		/// Sets the page width.

	float getWidth() const;
		/// Gets the page width.

	void setHeight(float value);
		/// Sets the page height.

	float getHeight() const;
		/// Gets the page height.

	void setSize(Size size);
		/// Sets the page size.

	Size getSize() const;
		/// Returns the page size.

	void setOrientation(Orientation orientation);
		/// Sets the page orientation.

	Orientation getOrientation() const;
		/// Returns the page orientation.

	void setSizeAndOrientation(Size size, Orientation orientation);
		/// Sets the page size and orientation.

	void setRotation(int angle);
		/// Sets the rotation for the page.
		/// Angle must be multiple of 90.

	void setFont(const Font& font, float size);
		/// Sets the font.

	void setFont(const std::string& fontName, float size, const std::string& encoding = "");
		/// Sets the font. The name must be a valid Base14 PDF internal font.

	void setTTFont(const std::string& name, float size, const std::string& encoding = "UTF-8", bool embed = true);
		/// Sets the external true type font. Name must be a valid path to .ttf file.
		/// If embed is tru, font will be embedded int othe document.

	float textWidth(const std::string& text);
		/// Returns the width of the supplied text.

	void  beginText();
		/// Begins a text object and sets the current text position to the point (0,0).

	void endText();
		/// Ends a text object.

	void write(float xPos, float yPos, const std::string& text);
		/// Write text at specified position.

	void writeOnce(float xPos, float yPos, const std::string& text);
		/// Begins, writes and ends text object.

	void write(const std::string& text);
		/// Writes the text at the current position on the page.

	void writeNextLine(const std::string& text);
		/// Moves the current text position to the start of the next line and
		/// prints the text at the current position on the page.

	void writeNextLineEx(float wordSpace, float charSpace, const std::string& text);
		/// Moves the current text position to the start of the next line, sets the word spacing,
		/// character spacing and prints the text at the current position on the page.

	int writeOnceInRectangle(float left,
		float top,
		float right,
		float bottom,
		const std::string& text,
		TextAlignment align = TEXT_ALIGN_LEFT);
		/// Begins, writes and ends text objectinside the specified region.
		/// Returns the number of characters written.

	int writeInRectangle(float left,
		float top,
		float right,
		float bottom,
		const std::string& text,
		TextAlignment align);
		/// Writes the text inside the specified region.
		/// Returns the number of characters written.

	void drawImage(Image image, float x, float y, float width, float height);
		/// Draws an image in one operation.

	const Destination& createDestination(const std::string& name);
		/// Creates ad returns reference to destination.

	const TextAnnotation& createTextAnnotation(const std::string& name,
		const Rectangle& rect,
		const std::string& text,
		const Encoder& encoder);
		/// Creates ad returns reference to text annotation.

	const LinkAnnotation& createLinkAnnotation(const std::string& name,
		const Rectangle& rect,
		const Destination& dest);
		/// Creates ad returns reference to destination link annotation.

	const LinkAnnotation& createURILinkAnnotation(const std::string& name,
		const Rectangle& rect,
		const std::string& uri);
		/// Creates ad returns reference to URI annotation.

	int getGraphicsMode() const;
		/// Returns current graphics mode.

	int getGraphicStateDepth() const;
		/// Returns current graphics mode.

	void setExtGraphicsState(ExtGraphicsState state);
		/// Sets the graphic state.

	void saveGraphics();
		/// Pushes graphics parameters on the stack.

	void restoreGraphics();
		/// Restores graphics parameters from the stack.

	void concatenate(const std::vector<float>& values);
		/// Concatenates the page's current transformation matrix and specified matrix.

	void moveTo(float x, float y);
		/// Starts a new subpath and move the current point for drawing path.
		/// Sets the start point for the path to the point (x, y).

	void lineTo(float x, float y);
		/// Appends a path from the current point to the specified point..

	void curveTo(const std::vector<float>& values);
		/// Appends a Bézier curve to the current path using two specified points.
		/// The point (x1, y1) and the point (x2, y2) are used as the control points 
		/// for a Bézier curve and current point is moved to the point (x3, y3)

	void curveToRight(float x2, float y2, float x3, float y3);
		/// Appends a Bézier curve to the right of the current point using two specified points.
		/// The current point and the point (x2, y2) are used as the control points 
		/// for a Bézier curve and current point is moved to the point (x3, y3)

	void curveToLeft(float x2, float y2, float x3, float y3);
		/// Appends a Bézier curve to the left of the current point using two specified points.
		/// The current point and the point (x2, y2) are used as the control points 
		/// for a Bézier curve and current point is moved to the point (x3, y3)

	void closePath();
		/// Appends a straight line from the current point to the start point of sub path.
		/// The current point is moved to the start point of sub path.

	void rectangle(float x, float y, float width, float height);
		/// Draws a rectangle.

	void circle(float x, float y, float radius);
		/// Draws a circle.

	void arc(float x, float y, float radius, float beginAngle, float endAngle);
		/// Draws an arc.

	void ellipse(float x, float y, float xRadius, float yRadius);
		/// Draws an ellips.

	void stroke();
		/// Paints the current path.

	void closeAndStroke();
		/// Closes the current path and paints it.

	void fill();
		/// Fills the current path using the nonzero winding number rule.

	void EOFill();
		/// Fills the current path using the even-odd rule.

	void fillStroke();
		/// Fills the current path using the nonzero winding number rule and then paints it.

	void EOFillStroke();
		/// Fills the current path using the even-odd rule and then paints it.

	void closeFillAndStroke();
		/// Closes the current path, fills the current path using the nonzero winding number
		/// rule and then paints it.

	void closeFillAndEOStroke();
		/// Closes the current path, fills the current path using the even-odd rule and then paints it.

	void endPath();
		/// Ends the path object without filling and painting operation.

	void clip();
		///

	void eoClip();
		///

	Point getPos() const;
		/// Returns the current position.

	Point getTextPos() const;
		/// Returns the current position for text showing.

	void moveTextPos(float x, float y);
		/// Moves the current text position to the start of the next line
		/// using specified offset values. If the start position of the current
		/// line is (x1, y1), the start of the next line is (x1 + x, y1 + y).

	void moveTextNextLine(float x, float y);
		/// Moves the current text position to the start of the next line
		/// using specified offset values, and sets the text leading to -y.
		/// If the start position of the current line is (x1, y1), the start
		/// of the next line is (x1 + x, y1 + y).

	void moveTextNextLine();
		/// Moves the current text position to the start of the next line.
		/// If the start position of the current line is (x1, y1), the start of
		/// the next line is (x1, y1 - text leading).
		///
		/// NOTE:
		/// Since the default value of Text Leading is 0,  an application has to
		/// invoke HPDF_Page_SetTextLeading() before HPDF_Page_MoveTextPos2() to set
		/// text leading.

	const Font& getFont() const;
		/// Returns the current font.

	float getFontSize() const;
		/// Returns the current Font size.

	TransMatrix getTransMatrix() const;
		/// Returns the current transformation matrix.

	TransMatrix getTextMatrix() const;
		/// Returns the current text transformation matrix.

	float getLineWidth() const;
		/// Returns the current line width.

	void setLineWidth(float width);
		/// Sets the line width.

	LineCap getLineCap() const;
		/// Returns the current line cap.

	void setLineCap(LineCap cap) const;
		/// Sets the line cap.

	LineJoin getLineJoin() const;
		/// Returns the current line join.

	void setLineJoin(LineJoin join) const;
		/// Returns the current line join.

	float getMiterLimit() const;
		/// Returns the current miter limit.

	void setMiterLimit(float limit) const;
		/// Sets the miter limit.

	DashMode getDashMode() const;
		/// Returns current dash mode.

	void setDashMode(const PatternVec& pattern, int paramNo, int phase) const;
		/// Sets teh dash mode.

	float getFlatness() const;
		/// Returns the current flatness.

	float getCharSpace() const;
		/// Returns the current character space.

	void setCharSpace(float value);
		/// Sets the current character space.

	float getWordSpace() const;
		/// Returns the current word space.

	void setWordSpace(float value);
		/// Sets the current word space.

	float getHorizontalScale() const;
		/// Returns the current horizontal scaling.

	void setHorizontalScale(float value);
		/// Sets the current horizontal scaling.

	float getTextLead() const;
		/// Returns the current text leading.

	void setTextLead(float value);
		/// Sets the current text leading.

	RenderMode getTextRenderMode() const;
		/// Returns the current text rendering mode.

	void setTextRenderMode(RenderMode value);
		/// Sets the current text rendering mode.

	float getTextRise() const;
		/// Returns the current text leading.

	void setTextRise(float value);
		/// Sets the current text leading.

	RGBColor getRGBFill() const;
		/// Returns current RGB fill.

	void setRGBFill(RGBColor value);
		/// Sets current RGB fill.

	RGBColor getRGBStroke() const;
		/// Returns current RGB stroke.

	void setRGBStroke(RGBColor value);
		/// Sets current RGB stroke.

	CMYKColor getCMYKFill() const;
		/// Returns current CMYK fill.

	void setCMYKFill(CMYKColor value);
		/// Sets current CMYK fill.

	CMYKColor getCMYKStroke() const;
		/// Returns current CMYK stroke.

	void setCMYKStroke(CMYKColor value);
		/// Returns current CMYK stroke.

	float getGreyFill() const;
		/// Returns current grey fill.

	void setGreyFill(float value);
		/// Sets current grey fill.

	float getGreyStroke() const;
		/// Returns current grey stroke.

	void setGreyStroke(float value);
		/// Sets current grey stroke.

	ColorSpace getStrokeColorSpace() const;
		/// Returns current stroking color space.

	ColorSpace getFillColorSpace() const;
		/// Returns current filling color space.

	void setSlideShow(TransitionStyle type, float displayTime, float transitionTime);
		/// Configures the setting for slide transition of the page

private:
	Page();

	Document*                _pDocument;
	HPDF_Page                _page;
	Size                     _size;
	Orientation              _orientation;
	DestinationContainer    _destinations;
	TextAnnotationContainer _textAnnotations;
	LinkAnnotationContainer _linkAnnotations;
	mutable Font*           _pCurrentFont;
};


} } // namespace Poco::PDF


#endif // PDF_Page_INCLUDED
