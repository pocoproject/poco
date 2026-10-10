// 
// GetAuthFailureWarningConfigurationResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetAuthFailureWarningConfigurationResponse_INCLUDED
#define ONVIF_Device_GetAuthFailureWarningConfigurationResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetAuthFailureWarningConfigurationResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetAuthFailureWarningConfigurationResponse
{
public:
	GetAuthFailureWarningConfigurationResponse();

	GetAuthFailureWarningConfigurationResponse(
		bool enabled, 
		int monitorPeriod, 
		int maxAuthFailures);

	virtual ~GetAuthFailureWarningConfigurationResponse();

	bool getEnabled() const;

	int getMaxAuthFailures() const;

	int getMonitorPeriod() const;

	void setEnabled(bool val);

	void setMaxAuthFailures(int val);

	void setMonitorPeriod(int val);

private:
	//@ name=Enabled
	//@ order=0
	bool _enabled;

	//@ name=MonitorPeriod
	//@ order=1
	int _monitorPeriod;

	//@ name=MaxAuthFailures
	//@ order=2
	int _maxAuthFailures;

};


inline bool GetAuthFailureWarningConfigurationResponse::getEnabled() const
{
	return _enabled;
}


inline int GetAuthFailureWarningConfigurationResponse::getMaxAuthFailures() const
{
	return _maxAuthFailures;
}


inline int GetAuthFailureWarningConfigurationResponse::getMonitorPeriod() const
{
	return _monitorPeriod;
}


inline void GetAuthFailureWarningConfigurationResponse::setEnabled(bool val)
{
	_enabled = val;
}


inline void GetAuthFailureWarningConfigurationResponse::setMaxAuthFailures(int val)
{
	_maxAuthFailures = val;
}


inline void GetAuthFailureWarningConfigurationResponse::setMonitorPeriod(int val)
{
	_monitorPeriod = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetAuthFailureWarningConfigurationResponse_INCLUDED
