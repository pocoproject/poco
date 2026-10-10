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


namespace Poco::XSD::Types {


Definitions::Definitions(const std::string& targetNS):
	_targetNamespace(targetNS)
{
}


Definitions::~Definitions() = default;


void Definitions::addMessage(Message::Ptr pMessage)
{
	poco_assert (!pMessage->name().empty());

	if (!_messages.try_emplace(pMessage->name(), pMessage).second)
		throw ElementAlreadyDefinedException("message", pMessage->name());
}


void Definitions::addPortType(PortType::Ptr pPortType)
{
	poco_assert (!pPortType->name().empty());
	
	if (!_portTypes.try_emplace(pPortType->name(), pPortType).second)
		throw ElementAlreadyDefinedException("portType", pPortType->name());
}


void Definitions::addBinding(Binding::Ptr pBinding)
{
	poco_assert (!pBinding->name().empty());

	if (!_bindings.try_emplace(pBinding->name(), pBinding).second)
		throw ElementAlreadyDefinedException("binding", pBinding->name());
}


void Definitions::addService(Service::Ptr pService)
{
	poco_assert (!pService->name().empty());

	if (!_services.try_emplace(pService->name(), pService).second)
		throw ElementAlreadyDefinedException("service", pService->name());
}


void Definitions::accept(Visitor& v) const
{
	v.visit(*this);
}


} // namespace Poco::XSD::Types
