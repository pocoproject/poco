//
// PKCS12ContainerTest.h
//
// Definition of the PKCS12ContainerTest class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PKCS12ContainerTest_INCLUDED
#define PKCS12ContainerTest_INCLUDED


#include "Poco/Crypto/Crypto.h"
#include "CppUnit/TestCase.h"


namespace Poco {
namespace Crypto {
	class PKCS12Container;
}
}

class PKCS12ContainerTest: public CppUnit::TestCase
{
public:
	PKCS12ContainerTest(const std::string& name);
	~PKCS12ContainerTest();

	void testFullPKCS12();
	void testCertsOnlyPKCS12();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:
	std::string getTestFilesPath(const std::string& name);
	void certsOnly(const Poco::Crypto::PKCS12Container& pkcs12);
	void full(const Poco::Crypto::PKCS12Container& pkcs12);
};


#endif // PKCS12ContainerTest_INCLUDED
