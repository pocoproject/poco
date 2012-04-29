//
// Text.cpp
//
// $Id: //poco/Main/PDF/samples/Text/src/Text.cpp#1 $
//
// This sample demonstrates the Data library.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics
// Software Engineering GmbH.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part, without
// prior written permission from Applied Informatics.
//

#include "Poco/PDF/Document.h"
#include "Poco/Path.h"
#include "Poco/File.h"


#if defined(POCO_OS_FAMILY_UNIX)
const std::string fileName = "${POCO_BASE}/PDF/samples/Text/Text.pdf";
#elif defined(POCO_OS_FAMILY_WINDOWS)
const std::string fileName = "%POCO_BASE%/PDF/samples/Text/Text.pdf";
#endif


using Poco::PDF::Document;
using Poco::PDF::Font;
using Poco::PDF::Page;
using Poco::Path;
using Poco::File;


int main(int argc, char** argv)
{
	File file(Path::expand(fileName));
	if (file.exists()) file.remove();

	Document document(file.path());

	Font helv = document.font("Helvetica");
	Page page = document[0];
	page.setFont(helv, 24);
	std::string hello = "Hello PDF World from C++ Portable Components";
	float tw = page.textWidth(hello);
	page.writeOnce((page.getWidth() - tw) / 2, page.getHeight() - 50, hello);

	document.save();
	return 0;
}
