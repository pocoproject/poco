//
// Definitions.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Definitions
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Definitions.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Definitions::Definitions(const std::string& targetNS):
	_targetNamespace(targetNS)
{
}


Definitions::~Definitions()
{
}


void Definitions::addMessage(Message::Ptr pMessage)
{
	poco_assert (!pMessage->name().empty());

	std::pair<Messages::iterator, bool> res = _messages.insert(std::make_pair(pMessage->name(), pMessage));
	if (!res.second)
		throw ElementAlreadyDefinedException("message", pMessage->name());
}


void Definitions::addPortType(PortType::Ptr pPortType)
{
	poco_assert (!pPortType->name().empty());
	
	std::pair<PortTypes::iterator, bool> res = _portTypes.insert(std::make_pair(pPortType->name(), pPortType));
	if (!res.second)
		throw ElementAlreadyDefinedException("portType", pPortType->name());
}


void Definitions::addBinding(Binding::Ptr pBinding)
{
	poco_assert (!pBinding->name().empty());

	std::pair<Bindings::iterator, bool> res = _bindings.insert(std::make_pair(pBinding->name(), pBinding));
	if (!res.second)
		throw ElementAlreadyDefinedException("binding", pBinding->name());
}


void Definitions::addService(Service::Ptr pService)
{
	poco_assert (!pService->name().empty());

	std::pair<Services::iterator, bool> res = _services.insert(std::make_pair(pService->name(), pService));
	if (!res.second)
		throw ElementAlreadyDefinedException("service", pService->name());
}


void Definitions::accept(Visitor& v) const
{
	v.visit(*this);
}


} } } // namespace Poco::XSD::Types
