//
// LocationEntitySerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_LocationEntity_INCLUDED
#define TypeSerializer_ONVIF_LocationEntity_INCLUDED


#include "ONVIF/GeoLocationDeserializer.h"
#include "ONVIF/GeoLocationSerializer.h"
#include "ONVIF/GeoOrientationDeserializer.h"
#include "ONVIF/GeoOrientationSerializer.h"
#include "ONVIF/LocalLocationDeserializer.h"
#include "ONVIF/LocalLocationSerializer.h"
#include "ONVIF/LocalOrientationDeserializer.h"
#include "ONVIF/LocalOrientationSerializer.h"
#include "ONVIF/LocationEntity.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::LocationEntity>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Entity"s,"Token"s,"Fixed"s,"GeoSource"s,"AutoGeo"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4]);
	}

	static void serialize(const std::string& name, const ONVIF::LocationEntity& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		prepareSerializeAttributes(ser);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeAttributes(value, ser);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeAttributes(const ONVIF::LocationEntity& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Entity"s,"Token"s,"Fixed"s,"GeoSource"s,"AutoGeo"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getEntity(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getToken(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[2], value.getFixed(), ser);
		TypeSerializer<Poco::Optional<Poco::URI>>::serialize(REMOTING__NAMES[3], value.getGeoSource(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getAutoGeo(), ser);
	}

	static void serializeImpl(const ONVIF::LocationEntity& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GeoLocation"s,"GeoOrientation"s,"LocalLocation"s,"LocalOrientation"s,""s};
		TypeSerializer<Poco::SharedPtr<ONVIF::GeoLocation>>::serialize(REMOTING__NAMES[0], value.getGeoLocation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::GeoOrientation>>::serialize(REMOTING__NAMES[1], value.getGeoOrientation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::LocalLocation>>::serialize(REMOTING__NAMES[2], value.getLocalLocation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::LocalOrientation>>::serialize(REMOTING__NAMES[3], value.getLocalOrientation(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_LocationEntity_INCLUDED

