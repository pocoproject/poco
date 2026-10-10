// 
// VideoAnalyticsConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoAnalyticsConfiguration_INCLUDED
#define ONVIF_VideoAnalyticsConfiguration_INCLUDED


#include "ONVIF/AnalyticsEngineConfiguration.h"
#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/RuleEngineConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoAnalyticsConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoAnalyticsConfiguration: public ConfigurationEntity
{
public:
	VideoAnalyticsConfiguration();

	VideoAnalyticsConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const AnalyticsEngineConfiguration& analyticsEngineConfiguration, 
		const RuleEngineConfiguration& ruleEngineConfiguration);

	virtual ~VideoAnalyticsConfiguration();

	const AnalyticsEngineConfiguration& getAnalyticsEngineConfiguration() const;

	const RuleEngineConfiguration& getRuleEngineConfiguration() const;

	void setAnalyticsEngineConfiguration(const AnalyticsEngineConfiguration& val);

	void setAnalyticsEngineConfiguration(AnalyticsEngineConfiguration&& val);

	void setRuleEngineConfiguration(const RuleEngineConfiguration& val);

	void setRuleEngineConfiguration(RuleEngineConfiguration&& val);

private:
	//@ name=AnalyticsEngineConfiguration
	//@ order=0
	AnalyticsEngineConfiguration _analyticsEngineConfiguration;

	//@ name=RuleEngineConfiguration
	//@ order=1
	RuleEngineConfiguration _ruleEngineConfiguration;

};


inline const AnalyticsEngineConfiguration& VideoAnalyticsConfiguration::getAnalyticsEngineConfiguration() const
{
	return _analyticsEngineConfiguration;
}


inline const RuleEngineConfiguration& VideoAnalyticsConfiguration::getRuleEngineConfiguration() const
{
	return _ruleEngineConfiguration;
}


inline void VideoAnalyticsConfiguration::setAnalyticsEngineConfiguration(const AnalyticsEngineConfiguration& val)
{
	_analyticsEngineConfiguration = val;
}


inline void VideoAnalyticsConfiguration::setAnalyticsEngineConfiguration(AnalyticsEngineConfiguration&& val)
{
	_analyticsEngineConfiguration = std::move(val);
}


inline void VideoAnalyticsConfiguration::setRuleEngineConfiguration(const RuleEngineConfiguration& val)
{
	_ruleEngineConfiguration = val;
}


inline void VideoAnalyticsConfiguration::setRuleEngineConfiguration(RuleEngineConfiguration&& val)
{
	_ruleEngineConfiguration = std::move(val);
}


} // ONVIF


#endif // ONVIF_VideoAnalyticsConfiguration_INCLUDED
