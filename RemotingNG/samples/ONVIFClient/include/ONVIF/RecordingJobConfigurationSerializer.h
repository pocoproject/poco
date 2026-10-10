//
// RecordingJobConfigurationSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingJobConfiguration_INCLUDED
#define TypeSerializer_ONVIF_RecordingJobConfiguration_INCLUDED


#include "ONVIF/RecordingEventFilterDeserializer.h"
#include "ONVIF/RecordingEventFilterSerializer.h"
#include "ONVIF/RecordingJobConfiguration.h"
#include "ONVIF/RecordingJobConfigurationExtensionDeserializer.h"
#include "ONVIF/RecordingJobConfigurationExtensionSerializer.h"
#include "ONVIF/RecordingJobSourceDeserializer.h"
#include "ONVIF/RecordingJobSourceSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingJobConfiguration>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ScheduleToken"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[1], REMOTING__NAMES[0]);
	}

	static void serialize(const std::string& name, const ONVIF::RecordingJobConfiguration& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::RecordingJobConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ScheduleToken"s};
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[0], value.getScheduleToken(), ser);
	}

	static void serializeImpl(const ONVIF::RecordingJobConfiguration& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RecordingToken"s,"Mode"s,"Priority"s,"Source"s,"Extension"s,"EventFilter"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getRecordingToken(), ser);
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[1], value.getMode(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[2], value.getPriority(), ser);
		TypeSerializer<std::vector<ONVIF::RecordingJobSource>>::serialize(REMOTING__NAMES[3], value.getSource(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RecordingJobConfigurationExtension>>::serialize(REMOTING__NAMES[4], value.getExtension(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::RecordingEventFilter>>::serialize(REMOTING__NAMES[5], value.getEventFilter(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingJobConfiguration_INCLUDED

