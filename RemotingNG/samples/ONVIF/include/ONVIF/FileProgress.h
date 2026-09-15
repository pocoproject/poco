// 
// FileProgress.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FileProgress_INCLUDED
#define ONVIF_FileProgress_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FileProgress
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FileProgress
{
public:
	FileProgress();

	FileProgress(
		const std::string& fileName, 
		float progress);

	virtual ~FileProgress();

	const std::string& getFileName() const;

	float getProgress() const;

	void setFileName(const std::string& val);

	void setFileName(std::string&& val);

	void setProgress(float val);

private:
	//@ name=FileName
	//@ order=0
	std::string _fileName;

	//@ name=Progress
	//@ order=1
	float _progress;

};


inline const std::string& FileProgress::getFileName() const
{
	return _fileName;
}


inline float FileProgress::getProgress() const
{
	return _progress;
}


inline void FileProgress::setFileName(const std::string& val)
{
	_fileName = val;
}


inline void FileProgress::setFileName(std::string&& val)
{
	_fileName = std::move(val);
}


inline void FileProgress::setProgress(float val)
{
	_progress = val;
}


} // ONVIF


#endif // ONVIF_FileProgress_INCLUDED
