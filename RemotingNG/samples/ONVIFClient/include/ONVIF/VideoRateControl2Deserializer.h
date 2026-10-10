//
// VideoRateControl2Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoRateControl2_INCLUDED
#define TypeDeserializer_ONVIF_VideoRateControl2_INCLUDED


#include "ONVIF/VideoRateControl2.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoRateControl2>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoRateControl2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::VideoRateControl2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ConstantBitRate"s};
		bool ret = false;
		Poco::Optional<bool> gen_constantBitRate;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_constantBitRate);
		if (ret) value.setConstantBitRate(gen_constantBitRate);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoRateControl2& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FrameRateLimit"s,"BitrateLimit"s};
		bool ret = false;
		float gen_frameRateLimit;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_frameRateLimit);
		if (ret) value.setFrameRateLimit(gen_frameRateLimit);
		int gen_bitrateLimit;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_bitrateLimit);
		if (ret) value.setBitrateLimit(gen_bitrateLimit);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ConstantBitRate"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoRateControl2_INCLUDED

