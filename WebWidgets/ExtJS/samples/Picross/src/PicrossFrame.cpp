#include "PicrossFrame.h"
#include "Poco/WebWidgets/NumberField.h"
#include "Poco/WebWidgets/NumberFieldCell.h"
#include "Poco/WebWidgets/ImageButtonCell.h"
#include "Poco/WebWidgets/TextFieldCell.h"
#include "Poco/WebWidgets/SimpleTableModel.h"
#include "Poco/WebWidgets/Label.h"
#include "Poco/WebWidgets/VerticalLayout.h"
#include <sstream>


using namespace Poco::WebWidgets;


PicrossFrame::PicrossFrame(const Poco::File& bmp):
	_bmp(bmp)
{
	setWidth(800);
	setHeight(700);
	createComponents();
	initComponents();
}


void PicrossFrame::createComponents()
{
	GridLayout::Ptr pRoot(new GridLayout(2, 2));
	createUpperTable();
	createLeftTable();
	createGameTable();
	pRoot->set(_pUpperTable, 0, 1);
	pRoot->set(_pLeftTable, 1, 0);
	pRoot->set(_pGameTable, 1, 1);
	this->setChild (pRoot);
}


void PicrossFrame::initComponents()
{
	initGameTable();
	initLeftTable();
	initUpperTable();
}


void PicrossFrame::setupJavaScript(Page* pPage)
{
	{
		std::ostringstream str;
		str << "function handleClick(elem, val, guessSet) {" << std::endl;
		str <<		"if (val == 'O'){" << std::endl;
		str <<			"elem.innerHTML = '<img src=\"ok.png\" width=\"";
		str <<				"16\" height=\"13" << "\" border=\"0\" alt=\"\" />';" << std::endl;
		str <<		"}" << std::endl;
		str <<		"else {" << std::endl;
		str <<			"elem.innerHTML = '<img src=\"x.png\" width=\"";
		str <<				"16\" height=\"13" << "\" border=\"0\" alt=\"\" />';" << std::endl;
		str <<			"if (guessSet){" << std::endl; // means the user guesses there is an entry
		str <<				"if (val != 'O') {" << std::endl;
		str <<					"Ext.Msg.alert('Error', 'You guessed wrong');" << std::endl;
		str <<				"}" << std::endl;
		str <<			"}" << std::endl;
		str <<			"else {" << std::endl;
		str <<				"if (val == 'O') {" << std::endl;
		str <<					"Ext.Msg.alert('Error', 'You guessed wrong');" << std::endl;
		str <<				"}" << std::endl;
		str <<			"}" << std::endl;
		str <<		"}" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}
	{
		std::ostringstream str;
		str <<	"function renderColumn(val){" << std::endl;
		str <<		"var retVal = '<img src=\"default.png\" width=\"";
		str <<			"16\" height=\"13" << "\" border=\"0\" alt=\"\" ';"<< std::endl;
		str <<		"retVal += '/>';" << std::endl;
		str <<		"return retVal;" << std::endl;
		str <<	"}"<< std::endl;
		pPage->addDynamicFunction(str.str());
	}
	{
		// setup cellclick event
		//cellclick : ( Grid this, Number rowIndex, Number columnIndex, Ext.EventObject e )
		std::ostringstream str;
		str << "function(grid, row, col, e) {" << std::endl;
		str <<		"var rec = grid.store.getAt(row);" << std::endl;
		str <<		"var val = rec.get(''+col);" << std::endl;
		str <<		"var html = grid.getView().getCell(row,col);" << std::endl;
		str <<		"handleClick(html.firstChild, val, true);" << std::endl;
		str <<	"}" << std::endl;
		_pGameTable->cellClicked.add(jsDelegate(str.str()));
	}
}


void PicrossFrame::createUpperTable()
{
	// the upper grid can have at most (height+1)/2 entries
	int w = _bmp.width();
	int h = (_bmp.height()+1)/2;
	Table::TableColumns tc;
	for (int i = 0; i < w; ++i)
	{
		TableColumn::Ptr pCol = new TableColumn(new NumberFieldCell(0), "", PI_CELLWIDTH, false);
		pCol->resizable(false);
		pCol->setEditable(false);
		tc.push_back(pCol);
	}
	
	_pUpperStore = new SimpleTableModel(tc.size());
	_pUpperTable = new Table(tc, _pUpperStore);
	_pUpperTable->setWidth(w*(PI_CELLWIDTH+2));
	_pUpperTable->setHeight(h*(PI_CELLHEIGHT));
	_pUpperTable->hideHeaders(true);
}


void PicrossFrame::createLeftTable()
{
	// the left grid can have at most (width+1)/2 entries
	int h = _bmp.height();
	int w = (_bmp.width()+1)/2;
	Table::TableColumns tc;
	for (int i = 0; i < w; ++i)
	{
		TableColumn::Ptr pCol = new TableColumn(new NumberFieldCell(0), "", PI_CELLWIDTH, false);
		pCol->resizable(false);
		pCol->setEditable(false);
		tc.push_back(pCol);
	}
	_pLeftStore = new SimpleTableModel(tc.size());
	_pLeftTable = new Table(tc, _pLeftStore);
	_pLeftTable->setWidth(w*(PI_CELLWIDTH+1));
	_pLeftTable->setHeight(h*(PI_CELLHEIGHT));
	_pLeftTable->hideHeaders(true);
}


void PicrossFrame::createGameTable()
{
	Table::TableColumns tc;
	Image::Ptr pImg(new Image(Poco::URI("default.png")));
	for (int i = 0; i < _bmp.width(); ++i)
	{
		TableColumn::Ptr pCol = new TableColumn(new TextFieldCell(0), "", PI_CELLWIDTH, false);
		pCol->resizable(false);
		pCol->setEditable(false);
		pCol->setCustomRenderer("renderColumn");
		tc.push_back(pCol);
	}

	_pGameStore = new SimpleTableModel(tc.size());
	_pGameTable = new Table(tc, _pGameStore);
	_pGameTable->setWidth(_bmp.width()*(PI_CELLWIDTH+2));
	_pGameTable->setHeight(_bmp.height()*(PI_CELLHEIGHT));
	_pGameTable->hideHeaders(true);
	_pGameTable->showBorder(false);
	_pGameTable->autoEdit(true);
}


void PicrossFrame::initGameTable()
{
	for (int x = 0; x < _bmp.width(); ++x)
	{
		for (int y = 0; y < _bmp.height(); ++y)
		{
			std::string val;
			if (_bmp.get(x, y))
				val = "O";
			_pGameStore->setValue(val, y, x);
		}
	}
}


void PicrossFrame::initUpperTable()
{
	int rows = (_bmp.height()+1)/2;
	// count consecutive blocks in columns
	for (int x = 0; x < _bmp.width(); ++x)
	{
		bool lastVal(false);
		int lastCnt(0);
		std::vector<int> cnt;
		for (int y = 0; y < _bmp.height(); ++y)
		{
			if (lastVal)
			{
				lastVal =_bmp.get(x, y);
				if (lastVal) ++lastCnt;
				else
				{
					cnt.push_back(lastCnt);
					lastCnt = 0;
				}
			}
			else
			{
				lastVal =_bmp.get(x, y);
				if (lastVal) lastCnt = 1;
			}
		}
		if (lastCnt > 0)
			cnt.push_back(lastCnt);
		
		int offset = rows - cnt.size();
		for (int i = 0; i < cnt.size(); ++i)
			_pUpperTable->setValue(cnt[i], i+offset, x);
	}
}


void PicrossFrame::initLeftTable()
{
	// count consecutive blocks in rows
	for (int y = 0; y < _bmp.height(); ++y)
	{
		bool lastVal(false);
		int lastCnt(0);
		std::vector<int> cnt;
		for (int x = 0; x < _bmp.width(); ++x)
		{
			if (lastVal)
			{
				lastVal =_bmp.get(x, y);
				if (lastVal) ++lastCnt;
				else
				{
					cnt.push_back(lastCnt);
					lastCnt = 0;
				}
			}
			else
			{
				lastVal =_bmp.get(x, y);
				if (lastVal) lastCnt = 1;
			}
		}
		if (lastCnt > 0)
			cnt.push_back(lastCnt);

		int cols = (_bmp.width()+1)/2;
		int offset = cols - cnt.size();
		for (int i = 0; i < cnt.size(); ++i)
			_pLeftTable->setValue(cnt[i], y, offset + i);
	}
}