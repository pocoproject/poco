// 
// DeleteUsers.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_DeleteUsers_INCLUDED
#define ONVIF_Device_DeleteUsers_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#DeleteUsers"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API DeleteUsers
{
public:
	DeleteUsers();

	DeleteUsers(const std::vector<std::string>& username);

	virtual ~DeleteUsers();

	const std::vector<std::string>& getUsername() const;

	std::vector<std::string>& getUsername();

	void setUsername(const std::vector<std::string>& val);

	void setUsername(std::vector<std::string>&& val);

private:
	//@ name=Username
	//@ order=0
	std::vector<std::string> _username;

};


inline const std::vector<std::string>& DeleteUsers::getUsername() const
{
	return _username;
}


inline std::vector<std::string>& DeleteUsers::getUsername()
{
	return _username;
}


inline void DeleteUsers::setUsername(const std::vector<std::string>& val)
{
	_username = val;
}


inline void DeleteUsers::setUsername(std::vector<std::string>&& val)
{
	_username = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_DeleteUsers_INCLUDED
