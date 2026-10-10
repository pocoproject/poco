// 
// StartSystemRestoreResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_StartSystemRestoreResponse_INCLUDED
#define ONVIF_Device_StartSystemRestoreResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#StartSystemRestoreResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API StartSystemRestoreResponse
{
public:
	StartSystemRestoreResponse();

	StartSystemRestoreResponse(
		const Poco::URI& uploadUri, 
		const std::string& expectedDownTime);

	virtual ~StartSystemRestoreResponse();

	const std::string& getExpectedDownTime() const;

	const Poco::URI& getUploadUri() const;

	void setExpectedDownTime(const std::string& val);

	void setExpectedDownTime(std::string&& val);

	void setUploadUri(const Poco::URI& val);

	void setUploadUri(Poco::URI&& val);

private:
	//@ name=UploadUri
	//@ order=0
	Poco::URI _uploadUri;

	//@ name=ExpectedDownTime
	//@ order=1
	std::string _expectedDownTime;

};


inline const std::string& StartSystemRestoreResponse::getExpectedDownTime() const
{
	return _expectedDownTime;
}


inline const Poco::URI& StartSystemRestoreResponse::getUploadUri() const
{
	return _uploadUri;
}


inline void StartSystemRestoreResponse::setExpectedDownTime(const std::string& val)
{
	_expectedDownTime = val;
}


inline void StartSystemRestoreResponse::setExpectedDownTime(std::string&& val)
{
	_expectedDownTime = std::move(val);
}


inline void StartSystemRestoreResponse::setUploadUri(const Poco::URI& val)
{
	_uploadUri = val;
}


inline void StartSystemRestoreResponse::setUploadUri(Poco::URI&& val)
{
	_uploadUri = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_StartSystemRestoreResponse_INCLUDED
