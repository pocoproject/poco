// 
// UserCredential.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_UserCredential_INCLUDED
#define ONVIF_Device_UserCredential_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {
class Extension_1;
} } 


namespace ONVIF {
namespace Device {


//@ name=UserCredential
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API UserCredential
{
public:
	UserCredential();

	UserCredential(
		const std::string& userName, 
		const Poco::Optional<std::string>& password, 
		const Poco::SharedPtr<Extension_1>& extension);

	virtual ~UserCredential();

	const Poco::SharedPtr<Extension_1>& getExtension() const;

	const Poco::Optional<std::string>& getPassword() const;

	const std::string& getUserName() const;

	void setExtension(const Poco::SharedPtr<Extension_1>& val);

	void setExtension(Poco::SharedPtr<Extension_1>&& val);

	void setPassword(const Poco::Optional<std::string>& val);

	void setPassword(Poco::Optional<std::string>&& val);

	void setUserName(const std::string& val);

	void setUserName(std::string&& val);

private:
	//@ name=UserName
	//@ order=0
	std::string _userName;

	//@ mandatory=false
	//@ name=Password
	//@ order=1
	Poco::Optional<std::string> _password;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<Extension_1> _extension;

};


inline const Poco::SharedPtr<Extension_1>& UserCredential::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& UserCredential::getPassword() const
{
	return _password;
}


inline const std::string& UserCredential::getUserName() const
{
	return _userName;
}


inline void UserCredential::setExtension(const Poco::SharedPtr<Extension_1>& val)
{
	_extension = val;
}


inline void UserCredential::setExtension(Poco::SharedPtr<Extension_1>&& val)
{
	_extension = std::move(val);
}


inline void UserCredential::setPassword(const Poco::Optional<std::string>& val)
{
	_password = val;
}


inline void UserCredential::setPassword(Poco::Optional<std::string>&& val)
{
	_password = std::move(val);
}


inline void UserCredential::setUserName(const std::string& val)
{
	_userName = val;
}


inline void UserCredential::setUserName(std::string&& val)
{
	_userName = std::move(val);
}


} } // ONVIF::Device


// The following headers are required for template instantiation.
#include "ONVIF/Device/Extension_1.h"


#endif // ONVIF_Device_UserCredential_INCLUDED
