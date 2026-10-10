// 
// SetHostnameFromDHCP.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetHostnameFromDHCP_INCLUDED
#define ONVIF_Device_SetHostnameFromDHCP_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetHostnameFromDHCP"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetHostnameFromDHCP
{
public:
	SetHostnameFromDHCP();

	SetHostnameFromDHCP(bool fromDHCP);

	virtual ~SetHostnameFromDHCP();

	bool getFromDHCP() const;

	void setFromDHCP(bool val);

private:
	//@ name=FromDHCP
	//@ order=0
	bool _fromDHCP;

};


inline bool SetHostnameFromDHCP::getFromDHCP() const
{
	return _fromDHCP;
}


inline void SetHostnameFromDHCP::setFromDHCP(bool val)
{
	_fromDHCP = val;
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetHostnameFromDHCP_INCLUDED
