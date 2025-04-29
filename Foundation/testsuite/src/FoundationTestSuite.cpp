//
// FoundationTestSuite.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "FoundationTestSuite.h"
#include "CoreTestSuite.h"
#include "DateTimeTestSuite.h"
#include "StreamsTestSuite.h"
#include "CryptTestSuite.h"
#include "NotificationsTestSuite.h"
#include "ThreadingTestSuite.h"
#include "SharedLibraryTestSuite.h"
#include "LoggingTestSuite.h"
#include "FilesystemTestSuite.h"
#include "UUIDTestSuite.h"
#include "TextTestSuite.h"
#include "URITestSuite.h"
#if !defined(POCO_VXWORKS)
#include "ProcessesTestSuite.h"
#endif
#include "TaskTestSuite.h"
#include "EventTestSuite.h"
#include "CacheTestSuite.h"
#include "HashingTestSuite.h"


CppUnit::Test* FoundationTestSuite::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("FoundationTestSuite");

	pSuite->addTest(ThreadingTestSuite::suite());

	return pSuite;
}
