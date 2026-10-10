// 
// RemoteUser.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RemoteUser_INCLUDED
#define ONVIF_RemoteUser_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RemoteUser
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RemoteUser
{
public:
	RemoteUser();

	RemoteUser(
		const std::string& username, 
		const Poco::Optional<std::string>& password, 
		bool useDerivedPassword);

	virtual ~RemoteUser();

	const Poco::Optional<std::string>& getPassword() const;

	bool getUseDerivedPassword() const;

	const std::string& getUsername() const;

	void setPassword(const Poco::Optional<std::string>& val);

	void setPassword(Poco::Optional<std::string>&& val);

	void setUseDerivedPassword(bool val);

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

	//@ name=UseDerivedPassword
	//@ order=2
	bool _useDerivedPassword;

};


inline const Poco::Optional<std::string>& RemoteUser::getPassword() const
{
	return _password;
}


inline bool RemoteUser::getUseDerivedPassword() const
{
	return _useDerivedPassword;
}


inline const std::string& RemoteUser::getUsername() const
{
	return _username;
}


inline void RemoteUser::setPassword(const Poco::Optional<std::string>& val)
{
	_password = val;
}


inline void RemoteUser::setPassword(Poco::Optional<std::string>&& val)
{
	_password = std::move(val);
}


inline void RemoteUser::setUseDerivedPassword(bool val)
{
	_useDerivedPassword = val;
}


inline void RemoteUser::setUsername(const std::string& val)
{
	_username = val;
}


inline void RemoteUser::setUsername(std::string&& val)
{
	_username = std::move(val);
}


} // ONVIF


#endif // ONVIF_RemoteUser_INCLUDED
