// 
// GetSystemBackupResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetSystemBackupResponse_INCLUDED
#define ONVIF_Device_GetSystemBackupResponse_INCLUDED


#include "ONVIF/BackupFile.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetSystemBackupResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetSystemBackupResponse
{
public:
	GetSystemBackupResponse();

	GetSystemBackupResponse(const std::vector<ONVIF::BackupFile>& backupFiles);

	virtual ~GetSystemBackupResponse();

	const std::vector<ONVIF::BackupFile>& getBackupFiles() const;

	std::vector<ONVIF::BackupFile>& getBackupFiles();

	void setBackupFiles(const std::vector<ONVIF::BackupFile>& val);

	void setBackupFiles(std::vector<ONVIF::BackupFile>&& val);

private:
	//@ name=BackupFiles
	//@ order=0
	std::vector<ONVIF::BackupFile> _backupFiles;

};


inline const std::vector<ONVIF::BackupFile>& GetSystemBackupResponse::getBackupFiles() const
{
	return _backupFiles;
}


inline std::vector<ONVIF::BackupFile>& GetSystemBackupResponse::getBackupFiles()
{
	return _backupFiles;
}


inline void GetSystemBackupResponse::setBackupFiles(const std::vector<ONVIF::BackupFile>& val)
{
	_backupFiles = val;
}


inline void GetSystemBackupResponse::setBackupFiles(std::vector<ONVIF::BackupFile>&& val)
{
	_backupFiles = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetSystemBackupResponse_INCLUDED
