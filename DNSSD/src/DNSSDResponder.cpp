//
// DNSSDResponder.cpp
//
// $Id: //poco/1.7/DNSSD/src/DNSSDResponder.cpp#1 $
//
// Library: DNSSD
// Package: Core
// Module:  DNSSDResponder
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/DNSSDResponder.h"
#include "Poco/DNSSD/DNSSDResponderImpl.h"
#include "Poco/Exception.h"


namespace Poco {
namespace DNSSD {


DNSSDResponderImplFactory* DNSSDResponder::_pImplFactory(0);


DNSSDResponder::DNSSDResponder():
	_pImpl(0)
{
	if (_pImplFactory)
	{
		_pImpl = _pImplFactory->createResponderImpl(*this);
	}
	else
	{
		throw Poco::IllegalStateException("No DNSResponderImplFactory available.");
	}
}


DNSSDResponder::~DNSSDResponder()
{
	delete _pImpl;
}


DNSSDBrowser& DNSSDResponder::browser()
{
	return _pImpl->browser();
}


ServiceHandle DNSSDResponder::registerService(const Service& service, int options)
{
	return _pImpl->registerService(service, options);
}


void DNSSDResponder::unregisterService(ServiceHandle& serviceHandle)
{
	_pImpl->unregisterService(serviceHandle);
}


RecordHandle DNSSDResponder::addRecord(ServiceHandle serviceHandle, const Record& record)
{
	return _pImpl->addRecord(serviceHandle, record);
}


void DNSSDResponder::updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record)
{
	_pImpl->updateRecord(serviceHandle, recordHandle, record);
}


void DNSSDResponder::removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle)
{
	_pImpl->removeRecord(serviceHandle, recordHandle);
}


void DNSSDResponder::start()
{
	_pImpl->start();
}


void DNSSDResponder::stop()
{
	_pImpl->stop();
}


void DNSSDResponder::registerImplFactory(DNSSDResponderImplFactory& factory)
{
	_pImplFactory = &factory;
}


void DNSSDResponder::unregisterImplFactory()
{
	_pImplFactory = 0;
}


} } // namespace Poco::DNSSD
