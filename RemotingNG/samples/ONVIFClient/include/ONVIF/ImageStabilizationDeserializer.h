//
// ImageStabilizationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImageStabilization_INCLUDED
#define TypeDeserializer_ONVIF_ImageStabilization_INCLUDED


#include "ONVIF/ImageStabilization.h"
#include "ONVIF/ImageStabilizationExtensionDeserializer.h"
#include "ONVIF/ImageStabilizationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImageStabilization>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImageStabilization& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImageStabilization& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Level"s,"Extension"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::Optional<float> gen_level;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_level);
		if (ret) value.setLevel(gen_level);
		Poco::SharedPtr<ONVIF::ImageStabilizationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImageStabilizationExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImageStabilization_INCLUDED

