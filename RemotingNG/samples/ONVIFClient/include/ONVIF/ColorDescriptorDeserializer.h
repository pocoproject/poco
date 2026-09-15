//
// ColorDescriptorDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_ColorDescriptor_INCLUDED
#define TypeDeserializer_ONVIF_ColorDescriptor_INCLUDED


#include "ONVIF/ColorClusterDeserializer.h"
#include "ONVIF/ColorClusterSerializer.h"
#include "ONVIF/ColorDescriptor.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::ColorDescriptor>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::ColorDescriptor& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::ColorDescriptor& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"ColorCluster"s,"Extension"s};
		bool ret = false;
		std::vector<ONVIF::ColorCluster> gen_colorCluster;
		ret = TypeDeserializer<std::vector<ONVIF::ColorCluster>>::deserialize(REMOTING__NAMES[0], false, deser, gen_colorCluster);
		if (ret) value.setColorCluster(gen_colorCluster);
		Poco::Optional<std::string> gen_extension;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_ColorDescriptor_INCLUDED

