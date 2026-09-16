//
// AACDecOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AACDecOptions_INCLUDED
#define TypeSerializer_ONVIF_AACDecOptions_INCLUDED


#include "ONVIF/AACDecOptions.h"
#include "ONVIF/IntItemsDeserializer.h"
#include "ONVIF/IntItemsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AACDecOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::AACDecOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AACDecOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Bitrate"s,"SampleRateRange"s,""s};
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[0], value.getBitrate(), ser);
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[1], value.getSampleRateRange(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AACDecOptions_INCLUDED

