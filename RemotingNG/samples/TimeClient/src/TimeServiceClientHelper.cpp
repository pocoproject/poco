//
// TimeServiceClientHelper.cpp
//
// Package: Generated
// Module:  TimeServiceClientHelper
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "TimeServiceClientHelper.h"
#include "Poco/SingletonHolder.h"
#include "TimeServiceProxyFactory.h"


namespace Services {


namespace
{
	Poco::SingletonHolder<TimeServiceClientHelper> shTimeServiceClientHelper;
}


TimeServiceClientHelper::TimeServiceClientHelper():
	_pORB(0)
{
	_pORB = &Poco::RemotingNG::ORB::instance();
	_pORB->registerProxyFactory("Services.TimeService", new TimeServiceProxyFactory);
}


TimeServiceClientHelper::~TimeServiceClientHelper()
{
}


ITimeService::Ptr TimeServiceClientHelper::findImpl(const std::string& uri)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri);
	return pInterface.cast<ITimeService>();
}


ITimeService::Ptr TimeServiceClientHelper::findImpl(const std::string& uri, const std::string& protocol)
{
	Poco::RemotingNG::Identifiable::Ptr pInterface = _pORB->findObject(uri, ITimeService::remoting__typeId(), protocol);
	return pInterface.cast<ITimeService>();
}


TimeServiceClientHelper& TimeServiceClientHelper::instance()
{
	return *shTimeServiceClientHelper.get();
}


} // namespace Services

