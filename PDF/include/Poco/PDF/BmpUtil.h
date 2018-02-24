//
// BmpUtil.h
//
// Library: PDF
// Package: PDFCore
// Module:  BmpUtil
//
// Definition of the utiity finctions and classes.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#ifndef PDF_BmpUtil_INCLUDED
#define PDF_BmpUtil_INCLUDED

#include "hpdf_doc.h"

namespace Poco {
	namespace PDF {

		HPDF_Image LoadBMPImageFromFile(HPDF_Doc pdf,	const char    *filename);

	}
} // namespace Poco::PDF


#endif // PDF_BmpUtil_INCLUDED
