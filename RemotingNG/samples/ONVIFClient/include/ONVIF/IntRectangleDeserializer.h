//
// IntRectangleDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_IntRectangle_INCLUDED
#define TypeDeserializer_ONVIF_IntRectangle_INCLUDED


#include "ONVIF/IntRectangle.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::IntRectangle>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::IntRectangle& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::IntRectangle& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"x"s,"y"s,"width"s,"height"s};
		bool ret = false;
		int gen_x;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_x);
		if (ret) value.setX(gen_x);
		int gen_y;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_y);
		if (ret) value.setY(gen_y);
		int gen_width;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[2], true, deser, gen_width);
		if (ret) value.setWidth(gen_width);
		int gen_height;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[3], true, deser, gen_height);
		if (ret) value.setHeight(gen_height);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::IntRectangle& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"x"s,"y"s,"width"s,"height"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[1], true);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[2], true);
		deser.pushAttribute(REMOTING__NAMES[4], REMOTING__NAMES[3], true);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_IntRectangle_INCLUDED

