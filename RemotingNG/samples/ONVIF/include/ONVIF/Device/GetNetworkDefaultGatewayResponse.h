// 
// GetNetworkDefaultGatewayResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetNetworkDefaultGatewayResponse_INCLUDED
#define ONVIF_Device_GetNetworkDefaultGatewayResponse_INCLUDED


#include "ONVIF/NetworkGateway.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetNetworkDefaultGatewayResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetNetworkDefaultGatewayResponse
{
public:
	GetNetworkDefaultGatewayResponse();

	GetNetworkDefaultGatewayResponse(const ONVIF::NetworkGateway& networkGateway);

	virtual ~GetNetworkDefaultGatewayResponse();

	const ONVIF::NetworkGateway& getNetworkGateway() const;

	void setNetworkGateway(const ONVIF::NetworkGateway& val);

	void setNetworkGateway(ONVIF::NetworkGateway&& val);

private:
	//@ name=NetworkGateway
	//@ order=0
	ONVIF::NetworkGateway _networkGateway;

};


inline const ONVIF::NetworkGateway& GetNetworkDefaultGatewayResponse::getNetworkGateway() const
{
	return _networkGateway;
}


inline void GetNetworkDefaultGatewayResponse::setNetworkGateway(const ONVIF::NetworkGateway& val)
{
	_networkGateway = val;
}


inline void GetNetworkDefaultGatewayResponse::setNetworkGateway(ONVIF::NetworkGateway&& val)
{
	_networkGateway = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetNetworkDefaultGatewayResponse_INCLUDED
