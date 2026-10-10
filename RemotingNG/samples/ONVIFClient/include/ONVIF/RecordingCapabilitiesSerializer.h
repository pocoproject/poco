//
// RecordingCapabilitiesSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_RecordingCapabilities_INCLUDED
#define TypeSerializer_ONVIF_RecordingCapabilities_INCLUDED


#include "ONVIF/RecordingCapabilities.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::RecordingCapabilities>
{
public:
	static void serialize(const std::string& name, const ONVIF::RecordingCapabilities& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::RecordingCapabilities& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"ReceiverSource"s,"MediaProfileSource"s,"DynamicRecordings"s,"DynamicTracks"s,"MaxStringLength"s,""s};
		TypeSerializer<Poco::URI>::serialize(REMOTING__NAMES[0], value.getXAddr(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[1], value.getReceiverSource(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[2], value.getMediaProfileSource(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[3], value.getDynamicRecordings(), ser);
		TypeSerializer<bool>::serialize(REMOTING__NAMES[4], value.getDynamicTracks(), ser);
		TypeSerializer<int>::serialize(REMOTING__NAMES[5], value.getMaxStringLength(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_RecordingCapabilities_INCLUDED

