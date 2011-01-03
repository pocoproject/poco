//
// ApacheApplication.cpp
//
// $Id: //poco/1.4/ApacheConnector/src/ApacheChannel.cpp#1 $
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "ApacheChannel.h"
#include "ApacheConnector.h"
#include "Poco/Message.h"


ApacheChannel::ApacheChannel()
{
}


ApacheChannel::~ApacheChannel()
{
}


void ApacheChannel::log(const Poco::Message& msg)
{
	ApacheConnector::log(msg.getSource().c_str(), 0, msg.getPriority(), 0, msg.getText().c_str());
}
