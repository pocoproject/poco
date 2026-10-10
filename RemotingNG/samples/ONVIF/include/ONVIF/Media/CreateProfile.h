// 
// CreateProfile.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_CreateProfile_INCLUDED
#define ONVIF_Media_CreateProfile_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#CreateProfile"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API CreateProfile
{
public:
	CreateProfile();

	CreateProfile(
		const std::string& name, 
		const Poco::Optional<std::string>& token);

	virtual ~CreateProfile();

	const std::string& getName() const;

	const Poco::Optional<std::string>& getToken() const;

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setToken(const Poco::Optional<std::string>& val);

	void setToken(Poco::Optional<std::string>&& val);

private:
	//@ name=Name
	//@ order=0
	std::string _name;

	//@ mandatory=false
	//@ name=Token
	//@ order=1
	Poco::Optional<std::string> _token;

};


inline const std::string& CreateProfile::getName() const
{
	return _name;
}


inline const Poco::Optional<std::string>& CreateProfile::getToken() const
{
	return _token;
}


inline void CreateProfile::setName(const std::string& val)
{
	_name = val;
}


inline void CreateProfile::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void CreateProfile::setToken(const Poco::Optional<std::string>& val)
{
	_token = val;
}


inline void CreateProfile::setToken(Poco::Optional<std::string>&& val)
{
	_token = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_CreateProfile_INCLUDED
