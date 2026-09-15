//
// MediaUriDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MediaUri_INCLUDED
#define TypeDeserializer_ONVIF_MediaUri_INCLUDED


#include "ONVIF/MediaUri.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MediaUri>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MediaUri& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MediaUri& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Uri"s,"InvalidAfterConnect"s,"InvalidAfterReboot"s,"Timeout"s};
		bool ret = false;
		Poco::URI gen_uri;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_uri);
		if (ret) value.setUri(gen_uri);
		bool gen_invalidAfterConnect;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_invalidAfterConnect);
		if (ret) value.setInvalidAfterConnect(gen_invalidAfterConnect);
		bool gen_invalidAfterReboot;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_invalidAfterReboot);
		if (ret) value.setInvalidAfterReboot(gen_invalidAfterReboot);
		std::string gen_timeout;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[3], true, deser, gen_timeout);
		if (ret) value.setTimeout(gen_timeout);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MediaUri_INCLUDED

