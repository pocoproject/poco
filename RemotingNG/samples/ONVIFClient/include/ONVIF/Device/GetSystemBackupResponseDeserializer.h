//
// GetSystemBackupResponseDeserializer.h
//
// Package: Generated
// Module:  TypeDeserializer
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef TypeDeserializer_ONVIF_Device_GetSystemBackupResponse_INCLUDED
#define TypeDeserializer_ONVIF_Device_GetSystemBackupResponse_INCLUDED


#include "ONVIF/BackupFileDeserializer.h"
#include "ONVIF/BackupFileSerializer.h"
#include "ONVIF/Device/GetSystemBackupResponse.h"
#include "Poco/RemotingNG/TypeDeserializer.h"


namespace Poco {
namespace RemotingNG {


template <>
class TypeDeserializer<ONVIF::Device::GetSystemBackupResponse>
{
public:
	static bool deserialize(const std::string& name, bool isMandatory, Deserializer& deser, ONVIF::Device::GetSystemBackupResponse& value)
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

	static void deserializeImpl(Deserializer& deser, ONVIF::Device::GetSystemBackupResponse& value)
	{
		using namespace std::string_literals;
		
		static const std::string REMOTING__NAMES[] = {"BackupFiles"s};
		bool ret = false;
		std::vector<ONVIF::BackupFile> gen_backupFiles;
		ret = TypeDeserializer<std::vector<ONVIF::BackupFile>>::deserialize(REMOTING__NAMES[0], true, deser, gen_backupFiles);
		if (ret) value.setBackupFiles(gen_backupFiles);
	}

};


} // namespace RemotingNG
} // namespace Poco


#endif // TypeDeserializer_ONVIF_Device_GetSystemBackupResponse_INCLUDED

