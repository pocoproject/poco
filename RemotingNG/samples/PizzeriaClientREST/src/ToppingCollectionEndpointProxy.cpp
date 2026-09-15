//
// ToppingCollectionEndpointProxy.cpp
//
// Package: Generated
// Module:  ToppingCollectionEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/ToppingCollectionEndpointProxy.h"
#include "Pizzeria/ExtToppingDeserializer.h"
#include "Pizzeria/ExtToppingSerializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"


namespace Pizzeria {


ToppingCollectionEndpointProxy::ToppingCollectionEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	Pizzeria::IToppingCollectionEndpoint(),
	Poco::RemotingNG::Proxy(oid),
	_getRet()
{
}


ToppingCollectionEndpointProxy::~ToppingCollectionEndpointProxy()
{
}


std::vector < Pizzeria::ExtTopping > ToppingCollectionEndpointProxy::get() const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"get"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/toppings"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("getReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_getRet.clear();
	Poco::RemotingNG::TypeDeserializer<std::vector < Pizzeria::ExtTopping > >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
	return _getRet;
}


const std::string ToppingCollectionEndpointProxy::DEFAULT_NS;
} // namespace Pizzeria

