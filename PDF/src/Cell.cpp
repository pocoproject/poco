//
// Cell.cpp
//

#include "Poco/PDF/Cell.h"


namespace Poco {
namespace PDF {


Cell::Cell(const AttributedString& content, const std::string& name, FontMapPtr pFontMap):
	_content(content),
	_name(name),
	_outline(OUTLINE_NONE),
	_lineWidth(1.0f),
	_encoding("UTF-8"),
	_trueType(true),
	_widthAsPct(-1)
{
	setFonts(pFontMap);
}


Cell::Cell(const AttributedString& content, FontMapPtr pFontMap, const std::string& encoding, bool trueType, int widthAsPct):
	_content(content),
	_outline(OUTLINE_NONE),
	_lineWidth(1.0f),
	_encoding(encoding),
	_trueType(trueType),
	_widthAsPct(widthAsPct)
{
	setFonts(pFontMap);
}

Cell::~Cell() = default;

const std::string& Cell::getName() const
{
	return _name;
}


void Cell::setName(const std::string& name)
{
	_name = name;
}


const AttributedString& Cell::getContent() const
{
	return _content;
}


void Cell::setContent(const AttributedString& content)
{
	_content = content;
}


unsigned Cell::getOutline() const
{
	return _outline;
}


void Cell::setOutline(Cell::Outline outline, bool show)
{
	if (show) _outline |= outline;
	else      _outline &= ~outline;
}


void Cell::borderLeft(bool show)
{
	setOutline(OUTLINE_LEFT, show);
}


void Cell::borderTop(bool show)
{
	setOutline(OUTLINE_TOP, show);
}


void Cell::borderRight(bool show)
{
	setOutline(OUTLINE_RIGHT, show);
}


void Cell::borderBottom(bool show)
{
	setOutline(OUTLINE_BOTTOM, show);
}


float Cell::getLineWidth() const
{
	return _lineWidth;
}


void Cell::setLineWidth(float width)
{
	_lineWidth = width;
}


int Cell::getWidthAsPct() const
{
	return _widthAsPct;
}


void Cell::setWidthAsPct(int width)
{
	_widthAsPct = width;
}


bool Cell::hasWidth() const
{
	return _widthAsPct > 0;
}


void Cell::setFonts(FontMapPtr pFontMap)
{
	_pFontMap = pFontMap;
	poco_assert(!_pFontMap || (_pFontMap->size() == 4));
}


void Cell::borderTopBottom(bool show)
{
	borderTop(show);
	borderBottom(show);
}


void Cell::borderLeftRight(bool show)
{
	borderLeft(show);
	borderRight(show);
}


void Cell::borderAll(bool show)
{
	borderTop(show);
	borderBottom(show);
	borderLeft(show);
	borderRight(show);
}


void Cell::draw(Page& page, float x, float y, float width, float height)
{
	// uncomment to force showing of the cell outline regardless of settings
	//_outline = 15;

	if (_outline != OUTLINE_NONE)
	{
		page.setLineWidth(_lineWidth);
		page.moveTo(x, y);
		if (_outline & OUTLINE_LEFT  ) page.lineTo(x,       y+height);
		page.moveTo(x, y+height);
		if (_outline & OUTLINE_TOP   ) page.lineTo(x+width, y+height);
		page.moveTo(x+width, y+height);
		if (_outline & OUTLINE_RIGHT ) page.lineTo(x+width, y       );
		page.moveTo(x+width, y);
		if (_outline & OUTLINE_BOTTOM) page.lineTo(x,       y       );
		page.stroke();
	}

	std::string text = _content;
	if (!text.empty())
	{
		Font originalFont = page.getFont();
		float originalSize = page.getFontSize();
		try
		{
			// font names hard coded here, needs more work on the library level
			int fontStyle = _content.getAttribute(AttributedString::ATTR_STYLE);
			float fontSize = _content.getAttribute(AttributedString::ATTR_SIZE);
			if (fontStyle == AttributedString::STYLE_PLAIN)
			{
				if (!_pFontMap)     page.setFont("Helvetica", fontSize);
				else if (_trueType) page.setTTFont((*_pFontMap)[AttributedString::STYLE_PLAIN], fontSize, _encoding);
				else                page.setFont((*_pFontMap)[AttributedString::STYLE_PLAIN], fontSize, _encoding);
			}
			else if (fontStyle & AttributedString::STYLE_BOLD)
			{
				if (!_pFontMap)     page.setFont("Helvetica-Bold", fontSize);
				else if (_trueType) page.setTTFont((*_pFontMap)[AttributedString::STYLE_BOLD], fontSize, _encoding);
				else                page.setFont((*_pFontMap)[AttributedString::STYLE_BOLD], fontSize, _encoding);
			}
			else if (fontStyle & AttributedString::STYLE_ITALIC)
			{
				if (!_pFontMap)     page.setFont("Helvetica-Oblique", fontSize);
				else if (_trueType) page.setTTFont((*_pFontMap)[AttributedString::STYLE_ITALIC], fontSize, _encoding);
				else                page.setFont((*_pFontMap)[AttributedString::STYLE_ITALIC], fontSize, _encoding);
			}
			else if ((fontStyle & AttributedString::STYLE_BOLD) && (fontStyle & AttributedString::STYLE_ITALIC))
			{
				if (!_pFontMap)     page.setFont("Helvetica-BoldOblique", fontSize);
				else if (_trueType) page.setTTFont((*_pFontMap)[AttributedString::STYLE_BOLD | AttributedString::STYLE_ITALIC], fontSize, _encoding);
				else                page.setFont((*_pFontMap)[AttributedString::STYLE_BOLD | AttributedString::STYLE_ITALIC], fontSize, _encoding);
			}
			else
			{
				throw NotFoundException(format("Unknown font style: %d", fontStyle));
			}

			float tw = page.textWidth(text);
			// trim text that does not fit
			while (tw > width && text.size())
			{
				text = text.substr(0, text.size() - 1);
				tw = page.textWidth(text);
			}
			float th = page.getFontSize();
			float yPos = (height <= th) ? y : y + (height - th) / 2;
			int align = _content.getAttribute(AttributedString::ATTR_ALIGN);
			switch (align)
			{
			case AttributedString::ALIGN_LEFT:
				page.writeOnce(x + 5, yPos, text); break;
			case AttributedString::ALIGN_CENTER:
				page.writeOnce(x + (width - tw) / 2, yPos, text); break;
				break;
			case AttributedString::ALIGN_RIGHT:
				page.writeOnce(x + (width - tw), yPos, text); break;
				break;
			default:
				throw NotFoundException(format("Unknown alignment mode: %d", align));
			}
		}
		catch (Poco::Exception&)
		{
			page.setFont(originalFont, originalSize);
			throw;
		}
		page.setFont(originalFont, originalSize);
	}
}


} } // namespace Poco::PDF
