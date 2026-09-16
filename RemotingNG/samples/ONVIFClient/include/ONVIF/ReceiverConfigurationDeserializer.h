//
// ReceiverConfigurationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ReceiverConfiguration_INCLUDED
#define TypeDeserializer_ONVIF_ReceiverConfiguration_INCLUDED


#include "ONVIF/ReceiverConfiguration.h"
#include "ONVIF/StreamSetupDeserializer.h"
#include "ONVIF/StreamSetupSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ReceiverConfiguration>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ReceiverConfiguration& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ReceiverConfiguration& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"Mode"s,"MediaUri"s,"StreamSetup"s};
		bool ret = false;
		std::string gen_mode;
		ret = TypeDeserializer<std::string>::deserialize(REMOTING__NAMES[0], true, deser, gen_mode);
		if (ret) value.setMode(gen_mode);
		Poco::URI gen_mediaUri;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[1], true, deser, gen_mediaUri);
		if (ret) value.setMediaUri(gen_mediaUri);
		ONVIF::StreamSetup gen_streamSetup;
		ret = TypeDeserializer<ONVIF::StreamSetup>::deserialize(REMOTING__NAMES[2], true, deser, gen_streamSetup);
		if (ret) value.setStreamSetup(gen_streamSetup);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ReceiverConfiguration_INCLUDED

