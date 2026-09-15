//
// AuthTesterProxy.cpp
//
// Package: Generated
// Module:  AuthTesterProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "AuthTesterProxy.h"
#include "ObjectDeserializer.h"
#include "ObjectSerializer.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"


AuthTesterProxy::AuthTesterProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	IAuthTester(),
	Poco::RemotingNG::Proxy(oid),
	_getRet(),
	_postRet()
{
}


AuthTesterProxy::~AuthTesterProxy()
{
}


Object AuthTesterProxy::get(int id, const std::string& options)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"get"s,"id"s,"options"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/authtester/{id}"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int>::serialize(REMOTING__NAMES[1], id, remoting__ser);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "query"s);
	Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], options, remoting__ser);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("getReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Object>::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _getRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
	return _getRet;
}


Object AuthTesterProxy::post(int id, const Object& object)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"post"s,"id"s,"object"s};
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_PATH, "/authtester/{id}"s);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION, "path"s);
	Poco::RemotingNG::TypeSerializer<int>::serialize(REMOTING__NAMES[1], id, remoting__ser);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_LOCATION);
	Poco::RemotingNG::TypeSerializer<Object>::serialize(REMOTING__NAMES[2], object, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_PATH);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	static const std::string REMOTING__REPLY_NAME("postReply");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Object>::deserialize(Poco::RemotingNG::SerializerBase::RETURN_PARAM, true, remoting__deser, _postRet);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__trans.endRequest();
	return _postRet;
}


const std::string AuthTesterProxy::DEFAULT_NS;
