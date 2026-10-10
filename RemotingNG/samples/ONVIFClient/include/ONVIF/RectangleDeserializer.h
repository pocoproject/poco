//
// RectangleDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Rectangle_INCLUDED
#define TypeDeserializer_ONVIF_Rectangle_INCLUDED


#include "ONVIF/Rectangle.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Rectangle>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Rectangle& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Rectangle& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"bottom"s,"top"s,"right"s,"left"s};
		bool ret = false;
		Poco::Optional<float> gen_bottom;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[0], false, deser, gen_bottom);
		if (ret) value.setBottom(gen_bottom);
		Poco::Optional<float> gen_top;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_top);
		if (ret) value.setTop(gen_top);
		Poco::Optional<float> gen_right;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_right);
		if (ret) value.setRight(gen_right);
		Poco::Optional<float> gen_left;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[3], false, deser, gen_left);
		if (ret) value.setLeft(gen_left);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Rectangle& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"bottom"s,"top"s,"right"s,"left"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[3], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Rectangle_INCLUDED

