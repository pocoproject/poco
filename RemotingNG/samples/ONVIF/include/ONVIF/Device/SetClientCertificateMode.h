// 
// SetClientCertificateMode.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetClientCertificateMode_INCLUDED
#define ONVIF_Device_SetClientCertificateMode_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetClientCertificateMode"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetClientCertificateMode
{
public:
	SetClientCertificateMode();

	SetClientCertificateMode(bool enabled);

	virtual ~SetClientCertificateMode();

	bool getEnabled() const;

	void setEnabled(bool val);

private:
	//@ name=Enabled
	//@ order=0
	bool _enabled;

};


inline bool SetClientCertificateMode::getEnabled() const
{
	return _enabled;
}


inline void SetClientCertificateMode::setEnabled(bool val)
{
	_enabled = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetClientCertificateMode_INCLUDED
