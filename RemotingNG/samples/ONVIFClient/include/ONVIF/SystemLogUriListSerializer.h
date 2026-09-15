//
// SystemLogUriListSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_SystemLogUriList_INCLUDED
#define TypeSerializer_ONVIF_SystemLogUriList_INCLUDED


#include "ONVIF/SystemLogUriDeserializer.h"
#include "ONVIF/SystemLogUriList.h"
#include "ONVIF/SystemLogUriSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::SystemLogUriList>
{
public:
	static void serialize(const std::string& name, const ONVIF::SystemLogUriList& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::SystemLogUriList& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SystemLog"s,""s};
		TypeSerializer<std::vector<ONVIF::SystemLogUri>>::serialize(REMOTING__NAMES[0], value.getSystemLog(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_SystemLogUriList_INCLUDED

