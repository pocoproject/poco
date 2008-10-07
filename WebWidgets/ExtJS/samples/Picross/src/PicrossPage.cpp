#include "PicrossPage.h"


PicrossPage::PicrossPage(const Poco::Path& dataRoot):
	_dataRoot(dataRoot)
{
	setWidth(800);
	setHeight(600);
}

	
void PicrossPage::createComponents()
{
	Poco::Path file(_dataRoot);
	file.makeDirectory();
	file.setFileName("x_16x16.raw");
	_pFrame = new PicrossFrame(Poco::File(file));
	add (_pFrame);
	setupJavaScript();
}


void PicrossPage::initComponents()
{
}

void PicrossPage::setupJavaScript()
{
	_pFrame->setupJavaScript(this);
}
