// 
// GetRemoteUserResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetRemoteUserResponse_INCLUDED
#define ONVIF_Device_GetRemoteUserResponse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RemoteUser;
} 


namespace ONVIF {
namespace Device {


//@ name="#GetRemoteUserResponse"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetRemoteUserResponse
{
public:
	GetRemoteUserResponse();

	GetRemoteUserResponse(const Poco::SharedPtr<ONVIF::RemoteUser>& remoteUser);

	virtual ~GetRemoteUserResponse();

	const Poco::SharedPtr<ONVIF::RemoteUser>& getRemoteUser() const;

	void setRemoteUser(const Poco::SharedPtr<ONVIF::RemoteUser>& val);

	void setRemoteUser(Poco::SharedPtr<ONVIF::RemoteUser>&& val);

private:
	//@ mandatory=false
	//@ name=RemoteUser
	//@ order=0
	Poco::SharedPtr<ONVIF::RemoteUser> _remoteUser;

};


inline const Poco::SharedPtr<ONVIF::RemoteUser>& GetRemoteUserResponse::getRemoteUser() const
{
	return _remoteUser;
}


inline void GetRemoteUserResponse::setRemoteUser(const Poco::SharedPtr<ONVIF::RemoteUser>& val)
{
	_remoteUser = val;
}


inline void GetRemoteUserResponse::setRemoteUser(Poco::SharedPtr<ONVIF::RemoteUser>&& val)
{
	_remoteUser = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/RemoteUser.h"


#endif // ONVIF_Device_GetRemoteUserResponse_INCLUDED
