//
// ExposureDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Exposure_INCLUDED
#define TypeDeserializer_ONVIF_Exposure_INCLUDED


#include "ONVIF/Exposure.h"
#include "ONVIF/RectangleDeserializer.h"
#include "ONVIF/RectangleSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Exposure>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Exposure& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Exposure& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Priority"s,"Window"s,"MinExposureTime"s,"MaxExposureTime"s,"MinGain"s,"MaxGain"s,"MinIris"s,"MaxIris"s,"ExposureTime"s,"Gain"s,"Iris"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		std::string gen_priority;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[1], true, deser, gen_priority);
		if (ret) value.setPriority(gen_priority);
		ONVIF::Rectangle gen_window;
		ret = TypeDeserializer<ONVIF::Rectangle>::deserialize(REMOTING__NAMES[2], true, deser, gen_window);
		if (ret) value.setWindow(gen_window);
		float gen_minExposureTime;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[3], true, deser, gen_minExposureTime);
		if (ret) value.setMinExposureTime(gen_minExposureTime);
		float gen_maxExposureTime;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[4], true, deser, gen_maxExposureTime);
		if (ret) value.setMaxExposureTime(gen_maxExposureTime);
		float gen_minGain;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[5], true, deser, gen_minGain);
		if (ret) value.setMinGain(gen_minGain);
		float gen_maxGain;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[6], true, deser, gen_maxGain);
		if (ret) value.setMaxGain(gen_maxGain);
		float gen_minIris;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[7], true, deser, gen_minIris);
		if (ret) value.setMinIris(gen_minIris);
		float gen_maxIris;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[8], true, deser, gen_maxIris);
		if (ret) value.setMaxIris(gen_maxIris);
		float gen_exposureTime;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[9], true, deser, gen_exposureTime);
		if (ret) value.setExposureTime(gen_exposureTime);
		float gen_gain;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[10], true, deser, gen_gain);
		if (ret) value.setGain(gen_gain);
		float gen_iris;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[11], true, deser, gen_iris);
		if (ret) value.setIris(gen_iris);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Exposure_INCLUDED

