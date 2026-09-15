//
// TimeService.h
//
// Definition of the TimeService class.
//
// Copyright (c) 2006-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef TimeService_INCLUDED
#define TimeService_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/DateTime.h"


namespace Services {


//@ remote, namespace="http://www.appinf.com/webservices/TimeService/"
class TimeService
{
public:
	TimeService();
		/// Creates the TimeService.

	~TimeService();
		/// Destroys the TimeService.
	
	Poco::DateTime currentTime() const;
		/// Returns the current date and time.
};


} // namespace Services


#endif // TimeService_INCLUDED
