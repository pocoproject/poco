// 
// GetVideoAnalyticsConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetVideoAnalyticsConfiguration_INCLUDED
#define ONVIF_Media_GetVideoAnalyticsConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetVideoAnalyticsConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetVideoAnalyticsConfiguration
{
public:
	GetVideoAnalyticsConfiguration();

	GetVideoAnalyticsConfiguration(const std::string& configurationToken);

	virtual ~GetVideoAnalyticsConfiguration();

	const std::string& getConfigurationToken() const;

	void setConfigurationToken(const std::string& val);

	void setConfigurationToken(std::string&& val);

private:
	//@ name=ConfigurationToken
	//@ order=0
	std::string _configurationToken;

};


inline const std::string& GetVideoAnalyticsConfiguration::getConfigurationToken() const
{
	return _configurationToken;
}


inline void GetVideoAnalyticsConfiguration::setConfigurationToken(const std::string& val)
{
	_configurationToken = val;
}


inline void GetVideoAnalyticsConfiguration::setConfigurationToken(std::string&& val)
{
	_configurationToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetVideoAnalyticsConfiguration_INCLUDED
