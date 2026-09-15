//
// GetSystemUrisResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetSystemUrisResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetSystemUrisResponse_INCLUDED


#include "ONVIF/Device/ExtensionDeserializer.h"
#include "ONVIF/Device/ExtensionSerializer.h"
#include "ONVIF/Device/GetSystemUrisResponse.h"
#include "ONVIF/SystemLogUriListDeserializer.h"
#include "ONVIF/SystemLogUriListSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetSystemUrisResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetSystemUrisResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMESPACE("http://www.onvif.org/ver10/device/wsdl"s);
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetSystemUrisResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"SystemLogUris"s,"SupportInfoUri"s,"SystemBackupUri"s,"Extension"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::SystemLogUriList> gen_systemLogUris;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::SystemLogUriList>>::deserialize(REMOTING__NAMES[0], false, deser, gen_systemLogUris);
		if (ret) value.setSystemLogUris(gen_systemLogUris);
		Poco::Optional<Poco::URI> gen_supportInfoUri;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[1], false, deser, gen_supportInfoUri);
		if (ret) value.setSupportInfoUri(gen_supportInfoUri);
		Poco::Optional<Poco::URI> gen_systemBackupUri;
		ret = TypeDeserializer<Poco::Optional<Poco::URI>>::deserialize(REMOTING__NAMES[2], false, deser, gen_systemBackupUri);
		if (ret) value.setSystemBackupUri(gen_systemBackupUri);
		Poco::SharedPtr<ONVIF::Device::Extension> gen_extension;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::Device::Extension>>::deserialize(REMOTING__NAMES[3], false, deser, gen_extension);
		if (ret) value.setExtension(gen_extension);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetSystemUrisResponse_INCLUDED

