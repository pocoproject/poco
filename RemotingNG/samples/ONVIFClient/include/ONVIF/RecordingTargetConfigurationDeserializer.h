//
// RecordingTargetConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_RecordingTargetConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_RecordingTargetConfiguration_INCLUDED


#include "ONVIF/RecordingEncryptionDeserializer.h"
#include "ONVIF/RecordingEncryptionSerializer.h"
#include "ONVIF/RecordingTargetConfiguration.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::RecordingTargetConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::RecordingTargetConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::RecordingTargetConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Storage"s,"Format"s,"Prefix"s,"Postfix"s,"SpanDuration"s,"SegmentDuration"s,"Encryption"s};
		bool ret = false;
		std::string gen_storage;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_storage);
		if (ret) value.setStorage(gen_storage);
		std::string gen_format;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_format);
		if (ret) value.setFormat(gen_format);
		Poco::Optional<std::string> gen_prefix;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[2], false, deser, gen_prefix);
		if (ret) value.setPrefix(gen_prefix);
		Poco::Optional<std::string> gen_postfix;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[3], false, deser, gen_postfix);
		if (ret) value.setPostfix(gen_postfix);
		Poco::Optional<std::string> gen_spanDuration;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[4], false, deser, gen_spanDuration);
		if (ret) value.setSpanDuration(gen_spanDuration);
		std::string gen_segmentDuration;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[5], true, deser, gen_segmentDuration);
		if (ret) value.setSegmentDuration(gen_segmentDuration);
		std::vector<ONVIF::RecordingEncryption> gen_encryption;
		ret = TypeDeserializer<std::vector<ONVIF::RecordingEncryption>>::deserialize(REMOTING__NAMES[6], false, deser, gen_encryption);
		if (ret) value.setEncryption(gen_encryption);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_RecordingTargetConfiguration_INCLUDED

