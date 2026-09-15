// 
// GetRelayOutputOptionsResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetRelayOutputOptionsResponse_INCLUDED
#define ONVIF_DeviceIO_GetRelayOutputOptionsResponse_INCLUDED


#include "ONVIF/DeviceIO/RelayOutputOptions.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetRelayOutputOptionsResponse"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetRelayOutputOptionsResponse
{
public:
	GetRelayOutputOptionsResponse();

	GetRelayOutputOptionsResponse(const std::vector<RelayOutputOptions>& relayOutputOptions);

	virtual ~GetRelayOutputOptionsResponse();

	const std::vector<RelayOutputOptions>& getRelayOutputOptions() const;

	std::vector<RelayOutputOptions>& getRelayOutputOptions();

	void setRelayOutputOptions(const std::vector<RelayOutputOptions>& val);

	void setRelayOutputOptions(std::vector<RelayOutputOptions>&& val);

private:
	//@ mandatory=false
	//@ name=RelayOutputOptions
	//@ order=0
	std::vector<RelayOutputOptions> _relayOutputOptions;

};


inline const std::vector<RelayOutputOptions>& GetRelayOutputOptionsResponse::getRelayOutputOptions() const
{
	return _relayOutputOptions;
}


inline std::vector<RelayOutputOptions>& GetRelayOutputOptionsResponse::getRelayOutputOptions()
{
	return _relayOutputOptions;
}


inline void GetRelayOutputOptionsResponse::setRelayOutputOptions(const std::vector<RelayOutputOptions>& val)
{
	_relayOutputOptions = val;
}


inline void GetRelayOutputOptionsResponse::setRelayOutputOptions(std::vector<RelayOutputOptions>&& val)
{
	_relayOutputOptions = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetRelayOutputOptionsResponse_INCLUDED
