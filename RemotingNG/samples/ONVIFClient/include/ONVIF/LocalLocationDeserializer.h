//
// LocalLocationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_LocalLocation_INCLUDED
#define TypeDeserializer_ONVIF_LocalLocation_INCLUDED


#include "ONVIF/LocalLocation.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::LocalLocation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::LocalLocation& value)
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

	static void deserializeAttributes(Deserializer& deser, ONVIF::LocalLocation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"x"s,"y"s,"z"s};
		bool ret = false;
		Poco::Optional<float> gen_x;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[0], false, deser, gen_x);
		if (ret) value.setX(gen_x);
		Poco::Optional<float> gen_y;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[1], false, deser, gen_y);
		if (ret) value.setY(gen_y);
		Poco::Optional<float> gen_z;
		ret = TypeDeserializer<Poco::Optional<float>>::deserialize(REMOTING__NAMES[2], false, deser, gen_z);
		if (ret) value.setZ(gen_z);
	}

	static void deserializeImpl(Deserializer& deser, ONVIF::LocalLocation& value)
	{
		using namespace std::string_literals;
		
	}

	static void prepareDeserializeAttributes(Deserializer& deser)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"x"s,"y"s,"z"s,"http://www.onvif.org/ver10/schema"s};
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[0], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[1], false);
		deser.pushAttribute(REMOTING__NAMES[3], REMOTING__NAMES[2], false);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_LocalLocation_INCLUDED

