//
// Exposure20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Exposure20_INCLUDED
#define TypeDeserializer_ONVIF_Exposure20_INCLUDED


#include "ONVIF/Exposure20.h"
#include "ONVIF/RectangleDeserializer.h"
#include "ONVIF/RectangleSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Exposure20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Exposure20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Exposure20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Priority"s,"Window"s,"MinExposureTime"s,"MaxExposureTime"s,"MinGain"s,"MaxGain"s,"MinIris"s,"MaxIris"s,"ExposureTime"s,"Gain"s,"Iris"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::Optional<std::string> gen_priority;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_priority);
		if (ret) value.setPriority(gen_priority);
		Poco::SharedPtr<ONVIF::Rectangle> gen_window;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Rectangle>>::deserialize(REMOTING__NAMES[2], false, deser, gen_window);
		if (ret) value.setWindow(gen_window);
		Poco::Optional<float> gen_minExposureTime;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[3], false, deser, gen_minExposureTime);
		if (ret) value.setMinExposureTime(gen_minExposureTime);
		Poco::Optional<float> gen_maxExposureTime;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[4], false, deser, gen_maxExposureTime);
		if (ret) value.setMaxExposureTime(gen_maxExposureTime);
		Poco::Optional<float> gen_minGain;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[5], false, deser, gen_minGain);
		if (ret) value.setMinGain(gen_minGain);
		Poco::Optional<float> gen_maxGain;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[6], false, deser, gen_maxGain);
		if (ret) value.setMaxGain(gen_maxGain);
		Poco::Optional<float> gen_minIris;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[7], false, deser, gen_minIris);
		if (ret) value.setMinIris(gen_minIris);
		Poco::Optional<float> gen_maxIris;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[8], false, deser, gen_maxIris);
		if (ret) value.setMaxIris(gen_maxIris);
		Poco::Optional<float> gen_exposureTime;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[9], false, deser, gen_exposureTime);
		if (ret) value.setExposureTime(gen_exposureTime);
		Poco::Optional<float> gen_gain;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[10], false, deser, gen_gain);
		if (ret) value.setGain(gen_gain);
		Poco::Optional<float> gen_iris;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[11], false, deser, gen_iris);
		if (ret) value.setIris(gen_iris);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Exposure20_INCLUDED

