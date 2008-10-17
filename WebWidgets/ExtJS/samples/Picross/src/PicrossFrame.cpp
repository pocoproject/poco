#include "PicrossFrame.h"
#include "Poco/WebWidgets/NumberField.h"
#include "Poco/WebWidgets/NumberFieldCell.h"
#include "Poco/WebWidgets/ImageButtonCell.h"
#include "Poco/WebWidgets/TextFieldCell.h"
#include "Poco/WebWidgets/SimpleTableModel.h"
#include "Poco/WebWidgets/VerticalLayout.h"
#include "Poco/WebWidgets/HorizontalLayout.h"
#include "Poco/NumberFormatter.h"
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
	createUpperLeftComponents(pRoot);
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
	//already initialized initLeftTable();
	initUpperTable();
}


void PicrossFrame::setupJavaScript(Page* pPage)
{
	{
		std::ostringstream str;
		str <<	"function updateGlobalCntRecord(rec){" << std::endl;
		str <<		"var i = 0;" << std::endl;
		str <<		"var val = 1;" << std::endl;
		str <<		"for (i = 0; i < global.cols; ++i){" << std::endl;
		str <<			"val = rec.get(i.toString());" << std::endl;
		str <<			"if (val == 'O') ++global.cnt;" << std::endl;
		str <<		"}" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}
	
	{
		std::ostringstream str;
		str <<	"function updateGlobalCnt(table){" << std::endl;
		str <<		"global.cols = table.getColumnModel().getColumnCount() - 1;" << std::endl;
		str <<		"global.cnt = 0;" << std::endl;
		str <<		"table.store.each(updateGlobalCntRecord);" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}
	{
		std::ostringstream str;
		str <<	"function getImgClass(el) {" << std::endl;
		str <<		"var elem = Ext.Element.fly(el);" << std::endl;
		str <<		"var img = elem.child('.img', true);" << std::endl;
		str <<		"if (!img) return 'default.png';" << std::endl;
		str <<		"var result = img.attributes.getNamedItem('src');" << std::endl;
		str <<		"return result.value;" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}
	{
		std::ostringstream str;
		str <<	"function checkSuccess() {" << std::endl;
		str <<		"if (global.cnt == 0){" << std::endl;
		str <<			"stopClock();" << std::endl;
		str <<			"var err = parseInt(Ext.getCmp('" << _pErrors->id() << "').text);" << std::endl;
		str <<			"Ext.getCmp('" << _pGameTable->id() << "').disable();" << std::endl;
		str <<			"if (err == 0) " << std::endl;
		str <<				"Ext.Msg.alert('Success', 'Congratulations. Picross solved without a single error');" << std::endl;
		str <<			"else if (err == 1) " << std::endl;
		str <<				"Ext.Msg.alert('Success', 'Congratulations. Picross solved with one single error');" << std::endl;
		str <<			"else if (err <= 3) " << std::endl;
		str <<				"Ext.Msg.alert('Success', 'Congratulations. Picross solved with ' + err + ' errors');" << std::endl;
		str <<			"else " << std::endl;
		str <<				"Ext.Msg.alert('Success', 'Picross Solved. Too many errors though.');" << std::endl;
		str <<		"}" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}
	{
		std::ostringstream str;
		str << "function handleClick(elem, val) {" << std::endl;
		str <<		"var oldImgClass = getImgClass(elem);" << std::endl;
		str <<		"var defaultHTML = '<img class=\"img\" src=\"default.png\" width=\"";
		str <<				"16\" height=\"13" << "\" border=\"0\" alt=\"\" />';" << std::endl;
		str <<		"var okHTML = '<img class=\"img\" src=\"ok.png\" width=\"";
		str <<				"16\" height=\"13" << "\" border=\"0\" alt=\"\" />';" << std::endl;
		str <<		"var xHTML = '<img class=\"img\" src=\"x.png\" width=\"";
		str <<				"16\" height=\"13" << "\" border=\"0\" alt=\"\" />';" << std::endl;
		str <<		"if (oldImgClass == 'x.png'){" << std::endl;
		str <<			"if (global.selectMode){" << std::endl;
		str <<				"elem.innerHTML = defaultHTML;" << std::endl;
		str <<			"}" << std::endl;
		str <<		"}" << std::endl;
		str <<		"else if (oldImgClass == 'ok.png'){" << std::endl;
		str <<			"if (!global.selectMode){" << std::endl;
		str <<				"elem.innerHTML = xHTML;" << std::endl;
		str <<				"global.cnt++;" << std::endl;
		str <<			"}" << std::endl;
		str <<		"}" << std::endl;
		str <<		"else {" << std::endl; // default.png
		str <<			"if (val == 'O'){" << std::endl;
		str <<				"if (global.selectMode){" << std::endl;
		str <<					"global.cnt--;" << std::endl;
		str <<					"elem.innerHTML = okHTML;" << std::endl;
		str <<				"}" << std::endl;
		str <<				"else {" << std::endl;
		str <<					"elem.innerHTML = xHTML;" << std::endl;
		str <<				"}" << std::endl;
		str <<			"}" << std::endl;
		str <<			"else {" << std::endl;
		str <<				"elem.innerHTML = xHTML;" << std::endl;
		str <<				"if (global.selectMode){" << std::endl;
		str <<					"wrongClick();" << std::endl;
		str <<				"}" << std::endl;
		str <<			"}" << std::endl;
		str <<		"}" << std::endl;
		str <<		"checkSuccess();" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}
	{
		std::ostringstream str;
		str <<	"function setImageURI(e, uri){" << std::endl;
		str <<		"if (!e) return;" << std::endl;
		str <<		"var img = e.child('.img', 'true');" << std::endl;
		str <<		"var attr = document.createAttribute('src');" << std::endl;
		str <<		"attr.value = uri;" << std::endl;
		str <<		"img.attributes.setNamedItem(attr);" << std::endl;
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
		str <<		"handleClick(html.firstChild, val);" << std::endl;
		str <<	"}" << std::endl;
		_pGameTable->cellClicked.add(jsDelegate(str.str()));
	}

	{
		//pPage->appendPostRenderCode("global.cnt = " + Poco::NumberFormatter::format(_cnt) + ";");
		std::ostringstream str;
		str <<	"function(){" << std::endl;
		str <<		"var table = Ext.getCmp('" << _pGameTable->id() << "');" << std::endl;
		str <<		"updateGlobalCnt(table);" << std::endl;
		str <<	"}" << std::endl;
		_pGameTable->afterLoad.add(jsDelegate(str.str()));
	}
	{
		std::ostringstream str;
		str << "function updateClock(){" << std::endl;
		str <<		"var tDate = new Date();" << std::endl;
		str <<		"if (!global.puzzleStart)" << std::endl;
		str <<			"global.puzzleStart = new Date();" << std::endl;
		str <<		"var tDiff = Math.floor((tDate.getTime() - global.puzzleStart.getTime())/1000);" << std::endl;
		str <<		"var h = Math.floor(tDiff/3600);" << std::endl;
		str <<		"if (h<10) h = '0' + h.toString();" << std::endl;
		str <<		"var m = Math.floor((tDiff%3600)/60);" << std::endl;
		str <<		"if (m<10) m = '0' + m.toString();" << std::endl;
		str <<		"var s = tDiff%60;" << std::endl;
		str <<		"if (s<10) s = '0' + s.toString();" << std::endl;
		str <<		"var value = h + ':' + m + ':' + s;" << std::endl;
		str <<		"Ext.getCmp('" << _pTime->id() << "').setText(value);" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
		pPage->appendPostRenderCode("global.puzzleClock = window.setInterval(updateClock, 1000);");
	}
	{
		std::ostringstream str;
		str << "function stopClock(){" << std::endl;
		str <<		"window.clearInterval(global.puzzleClock);" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}

	{
		std::ostringstream str;
		str << "function wrongClick(){" << std::endl;
		str <<		"var err = Ext.getCmp('" << _pErrors->id() << "');" << std::endl;
		str <<		"var val = parseInt(err.text);" << std::endl;
		str <<		"err.setText(1+val);" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
	}

	{
		std::ostringstream str;
		str <<	"function onCancelClicked(){" << std::endl;
		str <<		"if (!global.selectMode) return;" << std::endl;
		str <<		"global.selectMode = false;" << std::endl;
		str <<		"var cancelElem = Ext.get('" << _pCancel->id() << "');" << std::endl;
		str <<		"var selectElem = Ext.get('" << _pSelect->id() << "');" << std::endl;
		str <<		"setImageURI(cancelElem, 'cancel.png');" << std::endl;
		str <<		"setImageURI(selectElem, 'selectGrey.png');" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
		_pCancel->buttonClicked.add(jsDelegate("onCancelClicked()"));
	}

	{
		std::ostringstream str;
		str <<	"function onSelectClicked(){" << std::endl;
		str <<		"if (global.selectMode) return;" << std::endl;
		str <<		"global.selectMode = true;" << std::endl;
		str <<		"var cancelElem = Ext.get('" << _pCancel->id() << "');" << std::endl;
		str <<		"var selectElem = Ext.get('" << _pSelect->id() << "');" << std::endl;
		str <<		"setImageURI(cancelElem, 'cancelGrey.png');" << std::endl;
		str <<		"setImageURI(selectElem, 'select.png');" << std::endl;
		str <<	"}" << std::endl;
		pPage->addDynamicFunction(str.str());
		_pSelect->buttonClicked.add(jsDelegate("onSelectClicked()"));
		pPage->appendPostRenderCode("global.selectMode = true;");
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
	//_pUpperTable->setHeight(h*(PI_CELLHEIGHT));
	_pUpperTable->hideHeaders(true);
}


void PicrossFrame::createLeftTable()
{
	// we create the left grid later when we have the data!
	initLeftTable();
	
}


void PicrossFrame::createGameTable()
{
	Table::TableColumns tc;
	Image::Ptr pImg(new Image(Poco::URI("default.png")));
	pImg->setClass("img");
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
	int cnt(0);
	for (int x = 0; x < _bmp.width(); ++x)
	{
		for (int y = 0; y < _bmp.height(); ++y)
		{
			std::string val;
			if (_bmp.get(x, y))
			{
				++cnt;
				val = "O";
			}
			_pGameStore->setValue(val, y, x);
		}
	}
	_cnt = cnt;
}


void PicrossFrame::initUpperTable()
{
	// count consecutive blocks in columns
	std::size_t maxRows(3); // make at least three rows, otherwise GUI looks ugly
	std::vector<std::vector<int> > cols;
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

		cols.push_back(cnt);
		if (maxRows < cnt.size())
			maxRows = cnt.size();
	}

	for (int j = 0; j < cols.size(); ++j)
	{
		const std::vector<int>& cnt = cols[j];
		std::size_t offset = maxRows - cnt.size();
		for (int i = 0; i < cnt.size(); ++i)
			_pUpperTable->setValue(cnt[i], offset+ i, j);
	}
	_pUpperTable->setHeight(_pUpperTable->getRowCount()*(PI_CELLHEIGHT));
}


void PicrossFrame::initLeftTable()
{
	// count consecutive blocks in rows

	std::vector<std::vector<int> > rows;
	std::size_t maxCols(5); // make at least five cols, otherwise GUI looks ugly
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

		rows.push_back(cnt);
	}

	
	int h = _bmp.height();
	int w = static_cast<int>(maxCols);;
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
	for (int j = 0; j < rows.size(); ++j)
	{
		const std::vector<int>& cnt = rows[j];
		std::size_t offset = maxCols - cnt.size();
		for (int i = 0; i < cnt.size(); ++i)
			_pLeftTable->setValue(cnt[i], j, offset+i);
	}
}


void PicrossFrame::createUpperLeftComponents(GridLayout::Ptr pRoot)
{
	VerticalLayout::Ptr pVert(new VerticalLayout(2));
	GridLayout::Ptr pGrid(new GridLayout(2,2));
	pGrid->setHorizontalPadding(2);
	Label::Ptr pTmp(new Label("", "Time:"));
	pTmp->setWidth(60);
	pGrid->add(pTmp);
	
	_pTime = new Label("", "00:00:00");
	pGrid->add(_pTime);

	pTmp = new Label("", "Errors:");
	pGrid->add(pTmp);
	_pErrors = new Label("", "0");
	_pErrors->setWidth(60);
	pGrid->add(_pErrors);
	pVert->add(pGrid);
	HorizontalLayout::Ptr pHor(new HorizontalLayout(10));
	Image::Ptr pSelectImg(new Image(Poco::URI("select.png")));
	pSelectImg->setClass("img");
	Image::Ptr pCancelImg(new Image(Poco::URI("cancelgrey.png")));
	pCancelImg->setClass("img");
	_pSelect = new ImageButton("select", pSelectImg);
	_pSelect->setText("");
	_pSelect->enable(false);
	_pCancel = new ImageButton("cancel", pCancelImg);
	_pCancel->setText("");
	
	pHor->add(_pSelect);
	pHor->add(_pCancel);
	pVert->add(pHor);
	pRoot->add(pVert);
}