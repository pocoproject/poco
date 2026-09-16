// 
// StartFirmwareUpgradeResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_StartFirmwareUpgradeResponse_INCLUDED
#define ONVIF_Device_StartFirmwareUpgradeResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#StartFirmwareUpgradeResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API StartFirmwareUpgradeResponse
{
public:
	StartFirmwareUpgradeResponse();

	StartFirmwareUpgradeResponse(
		const Poco::URI& uploadUri, 
		const std::string& uploadDelay, 
		const std::string& expectedDownTime);

	virtual ~StartFirmwareUpgradeResponse();

	const std::string& getExpectedDownTime() const;

	const std::string& getUploadDelay() const;

	const Poco::URI& getUploadUri() const;

	void setExpectedDownTime(const std::string& val);

	void setExpectedDownTime(std::string&& val);

	void setUploadDelay(const std::string& val);

	void setUploadDelay(std::string&& val);

	void setUploadUri(const Poco::URI& val);

	void setUploadUri(Poco::URI&& val);

private:
	//@ name=UploadUri
	//@ order=0
	Poco::URI _uploadUri;

	//@ name=UploadDelay
	//@ order=1
	std::string _uploadDelay;

	//@ name=ExpectedDownTime
	//@ order=2
	std::string _expectedDownTime;

};


inline const std::string& StartFirmwareUpgradeResponse::getExpectedDownTime() const
{
	return _expectedDownTime;
}


inline const std::string& StartFirmwareUpgradeResponse::getUploadDelay() const
{
	return _uploadDelay;
}


inline const Poco::URI& StartFirmwareUpgradeResponse::getUploadUri() const
{
	return _uploadUri;
}


inline void StartFirmwareUpgradeResponse::setExpectedDownTime(const std::string& val)
{
	_expectedDownTime = val;
}


inline void StartFirmwareUpgradeResponse::setExpectedDownTime(std::string&& val)
{
	_expectedDownTime = std::move(val);
}


inline void StartFirmwareUpgradeResponse::setUploadDelay(const std::string& val)
{
	_uploadDelay = val;
}


inline void StartFirmwareUpgradeResponse::setUploadDelay(std::string&& val)
{
	_uploadDelay = std::move(val);
}


inline void StartFirmwareUpgradeResponse::setUploadUri(const Poco::URI& val)
{
	_uploadUri = val;
}


inline void StartFirmwareUpgradeResponse::setUploadUri(Poco::URI&& val)
{
	_uploadUri = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_StartFirmwareUpgradeResponse_INCLUDED
