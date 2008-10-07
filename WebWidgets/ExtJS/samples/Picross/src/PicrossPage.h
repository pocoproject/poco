// PicrossPage
// Copyright 2008 by Peter Schojer


#ifndef PicrossPage_H_INCLUDED
#define PicrossPage_H_INCLUDED


#include "Poco/WebWidgets/Page.h"
#include "Poco/Path.h"
#include "PicrossFrame.h"

class PicrossPage: public Poco::WebWidgets::Page
{
public:
	typedef Poco::AutoPtr<PicrossPage> Ptr;
	PicrossPage(const Poco::Path& dataRoot);
	
	void createComponents();
	void initComponents();

private:
	void setupJavaScript();

private:
	PicrossFrame::Ptr _pFrame;
	Poco::Path _dataRoot;
};

#endif