// 
// GetVideoSourceConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoSourceConfiguration_INCLUDED
#define ONVIF_Media_GetVideoSourceConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoSourceConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoSourceConfiguration
{
public:
	GetVideoSourceConfiguration();

	GetVideoSourceConfiguration(const std::string& configurationToken);

	virtual ~GetVideoSourceConfiguration();

	const std::string& getConfigurationToken() const;

	void setConfigurationToken(const std::string& val);

	void setConfigurationToken(std::string&& val);

private:
	//@ name=ConfigurationToken
	//@ order=0
	std::string _configurationToken;

};


inline const std::string& GetVideoSourceConfiguration::getConfigurationToken() const
{
	return _configurationToken;
}


inline void GetVideoSourceConfiguration::setConfigurationToken(const std::string& val)
{
	_configurationToken = val;
}


inline void GetVideoSourceConfiguration::setConfigurationToken(std::string&& val)
{
	_configurationToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoSourceConfiguration_INCLUDED
