// 
// GetAuthFailureWarningOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetAuthFailureWarningOptionsResponse_INCLUDED
#define ONVIF_Device_GetAuthFailureWarningOptionsResponse_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetAuthFailureWarningOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetAuthFailureWarningOptionsResponse
{
public:
	GetAuthFailureWarningOptionsResponse();

	GetAuthFailureWarningOptionsResponse(
		const ONVIF::IntRange& monitorPeriodRange, 
		const ONVIF::IntRange& authFailureRange);

	virtual ~GetAuthFailureWarningOptionsResponse();

	const ONVIF::IntRange& getAuthFailureRange() const;

	const ONVIF::IntRange& getMonitorPeriodRange() const;

	void setAuthFailureRange(const ONVIF::IntRange& val);

	void setAuthFailureRange(ONVIF::IntRange&& val);

	void setMonitorPeriodRange(const ONVIF::IntRange& val);

	void setMonitorPeriodRange(ONVIF::IntRange&& val);

private:
	//@ name=MonitorPeriodRange
	//@ order=0
	ONVIF::IntRange _monitorPeriodRange;

	//@ name=AuthFailureRange
	//@ order=1
	ONVIF::IntRange _authFailureRange;

};


inline const ONVIF::IntRange& GetAuthFailureWarningOptionsResponse::getAuthFailureRange() const
{
	return _authFailureRange;
}


inline const ONVIF::IntRange& GetAuthFailureWarningOptionsResponse::getMonitorPeriodRange() const
{
	return _monitorPeriodRange;
}


inline void GetAuthFailureWarningOptionsResponse::setAuthFailureRange(const ONVIF::IntRange& val)
{
	_authFailureRange = val;
}


inline void GetAuthFailureWarningOptionsResponse::setAuthFailureRange(ONVIF::IntRange&& val)
{
	_authFailureRange = std::move(val);
}


inline void GetAuthFailureWarningOptionsResponse::setMonitorPeriodRange(const ONVIF::IntRange& val)
{
	_monitorPeriodRange = val;
}


inline void GetAuthFailureWarningOptionsResponse::setMonitorPeriodRange(ONVIF::IntRange&& val)
{
	_monitorPeriodRange = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetAuthFailureWarningOptionsResponse_INCLUDED
