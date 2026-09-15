// 
// ScanAvailableDot11NetworksResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_ScanAvailableDot11NetworksResponse_INCLUDED
#define ONVIF_Device_ScanAvailableDot11NetworksResponse_INCLUDED


#include "ONVIF/Dot11AvailableNetworks.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#ScanAvailableDot11NetworksResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API ScanAvailableDot11NetworksResponse
{
public:
	ScanAvailableDot11NetworksResponse();

	ScanAvailableDot11NetworksResponse(const std::vector<ONVIF::Dot11AvailableNetworks>& networks);

	virtual ~ScanAvailableDot11NetworksResponse();

	const std::vector<ONVIF::Dot11AvailableNetworks>& getNetworks() const;

	std::vector<ONVIF::Dot11AvailableNetworks>& getNetworks();

	void setNetworks(const std::vector<ONVIF::Dot11AvailableNetworks>& val);

	void setNetworks(std::vector<ONVIF::Dot11AvailableNetworks>&& val);

private:
	//@ mandatory=false
	//@ name=Networks
	//@ order=0
	std::vector<ONVIF::Dot11AvailableNetworks> _networks;

};


inline const std::vector<ONVIF::Dot11AvailableNetworks>& ScanAvailableDot11NetworksResponse::getNetworks() const
{
	return _networks;
}


inline std::vector<ONVIF::Dot11AvailableNetworks>& ScanAvailableDot11NetworksResponse::getNetworks()
{
	return _networks;
}


inline void ScanAvailableDot11NetworksResponse::setNetworks(const std::vector<ONVIF::Dot11AvailableNetworks>& val)
{
	_networks = val;
}


inline void ScanAvailableDot11NetworksResponse::setNetworks(std::vector<ONVIF::Dot11AvailableNetworks>&& val)
{
	_networks = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_ScanAvailableDot11NetworksResponse_INCLUDED
