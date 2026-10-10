//
// ActionEngineEventPayloadDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ActionEngineEventPayload_INCLUDED
#define TypeDeserializer_ONVIF_ActionEngineEventPayload_INCLUDED


#include "EnvelopeDeserializer.h"
#include "EnvelopeSerializer.h"
#include "FaultDeserializer.h"
#include "FaultSerializer.h"
#include "ONVIF/ActionEngineEventPayload.h"
#include "ONVIF/ActionEngineEventPayloadExtensionDeserializer.h"
#include "ONVIF/ActionEngineEventPayloadExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ActionEngineEventPayload>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ActionEngineEventPayload& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::ActionEngineEventPayload& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RequestInfo"s,"ResponseInfo"s,"Fault"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<W3::SOAP::Envelope::Envelope> gen_requestInfo;
		ret = TypeDeserializer<Poco::SharedPtr<W3::SOAP::Envelope::Envelope>>::deserialize(REMOTING__NAMES[0], false, deser, gen_requestInfo);
		if (ret) value.setRequestInfo(gen_requestInfo);
		Poco::SharedPtr<W3::SOAP::Envelope::Envelope> gen_responseInfo;
		ret = TypeDeserializer<Poco::SharedPtr<W3::SOAP::Envelope::Envelope>>::deserialize(REMOTING__NAMES[1], false, deser, gen_responseInfo);
		if (ret) value.setResponseInfo(gen_responseInfo);
		Poco::SharedPtr<W3::SOAP::Envelope::Fault> gen_fault;
		ret = TypeDeserializer<Poco::SharedPtr<W3::SOAP::Envelope::Fault>>::deserialize(REMOTING__NAMES[2], false, deser, gen_fault);
		if (ret) value.setFault(gen_fault);
		Poco::SharedPtr<ONVIF::ActionEngineEventPayloadExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ActionEngineEventPayloadExtension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ActionEngineEventPayload_INCLUDED

