//
// AuthTesterSkeleton.cpp
//
// Package: Generated
// Module:  AuthTesterSkeleton
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "AuthTesterSkeleton.h"
#include "ObjectDeserializer.h"
#include "ObjectSerializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/MethodHandler.h"
#include "Poco/RemotingNG/RemotingException.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/ServerTransport.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/SharedPtr.h"


class AuthTesterGetMethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"get"s,"id"s,"options"s};
		bool remoting__requestSucceeded = false;
		try
		{
			int id;
			std::string options("none");
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/authtester/{id}"s);
			remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
			Poco::RemotingNG::TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, remoting__deser, id);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "query"s);
			Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[2], false, remoting__deser, options);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
			remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
			if (!remoting__trans.authenticate(REMOTING__NAMES[0]))
				throw Poco::RemotingNG::AuthenticationFailedException();
			AuthTesterRemoteObject* remoting__pCastedRO = static_cast<AuthTesterRemoteObject*>(remoting__pRemoteObject.get());
			Object remoting__return = remoting__pCastedRO->get(id, options);
			remoting__requestSucceeded = true;
			Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			static const std::string REMOTING__REPLY_NAME("getReply");
			remoting__ser.serializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			Poco::RemotingNG::TypeSerializer<Object>::serialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, remoting__return, remoting__ser);
			remoting__ser.serializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		}
		catch (const Poco::Exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				remoting__trans.reportException("AuthTester::get"s, e);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], e);
			}
		}
		catch (const std::exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc(e.what());
				remoting__trans.reportException("AuthTester::get"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
		catch (...)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc("Unknown Exception"s);
				remoting__trans.reportException("AuthTester::get"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
	}

};


class AuthTesterPostMethodHandler: public Poco::RemotingNG::MethodHandler
{
public:
	void invoke(Poco::RemotingNG::ServerTransport& remoting__trans, Poco::RemotingNG::Deserializer& remoting__deser, Poco::RemotingNG::RemoteObject::Ptr remoting__pRemoteObject)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"post"s,"id"s,"object"s};
		bool remoting__requestSucceeded = false;
		try
		{
			int id;
			Object object;
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/authtester/{id}"s);
			remoting__deser.deserializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
			Poco::RemotingNG::TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, remoting__deser, id);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
			Poco::RemotingNG::TypeDeserializer<Object>::deserialize(REMOTING__NAMES[2], true, remoting__deser, object);
			remoting__deser.deserializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
			remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
			if (!remoting__trans.authenticate(REMOTING__NAMES[0]))
				throw Poco::RemotingNG::AuthenticationFailedException();
			static const std::string REMOTING__PERMISSION("perm1");
			if (!remoting__trans.authorize(REMOTING__NAMES[0], REMOTING__PERMISSION))
				throw Poco::RemotingNG::NoPermissionException(REMOTING__PERMISSION);
			AuthTesterRemoteObject* remoting__pCastedRO = static_cast<AuthTesterRemoteObject*>(remoting__pRemoteObject.get());
			Object remoting__return = remoting__pCastedRO->post(id, object);
			remoting__requestSucceeded = true;
			Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			static const std::string REMOTING__REPLY_NAME("postReply");
			remoting__ser.serializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
			Poco::RemotingNG::TypeSerializer<Object>::serialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, remoting__return, remoting__ser);
			remoting__ser.serializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
		}
		catch (const Poco::Exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				remoting__trans.reportException("AuthTester::post"s, e);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], e);
			}
		}
		catch (const std::exception& e)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc(e.what());
				remoting__trans.reportException("AuthTester::post"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
		catch (...)
		{
			if (!remoting__requestSucceeded)
			{
				const Poco::Exception exc("Unknown Exception"s);
				remoting__trans.reportException("AuthTester::post"s, exc);
				Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.sendReply(Poco::RemotingNG::SerializerBase::MESSAGE_FAULT);
				remoting__ser.serializeFaultMessage(REMOTING__NAMES[0], exc);
			}
		}
	}

};


AuthTesterSkeleton::AuthTesterSkeleton():
	Poco::RemotingNG::Skeleton()

{
	using namespace std::string_literals;
	
	addMethodHandler("get"s, new AuthTesterGetMethodHandler);
	addMethodHandler("post"s, new AuthTesterPostMethodHandler);
}


AuthTesterSkeleton::~AuthTesterSkeleton()
{
}


const std::string AuthTesterSkeleton::DEFAULT_NS;
