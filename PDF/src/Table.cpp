//
// Table.cpp
//

#include "Poco/PDF/Table.h"
#include <hpdf.h>

namespace Poco {
namespace PDF {


Table::Table(int columnCount, int rowCount, const std::string& name, Cell::FontMapPtr pFontMap):
	_name(name), _cells(rowCount, TableRow(columnCount))
{
	setFonts(pFontMap);
}

Table::~Table() = default;

const std::string Table::name() const
{
	return _name;
}


const Table::Cells& Table::cells() const
{
	return _cells;
}


std::size_t Table::rows() const
{
	return _cells.size();
}


std::size_t Table::columns() const
{
	return _cells[0].size();
}

void Table::addRow()
{
	_cells.emplace_back(columns());
}


void Table::addRow(const TableRow& row)
{
	if (_cells.empty())
	{
		_cells.push_back(row);
	}
	else
	{
		_cells.push_back(row);
		_cells.back().resize(_cells.front().size());
	}
}


void Table::setCell(int col, int row, const Cell& cell)
{
	_cells[row][col] = cell;
	if (_pFontMap && !cell.getFonts()) _cells[row][col].setFonts(_pFontMap);
}


void Table::setColumnWidth(int col, double width)
{
}


void Table::setFonts(Cell::FontMapPtr pFontMap)
{
	_pFontMap = pFontMap;
	if (_pFontMap) { poco_assert(_pFontMap->size() == 4); }
}


void Table::draw(Page& page, float x, float y, float width, float height)
{
	if (_cells.size())
	{
		int rows = static_cast<int>(_cells.size());
		int cols = static_cast<int>(_cells[0].size());
		int r = 0;
		for (auto& row : _cells)
		{
			float h = height / rows;
			int c = 0;
			float lastX = x;
			for (auto& cell : row)
			{
				float w = width / cols;
				if (!cell.hasWidth())
				{
					cell.draw(page, x + (w * c), y - (h * r), w, h);
					lastX += (w * c);
				}
				else
				{
					w = width * cell.getWidthAsPct() / 100.0f;
					cell.draw(page, lastX, y - (h * r), w, h);
					lastX += w;
				}
				++c;
			}
			++r;
		}
	}
}


} } // namespace Poco::PDF
