//
// SystemCapabilitiesDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_SystemCapabilities_INCLUDED
#define TypeDeserializer_ONVIF_SystemCapabilities_INCLUDED


#include "ONVIF/OnvifVersionDeserializer.h"
#include "ONVIF/OnvifVersionSerializer.h"
#include "ONVIF/SystemCapabilities.h"
#include "ONVIF/SystemCapabilitiesExtensionDeserializer.h"
#include "ONVIF/SystemCapabilitiesExtensionSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::SystemCapabilities>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::SystemCapabilities& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::SystemCapabilities& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"DiscoveryResolve"s,"DiscoveryBye"s,"RemoteDiscovery"s,"SystemBackup"s,"SystemLogging"s,"FirmwareUpgrade"s,"SupportedVersions"s,"Extension"s};
		bool ret = false;
		bool gen_discoveryResolve;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[0], true, deser, gen_discoveryResolve);
		if (ret) value.setDiscoveryResolve(gen_discoveryResolve);
		bool gen_discoveryBye;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[1], true, deser, gen_discoveryBye);
		if (ret) value.setDiscoveryBye(gen_discoveryBye);
		bool gen_remoteDiscovery;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[2], true, deser, gen_remoteDiscovery);
		if (ret) value.setRemoteDiscovery(gen_remoteDiscovery);
		bool gen_systemBackup;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[3], true, deser, gen_systemBackup);
		if (ret) value.setSystemBackup(gen_systemBackup);
		bool gen_systemLogging;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[4], true, deser, gen_systemLogging);
		if (ret) value.setSystemLogging(gen_systemLogging);
		bool gen_firmwareUpgrade;
		ret = TypeDeserializer<bool>::deserialize(REMOTING__NAMES[5], true, deser, gen_firmwareUpgrade);
		if (ret) value.setFirmwareUpgrade(gen_firmwareUpgrade);
		std::vector<ONVIF::OnvifVersion> gen_supportedVersions;
		ret = TypeDeserializer<std::vector<ONVIF::OnvifVersion>>::deserialize(REMOTING__NAMES[6], true, deser, gen_supportedVersions);
		if (ret) value.setSupportedVersions(gen_supportedVersions);
		Poco::SharedPtr<ONVIF::SystemCapabilitiesExtension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SystemCapabilitiesExtension>>::deserialize(REMOTING__NAMES[7], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_SystemCapabilities_INCLUDED

