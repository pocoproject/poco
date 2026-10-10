//
// ColorspaceRangeDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ColorspaceRange_INCLUDED
#define TypeDeserializer_ONVIF_ColorspaceRange_INCLUDED


#include "ONVIF/ColorspaceRange.h"
#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ColorspaceRange>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ColorspaceRange& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ColorspaceRange& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"X"s,"Y"s,"Z"s,"Colorspace"s};
		bool ret = false;
		ONVIF::FloatRange gen_x;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[0], true, deser, gen_x);
		if (ret) value.setX(gen_x);
		ONVIF::FloatRange gen_y;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[1], true, deser, gen_y);
		if (ret) value.setY(gen_y);
		ONVIF::FloatRange gen_z;
		ret = TypeDeserializer<ONVIF::FloatRange>::deserialize(REMOTING__NAMES[2], true, deser, gen_z);
		if (ret) value.setZ(gen_z);
		Poco::URI gen_colorspace;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[3], true, deser, gen_colorspace);
		if (ret) value.setColorspace(gen_colorspace);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ColorspaceRange_INCLUDED

