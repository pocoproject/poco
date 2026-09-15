//
// H264DecOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_H264DecOptions_INCLUDED
#define TypeSerializer_ONVIF_H264DecOptions_INCLUDED


#include "ONVIF/H264DecOptions.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/VideoResolutionDeserializer.h"
#include "ONVIF/VideoResolutionSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::H264DecOptions>
{
public:
	static void serialize(const std::string& name, const ONVIF::H264DecOptions& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::H264DecOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ResolutionsAvailable"s,"SupportedH264Profiles"s,"SupportedInputBitrate"s,"SupportedFrameRate"s,""s};
		TypeSerializer<std::vector<ONVIF::VideoResolution>>::serialize(REMOTING__NAMES[0], value.getResolutionsAvailable(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[1], value.getSupportedH264Profiles(), ser);
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[2], value.getSupportedInputBitrate(), ser);
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[3], value.getSupportedFrameRate(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_H264DecOptions_INCLUDED

