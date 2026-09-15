//
// OSDConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_OSDConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_OSDConfigurationOptions_INCLUDED


#include "ONVIF/MaximumNumberOfOSDsDeserializer.h"
#include "ONVIF/MaximumNumberOfOSDsSerializer.h"
#include "ONVIF/OSDConfigurationOptions.h"
#include "ONVIF/OSDConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/OSDConfigurationOptionsExtensionSerializer.h"
#include "ONVIF/OSDImgOptionsDeserializer.h"
#include "ONVIF/OSDImgOptionsSerializer.h"
#include "ONVIF/OSDTextOptionsDeserializer.h"
#include "ONVIF/OSDTextOptionsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::OSDConfigurationOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::OSDConfigurationOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::OSDConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfOSDs"s,"Type"s,"PositionOption"s,"TextOption"s,"ImageOption"s,"Extension"s,""s};
		TypeSerializer<ONVIF::MaximumNumberOfOSDs>::serialize(REMOTING__NAMES[0], value.getMaximumNumberOfOSDs(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[1], value.getType(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[2], value.getPositionOption(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDTextOptions>>::serialize(REMOTING__NAMES[3], value.getTextOption(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDImgOptions>>::serialize(REMOTING__NAMES[4], value.getImageOption(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::OSDConfigurationOptionsExtension>>::serialize(REMOTING__NAMES[5], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_OSDConfigurationOptions_INCLUDED

