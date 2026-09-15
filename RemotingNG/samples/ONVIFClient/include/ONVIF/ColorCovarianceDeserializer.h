//
// ColorCovarianceDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ColorCovariance_INCLUDED
#define TypeDeserializer_ONVIF_ColorCovariance_INCLUDED


#include "ONVIF/ColorCovariance.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ColorCovariance>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ColorCovariance& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/schema"s);
		prepareDeserializeAttributes(deser);
		bool ret = deser.deserializeStructBegin(name, isMandatory);
		deser.pushProperty(SerializerBase::PROP_NAMESPACE, REMOTING__NAMESPACE);
		if (ret)
		{
			deserializeAttributes(deser, value);
			deserializeImpl(deser, value);
			deser.popProperty(SerializerBase::PROP_NAMESPACE);
			deser.deserializeStructEnd(name);
		}
		else deser.popProperty(SerializerBase::PROP_NAMESPACE);
		return ret;
	}

	static void deserializeAttributes(Deserializer& deser, ONVIF::ColorCovariance& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XX"s,"YY"s,"ZZ"s,"XY"s,"XZ"s,"YZ"s,"Colorspace"s};
		bool ret = false;
		float gen_xX;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_xX);
		if (ret) value.setXX(gen_xX);
		float gen_yY;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_yY);
		if (ret) value.setYY(gen_yY);
		float gen_zZ;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[2], true, deser, gen_zZ);
		if (ret) value.setZZ(gen_zZ);
		Poco::Optional<float> gen_xY;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[3], false, deser, gen_xY);
		if (ret) value.setXY(gen_xY);
		Poco::Optional<float> gen_xZ;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[4], false, deser, gen_xZ);
		if (ret) value.setXZ(gen_xZ);
		Poco::Optional<float> gen_yZ;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[5], false, deser, gen_yZ);
		if (ret) value.setYZ(gen_yZ);
		Poco::Optional<Poco::URI> gen_colorspace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[6], false, deser, gen_colorspace);
		if (ret) value.setColorspace(gen_colorspace);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::ColorCovariance& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XX"s,"YY"s,"ZZ"s,"XY"s,"XZ"s,"YZ"s,"Colorspace"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[1], true);
		deser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[2], true);
		deser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[4], false);
		deser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[5], false);
		deser.pushAttribute(REMOTING__NAMES[7], REMOTING__NAMES[6], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ColorCovariance_INCLUDED

