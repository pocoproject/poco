//
// FocusOptions20ExtensionSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_FocusOptions20Extension_INCLUDED
#define TypeSerializer_ONVIF_FocusOptions20Extension_INCLUDED


#include "ONVIF/FocusOptions20Extension.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::FocusOptions20Extension>
{
public:
	static void serialize(const std::string& name, const ONVIF::FocusOptions20Extension& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::FocusOptions20Extension& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"AFModes"s,""s};
		TypeSerializer<Poco::Optional<std::vector<std::string>>>::serialize(REMOTING__NAMES[0], value.getAFModes(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_FocusOptions20Extension_INCLUDED

