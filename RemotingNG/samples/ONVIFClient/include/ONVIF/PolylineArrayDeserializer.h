//
// PolylineArrayDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_PolylineArray_INCLUDED
#define TypeDeserializer_ONVIF_PolylineArray_INCLUDED


#include "ONVIF/PolylineArray.h"
#include "ONVIF/PolylineArrayExtensionDeserializer.h"
#include "ONVIF/PolylineArrayExtensionSerializer.h"
#include "ONVIF/PolylineDeserializer.h"
#include "ONVIF/PolylineSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::PolylineArray>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::PolylineArray& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::PolylineArray& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Segment"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::Polyline> gen_segment;
		ret = TypeDeserializer<std::vector<ONVIF::Polyline>>::deserialize(REMOTING__NAMES[0], true, deser, gen_segment);
		if (ret) value.setSegment(gen_segment);
		Poco::SharedPtr<ONVIF::PolylineArrayExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::PolylineArrayExtension>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_PolylineArray_INCLUDED

