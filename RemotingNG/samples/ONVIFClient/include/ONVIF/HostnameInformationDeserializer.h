//
// HostnameInformationDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_HostnameInformation_INCLUDED
#define TypeDeserializer_ONVIF_HostnameInformation_INCLUDED


#include "ONVIF/HostnameInformation.h"
#include "ONVIF/HostnameInformationExtensionDeserializer.h"
#include "ONVIF/HostnameInformationExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::HostnameInformation>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::HostnameInformation& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::HostnameInformation& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"FromDHCP"s,"Name"s,"Extension"s};
		bool ret = false;
		bool gen_fromDHCP;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_fromDHCP);
		if (ret) value.setFromDHCP(gen_fromDHCP);
		Poco::Optional<std::string> gen_name;
		ret = TypeDeserializer<Poco::Optional<std::string>>::deserialize(REMOTING__NAMES[1], false, deser, gen_name);
		if (ret) value.setName(gen_name);
		Poco::SharedPtr<ONVIF::HostnameInformationExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::HostnameInformationExtension>>::deserialize(REMOTING__NAMES[2], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_HostnameInformation_INCLUDED

