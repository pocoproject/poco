//
// ExposureOptions20Deserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ExposureOptions20_INCLUDED
#define TypeDeserializer_ONVIF_ExposureOptions20_INCLUDED


#include "ONVIF/ExposureOptions20.h"
#include "ONVIF/FloatRangeDeserializer.h"
#include "ONVIF/FloatRangeSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ExposureOptions20>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ExposureOptions20& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ExposureOptions20& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Priority"s,"MinExposureTime"s,"MaxExposureTime"s,"MinGain"s,"MaxGain"s,"MinIris"s,"MaxIris"s,"ExposureTime"s,"Gain"s,"Iris"s};
		bool ret = false;
		std::vector<std::string> gen_mode;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		std::vector<std::string> gen_priority;
		ret = TypeDeserializer<std::vector<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_priority);
		if (ret) value.setPriority(gen_priority);
		Poco::SharedPtr<ONVIF::FloatRange> gen_minExposureTime;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[2], false, deser, gen_minExposureTime);
		if (ret) value.setMinExposureTime(gen_minExposureTime);
		Poco::SharedPtr<ONVIF::FloatRange> gen_maxExposureTime;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[3], false, deser, gen_maxExposureTime);
		if (ret) value.setMaxExposureTime(gen_maxExposureTime);
		Poco::SharedPtr<ONVIF::FloatRange> gen_minGain;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[4], false, deser, gen_minGain);
		if (ret) value.setMinGain(gen_minGain);
		Poco::SharedPtr<ONVIF::FloatRange> gen_maxGain;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[5], false, deser, gen_maxGain);
		if (ret) value.setMaxGain(gen_maxGain);
		Poco::SharedPtr<ONVIF::FloatRange> gen_minIris;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[6], false, deser, gen_minIris);
		if (ret) value.setMinIris(gen_minIris);
		Poco::SharedPtr<ONVIF::FloatRange> gen_maxIris;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[7], false, deser, gen_maxIris);
		if (ret) value.setMaxIris(gen_maxIris);
		Poco::SharedPtr<ONVIF::FloatRange> gen_exposureTime;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[8], false, deser, gen_exposureTime);
		if (ret) value.setExposureTime(gen_exposureTime);
		Poco::SharedPtr<ONVIF::FloatRange> gen_gain;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[9], false, deser, gen_gain);
		if (ret) value.setGain(gen_gain);
		Poco::SharedPtr<ONVIF::FloatRange> gen_iris;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::FloatRange>>::deserialize(REMOTING__NAMES[10], false, deser, gen_iris);
		if (ret) value.setIris(gen_iris);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ExposureOptions20_INCLUDED

