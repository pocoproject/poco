//
// OnvifVersionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OnvifVersion_INCLUDED
#define TypeSerializer_ONVIF_OnvifVersion_INCLUDED


#include "ONVIF/OnvifVersion.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OnvifVersion>
{
public:
	static void serialize(const std::string& name, const ONVIF::OnvifVersion& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::OnvifVersion& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Major"s,"Minor"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getMajor(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[1], value.getMinor(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OnvifVersion_INCLUDED

