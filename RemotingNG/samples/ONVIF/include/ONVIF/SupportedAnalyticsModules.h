// 
// SupportedAnalyticsModules.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SupportedAnalyticsModules_INCLUDED
#define ONVIF_SupportedAnalyticsModules_INCLUDED


#include "ONVIF/ConfigDescription.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class SupportedAnalyticsModulesExtension;
} 


namespace ONVIF {


//@ name=SupportedAnalyticsModules
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SupportedAnalyticsModules
{
public:
	SupportedAnalyticsModules();

	SupportedAnalyticsModules(
		Poco::Optional<int> limit, 
		const std::vector<Poco::URI>& analyticsModuleContentSchemaLocation, 
		const std::vector<ConfigDescription>& analyticsModuleDescription, 
		const Poco::SharedPtr<SupportedAnalyticsModulesExtension>& extension);

	virtual ~SupportedAnalyticsModules();

	const std::vector<Poco::URI>& getAnalyticsModuleContentSchemaLocation() const;

	std::vector<Poco::URI>& getAnalyticsModuleContentSchemaLocation();

	const std::vector<ConfigDescription>& getAnalyticsModuleDescription() const;

	std::vector<ConfigDescription>& getAnalyticsModuleDescription();

	const Poco::SharedPtr<SupportedAnalyticsModulesExtension>& getExtension() const;

	Poco::Optional<int> getLimit() const;

	void setAnalyticsModuleContentSchemaLocation(const std::vector<Poco::URI>& val);

	void setAnalyticsModuleContentSchemaLocation(std::vector<Poco::URI>&& val);

	void setAnalyticsModuleDescription(const std::vector<ConfigDescription>& val);

	void setAnalyticsModuleDescription(std::vector<ConfigDescription>&& val);

	void setExtension(const Poco::SharedPtr<SupportedAnalyticsModulesExtension>& val);

	void setExtension(Poco::SharedPtr<SupportedAnalyticsModulesExtension>&& val);

	void setLimit(Poco::Optional<int> val);

private:
	//@ mandatory=false
	//@ name=Limit
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _limit;

	//@ mandatory=false
	//@ name=AnalyticsModuleContentSchemaLocation
	//@ order=1
	std::vector<Poco::URI> _analyticsModuleContentSchemaLocation;

	//@ mandatory=false
	//@ name=AnalyticsModuleDescription
	//@ order=2
	std::vector<ConfigDescription> _analyticsModuleDescription;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<SupportedAnalyticsModulesExtension> _extension;

};


inline const std::vector<Poco::URI>& SupportedAnalyticsModules::getAnalyticsModuleContentSchemaLocation() const
{
	return _analyticsModuleContentSchemaLocation;
}


inline std::vector<Poco::URI>& SupportedAnalyticsModules::getAnalyticsModuleContentSchemaLocation()
{
	return _analyticsModuleContentSchemaLocation;
}


inline const std::vector<ConfigDescription>& SupportedAnalyticsModules::getAnalyticsModuleDescription() const
{
	return _analyticsModuleDescription;
}


inline std::vector<ConfigDescription>& SupportedAnalyticsModules::getAnalyticsModuleDescription()
{
	return _analyticsModuleDescription;
}


inline const Poco::SharedPtr<SupportedAnalyticsModulesExtension>& SupportedAnalyticsModules::getExtension() const
{
	return _extension;
}


inline Poco::Optional<int> SupportedAnalyticsModules::getLimit() const
{
	return _limit;
}


inline void SupportedAnalyticsModules::setAnalyticsModuleContentSchemaLocation(const std::vector<Poco::URI>& val)
{
	_analyticsModuleContentSchemaLocation = val;
}


inline void SupportedAnalyticsModules::setAnalyticsModuleContentSchemaLocation(std::vector<Poco::URI>&& val)
{
	_analyticsModuleContentSchemaLocation = std::move(val);
}


inline void SupportedAnalyticsModules::setAnalyticsModuleDescription(const std::vector<ConfigDescription>& val)
{
	_analyticsModuleDescription = val;
}


inline void SupportedAnalyticsModules::setAnalyticsModuleDescription(std::vector<ConfigDescription>&& val)
{
	_analyticsModuleDescription = std::move(val);
}


inline void SupportedAnalyticsModules::setExtension(const Poco::SharedPtr<SupportedAnalyticsModulesExtension>& val)
{
	_extension = val;
}


inline void SupportedAnalyticsModules::setExtension(Poco::SharedPtr<SupportedAnalyticsModulesExtension>&& val)
{
	_extension = std::move(val);
}


inline void SupportedAnalyticsModules::setLimit(Poco::Optional<int> val)
{
	_limit = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SupportedAnalyticsModulesExtension.h"


#endif // ONVIF_SupportedAnalyticsModules_INCLUDED
