// 
// RuleEngineConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RuleEngineConfiguration_INCLUDED
#define ONVIF_RuleEngineConfiguration_INCLUDED


#include "ONVIF/Config.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RuleEngineConfigurationExtension;
} 


namespace ONVIF {


//@ name=RuleEngineConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RuleEngineConfiguration
{
public:
	RuleEngineConfiguration();

	RuleEngineConfiguration(
		const std::vector<Config>& rule, 
		const Poco::SharedPtr<RuleEngineConfigurationExtension>& extension);

	virtual ~RuleEngineConfiguration();

	const Poco::SharedPtr<RuleEngineConfigurationExtension>& getExtension() const;

	const std::vector<Config>& getRule() const;

	std::vector<Config>& getRule();

	void setExtension(const Poco::SharedPtr<RuleEngineConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<RuleEngineConfigurationExtension>&& val);

	void setRule(const std::vector<Config>& val);

	void setRule(std::vector<Config>&& val);

private:
	//@ mandatory=false
	//@ name=Rule
	//@ order=0
	std::vector<Config> _rule;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<RuleEngineConfigurationExtension> _extension;

};


inline const Poco::SharedPtr<RuleEngineConfigurationExtension>& RuleEngineConfiguration::getExtension() const
{
	return _extension;
}


inline const std::vector<Config>& RuleEngineConfiguration::getRule() const
{
	return _rule;
}


inline std::vector<Config>& RuleEngineConfiguration::getRule()
{
	return _rule;
}


inline void RuleEngineConfiguration::setExtension(const Poco::SharedPtr<RuleEngineConfigurationExtension>& val)
{
	_extension = val;
}


inline void RuleEngineConfiguration::setExtension(Poco::SharedPtr<RuleEngineConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void RuleEngineConfiguration::setRule(const std::vector<Config>& val)
{
	_rule = val;
}


inline void RuleEngineConfiguration::setRule(std::vector<Config>&& val)
{
	_rule = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RuleEngineConfigurationExtension.h"


#endif // ONVIF_RuleEngineConfiguration_INCLUDED
