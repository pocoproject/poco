//
// MetadataAttributesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_MetadataAttributes_INCLUDED
#define TypeSerializer_ONVIF_MetadataAttributes_INCLUDED


#include "ONVIF/MetadataAttributes.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::MetadataAttributes>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PtzSpaces"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::MetadataAttributes& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::MetadataAttributes& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"PtzSpaces"s};
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[0], value.getPtzSpaces(), ser);
	}

	static void serializeImpl(const ONVIF::MetadataAttributes& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"CanContainPTZ"s,"CanContainAnalytics"s,"CanContainNotifications"s,""s};
		TypeSerializer<bool>::serialize(REMOTING__NAMES[0], value.getCanContainPTZ(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getCanContainAnalytics(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getCanContainNotifications(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_MetadataAttributes_INCLUDED

