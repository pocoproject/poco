// 
// GetVideoSourceConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoSourceConfigurationOptions_INCLUDED
#define ONVIF_Media_GetVideoSourceConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoSourceConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfigurationOptions
{
public:
	GetVideoSourceConfigurationOptions();

	GetVideoSourceConfigurationOptions(
		const Poco::Optional<std::string>& configurationToken, 
		const Poco::Optional<std::string>& profileToken);

	virtual ~GetVideoSourceConfigurationOptions();

	const Poco::Optional<std::string>& getConfigurationToken() const;

	const Poco::Optional<std::string>& getProfileToken() const;

	void setConfigurationToken(const Poco::Optional<std::string>& val);

	void setConfigurationToken(Poco::Optional<std::string>&& val);

	void setProfileToken(const Poco::Optional<std::string>& val);

	void setProfileToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=ConfigurationToken
	//@ order=0
	Poco::Optional<std::string> _configurationToken;

	//@ mandatory=false
	//@ name=ProfileToken
	//@ order=1
	Poco::Optional<std::string> _profileToken;

};


inline const Poco::Optional<std::string>& GetVideoSourceConfigurationOptions::getConfigurationToken() const
{
	return _configurationToken;
}


inline const Poco::Optional<std::string>& GetVideoSourceConfigurationOptions::getProfileToken() const
{
	return _profileToken;
}


inline void GetVideoSourceConfigurationOptions::setConfigurationToken(const Poco::Optional<std::string>& val)
{
	_configurationToken = val;
}


inline void GetVideoSourceConfigurationOptions::setConfigurationToken(Poco::Optional<std::string>&& val)
{
	_configurationToken = std::move(val);
}


inline void GetVideoSourceConfigurationOptions::setProfileToken(const Poco::Optional<std::string>& val)
{
	_profileToken = val;
}


inline void GetVideoSourceConfigurationOptions::setProfileToken(Poco::Optional<std::string>&& val)
{
	_profileToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoSourceConfigurationOptions_INCLUDED
