//
// VideoSourceConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoSourceConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_VideoSourceConfigurationOptions_INCLUDED


#include "ONVIF/IntRectangleRangeDeserializer.h"
#include "ONVIF/IntRectangleRangeSerializer.h"
#include "ONVIF/VideoSourceConfigurationOptions.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtensionDeserializer.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtensionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoSourceConfigurationOptions>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfProfiles"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::VideoSourceConfigurationOptions& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::VideoSourceConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"MaximumNumberOfProfiles"s};
		TypeSerializer<Poco::Optional<int>>::serialize(REMOTING__NAMES[0], value.getMaximumNumberOfProfiles(), ser);
	}

	static void serializeImpl(const ONVIF::VideoSourceConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BoundsRange"s,"VideoSourceTokensAvailable"s,"Extension"s,""s};
		TypeSerializer<ONVIF::IntRectangleRange>::serialize(REMOTING__NAMES[0], value.getBoundsRange(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[1], value.getVideoSourceTokensAvailable(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::VideoSourceConfigurationOptionsExtension>>::serialize(REMOTING__NAMES[2], value.getExtension(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoSourceConfigurationOptions_INCLUDED

