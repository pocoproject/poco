//
// SystemCapabilitiesExtensionDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SystemCapabilitiesExtension_INCLUDED
#define TypeDeserializer_ONVIF_SystemCapabilitiesExtension_INCLUDED


#include "ONVIF/SystemCapabilitiesExtension.h"
#include "ONVIF/SystemCapabilitiesExtension2Deserializer.h"
#include "ONVIF/SystemCapabilitiesExtension2Serializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SystemCapabilitiesExtension>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SystemCapabilitiesExtension& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SystemCapabilitiesExtension& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"HttpFirmwareUpgrade"s,"HttpSystemBackup"s,"HttpSystemLogging"s,"HttpSupportInformation"s,"Extension"s};
		bool ret = false;
		Poco::Optional<bool> gen_httpFirmwareUpgrade;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[0], false, deser, gen_httpFirmwareUpgrade);
		if (ret) value.setHttpFirmwareUpgrade(gen_httpFirmwareUpgrade);
		Poco::Optional<bool> gen_httpSystemBackup;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[1], false, deser, gen_httpSystemBackup);
		if (ret) value.setHttpSystemBackup(gen_httpSystemBackup);
		Poco::Optional<bool> gen_httpSystemLogging;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[2], false, deser, gen_httpSystemLogging);
		if (ret) value.setHttpSystemLogging(gen_httpSystemLogging);
		Poco::Optional<bool> gen_httpSupportInformation;
		ret = TypeDeserializer<Poco::Optional<bool>>::deserialize(REMOTING__NAMES[3], false, deser, gen_httpSupportInformation);
		if (ret) value.setHttpSupportInformation(gen_httpSupportInformation);
		Poco::SharedPtr<ONVIF::SystemCapabilitiesExtension2> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SystemCapabilitiesExtension2>>::deserialize(REMOTING__NAMES[4], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SystemCapabilitiesExtension_INCLUDED

