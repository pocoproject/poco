//
// OSDColorDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OSDColor_INCLUDED
#define TypeDeserializer_ONVIF_OSDColor_INCLUDED


#include "ONVIF/ColorDeserializer.h"
#include "ONVIF/ColorSerializer.h"
#include "ONVIF/OSDColor.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OSDColor>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OSDColor& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::OSDColor& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Transparent"s};
		bool ret = false;
		Poco::Optional<int> gen_transparent;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[0], false, deser, gen_transparent);
		if (ret) value.setTransparent(gen_transparent);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::OSDColor& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Color"s};
		bool ret = false;
		ONVIF::Color gen_color;
		ret = TypeDeserializer<ONVIF::Color>::deserialize(REMOTING__NAMES[0], true, deser, gen_color);
		if (ret) value.setColor(gen_color);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Transparent"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OSDColor_INCLUDED

