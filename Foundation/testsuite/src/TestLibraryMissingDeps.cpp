//
// TestLibraryMissingDeps.cpp
//
// A test library that depends on a non-existent library.
// Used to test findMissingDependencies functionality.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ClassLibrary.h"


// Declare an external function from a non-existent library
// This will cause the linker to add a dependency on the fake library
extern "C" int nonExistentFunction();


extern "C" POCO_LIBRARY_API int gimmeSix()
{
	// Reference the non-existent function to ensure the dependency is kept
	return 6 + (nonExistentFunction() * 0);
}
