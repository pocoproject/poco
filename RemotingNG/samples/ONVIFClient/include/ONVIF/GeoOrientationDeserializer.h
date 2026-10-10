//
// GeoOrientationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_GeoOrientation_INCLUDED
#define TypeDeserializer_ONVIF_GeoOrientation_INCLUDED


#include "ONVIF/GeoOrientation.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::GeoOrientation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::GeoOrientation& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::GeoOrientation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"roll"s,"pitch"s,"yaw"s};
		bool ret = false;
		Poco::Optional<float> gen_roll;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[0], false, deser, gen_roll);
		if (ret) value.setRoll(gen_roll);
		Poco::Optional<float> gen_pitch;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_pitch);
		if (ret) value.setPitch(gen_pitch);
		Poco::Optional<float> gen_yaw;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_yaw);
		if (ret) value.setYaw(gen_yaw);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::GeoOrientation& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"roll"s,"pitch"s,"yaw"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_GeoOrientation_INCLUDED

