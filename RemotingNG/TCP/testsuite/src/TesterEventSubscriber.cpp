//
// TesterEventSubscriber.cpp
//
// Package: Generated
// Module:  TesterEventSubscriber
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "TesterEventSubscriber.h"
#include "Class1Deserializer.h"
#include "Class1Serializer.h"
#include "Poco/Delegate.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/MethodHandler.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/SharedPtr.h"
#include "Struct1Deserializer.h"
#include "Struct1Serializer.h"
#include "Struct2Deserializer.h"
#include "Struct2Serializer.h"
#include "Struct3Deserializer.h"
#include "Struct3Serializer.h"
#include "Struct4Deserializer.h"
#include "Struct4Serializer.h"
#include "Struct5Deserializer.h"
#include "Struct5Serializer.h"


class TesterEvent__testEventMethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	TesterEvent__testEventMethodHandler(TesterProxy* pProxy)
	{
		_pProxy = pProxy;
	}

	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"testEvent"s,"data"s};
		bool remoting__requestSucceeded = false;
		try
		{
			std::string data;
			remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);
			Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, remoting__deser, data);
			remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);
			_pProxy->testEvent(nullptr, data);
			remoting__requestSucceeded = true;
			Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, TesterEventSubscriber::DEFAULT_NS);
			static const std::string REMOTING__REPLY_NAME("testEventReply");
			remoting__ser.serializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_EVENT_REPLY);
			Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], data, remoting__ser);
			remoting__ser.serializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_EVENT_REPLY);
		}
		catch (Poco::Exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], e);
			}
		}
		catch (std::exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				Poco::Exception exc(e.what());
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
		catch (...)
		{
			if (!remoting__requestSucceeded)
			{
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				Poco::Exception exc("Unknown Exception");
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
	}

private:
	TesterProxy* _pProxy;
};


class TesterEvent__testOneWayEventMethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	TesterEvent__testOneWayEventMethodHandler(TesterProxy* pProxy)
	{
		_pProxy = pProxy;
	}

	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"testOneWayEvent"s,"data"s};
		std::string data;
		remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);
		Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, remoting__deser, data);
		remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);
		try
		{
			_pProxy->testOneWayEvent(nullptr, data);
		}
		catch (...)
		{
		}
	}

private:
	TesterProxy* _pProxy;
};


class TesterEvent__testVoidEventMethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	TesterEvent__testVoidEventMethodHandler(TesterProxy* pProxy)
	{
		_pProxy = pProxy;
	}

	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"testVoidEvent"s};
		remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);
		remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_EVENT);
		try
		{
			_pProxy->testVoidEvent(nullptr);
		}
		catch (...)
		{
		}
	}

private:
	TesterProxy* _pProxy;
};


TesterEventSubscriber::TesterEventSubscriber(const std::string& uri, TesterProxy* pProxy):
	Poco::RemotingNG::EventSubscriber(uri)

{
	using namespace std::string_literals;
	
	addMethodHandler("testEvent"s, new TesterEvent__testEventMethodHandler(pProxy));
	addMethodHandler("testOneWayEvent"s, new TesterEvent__testOneWayEventMethodHandler(pProxy));
	addMethodHandler("testVoidEvent"s, new TesterEvent__testVoidEventMethodHandler(pProxy));
}


TesterEventSubscriber::~TesterEventSubscriber()
{
}


void TesterEventSubscriber::event__testEvent(std::string& data)
{
}


void TesterEventSubscriber::event__testOneWayEvent(std::string& data)
{
}


void TesterEventSubscriber::event__testVoidEvent()
{
}


const std::string TesterEventSubscriber::DEFAULT_NS("urn:appinf.com:service:Tester:1");
