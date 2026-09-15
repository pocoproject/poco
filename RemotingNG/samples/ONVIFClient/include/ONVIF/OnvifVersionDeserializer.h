//
// OnvifVersionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_OnvifVersion_INCLUDED
#define TypeDeserializer_ONVIF_OnvifVersion_INCLUDED


#include "ONVIF/OnvifVersion.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::OnvifVersion>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::OnvifVersion& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::OnvifVersion& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Major"s,"Minor"s};
		bool ret = false;
		int gen_major;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[0], true, deser, gen_major);
		if (ret) value.setMajor(gen_major);
		int gen_minor;
		ret = TypeDeserializer<int>::deserialize(REMOTING__NAMES[1], true, deser, gen_minor);
		if (ret) value.setMinor(gen_minor);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_OnvifVersion_INCLUDED

