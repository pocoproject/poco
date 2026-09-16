//
// ActionEngineEventPayloadSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ActionEngineEventPayload_INCLUDED
#define TypeSerializer_ONVIF_ActionEngineEventPayload_INCLUDED


#include "EnvelopeDeserializer.h"
#include "EnvelopeSerializer.h"
#include "FaultDeserializer.h"
#include "FaultSerializer.h"
#include "ONVIF/ActionEngineEventPayload.h"
#include "ONVIF/ActionEngineEventPayloadExtensionDeserializer.h"
#include "ONVIF/ActionEngineEventPayloadExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ActionEngineEventPayload>
{
public:
	static void serialize(const std::string& name, const ONVIF::ActionEngineEventPayload& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::ActionEngineEventPayload& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RequestInfo"s,"ResponseInfo"s,"Fault"s,"Extension"s,""s};
		TypeSerializer<Poco::SharedPtr<W3::SOAP::Envelope::Envelope>>::serialize(REMOTING__NAMES[0], value.getRequestInfo(), ser);
		TypeSerializer<Poco::SharedPtr<W3::SOAP::Envelope::Envelope>>::serialize(REMOTING__NAMES[1], value.getResponseInfo(), ser);
		TypeSerializer<Poco::SharedPtr<W3::SOAP::Envelope::Fault>>::serialize(REMOTING__NAMES[2], value.getFault(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::ActionEngineEventPayloadExtension>>::serialize(REMOTING__NAMES[3], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ActionEngineEventPayload_INCLUDED

