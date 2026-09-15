//
// Exposure20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_Exposure20_INCLUDED
#define TypeSerializer_ONVIF_Exposure20_INCLUDED


#include "ONVIF/Exposure20.h"
#include "ONVIF/RectangleDeserializer.h"
#include "ONVIF/RectangleSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::Exposure20>
{
public:
	static void serialize(const std::string& name, const ONVIF::Exposure20& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::Exposure20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Priority"s,"Window"s,"MinExposureTime"s,"MaxExposureTime"s,"MinGain"s,"MaxGain"s,"MinIris"s,"MaxIris"s,"ExposureTime"s,"Gain"s,"Iris"s,""s};
		TypeSerializer<std::string>::serialize(REMOTING__NAMES[0], value.getMode(), ser);
		TypeSerializer<Poco::Optional<std::string>>::serialize(REMOTING__NAMES[1], value.getPriority(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::Rectangle>>::serialize(REMOTING__NAMES[2], value.getWindow(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[3], value.getMinExposureTime(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[4], value.getMaxExposureTime(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[5], value.getMinGain(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[6], value.getMaxGain(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[7], value.getMinIris(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[8], value.getMaxIris(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[9], value.getExposureTime(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[10], value.getGain(), ser);
		TypeSerializer<Poco::Optional<float>>::serialize(REMOTING__NAMES[11], value.getIris(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_Exposure20_INCLUDED

