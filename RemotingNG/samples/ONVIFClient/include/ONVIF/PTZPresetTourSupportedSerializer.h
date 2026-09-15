//
// PTZPresetTourSupportedSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_PTZPresetTourSupported_INCLUDED
#define TypeSerializer_ONVIF_PTZPresetTourSupported_INCLUDED


#include "ONVIF/PTZPresetTourSupported.h"
#include "ONVIF/PTZPresetTourSupportedExtensionDeserializer.h"
#include "ONVIF/PTZPresetTourSupportedExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::PTZPresetTourSupported>
{
public:
	static void serialize(const std::string& name, const ONVIF::PTZPresetTourSupported& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::PTZPresetTourSupported& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfPresetTours"s,"PTZPresetTourOperation"s,"Extension"s,""s};
		TypeSerializer<int>::serialize(REMOTING__NAMES[0], value.getMaximumNumberOfPresetTours(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[1], value.getPTZPresetTourOperation(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::PTZPresetTourSupportedExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_PTZPresetTourSupported_INCLUDED

