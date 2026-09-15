//
// OSDColorOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDColorOptions_INCLUDED
#define TypeDeserializer_ONVIF_OSDColorOptions_INCLUDED


#include "ONVIF/ColorOptionsDeserializer.h"
#include "ONVIF/ColorOptionsSerializer.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/OSDColorOptions.h"
#include "ONVIF/OSDColorOptionsExtensionDeserializer.h"
#include "ONVIF/OSDColorOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDColorOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDColorOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDColorOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Color"s,"Transparent"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::ColorOptions> gen_color;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ColorOptions>>::deserialize(REMOTING__NAMES[0], false, deser, gen_color);
		if (ret) value.setColor(gen_color);
		Poco::SharedPtr<ONVIF::IntRange> gen_transparent;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_transparent);
		if (ret) value.setTransparent(gen_transparent);
		Poco::SharedPtr<ONVIF::OSDColorOptionsExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::OSDColorOptionsExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDColorOptions_INCLUDED

