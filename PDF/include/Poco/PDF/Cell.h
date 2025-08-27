//
// Cell.h
//


#ifndef PDF_Cell_INCLUDED
#define PDF_Cell_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/PDF/Page.h"
#include "Poco/PDF/AttributedString.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace Poco {
namespace PDF {


class PDF_API Cell
{
public:
	using Ptr = SharedPtr<Cell>;
	using FontMap = std::map<int, std::string>;
	using FontMapPtr = SharedPtr<FontMap>;

	enum Outline
	{
		OUTLINE_NONE = 0,
		OUTLINE_LEFT = 1,
		OUTLINE_TOP = 2,
		OUTLINE_RIGHT = 4,
		OUTLINE_BOTTOM = 8
	};

	Cell(const AttributedString& content = "",
		 const std::string& name = "",
		 FontMapPtr pFontMap = nullptr);
	Cell(const AttributedString& content, FontMapPtr pFontMap, const std::string& encoding = "UTF-8" , bool trueType = true, int widthAsPct=-1);
	~Cell();

	const std::string& getName() const;
	void setName(const std::string& name);
	const AttributedString& getContent() const;
	void setContent(const AttributedString& content);
	unsigned getOutline() const;
	void setOutline(Outline outline, bool show = true);
	void borderLeft(bool show = true);
	void borderTop(bool show = true);
	void borderRight(bool show = true);
	void borderBottom(bool show = true);
	void borderTopBottom(bool show = true);
	void borderLeftRight(bool show = true);
	void borderAll(bool show = true);
	float getLineWidth() const;
	void setLineWidth(float width);
	void setFonts(FontMapPtr pFontMap);
	FontMapPtr getFonts() const { return _pFontMap; }
	void draw(Page& page, float x, float y, float width, float height);
	int getWidthAsPct() const;
	void setWidthAsPct(int width);
	bool hasWidth() const;

private:
	AttributedString   _content;
	std::string        _name;
	unsigned           _outline;
	float              _lineWidth;
	FontMapPtr         _pFontMap;
	std::string        _encoding;
	bool               _trueType;
	int                _widthAsPct;
};

using TableRow = std::vector<Cell>;

} } // namespace Poco::PDF

#endif // PDF_Cell_INCLUDED
