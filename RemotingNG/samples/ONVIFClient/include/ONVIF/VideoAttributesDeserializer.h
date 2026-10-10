//
// VideoAttributesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_VideoAttributes_INCLUDED
#define TypeDeserializer_ONVIF_VideoAttributes_INCLUDED


#include "ONVIF/VideoAttributes.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::VideoAttributes>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::VideoAttributes& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::VideoAttributes& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Bitrate"s,"Width"s,"Height"s,"Encoding"s,"Framerate"s};
		bool ret = false;
		Poco::Optional<int> gen_bitrate;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_bitrate);
		if (ret) value.setBitrate(gen_bitrate);
		int gen_width;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_width);
		if (ret) value.setWidth(gen_width);
		int gen_height;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_height);
		if (ret) value.setHeight(gen_height);
		std::string gen_encoding;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[3], true, deser, gen_encoding);
		if (ret) value.setEncoding(gen_encoding);
		float gen_framerate;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[4], true, deser, gen_framerate);
		if (ret) value.setFramerate(gen_framerate);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_VideoAttributes_INCLUDED

