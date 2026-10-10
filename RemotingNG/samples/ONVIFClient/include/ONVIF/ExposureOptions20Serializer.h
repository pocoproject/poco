//
// ExposureOptions20Serializer.h
//
// Package: Generated
// Module:  TypeSerializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeSerializer_ONVIF_ExposureOptions20_INCLUDED
#define TypeSerializer_ONVIF_ExposureOptions20_INCLUDED


#include "ONVIF/ExposureOptions20.h"
#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "Poco/RemotingNG/TypeSerializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeSerializer<ONVIF::ExposureOptions20>
{
public:
	static void serialize(const std::string& name, const ONVIF::ExposureOptions20& value, Serializer& ser)
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

	static void serializeImpl(const ONVIF::ExposureOptions20& value, Serializer& ser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Priority"s,"MinExposureTime"s,"MaxExposureTime"s,"MinGain"s,"MaxGain"s,"MinIris"s,"MaxIris"s,"ExposureTime"s,"Gain"s,"Iris"s,""s};
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[0], value.getMode(), ser);
		TypeSerializer<std::vector<std::string>>::serialize(REMOTING__NAMES[1], value.getPriority(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[2], value.getMinExposureTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[3], value.getMaxExposureTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[4], value.getMinGain(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[5], value.getMaxGain(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[6], value.getMinIris(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[7], value.getMaxIris(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[8], value.getExposureTime(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[9], value.getGain(), ser);
		TypeSerializer<Poco::SharedPtr<ONVIF::FloatRange>>::serialize(REMOTING__NAMES[10], value.getIris(), ser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeSerializer_ONVIF_ExposureOptions20_INCLUDED

