// 
// AnalyticsEngine.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsEngine_INCLUDED
#define ONVIF_AnalyticsEngine_INCLUDED


#include "ONVIF/AnalyticsDeviceEngineConfiguration.h"
#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AnalyticsEngine
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsEngine: public ConfigurationEntity
{
public:
	AnalyticsEngine();

	AnalyticsEngine(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const AnalyticsDeviceEngineConfiguration& analyticsEngineConfiguration);

	virtual ~AnalyticsEngine();

	const AnalyticsDeviceEngineConfiguration& getAnalyticsEngineConfiguration() const;

	void setAnalyticsEngineConfiguration(const AnalyticsDeviceEngineConfiguration& val);

	void setAnalyticsEngineConfiguration(AnalyticsDeviceEngineConfiguration&& val);

private:
	//@ name=AnalyticsEngineConfiguration
	//@ order=0
	AnalyticsDeviceEngineConfiguration _analyticsEngineConfiguration;

};


inline const AnalyticsDeviceEngineConfiguration& AnalyticsEngine::getAnalyticsEngineConfiguration() const
{
	return _analyticsEngineConfiguration;
}


inline void AnalyticsEngine::setAnalyticsEngineConfiguration(const AnalyticsDeviceEngineConfiguration& val)
{
	_analyticsEngineConfiguration = val;
}


inline void AnalyticsEngine::setAnalyticsEngineConfiguration(AnalyticsDeviceEngineConfiguration&& val)
{
	_analyticsEngineConfiguration = std::move(val);
}


} // ONVIF


#endif // ONVIF_AnalyticsEngine_INCLUDED
