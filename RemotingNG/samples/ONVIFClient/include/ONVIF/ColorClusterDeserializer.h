//
// ColorClusterDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ColorCluster_INCLUDED
#define TypeDeserializer_ONVIF_ColorCluster_INCLUDED


#include "ONVIF/ColorCluster.h"
#include "ONVIF/ColorCovarianceDeserializer.h"
#include "ONVIF/ColorCovarianceSerializer.h"
#include "ONVIF/ColorDeserializer.h"
#include "ONVIF/ColorSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ColorCluster>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ColorCluster& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ColorCluster& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Color"s,"Weight"s,"Covariance"s};
		bool ret = false;
		ONVIF::Color gen_color;
		ret = TypeDeserializer<ONVIF::Color>::deserialize(REMOTING__NAMES[0], true, deser, gen_color);
		if (ret) value.setColor(gen_color);
		Poco::Optional<float> gen_weight;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_weight);
		if (ret) value.setWeight(gen_weight);
		Poco::SharedPtr<ONVIF::ColorCovariance> gen_covariance;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::ColorCovariance>>::deserialize(REMOTING__NAMES[2], false, deser, gen_covariance);
		if (ret) value.setCovariance(gen_covariance);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ColorCluster_INCLUDED

