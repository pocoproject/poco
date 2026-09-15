// 
// BackupFile.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_BackupFile_INCLUDED
#define ONVIF_BackupFile_INCLUDED


#include "ONVIF/AttachmentData.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=BackupFile
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API BackupFile
{
public:
	BackupFile();

	BackupFile(
		const std::string& name, 
		const AttachmentData& data);

	virtual ~BackupFile();

	const AttachmentData& getData() const;

	const std::string& getName() const;

	void setData(const AttachmentData& val);

	void setData(AttachmentData&& val);

	void setName(const std::string& val);

	void setName(std::string&& val);

private:
	//@ name=Name
	//@ order=0
	std::string _name;

	//@ name=Data
	//@ order=1
	AttachmentData _data;

};


inline const AttachmentData& BackupFile::getData() const
{
	return _data;
}


inline const std::string& BackupFile::getName() const
{
	return _name;
}


inline void BackupFile::setData(const AttachmentData& val)
{
	_data = val;
}


inline void BackupFile::setData(AttachmentData&& val)
{
	_data = std::move(val);
}


inline void BackupFile::setName(const std::string& val)
{
	_name = val;
}


inline void BackupFile::setName(std::string&& val)
{
	_name = std::move(val);
}


} // ONVIF


#endif // ONVIF_BackupFile_INCLUDED
