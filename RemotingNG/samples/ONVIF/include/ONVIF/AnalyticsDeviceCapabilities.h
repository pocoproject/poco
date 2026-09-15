// 
// AnalyticsDeviceCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsDeviceCapabilities_INCLUDED
#define ONVIF_AnalyticsDeviceCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class AnalyticsDeviceExtension;
} 


namespace ONVIF {


//@ name=AnalyticsDeviceCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsDeviceCapabilities
{
public:
	AnalyticsDeviceCapabilities();

	AnalyticsDeviceCapabilities(
		const Poco::URI& xAddr, 
		Poco::Optional<bool> ruleSupport, 
		const Poco::SharedPtr<AnalyticsDeviceExtension>& extension);

	virtual ~AnalyticsDeviceCapabilities();

	const Poco::SharedPtr<AnalyticsDeviceExtension>& getExtension() const;

	Poco::Optional<bool> getRuleSupport() const;

	const Poco::URI& getXAddr() const;

	void setExtension(const Poco::SharedPtr<AnalyticsDeviceExtension>& val);

	void setExtension(Poco::SharedPtr<AnalyticsDeviceExtension>&& val);

	void setRuleSupport(Poco::Optional<bool> val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ mandatory=false
	//@ name=RuleSupport
	//@ order=1
	Poco::Optional<bool> _ruleSupport;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<AnalyticsDeviceExtension> _extension;

};


inline const Poco::SharedPtr<AnalyticsDeviceExtension>& AnalyticsDeviceCapabilities::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> AnalyticsDeviceCapabilities::getRuleSupport() const
{
	return _ruleSupport;
}


inline const Poco::URI& AnalyticsDeviceCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void AnalyticsDeviceCapabilities::setExtension(const Poco::SharedPtr<AnalyticsDeviceExtension>& val)
{
	_extension = val;
}


inline void AnalyticsDeviceCapabilities::setExtension(Poco::SharedPtr<AnalyticsDeviceExtension>&& val)
{
	_extension = std::move(val);
}


inline void AnalyticsDeviceCapabilities::setRuleSupport(Poco::Optional<bool> val)
{
	_ruleSupport = val;
}


inline void AnalyticsDeviceCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void AnalyticsDeviceCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AnalyticsDeviceExtension.h"


#endif // ONVIF_AnalyticsDeviceCapabilities_INCLUDED
