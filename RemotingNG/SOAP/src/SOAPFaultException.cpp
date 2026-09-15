//
// SOAPFaultException.cpp
//
// Library: RemotingNG/SOAP
// Package: SOAP
// Module:  SOAPFaultException
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties,
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/RemotingNG/SOAP/SOAPFaultException.h"
#include "Poco/RemotingNG/SOAP/Serializer.h"


using namespace std::string_literals;


namespace Poco {
namespace RemotingNG {
namespace SOAP {


SOAPFaultException::SOAPFaultException(const std::string& message, const XMLElement& fault):
	Poco::RemotingNG::RemoteException(message),
	_fault(fault)
{
}


SOAPFaultException::SOAPFaultException(const SOAPFaultException& exc):
	Poco::RemotingNG::RemoteException(exc),
	_fault(exc._fault)
{
}


SOAPFaultException::~SOAPFaultException() noexcept
{
}


SOAPFaultException& SOAPFaultException::operator = (const SOAPFaultException& exc)
{
	if (&exc != this)
	{
		RemoteException::operator = (exc);
		_fault = exc._fault;
	}
	return *this;
}


const char* SOAPFaultException::name() const noexcept
{
	return "SOAP Fault";
}


const char* SOAPFaultException::className() const noexcept
{
	return typeid(*this).name();
}


Poco::Exception* SOAPFaultException::clone() const
{
	return new SOAPFaultException(*this);
}


void SOAPFaultException::rethrow() const
{
	throw *this;
}


std::string SOAPFaultException::faultCode() const
{
	if (_fault.namespaceURI() == Serializer::SOAP11NAMESPACE)
	{
		const XMLElement* pFaultCode = _fault.findChild(Serializer::FAULTCODE, Serializer::SOAP11NAMESPACE);
		if (pFaultCode) return pFaultCode->value();
	}
	else if (_fault.namespaceURI() == Serializer::SOAP12NAMESPACE)
	{
		const XMLElement* pCode = _fault.findChild(Serializer::CODE, Serializer::SOAP12NAMESPACE);
		if (pCode)
		{			
			const XMLElement* pValue = pCode->findChild(Serializer::CODE_VALUE, Serializer::SOAP12NAMESPACE);
			if (pValue) return pValue->value();
		}
	}
	return ""s;
}


std::string SOAPFaultException::faultReason() const
{
	if (_fault.namespaceURI() == Serializer::SOAP11NAMESPACE)
	{
		const XMLElement* pFaultString = _fault.findChild(Serializer::FAULTSTRING, Serializer::SOAP11NAMESPACE);
		if (pFaultString) return pFaultString->value();
	}
	else if (_fault.namespaceURI() == Serializer::SOAP12NAMESPACE)
	{
		const XMLElement* pReason = _fault.findChild(Serializer::REASON, Serializer::SOAP12NAMESPACE);
		if (pReason)
		{			
			const XMLElement* pText = pReason->findChild(Serializer::REASON_TEXT, Serializer::SOAP12NAMESPACE);
			if (pText) return pText->value();
		}
	}
	return ""s;
}


const XMLElement* SOAPFaultException::faultDetail() const
{
	if (_fault.namespaceURI() == Serializer::SOAP11NAMESPACE)
	{
		return _fault.findChild(Serializer::DETAIL, Serializer::SOAP11NAMESPACE);
	}
	else
	{
		return _fault.findChild(Serializer::DETAIL12, Serializer::SOAP12NAMESPACE);
	}
}


} } } // namespace Poco::RemotingNG::SOAP
