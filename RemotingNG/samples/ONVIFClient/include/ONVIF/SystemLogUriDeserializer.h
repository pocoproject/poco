//
// SystemLogUriDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SystemLogUri_INCLUDED
#define TypeDeserializer_ONVIF_SystemLogUri_INCLUDED


#include "ONVIF/SystemLogDeserializer.h"
#include "ONVIF/SystemLogSerializer.h"
#include "ONVIF/SystemLogUri.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SystemLogUri>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SystemLogUri& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SystemLogUri& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Type"s,"Uri"s};
		bool ret = false;
		ONVIF::SystemLog gen_type;
		ret = TypeDeserializer<ONVIF::SystemLog>::deserialize(REMOTING__NAMES[0], true, deser, gen_type);
		if (ret) value.setType(gen_type);
		Poco::URI gen_uri;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[1], true, deser, gen_uri);
		if (ret) value.setUri(gen_uri);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SystemLogUri_INCLUDED

