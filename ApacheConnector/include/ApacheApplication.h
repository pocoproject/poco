//
// ApacheApplication.h
//
// $Id: //poco/Main/ApacheConnector/include/ApacheApplication.h#2 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef ApacheConnector_ApacheApplication_INCLUDED
#define ApacheConnector_ApacheApplication_INCLUDED


#include "ApacheRequestHandlerFactory.h"
#include "Poco/Util/Application.h"
#include "Poco/Mutex.h"


class ApacheApplication: public Poco::Util::Application
{
public:
	ApacheApplication();
		/// Creates the ApacheApplication and sets the
		/// ApacheChannel as the root logger channel.

	~ApacheApplication();
		/// Destroys the ApacheApplication.

	void setup();
		/// Initializes the application if called for the first
		/// time; does nothing in later calls.

	ApacheRequestHandlerFactory& factory();
		/// Returns the ApacheRequestHandlerFactory.

	static ApacheApplication& instance();
		/// Returns the application instance.

private:
	bool _ready;
	ApacheRequestHandlerFactory _factory;
	Poco::FastMutex _mutex;
};


//
// inlines
//
inline ApacheRequestHandlerFactory& ApacheApplication::factory()
{
	return _factory;
}


#endif // ApacheConnector_ApacheApplication_INCLUDED
