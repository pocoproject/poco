//
// Skeleton.cpp
//
// Library: RemotingNG
// Package: ORB
// Module:  Skeleton
//
// Copyright (c) 2006-2014, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/RemotingNG/Skeleton.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/MethodHandler.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/RemotingException.h"


namespace Poco {
namespace RemotingNG {


Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
{
}


bool Skeleton::invoke(ServerTransport& transport, RemoteObject::Ptr pRemoteObject)
{
	bool result = true;
	Deserializer& deser = transport.beginRequest();
	std::string messageName;
	SerializerBase::MessageType messageType = deser.findMessage(messageName);
	if (messageType != SerializerBase::MESSAGE_REQUEST && messageType != SerializerBase::MESSAGE_EVENT)
		throw UnexpectedMessageException("request or event message expected");
	if (_logger.information())
		_logger.information("Invoking method: %s", messageName);

	Skeleton::MethodHandlers::iterator it = _handlers.find(messageName);
	if (it != _handlers.end())
	{
		it->second->invoke(transport, deser, pRemoteObject);
	}
	else
	{
		result = false;
		RemotingNG::Serializer& ser = transport.sendReply(SerializerBase::MESSAGE_FAULT);
		MethodNotFoundException exc(messageName);
		ser.serializeFaultMessage(messageName, exc);
	}
	transport.endRequest();
	return result;
}


void Skeleton::addMethodHandler(const std::string& name, MethodHandler::Ptr pMethodHandler)
{
	_handlers[name] = pMethodHandler;
}


} } // namespace Poco::RemotingNG
