//
// LocationEntityDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_LocationEntity_INCLUDED
#define TypeDeserializer_ONVIF_LocationEntity_INCLUDED


#include "ONVIF/GeoLocationDeserializer.h"
#include "ONVIF/GeoLocationSerializer.h"
#include "ONVIF/GeoOrientationDeserializer.h"
#include "ONVIF/GeoOrientationSerializer.h"
#include "ONVIF/LocalLocationDeserializer.h"
#include "ONVIF/LocalLocationSerializer.h"
#include "ONVIF/LocalOrientationDeserializer.h"
#include "ONVIF/LocalOrientationSerializer.h"
#include "ONVIF/LocationEntity.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::LocationEntity>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::LocationEntity& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::LocationEntity& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Entity"s,"Token"s,"Fixed"s,"GeoSource"s,"AutoGeo"s};
		bool ret = false;
		Poco::Optional<std::string> gen_entity;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[0], false, deser, gen_entity);
		if (ret) value.setEntity(gen_entity);
		Poco::Optional<std::string> gen_token;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_token);
		if (ret) value.setToken(gen_token);
		Poco::Optional<bool> gen_fixed;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_fixed);
		if (ret) value.setFixed(gen_fixed);
		Poco::Optional<Poco::URI> gen_geoSource;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[3], false, deser, gen_geoSource);
		if (ret) value.setGeoSource(gen_geoSource);
		Poco::Optional<bool> gen_autoGeo;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[4], false, deser, gen_autoGeo);
		if (ret) value.setAutoGeo(gen_autoGeo);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::LocationEntity& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GeoLocation"s,"GeoOrientation"s,"LocalLocation"s,"LocalOrientation"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::GeoLocation> gen_geoLocation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::GeoLocation>>::deserialize(REMOTING__NAMES[0], false, deser, gen_geoLocation);
		if (ret) value.setGeoLocation(gen_geoLocation);
		Poco::SharedPtr<ONVIF::GeoOrientation> gen_geoOrientation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::GeoOrientation>>::deserialize(REMOTING__NAMES[1], false, deser, gen_geoOrientation);
		if (ret) value.setGeoOrientation(gen_geoOrientation);
		Poco::SharedPtr<ONVIF::LocalLocation> gen_localLocation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::LocalLocation>>::deserialize(REMOTING__NAMES[2], false, deser, gen_localLocation);
		if (ret) value.setLocalLocation(gen_localLocation);
		Poco::SharedPtr<ONVIF::LocalOrientation> gen_localOrientation;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::LocalOrientation>>::deserialize(REMOTING__NAMES[3], false, deser, gen_localOrientation);
		if (ret) value.setLocalOrientation(gen_localOrientation);
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Entity"s,"Token"s,"Fixed"s,"GeoSource"s,"AutoGeo"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_LocationEntity_INCLUDED

