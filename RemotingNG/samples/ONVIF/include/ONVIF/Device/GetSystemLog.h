// 
// GetSystemLog.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetSystemLog_INCLUDED
#define ONVIF_Device_GetSystemLog_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/SystemLog.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetSystemLog"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetSystemLog
{
public:
	GetSystemLog();

	GetSystemLog(const ONVIF::SystemLog& logType);

	virtual ~GetSystemLog();

	const ONVIF::SystemLog& getLogType() const;

	void setLogType(const ONVIF::SystemLog& val);

	void setLogType(ONVIF::SystemLog&& val);

private:
	//@ name=LogType
	//@ order=0
	ONVIF::SystemLog _logType;

};


inline const ONVIF::SystemLog& GetSystemLog::getLogType() const
{
	return _logType;
}


inline void GetSystemLog::setLogType(const ONVIF::SystemLog& val)
{
	_logType = val;
}


inline void GetSystemLog::setLogType(ONVIF::SystemLog&& val)
{
	_logType = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetSystemLog_INCLUDED
