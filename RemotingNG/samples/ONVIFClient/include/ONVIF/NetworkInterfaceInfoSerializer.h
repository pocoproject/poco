//
// NetworkInterfaceInfoSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_NetworkInterfaceInfo_INCLUDED
#define TypeSerializer_ONVIF_NetworkInterfaceInfo_INCLUDED


#include "ONVIF/NetworkInterfaceInfo.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::NetworkInterfaceInfo>
{
public:
	static void serialize(const std::string& name, const ONVIF::NetworkInterfaceInfo& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::NetworkInterfaceInfo& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Name"s,"HwAddress"s,"MTU"s,""s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getName(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getHwAddress(), ser);
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[2], value.getMTU(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_NetworkInterfaceInfo_INCLUDED

