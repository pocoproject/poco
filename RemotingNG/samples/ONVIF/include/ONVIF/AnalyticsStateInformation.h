// 
// AnalyticsStateInformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsStateInformation_INCLUDED
#define ONVIF_AnalyticsStateInformation_INCLUDED


#include "ONVIF/AnalyticsState.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AnalyticsStateInformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsStateInformation
{
public:
	AnalyticsStateInformation();

	AnalyticsStateInformation(
		const std::string& analyticsEngineControlToken, 
		const AnalyticsState& state);

	virtual ~AnalyticsStateInformation();

	const std::string& getAnalyticsEngineControlToken() const;

	const AnalyticsState& getState() const;

	void setAnalyticsEngineControlToken(const std::string& val);

	void setAnalyticsEngineControlToken(std::string&& val);

	void setState(const AnalyticsState& val);

	void setState(AnalyticsState&& val);

private:
	//@ name=AnalyticsEngineControlToken
	//@ order=0
	std::string _analyticsEngineControlToken;

	//@ name=State
	//@ order=1
	AnalyticsState _state;

};


inline const std::string& AnalyticsStateInformation::getAnalyticsEngineControlToken() const
{
	return _analyticsEngineControlToken;
}


inline const AnalyticsState& AnalyticsStateInformation::getState() const
{
	return _state;
}


inline void AnalyticsStateInformation::setAnalyticsEngineControlToken(const std::string& val)
{
	_analyticsEngineControlToken = val;
}


inline void AnalyticsStateInformation::setAnalyticsEngineControlToken(std::string&& val)
{
	_analyticsEngineControlToken = std::move(val);
}


inline void AnalyticsStateInformation::setState(const AnalyticsState& val)
{
	_state = val;
}


inline void AnalyticsStateInformation::setState(AnalyticsState&& val)
{
	_state = std::move(val);
}


} // ONVIF


#endif // ONVIF_AnalyticsStateInformation_INCLUDED
