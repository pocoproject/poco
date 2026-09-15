// 
// ArrayOfFileProgress.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ArrayOfFileProgress_INCLUDED
#define ONVIF_ArrayOfFileProgress_INCLUDED


#include "ONVIF/FileProgress.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ArrayOfFileProgressExtension;
} 


namespace ONVIF {


//@ name=ArrayOfFileProgress
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ArrayOfFileProgress
{
public:
	ArrayOfFileProgress();

	ArrayOfFileProgress(
		const std::vector<FileProgress>& fileProgress, 
		const Poco::SharedPtr<ArrayOfFileProgressExtension>& extension);

	virtual ~ArrayOfFileProgress();

	const Poco::SharedPtr<ArrayOfFileProgressExtension>& getExtension() const;

	const std::vector<FileProgress>& getFileProgress() const;

	std::vector<FileProgress>& getFileProgress();

	void setExtension(const Poco::SharedPtr<ArrayOfFileProgressExtension>& val);

	void setExtension(Poco::SharedPtr<ArrayOfFileProgressExtension>&& val);

	void setFileProgress(const std::vector<FileProgress>& val);

	void setFileProgress(std::vector<FileProgress>&& val);

private:
	//@ mandatory=false
	//@ name=FileProgress
	//@ order=0
	std::vector<FileProgress> _fileProgress;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ArrayOfFileProgressExtension> _extension;

};


inline const Poco::SharedPtr<ArrayOfFileProgressExtension>& ArrayOfFileProgress::getExtension() const
{
	return _extension;
}


inline const std::vector<FileProgress>& ArrayOfFileProgress::getFileProgress() const
{
	return _fileProgress;
}


inline std::vector<FileProgress>& ArrayOfFileProgress::getFileProgress()
{
	return _fileProgress;
}


inline void ArrayOfFileProgress::setExtension(const Poco::SharedPtr<ArrayOfFileProgressExtension>& val)
{
	_extension = val;
}


inline void ArrayOfFileProgress::setExtension(Poco::SharedPtr<ArrayOfFileProgressExtension>&& val)
{
	_extension = std::move(val);
}


inline void ArrayOfFileProgress::setFileProgress(const std::vector<FileProgress>& val)
{
	_fileProgress = val;
}


inline void ArrayOfFileProgress::setFileProgress(std::vector<FileProgress>&& val)
{
	_fileProgress = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ArrayOfFileProgressExtension.h"


#endif // ONVIF_ArrayOfFileProgress_INCLUDED
