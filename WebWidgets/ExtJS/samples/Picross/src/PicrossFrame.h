// PicrossFrame
// Copyright 2008 by Peter Schojer


#ifndef PicrossFrame_H_INCLUDED
#define PicrossFrame_H_INCLUDED


#include "Poco/WebWidgets/Frame.h"
#include "Poco/WebWidgets/Page.h"
#include "Poco/WebWidgets/Table.h"
#include "Poco/WebWidgets/GridLayout.h"
#include "RAWReader.h"


class PicrossFrame: public Poco::WebWidgets::Frame
{
public:
	typedef Poco::AutoPtr<PicrossFrame> Ptr;
	enum
	{
		PI_CELLWIDTH = 22,
		PI_CELLHEIGHT = 22
	};

	PicrossFrame(const Poco::File& bmp);
	
	void setupJavaScript(Poco::WebWidgets::Page* pPage);

private:
	void createComponents();
	void initComponents();
	void createUpperTable();
	void createLeftTable();
	void createGameTable();

	void initGameTable();
	void initUpperTable();
	void initLeftTable();

private:
	RAWReader                         _bmp;
	Poco::WebWidgets::Table::Ptr      _pUpperTable;
	Poco::WebWidgets::Table::Ptr      _pLeftTable;
	Poco::WebWidgets::Table::Ptr      _pGameTable;
	Poco::WebWidgets::TableModel::Ptr _pUpperStore;
	Poco::WebWidgets::TableModel::Ptr _pLeftStore;
	Poco::WebWidgets::TableModel::Ptr _pGameStore;
};

#endif