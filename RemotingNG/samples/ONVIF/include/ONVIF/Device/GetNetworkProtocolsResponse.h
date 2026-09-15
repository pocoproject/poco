// 
// GetNetworkProtocolsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetNetworkProtocolsResponse_INCLUDED
#define ONVIF_Device_GetNetworkProtocolsResponse_INCLUDED


#include "ONVIF/NetworkProtocol.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetNetworkProtocolsResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetNetworkProtocolsResponse
{
public:
	GetNetworkProtocolsResponse();

	GetNetworkProtocolsResponse(const std::vector<ONVIF::NetworkProtocol>& networkProtocols);

	virtual ~GetNetworkProtocolsResponse();

	const std::vector<ONVIF::NetworkProtocol>& getNetworkProtocols() const;

	std::vector<ONVIF::NetworkProtocol>& getNetworkProtocols();

	void setNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& val);

	void setNetworkProtocols(std::vector<ONVIF::NetworkProtocol>&& val);

private:
	//@ mandatory=false
	//@ name=NetworkProtocols
	//@ order=0
	std::vector<ONVIF::NetworkProtocol> _networkProtocols;

};


inline const std::vector<ONVIF::NetworkProtocol>& GetNetworkProtocolsResponse::getNetworkProtocols() const
{
	return _networkProtocols;
}


inline std::vector<ONVIF::NetworkProtocol>& GetNetworkProtocolsResponse::getNetworkProtocols()
{
	return _networkProtocols;
}


inline void GetNetworkProtocolsResponse::setNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& val)
{
	_networkProtocols = val;
}


inline void GetNetworkProtocolsResponse::setNetworkProtocols(std::vector<ONVIF::NetworkProtocol>&& val)
{
	_networkProtocols = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetNetworkProtocolsResponse_INCLUDED
