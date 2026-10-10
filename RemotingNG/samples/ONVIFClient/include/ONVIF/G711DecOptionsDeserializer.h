//
// G711DecOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_G711DecOptions_INCLUDED
#define TypeDeserializer_ONVIF_G711DecOptions_INCLUDED


#include "ONVIF/G711DecOptions.h"
#include "ONVIF/IntItemsDeserializer.h"
#include "ONVIF/IntItemsSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::G711DecOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::G711DecOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::G711DecOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Bitrate"s,"SampleRateRange"s};
		bool ret = false;
		ONVIF::IntItems gen_bitrate;
		ret = TypeDeserializer<ONVIF::IntItems>::deserialize(REMOTING__NAMES[0], true, deser, gen_bitrate);
		if (ret) value.setBitrate(gen_bitrate);
		ONVIF::IntItems gen_sampleRateRange;
		ret = TypeDeserializer<ONVIF::IntItems>::deserialize(REMOTING__NAMES[1], true, deser, gen_sampleRateRange);
		if (ret) value.setSampleRateRange(gen_sampleRateRange);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_G711DecOptions_INCLUDED

