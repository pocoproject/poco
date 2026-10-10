//
// PolylineArrayConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PolylineArrayConfiguration_INCLUDED
#define TypeSerializer_ONVIF_PolylineArrayConfiguration_INCLUDED


#include "ONVIF/PolylineArrayConfiguration.h"
#include "ONVIF/PolylineArrayDeserializer.h"
#include "ONVIF/PolylineArraySerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PolylineArrayConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::PolylineArrayConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		ser.registerNamespace(REMOTING__NAMESPACE);
		ser.serializeStructBegin(name);
		ser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		serializeImpl(value, ser);
		ser.popProperty(SerializerBase::PROP_NAMESPACE);
		ser.serializeStructEnd(name);
	}

	static void serializeImpl(const ONVIF::PolylineArrayConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PolylineArray"s,""s};
		TypeSerializer<ONVIF::PolylineArray>::serialize(REMOTING__NAMES[0], value.getPolylineArray(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PolylineArrayConfiguration_INCLUDED

