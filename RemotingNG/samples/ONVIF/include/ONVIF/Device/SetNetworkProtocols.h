// 
// SetNetworkProtocols.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetNetworkProtocols_INCLUDED
#define ONVIF_Device_SetNetworkProtocols_INCLUDED


#include "ONVIF/NetworkProtocol.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetNetworkProtocols"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetNetworkProtocols
{
public:
	SetNetworkProtocols();

	SetNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& networkProtocols);

	virtual ~SetNetworkProtocols();

	const std::vector<ONVIF::NetworkProtocol>& getNetworkProtocols() const;

	std::vector<ONVIF::NetworkProtocol>& getNetworkProtocols();

	void setNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& val);

	void setNetworkProtocols(std::vector<ONVIF::NetworkProtocol>&& val);

private:
	//@ name=NetworkProtocols
	//@ order=0
	std::vector<ONVIF::NetworkProtocol> _networkProtocols;

};


inline const std::vector<ONVIF::NetworkProtocol>& SetNetworkProtocols::getNetworkProtocols() const
{
	return _networkProtocols;
}


inline std::vector<ONVIF::NetworkProtocol>& SetNetworkProtocols::getNetworkProtocols()
{
	return _networkProtocols;
}


inline void SetNetworkProtocols::setNetworkProtocols(const std::vector<ONVIF::NetworkProtocol>& val)
{
	_networkProtocols = val;
}


inline void SetNetworkProtocols::setNetworkProtocols(std::vector<ONVIF::NetworkProtocol>&& val)
{
	_networkProtocols = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetNetworkProtocols_INCLUDED
