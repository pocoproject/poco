//
// ConvertTemperatureProxy.cpp
//
// Package: Generated
// Module:  ConvertTemperatureProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "WebServiceX/ConvertTemperatureProxy.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"


namespace WebServiceX {


ConvertTemperatureProxy::ConvertTemperatureProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	WebServiceX::IConvertTemperature(),
	Poco::RemotingNG::Proxy(oid)

{
}


ConvertTemperatureProxy::~ConvertTemperatureProxy()
{
}


void ConvertTemperatureProxy::convertTemp(double temperature, const std::string& fromUnit, const std::string& toUnit, double& convertTempResult)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"ConvertTemp"s,"Temperature"s,"FromUnit"s,"ToUnit"s,"ConvertTempResult"s};
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_ACTION, "http://www.webserviceX.NET/ConvertTemp"s);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<double>::serialize(REMOTING__NAMES[1], temperature, remoting__ser);
	Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], fromUnit, remoting__ser);
	Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[3], toUnit, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("ConvertTempResponse");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<double>::deserialize(REMOTING__NAMES[4], true, remoting__deser, convertTempResult);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
}


const std::string ConvertTemperatureProxy::DEFAULT_NS("http://www.webserviceX.NET/");
} // namespace WebServiceX

