//
// PDF.h
//
// Library: PDF
// Package: PDFCore
// Module:  PDF
//
// Basic definitions for the Poco PDF library.
// This file must be the first file included by every other PDF
// header file.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDF_PDF_INCLUDED
#define PDF_PDF_INCLUDED

#if defined(_MSC_VER) && !defined(POCO_MSVC_SECURE_WARNINGS) && (!defined(_CRT_SECURE_NO_WARNINGS) || !defined(_CRT_SECURE_NO_DEPRECATE))
	#ifndef _CRT_SECURE_NO_WARNINGS
		#define _CRT_SECURE_NO_WARNINGS
	#endif
	#ifndef _CRT_SECURE_NO_DEPRECATE
		#define _CRT_SECURE_NO_DEPRECATE
	#endif
#endif

#include "Poco/Foundation.h"


//
// PDF_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define PDF_API POCO_API(PDF_EXPORTS)


//
// Automatically link PDF library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(PDF_EXPORTS)
		#pragma comment(lib, "PocoPDF" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // PDF_PDF_INCLUDED
