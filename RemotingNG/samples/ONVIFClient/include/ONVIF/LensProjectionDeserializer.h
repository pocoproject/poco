//
// LensProjectionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_LensProjection_INCLUDED
#define TypeDeserializer_ONVIF_LensProjection_INCLUDED


#include "ONVIF/LensProjection.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::LensProjection>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::LensProjection& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::LensProjection& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Angle"s,"Radius"s,"Transmittance"s};
		bool ret = false;
		float gen_angle;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_angle);
		if (ret) value.setAngle(gen_angle);
		float gen_radius;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_radius);
		if (ret) value.setRadius(gen_radius);
		Poco::Optional<float> gen_transmittance;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_transmittance);
		if (ret) value.setTransmittance(gen_transmittance);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_LensProjection_INCLUDED

