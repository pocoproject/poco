// 
// SupportedRules.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SupportedRules_INCLUDED
#define ONVIF_SupportedRules_INCLUDED


#include "ONVIF/ConfigDescription.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class SupportedRulesExtension;
} 


namespace ONVIF {


//@ name=SupportedRules
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SupportedRules
{
public:
	SupportedRules();

	SupportedRules(
		Poco::Optional<int> limit, 
		const std::vector<Poco::URI>& ruleContentSchemaLocation, 
		const std::vector<ConfigDescription>& ruleDescription, 
		const Poco::SharedPtr<SupportedRulesExtension>& extension);

	virtual ~SupportedRules();

	const Poco::SharedPtr<SupportedRulesExtension>& getExtension() const;

	Poco::Optional<int> getLimit() const;

	const std::vector<Poco::URI>& getRuleContentSchemaLocation() const;

	std::vector<Poco::URI>& getRuleContentSchemaLocation();

	const std::vector<ConfigDescription>& getRuleDescription() const;

	std::vector<ConfigDescription>& getRuleDescription();

	void setExtension(const Poco::SharedPtr<SupportedRulesExtension>& val);

	void setExtension(Poco::SharedPtr<SupportedRulesExtension>&& val);

	void setLimit(Poco::Optional<int> val);

	void setRuleContentSchemaLocation(const std::vector<Poco::URI>& val);

	void setRuleContentSchemaLocation(std::vector<Poco::URI>&& val);

	void setRuleDescription(const std::vector<ConfigDescription>& val);

	void setRuleDescription(std::vector<ConfigDescription>&& val);

private:
	//@ mandatory=false
	//@ name=Limit
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _limit;

	//@ mandatory=false
	//@ name=RuleContentSchemaLocation
	//@ order=1
	std::vector<Poco::URI> _ruleContentSchemaLocation;

	//@ mandatory=false
	//@ name=RuleDescription
	//@ order=2
	std::vector<ConfigDescription> _ruleDescription;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<SupportedRulesExtension> _extension;

};


inline const Poco::SharedPtr<SupportedRulesExtension>& SupportedRules::getExtension() const
{
	return _extension;
}


inline Poco::Optional<int> SupportedRules::getLimit() const
{
	return _limit;
}


inline const std::vector<Poco::URI>& SupportedRules::getRuleContentSchemaLocation() const
{
	return _ruleContentSchemaLocation;
}


inline std::vector<Poco::URI>& SupportedRules::getRuleContentSchemaLocation()
{
	return _ruleContentSchemaLocation;
}


inline const std::vector<ConfigDescription>& SupportedRules::getRuleDescription() const
{
	return _ruleDescription;
}


inline std::vector<ConfigDescription>& SupportedRules::getRuleDescription()
{
	return _ruleDescription;
}


inline void SupportedRules::setExtension(const Poco::SharedPtr<SupportedRulesExtension>& val)
{
	_extension = val;
}


inline void SupportedRules::setExtension(Poco::SharedPtr<SupportedRulesExtension>&& val)
{
	_extension = std::move(val);
}


inline void SupportedRules::setLimit(Poco::Optional<int> val)
{
	_limit = val;
}


inline void SupportedRules::setRuleContentSchemaLocation(const std::vector<Poco::URI>& val)
{
	_ruleContentSchemaLocation = val;
}


inline void SupportedRules::setRuleContentSchemaLocation(std::vector<Poco::URI>&& val)
{
	_ruleContentSchemaLocation = std::move(val);
}


inline void SupportedRules::setRuleDescription(const std::vector<ConfigDescription>& val)
{
	_ruleDescription = val;
}


inline void SupportedRules::setRuleDescription(std::vector<ConfigDescription>&& val)
{
	_ruleDescription = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SupportedRulesExtension.h"


#endif // ONVIF_SupportedRules_INCLUDED
