//
// OrderCollectionEndpointProxy.cpp
//
// Package: Generated
// Module:  OrderCollectionEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderCollectionEndpointProxy.h"
#include "Pizzeria/OrderDeserializer.h"
#include "Pizzeria/OrderSerializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"


namespace Pizzeria {


OrderCollectionEndpointProxy::OrderCollectionEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	Pizzeria::IOrderCollectionEndpoint(),
	Poco::RemotingNG::Proxy(oid),
	_getRet(),
	_postRet()
{
}


OrderCollectionEndpointProxy::~OrderCollectionEndpointProxy()
{
}


std::vector < Pizzeria::Order > OrderCollectionEndpointProxy::get(int maxOrders) const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"get"s,"maxOrders"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "query"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize(REMOTING__NAMES[1], maxOrders, remoting__ser);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("getReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_getRet.clear();
	Poco::RemotingNG::TypeDeserializer<std::vector < Pizzeria::Order > >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
	return _getRet;
}


Pizzeria::Order OrderCollectionEndpointProxy::post(const Pizzeria::Order& order)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"post"s,"order"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<Pizzeria::Order >::serialize(REMOTING__NAMES[1], order, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("postReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Pizzeria::Order >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _postRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
	return _postRet;
}


const std::string OrderCollectionEndpointProxy::DEFAULT_NS;
} // namespace Pizzeria

