//
// RecordingConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingConfiguration_INCLUDED
#define TypeSerializer_ONVIF_RecordingConfiguration_INCLUDED


#include "ONVIF/RecordingConfiguration.h"
#include "ONVIF/RecordingSourceInformationDeserializer.h"
#include "ONVIF/RecordingSourceInformationSerializer.h"
#include "ONVIF/RecordingTargetConfigurationDeserializer.h"
#include "ONVIF/RecordingTargetConfigurationSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingConfiguration>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingConfiguration& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Source"s,"Content"s,"MaximumRetentionTime"s,"Target"s,""s};
		TypeSerializer<ONVIF::RecordingSourceInformation>::serialize(REMOTING__NAMES[0], value.getSource(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getContent(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[2], value.getMaximumRetentionTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RecordingTargetConfiguration>>::serialize(REMOTING__NAMES[3], value.getTarget(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingConfiguration_INCLUDED

