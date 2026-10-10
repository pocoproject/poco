// 
// GetNetworkInterfacesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetNetworkInterfacesResponse_INCLUDED
#define ONVIF_Device_GetNetworkInterfacesResponse_INCLUDED


#include "ONVIF/NetworkInterface.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetNetworkInterfacesResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetNetworkInterfacesResponse
{
public:
	GetNetworkInterfacesResponse();

	GetNetworkInterfacesResponse(const std::vector<ONVIF::NetworkInterface>& networkInterfaces);

	virtual ~GetNetworkInterfacesResponse();

	const std::vector<ONVIF::NetworkInterface>& getNetworkInterfaces() const;

	std::vector<ONVIF::NetworkInterface>& getNetworkInterfaces();

	void setNetworkInterfaces(const std::vector<ONVIF::NetworkInterface>& val);

	void setNetworkInterfaces(std::vector<ONVIF::NetworkInterface>&& val);

private:
	//@ name=NetworkInterfaces
	//@ order=0
	std::vector<ONVIF::NetworkInterface> _networkInterfaces;

};


inline const std::vector<ONVIF::NetworkInterface>& GetNetworkInterfacesResponse::getNetworkInterfaces() const
{
	return _networkInterfaces;
}


inline std::vector<ONVIF::NetworkInterface>& GetNetworkInterfacesResponse::getNetworkInterfaces()
{
	return _networkInterfaces;
}


inline void GetNetworkInterfacesResponse::setNetworkInterfaces(const std::vector<ONVIF::NetworkInterface>& val)
{
	_networkInterfaces = val;
}


inline void GetNetworkInterfacesResponse::setNetworkInterfaces(std::vector<ONVIF::NetworkInterface>&& val)
{
	_networkInterfaces = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetNetworkInterfacesResponse_INCLUDED
