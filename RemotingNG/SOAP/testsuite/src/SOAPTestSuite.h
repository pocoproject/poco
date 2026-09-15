//
// SOAPTestSuite.h
//
// Definition of the SOAPTestSuite class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef SOAPTestSuite_INCLUDED
#define SOAPTestSuite_INCLUDED


#include "CppUnit/TestSuite.h"


class SOAPTestSuite
{
public:
	static CppUnit::Test* suite();
};


#endif // SOAPTestSuite_INCLUDED
