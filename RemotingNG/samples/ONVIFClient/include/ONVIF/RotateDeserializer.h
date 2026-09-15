//
// RotateDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Rotate_INCLUDED
#define TypeDeserializer_ONVIF_Rotate_INCLUDED


#include "ONVIF/Rotate.h"
#include "ONVIF/RotateExtensionDeserializer.h"
#include "ONVIF/RotateExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Rotate>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Rotate& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Rotate& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"Degree"s,"Extension"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::Optional<int> gen_degree;
		ret = TypeDeserializer<Poco::Optional<int>>::deserialize(REMOTING__NAMES[1], false, deser, gen_degree);
		if (ret) value.setDegree(gen_degree);
		Poco::SharedPtr<ONVIF::RotateExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RotateExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Rotate_INCLUDED

