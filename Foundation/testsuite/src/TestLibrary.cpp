//
// TestLibrary.cpp
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
	PluginA() = default;

	~PluginA() override = default;

	std::string name() const override
	{
		return "PluginA";
	}
};


class PluginB: public TestPlugin
{
public:
	PluginB() = default;

	~PluginB() override = default;

	std::string name() const override
	{
		return "PluginB";
	}
};


class PluginC: public TestPlugin
{
public:
	PluginC() = default;

	~PluginC() override = default;

	std::string name() const override
	{
		return "PluginC";
	}
};


POCO_BEGIN_MANIFEST(TestPlugin)
	POCO_EXPORT_CLASS(PluginA)
	POCO_EXPORT_CLASS(PluginB)
	POCO_EXPORT_SINGLETON(PluginC)
POCO_END_MANIFEST


void pocoInitializeLibrary()
{
	std::cout << "TestLibrary initializing" << std::endl;
}


void pocoUninitializeLibrary()
{
	std::cout << "TestLibrary uninitializing" << std::endl;
}


int gimmeFive()
{
	return 5;
}
