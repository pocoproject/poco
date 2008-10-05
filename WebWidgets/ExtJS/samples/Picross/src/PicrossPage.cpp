#include "PicrossPage.h"


PicrossPage::PicrossPage()
{
	setWidth(800);
	setHeight(600);
}

	
void PicrossPage::createComponents()
{
	_pFrame = new PicrossFrame(Poco::File("x_16x16.raw"));
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
