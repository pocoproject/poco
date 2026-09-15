//
// PolygonOptionsDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PolygonOptions_INCLUDED
#define TypeDeserializer_ONVIF_PolygonOptions_INCLUDED


#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/PolygonOptions.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PolygonOptions>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PolygonOptions& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PolygonOptions& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RectangleOnly"s,"VertexLimits"s};
		bool ret = false;
		Poco::Optional<bool> gen_rectangleOnly;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_rectangleOnly);
		if (ret) value.setRectangleOnly(gen_rectangleOnly);
		Poco::SharedPtr<ONVIF::IntRange> gen_vertexLimits;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::IntRange>>::deserialize(REMOTING__NAMES[1], false, deser, gen_vertexLimits);
		if (ret) value.setVertexLimits(gen_vertexLimits);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PolygonOptions_INCLUDED

