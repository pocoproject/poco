//
// ITimeService.cpp
//
// Package: Generated
// Module:  ITimeService
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "ITimeService.h"


namespace Services {


ITimeService::ITimeService():
	Poco::RefCountedObject(),
	wakeUp()
{
}


ITimeService::~ITimeService()
{
}


const Poco::RemotingNG::Identifiable::TypeId& ITimeService::remoting__typeId()
{
	static const std::string REMOTING__TYPE_ID("Services.TimeService");
	return REMOTING__TYPE_ID;
}


} // namespace Services

