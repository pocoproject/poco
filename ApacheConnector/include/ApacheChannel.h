//
// ApacheChannel.h
//
// $Id: //poco/1.4/ApacheConnector/include/ApacheChannel.h#1 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef ApacheConnector_ApacheChannel_INCLUDED
#define ApacheConnector_ApacheChannel_INCLUDED


#include "Poco/Channel.h"


class ApacheChannel: public Poco::Channel
	/// This class implements a logging channel
	/// that uses the Apache logging facilities.
{
public:
	ApacheChannel();
	~ApacheChannel();

	void log(const Poco::Message& msg);
};


#endif // ApacheConnector_ApacheChannel_INCLUDED
