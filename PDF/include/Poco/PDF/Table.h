//
// Table.h
//


#ifndef PDF_Table_INCLUDED
#define PDF_Table_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/PDF/Page.h"
#include "Poco/PDF/Cell.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace Poco {
namespace PDF {


class PDF_API Table
{
public:
	using Ptr = SharedPtr<Table>;
	using Cells = std::vector<TableRow>;

	Table(int columnCount, int rowCount, const std::string& name, Cell::FontMapPtr pFontMap = nullptr);
	~Table();

	void setCell(int col, int row, const Cell& cell);
	void setColumnWidth(int col, double width);
	void setFonts(Cell::FontMapPtr pFontMap);

	const std::string name() const;
	const Cells& cells() const;

	void addRow();
	void addRow(const TableRow& row);

	std::size_t rows() const;
	std::size_t columns() const;

	void draw(Page& page, float x, float y, float width, float height);

private:
	Table();

	std::string      _name;
	Cells            _cells;
	Cell::FontMapPtr _pFontMap;
};


} } // namespace Poco::PDF


#endif // PDF_Table_INCLUDED
