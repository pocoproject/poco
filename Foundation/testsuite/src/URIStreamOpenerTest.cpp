//
// URIStreamOpenerTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "URIStreamOpenerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/URIStreamFactory.h"
#include "Poco/URI.h"
#include "Poco/TemporaryFile.h"
#include "Poco/Path.h"
#include "Poco/Exception.h"
#include <fstream>
#include <sstream>


using Poco::URIStreamOpener;
using Poco::URIStreamFactory;
using Poco::URI;
using Poco::TemporaryFile;
using Poco::Path;


namespace
{
	class StringStreamFactory: public URIStreamFactory
	{
	public:
		StringStreamFactory()
		{
		}

		std::istream* open(const URI& uri)
		{
			return new std::istringstream(uri.toString());
		}
	};

	class RedirectingStreamFactory: public URIStreamFactory
		// Stands in for an HTTP server answering with a 302 to _target.
	{
	public:
		RedirectingStreamFactory(const std::string& target): _target(target)
		{
		}

		std::istream* open(const URI& /*uri*/)
		{
			throw Poco::URIRedirection(_target);
		}

	private:
		std::string _target;
	};
}


URIStreamOpenerTest::URIStreamOpenerTest(const std::string& name): CppUnit::TestCase(name)
{
}


URIStreamOpenerTest::~URIStreamOpenerTest()
{
}


void URIStreamOpenerTest::testStreamOpenerFile()
{
	TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::ofstream ostr(path.c_str());
	assertTrue (ostr.good());
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	URI uri;
	uri.setScheme("file");
	uri.setPath(Path(path).toString(Path::PATH_UNIX));
	std::string uriString = uri.toString();

	URIStreamOpener opener;
	std::istream* istr = opener.open(uri);
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
}


void URIStreamOpenerTest::testStreamOpenerRelative()
{
	TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::ofstream ostr(path.c_str());
	assertTrue (ostr.good());
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	URI uri(Path(path).toString(Path::PATH_UNIX));
	std::string uriString = uri.toString();

	URIStreamOpener opener;
	std::istream* istr = opener.open(uri);
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
}


void URIStreamOpenerTest::testStreamOpenerURI()
{
	TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::ofstream ostr(path.c_str());
	assertTrue (ostr.good());
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	URI uri;
	uri.setScheme("file");
	uri.setPath(Path(path).toString(Path::PATH_UNIX));
	std::string uriString = uri.toString();

	URIStreamOpener opener;
	std::istream* istr = opener.open(uriString);
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
}


void URIStreamOpenerTest::testStreamOpenerURIResolve()
{
	TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::ofstream ostr(path.c_str());
	assertTrue (ostr.good());
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	Path p(path);
	p.makeAbsolute();
	Path parent(p.parent());

	URI uri;
	uri.setScheme("file");
	uri.setPath(parent.toString(Path::PATH_UNIX));
	std::string uriString = uri.toString();

	URIStreamOpener opener;
	std::istream* istr = opener.open(uriString, p.getFileName());
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
}


void URIStreamOpenerTest::testStreamOpenerPath()
{
	TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::ofstream ostr(path.c_str());
	assertTrue (ostr.good());
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	URIStreamOpener opener;
	std::istream* istr = opener.open(path);
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
}


void URIStreamOpenerTest::testStreamOpenerPathResolve()
{
	TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::ofstream ostr(path.c_str());
	assertTrue (ostr.good());
	ostr << "Hello, world!" << std::endl;
	ostr.close();

	Path p(path);
	Path parent(p.parent());
	std::string base = parent.toString();

	URIStreamOpener opener;
	std::istream* istr = opener.open(base, p.getFileName());
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
}


void URIStreamOpenerTest::testRegisterUnregister()
{
	URIStreamOpener opener;
	assertTrue (!opener.supportsScheme("string"));
	opener.registerStreamFactory("string", new StringStreamFactory);
	assertTrue (opener.supportsScheme("string"));
	URI uri("string:foobar");
	std::istream* istr = opener.open(uri);
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
	opener.unregisterStreamFactory("string");
	assertTrue (!opener.supportsScheme("string"));
}


void URIStreamOpenerTest::testRedirectToFileSchemeRejected()
{
	TemporaryFile tempFile;
	std::string path = tempFile.path();
	std::ofstream ostr(path.c_str());
	assertTrue (ostr.good());
	ostr << "secret" << std::endl;
	ostr.close();

	URI fileURI;
	fileURI.setScheme("file");
	fileURI.setPath(Path(path).toString(Path::PATH_UNIX));

	URIStreamOpener opener;
	opener.registerStreamFactory("http", new RedirectingStreamFactory(fileURI.toString()));
	try
	{
		std::istream* istr = opener.open(URI("http://localhost/redirect"));
		delete istr;
		fail("redirect into the file scheme must be rejected");
	}
	catch (Poco::UnknownURISchemeException&)
	{
	}

	// The string overload must reject it too: it catches URISyntaxException and
	// retries the input as a filesystem path, so the rejection must not be one.
	try
	{
		std::istream* istr = opener.open(std::string("http://localhost/redirect"));
		delete istr;
		fail("redirect into the file scheme must be rejected");
	}
	catch (Poco::UnknownURISchemeException&)
	{
	}
}


void URIStreamOpenerTest::testRedirectToWebSchemeAllowed()
{
	URIStreamOpener opener;
	opener.registerStreamFactory("http", new RedirectingStreamFactory("https://localhost/target"));
	opener.registerStreamFactory("https", new StringStreamFactory);
	std::istream* istr = opener.open(URI("http://localhost/redirect"));
	assertTrue (istr != nullptr);
	assertTrue (istr->good());
	delete istr;
}


void URIStreamOpenerTest::setUp()
{
}


void URIStreamOpenerTest::tearDown()
{
}


CppUnit::Test* URIStreamOpenerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("URIStreamOpenerTest");

	CppUnit_addTest(pSuite, URIStreamOpenerTest, testStreamOpenerFile);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testStreamOpenerRelative);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testStreamOpenerURI);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testStreamOpenerURIResolve);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testStreamOpenerPath);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testStreamOpenerPathResolve);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testRegisterUnregister);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testRedirectToFileSchemeRejected);
	CppUnit_addTest(pSuite, URIStreamOpenerTest, testRedirectToWebSchemeAllowed);

	return pSuite;
}
