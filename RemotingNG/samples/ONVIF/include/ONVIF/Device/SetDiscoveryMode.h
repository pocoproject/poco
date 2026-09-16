// 
// SetDiscoveryMode.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetDiscoveryMode_INCLUDED
#define ONVIF_Device_SetDiscoveryMode_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetDiscoveryMode"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetDiscoveryMode
{
public:
	SetDiscoveryMode();

	SetDiscoveryMode(const std::string& discoveryMode);

	virtual ~SetDiscoveryMode();

	const std::string& getDiscoveryMode() const;

	void setDiscoveryMode(const std::string& val);

	void setDiscoveryMode(std::string&& val);

private:
	//@ name=DiscoveryMode
	//@ order=0
	std::string _discoveryMode;

};


inline const std::string& SetDiscoveryMode::getDiscoveryMode() const
{
	return _discoveryMode;
}


inline void SetDiscoveryMode::setDiscoveryMode(const std::string& val)
{
	_discoveryMode = val;
}


inline void SetDiscoveryMode::setDiscoveryMode(std::string&& val)
{
	_discoveryMode = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetDiscoveryMode_INCLUDED
