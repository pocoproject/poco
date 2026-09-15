// 
// RestoreSystem.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_RestoreSystem_INCLUDED
#define ONVIF_Device_RestoreSystem_INCLUDED


#include "ONVIF/BackupFile.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#RestoreSystem"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API RestoreSystem
{
public:
	RestoreSystem();

	RestoreSystem(const std::vector<ONVIF::BackupFile>& backupFiles);

	virtual ~RestoreSystem();

	const std::vector<ONVIF::BackupFile>& getBackupFiles() const;

	std::vector<ONVIF::BackupFile>& getBackupFiles();

	void setBackupFiles(const std::vector<ONVIF::BackupFile>& val);

	void setBackupFiles(std::vector<ONVIF::BackupFile>&& val);

private:
	//@ name=BackupFiles
	//@ order=0
	std::vector<ONVIF::BackupFile> _backupFiles;

};


inline const std::vector<ONVIF::BackupFile>& RestoreSystem::getBackupFiles() const
{
	return _backupFiles;
}


inline std::vector<ONVIF::BackupFile>& RestoreSystem::getBackupFiles()
{
	return _backupFiles;
}


inline void RestoreSystem::setBackupFiles(const std::vector<ONVIF::BackupFile>& val)
{
	_backupFiles = val;
}


inline void RestoreSystem::setBackupFiles(std::vector<ONVIF::BackupFile>&& val)
{
	_backupFiles = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_RestoreSystem_INCLUDED
