//
// CheckVatServiceProxy.cpp
//
// Package: Generated
// Module:  CheckVatServiceProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#include "CheckVat/CheckVatServiceProxy.h"
#include "Poco/RemotingNG/Deserializer.h"
#include "Poco/RemotingNG/Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"
#include "Poco/RemotingNG/URIUtility.h"


namespace CheckVat {


CheckVatServiceProxy::CheckVatServiceProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid):
	CheckVat::ICheckVatService(),
	Poco::RemotingNG::Proxy(oid)

{
}


CheckVatServiceProxy::~CheckVatServiceProxy()
{
}


void CheckVatServiceProxy::checkVat(const std::string& countryCode, const std::string& vatNumber, std::string& countryCode2, std::string& vatNumber2, Poco::DateTime& requestDate, bool& valid, Poco::Nullable<std::string>& name, Poco::Nullable<std::string>& address)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"checkVat"s,"countryCode"s,"vatNumber"s,"countryCode"s,"vatNumber"s,"requestDate"s,"valid"s,"name"s,"address"s,"urn:ec.europa.eu:taxud:vies:services:checkVat:types"s};
	const std::string& remoting__namespace(REMOTING__NAMES[9]);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], countryCode, remoting__ser);
	Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], vatNumber, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[9]);
	static const std::string REMOTING__REPLY_NAME("checkVatResponse");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[3], true, remoting__deser, countryCode2);
	Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[4], true, remoting__deser, vatNumber2);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "date"s);
	Poco::RemotingNG::TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[5], true, remoting__deser, requestDate);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	Poco::RemotingNG::TypeDeserializer<bool>::deserialize(REMOTING__NAMES[6], true, remoting__deser, valid);
	Poco::RemotingNG::TypeDeserializer<Poco::Nullable<std::string>>::deserialize(REMOTING__NAMES[7], true, remoting__deser, name);
	Poco::RemotingNG::TypeDeserializer<Poco::Nullable<std::string>>::deserialize(REMOTING__NAMES[8], true, remoting__deser, address);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
}


void CheckVatServiceProxy::checkVatApprox(const std::string& countryCode, const std::string& vatNumber, const Poco::Optional<std::string>& traderName, const Poco::Optional<std::string>& traderCompanyType, const Poco::Optional<std::string>& traderStreet, const Poco::Optional<std::string>& traderPostcode, const Poco::Optional<std::string>& traderCity, const Poco::Optional<std::string>& requesterCountryCode, const Poco::Optional<std::string>& requesterVatNumber, std::string& countryCode2, std::string& vatNumber2, Poco::DateTime& requestDate, bool& valid, Poco::Nullable<std::string>& traderName2, Poco::Nullable<std::string>& traderCompanyType2, Poco::Optional<std::string>& traderAddress, Poco::Optional<std::string>& traderStreet2, Poco::Optional<std::string>& traderPostcode2, Poco::Optional<std::string>& traderCity2, Poco::Optional<std::string>& traderNameMatch, Poco::Optional<std::string>& traderCompanyTypeMatch, Poco::Optional<std::string>& traderStreetMatch, Poco::Optional<std::string>& traderPostcodeMatch, Poco::Optional<std::string>& traderCityMatch, std::string& requestIdentifier)
{
	using namespace std::string_literals;
	
	static const std::string REMOTING__NAMES[] = {"checkVatApprox"s,"countryCode"s,"vatNumber"s,"traderName"s,"traderCompanyType"s,"traderStreet"s,"traderPostcode"s,"traderCity"s,"requesterCountryCode"s,"requesterVatNumber"s,"countryCode"s,"vatNumber"s,"requestDate"s,"valid"s,"traderName"s,"traderCompanyType"s,"traderAddress"s,"traderStreet"s,"traderPostcode"s,"traderCity"s,"traderNameMatch"s,"traderCompanyTypeMatch"s,"traderStreetMatch"s,"traderPostcodeMatch"s,"traderCityMatch"s,"requestIdentifier"s,"urn:ec.europa.eu:taxud:vies:services:checkVat:types"s};
	const std::string& remoting__namespace(REMOTING__NAMES[26]);
	Poco::RemotingNG::Transport& remoting__trans = remoting__transport();
	remoting__trans.setAttribute(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	Poco::RemotingNG::Serializer& remoting__ser = remoting__trans.beginRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, remoting__namespace);
	remoting__ser.serializeMessageBegin(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], countryCode, remoting__ser);
	Poco::RemotingNG::TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], vatNumber, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[3], traderName, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[4], traderCompanyType, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[5], traderStreet, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[6], traderPostcode, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[7], traderCity, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[8], requesterCountryCode, remoting__ser);
	Poco::RemotingNG::TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[9], requesterVatNumber, remoting__ser);
	remoting__ser.serializeMessageEnd(REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__ser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	Poco::RemotingNG::Deserializer& remoting__deser = remoting__trans.sendRequest(remoting__objectId(), remoting__typeId(), REMOTING__NAMES[0], Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE, REMOTING__NAMES[26]);
	static const std::string REMOTING__REPLY_NAME("checkVatApproxResponse");
	remoting__deser.deserializeMessageBegin(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[10], true, remoting__deser, countryCode2);
	Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[11], true, remoting__deser, vatNumber2);
	remoting__deser.pushProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE, "date"s);
	Poco::RemotingNG::TypeDeserializer<Poco::DateTime>::deserialize(REMOTING__NAMES[12], true, remoting__deser, requestDate);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_XSDTYPE);
	Poco::RemotingNG::TypeDeserializer<bool>::deserialize(REMOTING__NAMES[13], true, remoting__deser, valid);
	Poco::RemotingNG::TypeDeserializer<Poco::Nullable<std::string>>::deserialize(REMOTING__NAMES[14], true, remoting__deser, traderName2);
	Poco::RemotingNG::TypeDeserializer<Poco::Nullable<std::string>>::deserialize(REMOTING__NAMES[15], true, remoting__deser, traderCompanyType2);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[16], true, remoting__deser, traderAddress);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[17], true, remoting__deser, traderStreet2);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[18], true, remoting__deser, traderPostcode2);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[19], true, remoting__deser, traderCity2);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[20], true, remoting__deser, traderNameMatch);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[21], true, remoting__deser, traderCompanyTypeMatch);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[22], true, remoting__deser, traderStreetMatch);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[23], true, remoting__deser, traderPostcodeMatch);
	Poco::RemotingNG::TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[24], true, remoting__deser, traderCityMatch);
	Poco::RemotingNG::TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[25], true, remoting__deser, requestIdentifier);
	remoting__deser.deserializeMessageEnd(REMOTING__REPLY_NAME, Poco::RemotingNG::SerializerBase::MESSAGE_REPLY);
	remoting__deser.popProperty(Poco::RemotingNG::SerializerBase::PROP_NAMESPACE);
	remoting__trans.endRequest();
}


const std::string CheckVatServiceProxy::DEFAULT_NS("urn:ec.europa.eu:taxud:vies:services:checkVat");
} // namespace CheckVat

