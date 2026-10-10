// 
// GetServices.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetServices_INCLUDED
#define ONVIF_Device_GetServices_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetServices"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetServices
{
public:
	GetServices();

	GetServices(bool includeCapability);

	virtual ~GetServices();

	bool getIncludeCapability() const;

	void setIncludeCapability(bool val);

private:
	//@ name=IncludeCapability
	//@ order=0
	bool _includeCapability;

};


inline bool GetServices::getIncludeCapability() const
{
	return _includeCapability;
}


inline void GetServices::setIncludeCapability(bool val)
{
	_includeCapability = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetServices_INCLUDED
