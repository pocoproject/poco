//
// OrderEndpointSkeleton.cpp
//
// Package: Generated
// Module:  OrderEndpointSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//
// Copyright (c) 2016-2020, Applied Informatics Software Engineering GmbH.
//


#include "Pizzeria/OrderEndpointSkeleton.h"
#include "Pizzeria/OrderDeserializer.h"
#include "Pizzeria/OrderSerializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/MethodHandler.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/SharedPtr.h"


namespace Pizzeria {


class OrderEndpointDelete_MethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"delete_"s,"orderNumber"s};
		bool remoting__requestSucceeded = false;
		try
		{
			int orderNumber;
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders/{orderNumber}"s);
			remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
			Poco::RemotingNG::TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, remoting__deser, orderNumber);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
			remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
			Pizzeria::OrderEndpointRemoteObject* remoting__pCastedRO = static_cast<Pizzeria::OrderEndpointRemoteObject*>(remoting__pRemoteObject.get());
			remoting__pCastedRO->delete_(orderNumber);
			remoting__requestSucceeded = true;
			Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			static const std::string REMOTING__REPLY_NAME("delete_Reply");
			remoting__ser.serializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			remoting__ser.serializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		}
		catch (const Poco::Exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				remoting__trans.reportException("Pizzeria::OrderEndpoint::delete_"s, e);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], e);
			}
		}
		catch (const std::exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc(e.what());
				remoting__trans.reportException("Pizzeria::OrderEndpoint::delete_"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
		catch (...)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc("Unknown Exception"s);
				remoting__trans.reportException("Pizzeria::OrderEndpoint::delete_"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
	}

};


class OrderEndpointGetMethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"get"s,"orderNumber"s};
		bool remoting__requestSucceeded = false;
		try
		{
			int orderNumber;
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders/{orderNumber}"s);
			remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
			Poco::RemotingNG::TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, remoting__deser, orderNumber);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
			remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
			Pizzeria::OrderEndpointRemoteObject* remoting__pCastedRO = static_cast<Pizzeria::OrderEndpointRemoteObject*>(remoting__pRemoteObject.get());
			Pizzeria::Order remoting__return = remoting__pCastedRO->get(orderNumber);
			remoting__requestSucceeded = true;
			Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			static const std::string REMOTING__REPLY_NAME("getReply");
			remoting__ser.serializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			Poco::RemotingNG::TypeSerializer<Pizzeria::Order>::serialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, remoting__return, remoting__ser);
			remoting__ser.serializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		}
		catch (const Poco::Exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				remoting__trans.reportException("Pizzeria::OrderEndpoint::get"s, e);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], e);
			}
		}
		catch (const std::exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc(e.what());
				remoting__trans.reportException("Pizzeria::OrderEndpoint::get"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
		catch (...)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc("Unknown Exception"s);
				remoting__trans.reportException("Pizzeria::OrderEndpoint::get"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
	}

};


class OrderEndpointPatchMethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"patch"s,"orderNumber"s,"order"s};
		bool remoting__requestSucceeded = false;
		try
		{
			int orderNumber;
			Pizzeria::Order order;
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/pizzeria/orders/{orderNumber}"s);
			remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
			Poco::RemotingNG::TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, remoting__deser, orderNumber);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
			Poco::RemotingNG::TypeDeserializer<Pizzeria::Order>::deserialize(REMOTING__NAMES[2], true, remoting__deser, order);
			remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
			Pizzeria::OrderEndpointRemoteObject* remoting__pCastedRO = static_cast<Pizzeria::OrderEndpointRemoteObject*>(remoting__pRemoteObject.get());
			Pizzeria::Order remoting__return = remoting__pCastedRO->patch(orderNumber, order);
			remoting__requestSucceeded = true;
			Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			static const std::string REMOTING__REPLY_NAME("patchReply");
			remoting__ser.serializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			Poco::RemotingNG::TypeSerializer<Pizzeria::Order>::serialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, remoting__return, remoting__ser);
			remoting__ser.serializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		}
		catch (const Poco::Exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				remoting__trans.reportException("Pizzeria::OrderEndpoint::patch"s, e);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], e);
			}
		}
		catch (const std::exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc(e.what());
				remoting__trans.reportException("Pizzeria::OrderEndpoint::patch"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
		catch (...)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc("Unknown Exception"s);
				remoting__trans.reportException("Pizzeria::OrderEndpoint::patch"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
	}

};


OrderEndpointSkeleton::OrderEndpointSkeleton():
	Poco::RemotingNG::Skeleton()

{
	using namespace std::string_literals;
	
	addMethodHandler("delete_"s, new Pizzeria::OrderEndpointDelete_MethodHandler);
	addMethodHandler("get"s, new Pizzeria::OrderEndpointGetMethodHandler);
	addMethodHandler("patch"s, new Pizzeria::OrderEndpointPatchMethodHandler);
}


OrderEndpointSkeleton::~OrderEndpointSkeleton()
{
}


const std::string OrderEndpointSkeleton::DEFAULT_NS;
} // namespace Pizzeria

