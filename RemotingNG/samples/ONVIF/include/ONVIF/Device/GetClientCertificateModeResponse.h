// 
// GetClientCertificateModeResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetClientCertificateModeResponse_INCLUDED
#define ONVIF_Device_GetClientCertificateModeResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetClientCertificateModeResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetClientCertificateModeResponse
{
public:
	GetClientCertificateModeResponse();

	GetClientCertificateModeResponse(bool enabled);

	virtual ~GetClientCertificateModeResponse();

	bool getEnabled() const;

	void setEnabled(bool val);

private:
	//@ name=Enabled
	//@ order=0
	bool _enabled;

};


inline bool GetClientCertificateModeResponse::getEnabled() const
{
	return _enabled;
}


inline void GetClientCertificateModeResponse::setEnabled(bool val)
{
	_enabled = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetClientCertificateModeResponse_INCLUDED
