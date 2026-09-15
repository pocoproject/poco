//
// AudioEncoder2ConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED


#include "ONVIF/AudioEncoder2ConfigurationOptions.h"
#include "ONVIF/IntItemsDeserializer.h"
#include "ONVIF/IntItemsSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::AudioEncoder2ConfigurationOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::AudioEncoder2ConfigurationOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::AudioEncoder2ConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"BitrateList"s,"SampleRateList"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getEncoding(), ser);
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[1], value.getBitrateList(), ser);
		TypeSerializer<ONVIF::IntItems>::serialize(REMOTING__NAMES[2], value.getSampleRateList(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_AudioEncoder2ConfigurationOptions_INCLUDED

