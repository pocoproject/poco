//
// PizzaPickupServiceProxy.cpp
//
// Package: Generated
// Module:  PizzaPickupServiceProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2006-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/PizzaPickupServiceProxy.h"
#include "Pizzeria/DeliveryAddressDeserializer.h"
#include "Pizzeria/DeliveryAddressSerializer.h"
#include "Pizzeria/ExtToppingDeserializer.h"
#include "Pizzeria/ExtToppingSerializer.h"
#include "Pizzeria/PizzaDeserializer.h"
#include "Pizzeria/PizzaSerializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"


namespace Pizzeria {


PizzaPickupServiceProxy::PizzaPickupServiceProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	Pizzeria::IPizzaPickupService(),
	Poco::RemotingNG::Proxy(oid),
	_cache(),
	_getAnyPizzaRet(),
	_getMostPopularToppingRet(),
	_getPizzaNamesRet(),
	_getPizzasRet(),
	_getToppingsResultIsSet(false),
	_getToppingsRet(),
	_getWaitTimeRet(),
	_orderForSelfPickupRet(),
	_orderRet()
{
}


PizzaPickupServiceProxy::~PizzaPickupServiceProxy()
{
}


const Pizzeria::Pizza& PizzaPickupServiceProxy::getAnyPizza() const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"getAnyPizza"s};
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("getAnyPizzaReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Pizzeria::Pizza >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getAnyPizzaRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _getAnyPizzaRet;
}


const Pizzeria::ExtTopping& PizzaPickupServiceProxy::getMostPopularTopping() const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"getMostPopularTopping"s};
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("getMostPopularToppingReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Pizzeria::ExtTopping >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getMostPopularToppingRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _getMostPopularToppingRet;
}


const std::set < std::string >& PizzaPickupServiceProxy::getPizzaNames() const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"getPizzaNames"s};
	if (_cache.has(REMOTING__NAMES[0]))
	{
		return _getPizzaNamesRet;
	}
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("getPizzaNamesReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_getPizzaNamesRet.clear();
	Poco::RemotingNG::TypeDeserializer<std::set < std::string > >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getPizzaNamesRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_cache.add(REMOTING__NAMES[0], Poco::ExpirationDecorator<int>(0, 28800000));
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _getPizzaNamesRet;
}


const std::vector < Pizzeria::Pizza >& PizzaPickupServiceProxy::getPizzas() const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"getPizzas"s};
	if (_cache.has(REMOTING__NAMES[0]))
	{
		return _getPizzasRet;
	}
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("getPizzasReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_getPizzasRet.clear();
	Poco::RemotingNG::TypeDeserializer<std::vector < Pizzeria::Pizza > >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getPizzasRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_cache.add(REMOTING__NAMES[0], Poco::ExpirationDecorator<int>(0, 28800000));
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _getPizzasRet;
}


const std::vector < Pizzeria::ExtTopping >& PizzaPickupServiceProxy::getToppings() const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"getToppings"s};
	if (_getToppingsResultIsSet)
	{
		return _getToppingsRet;
	}
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("getToppingsReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_getToppingsRet.clear();
	Poco::RemotingNG::TypeDeserializer<std::vector < Pizzeria::ExtTopping > >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getToppingsRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_getToppingsResultIsSet = true;
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _getToppingsRet;
}


Poco::Timespan PizzaPickupServiceProxy::getWaitTime() const
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"getWaitTime"s};
	if (_cache.has(REMOTING__NAMES[0]))
	{
		return _getWaitTimeRet;
	}
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("getWaitTimeReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Poco::Timespan >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getWaitTimeRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	_cache.add(REMOTING__NAMES[0], Poco::ExpirationDecorator<int>(0, 300000));
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _getWaitTimeRet;
}


Poco::DateTime PizzaPickupServiceProxy::order(const Pizzeria::Pizza& pizza, const Pizzeria::DeliveryAddress& deliverTo)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"order"s,"deliverTo"s,"pizza"s};
	Poco::FastMutex::ScopedLock remoting__lock(remoting__mutex());
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<Pizzeria::DeliveryAddress >::serialize(REMOTING__NAMES[1], deliverTo, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Pizzeria::Pizza >::serialize(REMOTING__NAMES[2], pizza, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("orderReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Poco::DateTime >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _orderRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _orderRet;
}


Poco::DateTime PizzaPickupServiceProxy::orderForSelfPickup(const Pizzeria::Pizza& pizza, const std::string& phoneNumber)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"orderForSelfPickup"s,"phoneNumber"s,"pizza"s};
	Poco::FastMutex::ScopedLock remoting__lock(remoting__mutex());
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<std::string >::serialize(REMOTING__NAMES[1], phoneNumber, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Pizzeria::Pizza >::serialize(REMOTING__NAMES[2], pizza, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("orderForSelfPickupReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Poco::DateTime >::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _orderForSelfPickupRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
	return _orderForSelfPickupRet;
}


const std::string PizzaPickupServiceProxy::DEFAULT_NS("http://www.appinf.com/webservices/PizzaDeliveryService/");
} // namespace Pizzeria

