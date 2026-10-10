// 
// GetSystemLogResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetSystemLogResponse_INCLUDED
#define ONVIF_Device_GetSystemLogResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/SystemLog.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetSystemLogResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetSystemLogResponse
{
public:
	GetSystemLogResponse();

	GetSystemLogResponse(const ONVIF::SystemLog& systemLog);

	virtual ~GetSystemLogResponse();

	const ONVIF::SystemLog& getSystemLog() const;

	void setSystemLog(const ONVIF::SystemLog& val);

	void setSystemLog(ONVIF::SystemLog&& val);

private:
	//@ name=SystemLog
	//@ order=0
	ONVIF::SystemLog _systemLog;

};


inline const ONVIF::SystemLog& GetSystemLogResponse::getSystemLog() const
{
	return _systemLog;
}


inline void GetSystemLogResponse::setSystemLog(const ONVIF::SystemLog& val)
{
	_systemLog = val;
}


inline void GetSystemLogResponse::setSystemLog(ONVIF::SystemLog&& val)
{
	_systemLog = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetSystemLogResponse_INCLUDED
