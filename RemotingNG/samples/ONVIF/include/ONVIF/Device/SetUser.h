// 
// SetUser.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetUser_INCLUDED
#define ONVIF_Device_SetUser_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/User.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetUser"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetUser
{
public:
	SetUser();

	SetUser(const std::vector<ONVIF::User>& user);

	virtual ~SetUser();

	const std::vector<ONVIF::User>& getUser() const;

	std::vector<ONVIF::User>& getUser();

	void setUser(const std::vector<ONVIF::User>& val);

	void setUser(std::vector<ONVIF::User>&& val);

private:
	//@ name=User
	//@ order=0
	std::vector<ONVIF::User> _user;

};


inline const std::vector<ONVIF::User>& SetUser::getUser() const
{
	return _user;
}


inline std::vector<ONVIF::User>& SetUser::getUser()
{
	return _user;
}


inline void SetUser::setUser(const std::vector<ONVIF::User>& val)
{
	_user = val;
}


inline void SetUser::setUser(std::vector<ONVIF::User>&& val)
{
	_user = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetUser_INCLUDED
