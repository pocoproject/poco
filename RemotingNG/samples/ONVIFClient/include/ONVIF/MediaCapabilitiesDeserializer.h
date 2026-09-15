//
// MediaCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_MediaCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_MediaCapabilities_INCLUDED


#include "ONVIF/MediaCapabilities.h"
#include "ONVIF/MediaCapabilitiesExtensionDeserializer.h"
#include "ONVIF/MediaCapabilitiesExtensionSerializer.h"
#include "ONVIF/RealTimeStreamingCapabilitiesDeserializer.h"
#include "ONVIF/RealTimeStreamingCapabilitiesSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::MediaCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::MediaCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::MediaCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"XAddr"s,"StreamingCapabilities"s,"Extension"s};
		bool ret = false;
		Poco::URI gen_xAddr;
		ret = TypeDeserializer<Poco::URI>::deserialize(REMOTING__NAMES[0], true, deser, gen_xAddr);
		if (ret) value.setXAddr(gen_xAddr);
		ONVIF::RealTimeStreamingCapabilities gen_streamingCapabilities;
		ret = TypeDeserializer<ONVIF::RealTimeStreamingCapabilities>::deserialize(REMOTING__NAMES[1], true, deser, gen_streamingCapabilities);
		if (ret) value.setStreamingCapabilities(gen_streamingCapabilities);
		Poco::SharedPtr<ONVIF::MediaCapabilitiesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::MediaCapabilitiesExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_MediaCapabilities_INCLUDED

