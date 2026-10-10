//
// LensDescriptionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_LensDescription_INCLUDED
#define TypeDeserializer_ONVIF_LensDescription_INCLUDED


#include "ONVIF/LensDescription.h"
#include "ONVIF/LensOffsetDeserializer.h"
#include "ONVIF/LensOffsetSerializer.h"
#include "ONVIF/LensProjectionDeserializer.h"
#include "ONVIF/LensProjectionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::LensDescription>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::LensDescription& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::LensDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FocalLength"s};
		bool ret = false;
		Poco::Optional<float> gen_focalLength;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[0], false, deser, gen_focalLength);
		if (ret) value.setFocalLength(gen_focalLength);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::LensDescription& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Offset"s,"Projection"s,"XFactor"s};
		bool ret = false;
		ONVIF::LensOffset gen_offset;
		ret = TypeDeserializer<ONVIF::LensOffset>::deserialize(REMOTING__NAMES[0], true, deser, gen_offset);
		if (ret) value.setOffset(gen_offset);
		std::vector<ONVIF::LensProjection> gen_projection;
		ret = TypeDeserializer<std::vector<ONVIF::LensProjection>>::deserialize(REMOTING__NAMES[1], true, deser, gen_projection);
		if (ret) value.setProjection(gen_projection);
		float gen_xFactor;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[2], true, deser, gen_xFactor);
		if (ret) value.setXFactor(gen_xFactor);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FocalLength"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_LensDescription_INCLUDED

