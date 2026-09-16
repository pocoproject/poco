//
// Mpeg4OptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Mpeg4Options_INCLUDED
#define TypeSerializer_ONVIF_Mpeg4Options_INCLUDED


#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/Mpeg4Options.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Mpeg4Options>
{
public:
	static void serialize(const std::string& name, const ONVIF::Mpeg4Options& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Mpeg4Options& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ResolutionsAvailable"s,"GovLengthRange"s,"FrameRateRange"s,"EncodingIntervalRange"s,"Mpeg4ProfilesSupported"s,""s};
		TypeSerializer<std::vector<ONVIF::VideoResolution>>::serialize(REMOTING__NAMES[0], value.getResolutionsAvailable(), ser);
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[1], value.getGovLengthRange(), ser);
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[2], value.getFrameRateRange(), ser);
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[3], value.getEncodingIntervalRange(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[4], value.getMpeg4ProfilesSupported(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Mpeg4Options_INCLUDED

