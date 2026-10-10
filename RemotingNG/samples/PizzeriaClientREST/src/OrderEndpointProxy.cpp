//
// OrderEndpointProxy.cpp
//
// Package: Generated
// Module:  OrderEndpointProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderEndpointProxy.h"
#include "Pizzeria/OrderDeserializer.h"
#include "Pizzeria/OrderSerializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"


namespace Pizzeria {


OrderEndpointProxy::OrderEndpointProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	Pizzeria::IOrderEndpoint(),
	Poco::RemotingNG::Proxy(oid),
	_getRet(),
	_patchRet()
{
}


OrderEndpointProxy::~OrderEndpointProxy()
{
}


void OrderEndpointProxy::delete_(int orderNumber)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"delete_"s,"orderNumber"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders/{orderNumber}"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize(REMOTING__NAMES[1], orderNumber, remoting__ser);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("delete_Reply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
}


Pizzeria::Order OrderEndpointProxy::get(int orderNumber) const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"get"s,"orderNumber"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders/{orderNumber}"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize(REMOTING__NAMES[1], orderNumber, remoting__ser);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("getReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Pizzeria::Order >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
	return _getRet;
}


Pizzeria::Order OrderEndpointProxy::patch(int orderNumber, const Pizzeria::Order& order)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"patch"s,"orderNumber"s,"order"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders/{orderNumber}"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int >::serialize(REMOTING__NAMES[1], orderNumber, remoting__ser);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	Poco::RemotingNG::TypeSerializer<Pizzeria::Order >::serialize(REMOTING__NAMES[2], order, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("patchReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Pizzeria::Order >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _patchRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
	return _patchRet;
}


const std::string OrderEndpointProxy::DEFAULT_NS;
} // namespace Pizzeria

