//
// CodeGeneration.h
//
// Library: CodeGeneration
// Package: CodeGeneration
// Module:  CodeGeneration
//
// Basic definitions for the Poco CodeGeneration library.
// This file must be the first file included by every other CodeGeneration
// header file.
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGeneration_CodeGeneration_INCLUDED
#define CodeGeneration_CodeGeneration_INCLUDED


#ifndef Foundation_Foundation_INCLUDED
#include "Poco/Foundation.h"
#endif


//
// CodeGeneration_EXPORTS is defined only while this library is compiled (see POCO_API in Poco/Foundation.h).
//
#define CodeGeneration_API POCO_API(CodeGeneration_EXPORTS)


//
// Automatically link CodeGeneration library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(CodeGeneration_EXPORTS)
		#pragma comment(lib, "PocoCodeGeneration" POCO_LIB_SUFFIX)
	#endif
#endif


#endif // CodeGeneration_CodeGeneration_INCLUDED
