//
// WeatherProxy.cpp
//
// Package: Generated
// Module:  WeatherProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "WeatherWS/WeatherProxy.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"
#include "WeatherWS/ArrayOfWeatherDescriptionDeserializer.h"
#include "WeatherWS/ArrayOfWeatherDescriptionSerializer.h"
#include "WeatherWS/ForecastReturnDeserializer.h"
#include "WeatherWS/ForecastReturnSerializer.h"
#include "WeatherWS/WeatherReturnDeserializer.h"
#include "WeatherWS/WeatherReturnSerializer.h"


namespace WeatherWS {


WeatherProxy::WeatherProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	WeatherWS::IWeather(),
	Poco::RemotingNG::Proxy(oid)

{
}


WeatherProxy::~WeatherProxy()
{
}


void WeatherProxy::getCityForecastByZIP(const Poco::Optional<std::string>& zIP, Poco::Optional<WeatherWS::ForecastReturn>& getCityForecastByZIPResult)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"GetCityForecastByZIP"s,"ZIP"s,"GetCityForecastByZIPResult"s};
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_ACTION, "http://ws.cdyne.com/WeatherWS/GetCityForecastByZIP"s);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], zIP, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("GetCityForecastByZIPResponse");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<WeatherWS::ForecastReturn>>::deserialize(REMOTING__NAMES[2], true, remoting__deser, getCityForecastByZIPResult);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
}


void WeatherProxy::getCityWeatherByZIP(const Poco::Optional<std::string>& zIP, WeatherWS::WeatherReturn& getCityWeatherByZIPResult)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"GetCityWeatherByZIP"s,"ZIP"s,"GetCityWeatherByZIPResult"s};
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_ACTION, "http://ws.cdyne.com/WeatherWS/GetCityWeatherByZIP"s);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], zIP, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("GetCityWeatherByZIPResponse");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<WeatherWS::WeatherReturn>::deserialize(REMOTING__NAMES[2], true, remoting__deser, getCityWeatherByZIPResult);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
}


void WeatherProxy::getWeatherInformation(Poco::Optional<WeatherWS::ArrayOfWeatherDescription>& getWeatherInformationResult)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"GetWeatherInformation"s,"GetWeatherInformationResult"s};
	const std::string& remoting__namespace(DEFAULT_NS);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_ACTION, "http://ws.cdyne.com/WeatherWS/GetWeatherInformation"s);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, DEFAULT_NS);
	static const std::string REMOTING__REPLY_NAME("GetWeatherInformationResponse");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<WeatherWS::ArrayOfWeatherDescription>>::deserialize(REMOTING__NAMES[1], true, remoting__deser, getWeatherInformationResult);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
}


const std::string WeatherProxy::DEFAULT_NS("http://ws.cdyne.com/WeatherWS/");
} // namespace WeatherWS

