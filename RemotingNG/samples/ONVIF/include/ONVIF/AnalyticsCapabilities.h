// 
// AnalyticsCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsCapabilities_INCLUDED
#define ONVIF_AnalyticsCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AnalyticsCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsCapabilities
{
public:
	AnalyticsCapabilities();

	AnalyticsCapabilities(
		const Poco::URI& xAddr, 
		bool ruleSupport, 
		bool analyticsModuleSupport);

	virtual ~AnalyticsCapabilities();

	bool getAnalyticsModuleSupport() const;

	bool getRuleSupport() const;

	const Poco::URI& getXAddr() const;

	void setAnalyticsModuleSupport(bool val);

	void setRuleSupport(bool val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=RuleSupport
	//@ order=1
	bool _ruleSupport;

	//@ name=AnalyticsModuleSupport
	//@ order=2
	bool _analyticsModuleSupport;

};


inline bool AnalyticsCapabilities::getAnalyticsModuleSupport() const
{
	return _analyticsModuleSupport;
}


inline bool AnalyticsCapabilities::getRuleSupport() const
{
	return _ruleSupport;
}


inline const Poco::URI& AnalyticsCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void AnalyticsCapabilities::setAnalyticsModuleSupport(bool val)
{
	_analyticsModuleSupport = val;
}


inline void AnalyticsCapabilities::setRuleSupport(bool val)
{
	_ruleSupport = val;
}


inline void AnalyticsCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void AnalyticsCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_AnalyticsCapabilities_INCLUDED
