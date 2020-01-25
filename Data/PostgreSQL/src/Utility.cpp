//
// Utility.cpp
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Utility
//
// Implementation of Utility
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Data/PostgreSQL/Utility.h"
#include "Poco/Data/PostgreSQL/SessionImpl.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace Data {
namespace PostgreSQL {


std::string Utility::serverInfo(SessionHandle* aHandlePtr)
{
	std::string srvrInfo = "Process ID: ";

	srvrInfo.append(Poco::NumberFormatter::format(aHandlePtr->serverProcessID()));

	srvrInfo.append(" Protocol Version: ");

	srvrInfo.append(Poco::NumberFormatter::format(aHandlePtr->protocoVersion()));

	return srvrInfo;
}


std::string Utility::serverInfo(Session& aSession)
{
	return serverInfo(handle(aSession));
}


int Utility::serverVersion(SessionHandle* aHandlePtr)
{
	return aHandlePtr->serverVersion();
}


int Utility::serverVersion(Session& aSession)
{
	return serverVersion(handle(aSession));
}


std::string Utility::hostInfo(SessionHandle* aHandlePtr)
{
	SessionParametersMap parametersMap = aHandlePtr->connectionParameters();

	SessionParametersMap::const_iterator cItr = parametersMap.find("host");

	if (parametersMap.end() == cItr)
	{
		return std::string();
	}

	return cItr->second.currentValue();
}


std::string Utility::hostInfo(Session& aSession)
{
	return hostInfo(handle(aSession));
}


std::string Utility::sessionEncoding(SessionHandle* aHandlePtr)
{
	return aHandlePtr->clientEncoding();
}


std::string Utility::sessionEncoding(Poco::Data::Session& aSession)
{
	return sessionEncoding(handle(aSession));
}


} } } // namespace Poco::Data::PostgreSQL
