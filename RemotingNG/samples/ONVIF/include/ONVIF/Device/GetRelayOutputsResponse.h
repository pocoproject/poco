// 
// GetRelayOutputsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetRelayOutputsResponse_INCLUDED
#define ONVIF_Device_GetRelayOutputsResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/RelayOutput.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetRelayOutputsResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetRelayOutputsResponse
{
public:
	GetRelayOutputsResponse();

	GetRelayOutputsResponse(const std::vector<ONVIF::RelayOutput>& relayOutputs);

	virtual ~GetRelayOutputsResponse();

	const std::vector<ONVIF::RelayOutput>& getRelayOutputs() const;

	std::vector<ONVIF::RelayOutput>& getRelayOutputs();

	void setRelayOutputs(const std::vector<ONVIF::RelayOutput>& val);

	void setRelayOutputs(std::vector<ONVIF::RelayOutput>&& val);

private:
	//@ mandatory=false
	//@ name=RelayOutputs
	//@ order=0
	std::vector<ONVIF::RelayOutput> _relayOutputs;

};


inline const std::vector<ONVIF::RelayOutput>& GetRelayOutputsResponse::getRelayOutputs() const
{
	return _relayOutputs;
}


inline std::vector<ONVIF::RelayOutput>& GetRelayOutputsResponse::getRelayOutputs()
{
	return _relayOutputs;
}


inline void GetRelayOutputsResponse::setRelayOutputs(const std::vector<ONVIF::RelayOutput>& val)
{
	_relayOutputs = val;
}


inline void GetRelayOutputsResponse::setRelayOutputs(std::vector<ONVIF::RelayOutput>&& val)
{
	_relayOutputs = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetRelayOutputsResponse_INCLUDED
