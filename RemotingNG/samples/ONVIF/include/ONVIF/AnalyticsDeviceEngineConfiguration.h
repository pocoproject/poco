// 
// AnalyticsDeviceEngineConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsDeviceEngineConfiguration_INCLUDED
#define ONVIF_AnalyticsDeviceEngineConfiguration_INCLUDED


#include "ONVIF/EngineConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AnalyticsDeviceEngineConfigurationExtension;
} 


namespace ONVIF {


//@ name=AnalyticsDeviceEngineConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsDeviceEngineConfiguration
{
public:
	AnalyticsDeviceEngineConfiguration();

	AnalyticsDeviceEngineConfiguration(
		const std::vector<EngineConfiguration>& engineConfiguration, 
		const Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>& extension);

	virtual ~AnalyticsDeviceEngineConfiguration();

	const std::vector<EngineConfiguration>& getEngineConfiguration() const;

	std::vector<EngineConfiguration>& getEngineConfiguration();

	const Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>& getExtension() const;

	void setEngineConfiguration(const std::vector<EngineConfiguration>& val);

	void setEngineConfiguration(std::vector<EngineConfiguration>&& val);

	void setExtension(const Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>&& val);

private:
	//@ name=EngineConfiguration
	//@ order=0
	std::vector<EngineConfiguration> _engineConfiguration;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension> _extension;

};


inline const std::vector<EngineConfiguration>& AnalyticsDeviceEngineConfiguration::getEngineConfiguration() const
{
	return _engineConfiguration;
}


inline std::vector<EngineConfiguration>& AnalyticsDeviceEngineConfiguration::getEngineConfiguration()
{
	return _engineConfiguration;
}


inline const Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>& AnalyticsDeviceEngineConfiguration::getExtension() const
{
	return _extension;
}


inline void AnalyticsDeviceEngineConfiguration::setEngineConfiguration(const std::vector<EngineConfiguration>& val)
{
	_engineConfiguration = val;
}


inline void AnalyticsDeviceEngineConfiguration::setEngineConfiguration(std::vector<EngineConfiguration>&& val)
{
	_engineConfiguration = std::move(val);
}


inline void AnalyticsDeviceEngineConfiguration::setExtension(const Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>& val)
{
	_extension = val;
}


inline void AnalyticsDeviceEngineConfiguration::setExtension(Poco::SharedPtr<AnalyticsDeviceEngineConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AnalyticsDeviceEngineConfigurationExtension.h"


#endif // ONVIF_AnalyticsDeviceEngineConfiguration_INCLUDED
