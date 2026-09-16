// 
// AnalyticsState.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsState_INCLUDED
#define ONVIF_AnalyticsState_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AnalyticsState
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsState
{
public:
	AnalyticsState();

	AnalyticsState(
		const Poco::Optional<std::string>& error, 
		const std::string& state);

	virtual ~AnalyticsState();

	const Poco::Optional<std::string>& getError() const;

	const std::string& getState() const;

	void setError(const Poco::Optional<std::string>& val);

	void setError(Poco::Optional<std::string>&& val);

	void setState(const std::string& val);

	void setState(std::string&& val);

private:
	//@ mandatory=false
	//@ name=Error
	//@ order=0
	Poco::Optional<std::string> _error;

	//@ name=State
	//@ order=1
	std::string _state;

};


inline const Poco::Optional<std::string>& AnalyticsState::getError() const
{
	return _error;
}


inline const std::string& AnalyticsState::getState() const
{
	return _state;
}


inline void AnalyticsState::setError(const Poco::Optional<std::string>& val)
{
	_error = val;
}


inline void AnalyticsState::setError(Poco::Optional<std::string>&& val)
{
	_error = std::move(val);
}


inline void AnalyticsState::setState(const std::string& val)
{
	_state = val;
}


inline void AnalyticsState::setState(std::string&& val)
{
	_state = std::move(val);
}


} // ONVIF


#endif // ONVIF_AnalyticsState_INCLUDED
