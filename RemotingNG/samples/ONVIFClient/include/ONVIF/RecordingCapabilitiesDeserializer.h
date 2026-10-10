//
// RecordingCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_RecordingCapabilities_INCLUDED


#include "ONVIF/RecordingCapabilities.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"ReceiverSource"s,"MediaProfileSource"s,"DynamicRecordings"s,"DynamicTracks"s,"MaxStringLength"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		bool gen_receiverSource;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_receiverSource);
		if (ret) value.setReceiverSource(gen_receiverSource);
		bool gen_mediaProfileSource;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_mediaProfileSource);
		if (ret) value.setMediaProfileSource(gen_mediaProfileSource);
		bool gen_dynamicRecordings;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_dynamicRecordings);
		if (ret) value.setDynamicRecordings(gen_dynamicRecordings);
		bool gen_dynamicTracks;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[4], true, deser, gen_dynamicTracks);
		if (ret) value.setDynamicTracks(gen_dynamicTracks);
		int gen_maxStringLength;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[5], true, deser, gen_maxStringLength);
		if (ret) value.setMaxStringLength(gen_maxStringLength);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingCapabilities_INCLUDED

