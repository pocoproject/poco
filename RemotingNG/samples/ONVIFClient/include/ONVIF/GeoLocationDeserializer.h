//
// GeoLocationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_GeoLocation_INCLUDED
#define TypeDeserializer_ONVIF_GeoLocation_INCLUDED


#include "ONVIF/GeoLocation.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::GeoLocation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::GeoLocation& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::GeoLocation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"lon"s,"lat"s,"elevation"s};
		bool ret = false;
		Poco::Optional<double> gen_lon;
		ret = TypeDeserializer<Poco::Optional<double>>::deserialize(REMOTING__NAMES[0], false, deser, gen_lon);
		if (ret) value.setLon(gen_lon);
		Poco::Optional<double> gen_lat;
		ret = TypeDeserializer<Poco::Optional<double>>::deserialize(REMOTING__NAMES[1], false, deser, gen_lat);
		if (ret) value.setLat(gen_lat);
		Poco::Optional<float> gen_elevation;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_elevation);
		if (ret) value.setElevation(gen_elevation);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::GeoLocation& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"lon"s,"lat"s,"elevation"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_GeoLocation_INCLUDED

