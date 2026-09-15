//
// ColorDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Color_INCLUDED
#define TypeDeserializer_ONVIF_Color_INCLUDED


#include "ONVIF/Color.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Color>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Color& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::Color& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"X"s,"Y"s,"Z"s,"Colorspace"s,"Likelihood"s};
		bool ret = false;
		float gen_x;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[0], true, deser, gen_x);
		if (ret) value.setX(gen_x);
		float gen_y;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[1], true, deser, gen_y);
		if (ret) value.setY(gen_y);
		float gen_z;
		ret = TypeDeserializer<float>::deserialize(REMOTING__NAMES[2], true, deser, gen_z);
		if (ret) value.setZ(gen_z);
		Poco::Optional<Poco::URI> gen_colorspace;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[3], false, deser, gen_colorspace);
		if (ret) value.setColorspace(gen_colorspace);
		Poco::Optional<float> gen_likelihood;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[4], false, deser, gen_likelihood);
		if (ret) value.setLikelihood(gen_likelihood);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::Color& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"X"s,"Y"s,"Z"s,"Colorspace"s,"Likelihood"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[0], true);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[1], true);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[2], true);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[3], false);
		deser.pushAttribute(REMOTING__NAMES[5], REMOTING__NAMES[4], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Color_INCLUDED

