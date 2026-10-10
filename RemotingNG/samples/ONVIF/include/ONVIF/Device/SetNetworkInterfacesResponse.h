// 
// SetNetworkInterfacesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetNetworkInterfacesResponse_INCLUDED
#define ONVIF_Device_SetNetworkInterfacesResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetNetworkInterfacesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetNetworkInterfacesResponse
{
public:
	SetNetworkInterfacesResponse();

	SetNetworkInterfacesResponse(bool rebootNeeded);

	virtual ~SetNetworkInterfacesResponse();

	bool getRebootNeeded() const;

	void setRebootNeeded(bool val);

private:
	//@ name=RebootNeeded
	//@ order=0
	bool _rebootNeeded;

};


inline bool SetNetworkInterfacesResponse::getRebootNeeded() const
{
	return _rebootNeeded;
}


inline void SetNetworkInterfacesResponse::setRebootNeeded(bool val)
{
	_rebootNeeded = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetNetworkInterfacesResponse_INCLUDED
