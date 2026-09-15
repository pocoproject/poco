//
// GetRemoteUserResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetRemoteUserResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetRemoteUserResponse_INCLUDED


#include "ONVIF/Device/GetRemoteUserResponse.h"
#include "ONVIF/RemoteUserDeserializer.h"
#include "ONVIF/RemoteUserSerializer.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetRemoteUserResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetRemoteUserResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetRemoteUserResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"RemoteUser"s};
		bool ret = false;
		Poco::SharedPtr<ONVIF::RemoteUser> gen_remoteUser;
		ret = TypeDeserializer<Poco::SharedPtr<ONVIF::RemoteUser>>::deserialize(REMOTING__NAMES[0], false, deser, gen_remoteUser);
		if (ret) value.setRemoteUser(gen_remoteUser);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetRemoteUserResponse_INCLUDED

