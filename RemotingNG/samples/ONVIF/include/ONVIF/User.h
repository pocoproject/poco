// 
// User.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_User_INCLUDED
#define ONVIF_User_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class UserExtension;
} 


namespace ONVIF {


//@ name=User
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API User
{
public:
	User();

	User(
		const std::string& username, 
		const Poco::Optional<std::string>& password, 
		const std::string& userLevel, 
		const Poco::SharedPtr<UserExtension>& extension);

	virtual ~User();

	const Poco::SharedPtr<UserExtension>& getExtension() const;

	const Poco::Optional<std::string>& getPassword() const;

	const std::string& getUserLevel() const;

	const std::string& getUsername() const;

	void setExtension(const Poco::SharedPtr<UserExtension>& val);

	void setExtension(Poco::SharedPtr<UserExtension>&& val);

	void setPassword(const Poco::Optional<std::string>& val);

	void setPassword(Poco::Optional<std::string>&& val);

	void setUserLevel(const std::string& val);

	void setUserLevel(std::string&& val);

	void setUsername(const std::string& val);

	void setUsername(std::string&& val);

private:
	//@ name=Username
	//@ order=0
	std::string _username;

	//@ mandatory=false
	//@ name=Password
	//@ order=1
	Poco::Optional<std::string> _password;

	//@ name=UserLevel
	//@ order=2
	std::string _userLevel;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<UserExtension> _extension;

};


inline const Poco::SharedPtr<UserExtension>& User::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& User::getPassword() const
{
	return _password;
}


inline const std::string& User::getUserLevel() const
{
	return _userLevel;
}


inline const std::string& User::getUsername() const
{
	return _username;
}


inline void User::setExtension(const Poco::SharedPtr<UserExtension>& val)
{
	_extension = val;
}


inline void User::setExtension(Poco::SharedPtr<UserExtension>&& val)
{
	_extension = std::move(val);
}


inline void User::setPassword(const Poco::Optional<std::string>& val)
{
	_password = val;
}


inline void User::setPassword(Poco::Optional<std::string>&& val)
{
	_password = std::move(val);
}


inline void User::setUserLevel(const std::string& val)
{
	_userLevel = val;
}


inline void User::setUserLevel(std::string&& val)
{
	_userLevel = std::move(val);
}


inline void User::setUsername(const std::string& val)
{
	_username = val;
}


inline void User::setUsername(std::string&& val)
{
	_username = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/UserExtension.h"


#endif // ONVIF_User_INCLUDED
