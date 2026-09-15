// 
// SetRemoteDiscoveryMode.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetRemoteDiscoveryMode_INCLUDED
#define ONVIF_Device_SetRemoteDiscoveryMode_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetRemoteDiscoveryMode"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetRemoteDiscoveryMode
{
public:
	SetRemoteDiscoveryMode();

	SetRemoteDiscoveryMode(const std::string& remoteDiscoveryMode);

	virtual ~SetRemoteDiscoveryMode();

	const std::string& getRemoteDiscoveryMode() const;

	void setRemoteDiscoveryMode(const std::string& val);

	void setRemoteDiscoveryMode(std::string&& val);

private:
	//@ name=RemoteDiscoveryMode
	//@ order=0
	std::string _remoteDiscoveryMode;

};


inline const std::string& SetRemoteDiscoveryMode::getRemoteDiscoveryMode() const
{
	return _remoteDiscoveryMode;
}


inline void SetRemoteDiscoveryMode::setRemoteDiscoveryMode(const std::string& val)
{
	_remoteDiscoveryMode = val;
}


inline void SetRemoteDiscoveryMode::setRemoteDiscoveryMode(std::string&& val)
{
	_remoteDiscoveryMode = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetRemoteDiscoveryMode_INCLUDED
