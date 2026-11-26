//
// TestPlugin.h
//
// Definition of the TestPlugin class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef TestPlugin_INCLUDED
#define TestPlugin_INCLUDED


#include "Poco/Foundation.h"

#include <string>

#if !defined(POCO_OS_FAMILY_WINDOWS)
	#define PLUGIN_EXPORT __attribute__((visibility("default")))
#elif defined(POCO_TESTPLUGIN_SHARED)
	#if defined(TestPlugin_EXPORTS)
		#define PLUGIN_EXPORT __declspec(dllexport)
	#else
		#define PLUGIN_EXPORT __declspec(dllimport)
	#endif
#else
#define PLUGIN_EXPORT
#endif

class PLUGIN_EXPORT TestPlugin
{
public:
	TestPlugin();
	virtual ~TestPlugin();
	virtual std::string name() const = 0;
};


#endif // TestPlugin_INCLUDED
