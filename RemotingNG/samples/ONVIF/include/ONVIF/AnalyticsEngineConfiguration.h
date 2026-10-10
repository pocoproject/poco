// 
// AnalyticsEngineConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsEngineConfiguration_INCLUDED
#define ONVIF_AnalyticsEngineConfiguration_INCLUDED


#include "ONVIF/Config.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AnalyticsEngineConfigurationExtension;
} 


namespace ONVIF {


//@ name=AnalyticsEngineConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsEngineConfiguration
{
public:
	AnalyticsEngineConfiguration();

	AnalyticsEngineConfiguration(
		const std::vector<Config>& analyticsModule, 
		const Poco::SharedPtr<AnalyticsEngineConfigurationExtension>& extension);

	virtual ~AnalyticsEngineConfiguration();

	const std::vector<Config>& getAnalyticsModule() const;

	std::vector<Config>& getAnalyticsModule();

	const Poco::SharedPtr<AnalyticsEngineConfigurationExtension>& getExtension() const;

	void setAnalyticsModule(const std::vector<Config>& val);

	void setAnalyticsModule(std::vector<Config>&& val);

	void setExtension(const Poco::SharedPtr<AnalyticsEngineConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<AnalyticsEngineConfigurationExtension>&& val);

private:
	//@ mandatory=false
	//@ name=AnalyticsModule
	//@ order=0
	std::vector<Config> _analyticsModule;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<AnalyticsEngineConfigurationExtension> _extension;

};


inline const std::vector<Config>& AnalyticsEngineConfiguration::getAnalyticsModule() const
{
	return _analyticsModule;
}


inline std::vector<Config>& AnalyticsEngineConfiguration::getAnalyticsModule()
{
	return _analyticsModule;
}


inline const Poco::SharedPtr<AnalyticsEngineConfigurationExtension>& AnalyticsEngineConfiguration::getExtension() const
{
	return _extension;
}


inline void AnalyticsEngineConfiguration::setAnalyticsModule(const std::vector<Config>& val)
{
	_analyticsModule = val;
}


inline void AnalyticsEngineConfiguration::setAnalyticsModule(std::vector<Config>&& val)
{
	_analyticsModule = std::move(val);
}


inline void AnalyticsEngineConfiguration::setExtension(const Poco::SharedPtr<AnalyticsEngineConfigurationExtension>& val)
{
	_extension = val;
}


inline void AnalyticsEngineConfiguration::setExtension(Poco::SharedPtr<AnalyticsEngineConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AnalyticsEngineConfigurationExtension.h"


#endif // ONVIF_AnalyticsEngineConfiguration_INCLUDED
