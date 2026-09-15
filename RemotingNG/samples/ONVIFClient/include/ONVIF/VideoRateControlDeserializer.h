//
// VideoRateControlDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoRateControl_INCLUDED
#define TypeDeserializer_ONVIF_VideoRateControl_INCLUDED


#include "ONVIF/VideoRateControl.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoRateControl>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoRateControl& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoRateControl& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FrameRateLimit"s,"EncodingInterval"s,"BitrateLimit"s};
		bool ret = false;
		int gen_frameRateLimit;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_frameRateLimit);
		if (ret) value.setFrameRateLimit(gen_frameRateLimit);
		int gen_encodingInterval;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_encodingInterval);
		if (ret) value.setEncodingInterval(gen_encodingInterval);
		int gen_bitrateLimit;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_bitrateLimit);
		if (ret) value.setBitrateLimit(gen_bitrateLimit);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoRateControl_INCLUDED

