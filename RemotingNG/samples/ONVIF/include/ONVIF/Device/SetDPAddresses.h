// 
// SetDPAddresses.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetDPAddresses_INCLUDED
#define ONVIF_Device_SetDPAddresses_INCLUDED


#include "ONVIF/NetworkHost.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetDPAddresses"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetDPAddresses
{
public:
	SetDPAddresses();

	SetDPAddresses(const std::vector<ONVIF::NetworkHost>& dPAddress);

	virtual ~SetDPAddresses();

	const std::vector<ONVIF::NetworkHost>& getDPAddress() const;

	std::vector<ONVIF::NetworkHost>& getDPAddress();

	void setDPAddress(const std::vector<ONVIF::NetworkHost>& val);

	void setDPAddress(std::vector<ONVIF::NetworkHost>&& val);

private:
	//@ mandatory=false
	//@ name=DPAddress
	//@ order=0
	std::vector<ONVIF::NetworkHost> _dPAddress;

};


inline const std::vector<ONVIF::NetworkHost>& SetDPAddresses::getDPAddress() const
{
	return _dPAddress;
}


inline std::vector<ONVIF::NetworkHost>& SetDPAddresses::getDPAddress()
{
	return _dPAddress;
}


inline void SetDPAddresses::setDPAddress(const std::vector<ONVIF::NetworkHost>& val)
{
	_dPAddress = val;
}


inline void SetDPAddresses::setDPAddress(std::vector<ONVIF::NetworkHost>&& val)
{
	_dPAddress = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetDPAddresses_INCLUDED
