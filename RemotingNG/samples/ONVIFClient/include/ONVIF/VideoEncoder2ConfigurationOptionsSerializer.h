//
// VideoEncoder2ConfigurationOptionsSerializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED
#define TypeSerializer_ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED


#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "ONVIF/IntRangeDeserializer.h"
#include "ONVIF/IntRangeSerializer.h"
#include "ONVIF/VideoEncoder2ConfigurationOptions.h"
#include "ONVIF/VideoResolution2Deserializer.h"
#include "ONVIF/VideoResolution2Serializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::VideoEncoder2ConfigurationOptions>
{
public:
	static void prepareSerializeAttributes(Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GovLengthRange"s,"FrameRatesSupported"s,"ProfilesSupported"s,"ConstantBitRateSupported"s,"GuaranteedFrameRateSupported"s,"http://www.onvif.org/ver10/schema"s};
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3]);
		ser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4]);
	}

	static void serialize(const std::string& name, const ONVIF::VideoEncoder2ConfigurationOptions& value, Serializer& ser)
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

	static void serializeAttributes(const ONVIF::VideoEncoder2ConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"GovLengthRange"s,"FrameRatesSupported"s,"ProfilesSupported"s,"ConstantBitRateSupported"s,"GuaranteedFrameRateSupported"s};
		TypeSerializer<std::vector<std::vector<int>>>::serialize(REMOTING__NAMES[0], value.getGovLengthRange(), ser);
		TypeSerializer<std::vector<std::vector<float>>>::serialize(REMOTING__NAMES[1], value.getFrameRatesSupported(), ser);
		TypeSerializer<std::vector<std::vector<std::string>>>::serialize(REMOTING__NAMES[2], value.getProfilesSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[3], value.getConstantBitRateSupported(), ser);
		TypeSerializer<Poco::Optional<bool>>::serialize(REMOTING__NAMES[4], value.getGuaranteedFrameRateSupported(), ser);
	}

	static void serializeImpl(const ONVIF::VideoEncoder2ConfigurationOptions& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Encoding"s,"QualityRange"s,"ResolutionsAvailable"s,"BitrateRange"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getEncoding(), ser);
		TypeSerializer<ONVIF::FloatRange>::serialize(REMOTING__NAMES[1], value.getQualityRange(), ser);
		TypeSerializer<std::vector<ONVIF::VideoResolution2>>::serialize(REMOTING__NAMES[2], value.getResolutionsAvailable(), ser);
		TypeSerializer<ONVIF::IntRange>::serialize(REMOTING__NAMES[3], value.getBitrateRange(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_VideoEncoder2ConfigurationOptions_INCLUDED

