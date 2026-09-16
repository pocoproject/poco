// 
// GetSystemDateAndTimeResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetSystemDateAndTimeResponse_INCLUDED
#define ONVIF_Device_GetSystemDateAndTimeResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/SystemDateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetSystemDateAndTimeResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetSystemDateAndTimeResponse
{
public:
	GetSystemDateAndTimeResponse();

	GetSystemDateAndTimeResponse(const ONVIF::SystemDateTime& systemDateAndTime);

	virtual ~GetSystemDateAndTimeResponse();

	const ONVIF::SystemDateTime& getSystemDateAndTime() const;

	void setSystemDateAndTime(const ONVIF::SystemDateTime& val);

	void setSystemDateAndTime(ONVIF::SystemDateTime&& val);

private:
	//@ name=SystemDateAndTime
	//@ order=0
	ONVIF::SystemDateTime _systemDateAndTime;

};


inline const ONVIF::SystemDateTime& GetSystemDateAndTimeResponse::getSystemDateAndTime() const
{
	return _systemDateAndTime;
}


inline void GetSystemDateAndTimeResponse::setSystemDateAndTime(const ONVIF::SystemDateTime& val)
{
	_systemDateAndTime = val;
}


inline void GetSystemDateAndTimeResponse::setSystemDateAndTime(ONVIF::SystemDateTime&& val)
{
	_systemDateAndTime = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetSystemDateAndTimeResponse_INCLUDED
