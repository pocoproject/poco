//
// ColorOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ColorOptions_INCLUDED
#define TypeDeserializer_ONVIF_ColorOptions_INCLUDED


#include "ONVIF/ColorDeserializer.h"
#include "ONVIF/ColorOptions.h"
#include "ONVIF/ColorSerializer.h"
#include "ONVIF/ColorspaceRangeDeserializer.h"
#include "ONVIF/ColorspaceRangeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ColorOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ColorOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ColorOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ColorList"s,"ColorspaceRange"s};
		bool ret = false;
		std::vector<ONVIF::Color> gen_colorList;
		ret = TypeDeserializer<std::vector<ONVIF::Color>>::deserialize(REMOTING__NAMES[0], false, deser, gen_colorList);
		if (ret) value.setColorList(gen_colorList);
		std::vector<ONVIF::ColorspaceRange> gen_colorspaceRange;
		ret = TypeDeserializer<std::vector<ONVIF::ColorspaceRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_colorspaceRange);
		if (ret) value.setColorspaceRange(gen_colorspaceRange);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ColorOptions_INCLUDED

