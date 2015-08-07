//
// TestLibrary.cpp
//
// $Id: //poco/1.4/Foundation/testsuite/src/TestLibrary.cpp#1 $
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "TestPlugin.h"
#include "Poco/ClassLibrary.h"
#include <iostream>


extern "C" int POCO_LIBRARY_API gimmeFive();


class PluginA: public TestPlugin
{
public:
	PluginA()
	{
	}
	
	~PluginA()
	{
	}
	
	std::string name() const
	{
		return "PluginA";
	}
};


class PluginB: public TestPlugin
{
public:
	PluginB()
	{
	}
	
	~PluginB()
	{
	}
	
	std::string name() const
	{
		return "PluginB";
	}
};


class PluginC: public TestPlugin
{
public:
	PluginC()
	{
	}
	
	~PluginC()
	{
	}
	
	std::string name() const
	{
		return "PluginC";
	}
};

class PluginD: public TestPluginWithOneArg
{
public:
	PluginD(int no) : _no(no)
	{
	}

	~PluginD()
	{
	}

	std::string name() const
	{
		return "PluginD";
	}
	int no() const
	{
		return _no;
	}
private:
	int _no;
};


POCO_BEGIN_MANIFEST(TestPlugin)
	POCO_EXPORT_CLASS(PluginA)
	POCO_EXPORT_CLASS(PluginB)
	POCO_EXPORT_SINGLETON(PluginC)
POCO_END_MANIFEST


#ifdef POCO_ENABLE_CPP11
POCO_BEGIN_NAMED_MANIFEST_WITH_ONE_ARG(TestPluginWithOneArg, TestPluginWithOneArg, int)
	POCO_EXPORT_CLASS_WITH_ONE_ARG(PluginD, int)
POCO_END_MANIFEST
#endif

void pocoInitializeLibrary()
{
	std::cout << "TestLibrary initializing" << std::endl;
}


void pocoUninitializeLibrary()
{
	std::cout << "TestLibrary uninitialzing" << std::endl;
}


int gimmeFive()
{
	return 5;
}
