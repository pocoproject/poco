// 
// SetHostnameFromDHCPResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetHostnameFromDHCPResponse_INCLUDED
#define ONVIF_Device_SetHostnameFromDHCPResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetHostnameFromDHCPResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetHostnameFromDHCPResponse
{
public:
	SetHostnameFromDHCPResponse();

	SetHostnameFromDHCPResponse(bool rebootNeeded);

	virtual ~SetHostnameFromDHCPResponse();

	bool getRebootNeeded() const;

	void setRebootNeeded(bool val);

private:
	//@ name=RebootNeeded
	//@ order=0
	bool _rebootNeeded;

};


inline bool SetHostnameFromDHCPResponse::getRebootNeeded() const
{
	return _rebootNeeded;
}


inline void SetHostnameFromDHCPResponse::setRebootNeeded(bool val)
{
	_rebootNeeded = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetHostnameFromDHCPResponse_INCLUDED
