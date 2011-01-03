//
// ApacheApplication.cpp
//
// $Id: //poco/1.4/ApacheConnector/src/ApacheApplication.cpp#1 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "ApacheApplication.h"
#include "ApacheChannel.h"
#include "Poco/Logger.h"
#include "Poco/SingletonHolder.h"
#include <vector>


using Poco::Logger;
using Poco::FastMutex;


ApacheApplication::ApacheApplication():
	_ready(false)
{
	Logger::root().setChannel(new ApacheChannel);
}


ApacheApplication::~ApacheApplication()
{
	Logger::shutdown();
}


void ApacheApplication::setup()
{
	FastMutex::ScopedLock lock(_mutex);
	
	if (!_ready)
	{
		std::vector<std::string> cmdLine;
		cmdLine.push_back("mod_poco");
		init(cmdLine);
		_ready = true;
	}
}


ApacheApplication& ApacheApplication::instance()
{
	static Poco::SingletonHolder<ApacheApplication> sh;
	return *sh.get();
}
