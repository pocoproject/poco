//
// Driver.cpp
//
// $Id: //poco/1.4/NetSSL_OpenSSL/testsuite/src/Driver.cpp#1 $
//
// Console-based test driver for Poco NetSSL.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "CppUnit/TestRunner.h"
#include "NetSSLTestSuite.h"
#include "Poco/Path.h"
#include "Poco/Util/Application.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include <iostream>


class NetSSLApp: public Poco::Util::Application
{
public:
	NetSSLApp()
	{
		Poco::Net::initializeSSL();
		Poco::Net::HTTPStreamFactory::registerFactory();
		Poco::Net::HTTPSStreamFactory::registerFactory();
	}

	~NetSSLApp()
	{
		Poco::Net::uninitializeSSL();
	}

	int main(const std::vector<std::string>& args)
	{
		CppUnit::TestRunner runner;
		runner.addTest("NetSSLTestSuite", NetSSLTestSuite::suite());
		return runner.run(_targs) ? 0 : 1;
	}
	
	void setup(int argc, char** argv)
	{
		init(1, argv);
		for (int i = 0; i < argc; ++i)
			_targs.push_back(std::string(argv[i]));
	}

protected:
	void initialize(Poco::Util::Application& self)
	{
#ifdef POCO_OS_FAMILY_UNIX
		std::string pocoBase(Poco::Environment::get("POCO_BASE"));
		if (pocoBase.empty())
			throw Poco::LogicException("POCO_BASE should be defined");
		
		Poco::Path configPath(pocoBase);
		configPath.makeDirectory();
		configPath.pushDirectory("NetSSL_OpenSSL");
		configPath.pushDirectory("testsuite");
		configPath.setFileName("TestSuite.xml");

		loadConfiguration(configPath.toString()); 
#else
		loadConfiguration(); 
#endif
		Poco::Util::Application::initialize(self);
	}
	
private:
	std::vector<std::string> _targs;
};


int main(int ac, char **av)
{
	NetSSLApp app;
	try
	{
		app.setup(ac, av);
		return app.run();
	}
	catch (Poco::Exception& exc)
	{
		std::cout << exc.displayText() << std::endl;
		return 1;
	}
}
