//
// ImageStabilizationOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ImageStabilizationOptions_INCLUDED
#define TypeDeserializer_ONVIF_ImageStabilizationOptions_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/ImageStabilizationOptions.h"
#include "ONVIF/ImageStabilizationOptionsExtensionDeserializer.h"
#include "ONVIF/ImageStabilizationOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ImageStabilizationOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ImageStabilizationOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ImageStabilizationOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Level"s,"Extension"s};
		bool ret = false;
		std::vector<std::string> gen_mode;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::SharedPtr<ONVIF::FloatRange> gen_level;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_level);
		if (ret) value.setLevel(gen_level);
		Poco::SharedPtr<ONVIF::ImageStabilizationOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ImageStabilizationOptionsExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ImageStabilizationOptions_INCLUDED

