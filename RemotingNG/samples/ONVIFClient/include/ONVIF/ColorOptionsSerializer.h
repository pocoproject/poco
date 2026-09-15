//
// ColorOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ColorOptions_INCLUDED
#define TypeSerializer_ONVIF_ColorOptions_INCLUDED


#include "ONVIF/ColorDeserializer.h"
#include "ONVIF/ColorOptions.h"
#include "ONVIF/ColorSerializer.h"
#include "ONVIF/ColorspaceRangeDeserializer.h"
#include "ONVIF/ColorspaceRangeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ColorOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::ColorOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ColorOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ColorList"s,"ColorspaceRange"s,""s};
		TypeSerializer<std::vector<ONVIF::Color>>::serialize(REMOTING__NAMES[0], value.getColorList(), ser);
		TypeSerializer<std::vector<ONVIF::ColorspaceRange>>::serialize(REMOTING__NAMES[1], value.getColorspaceRange(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ColorOptions_INCLUDED

