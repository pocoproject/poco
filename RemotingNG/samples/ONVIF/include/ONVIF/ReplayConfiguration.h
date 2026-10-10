// 
// ReplayConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ReplayConfiguration_INCLUDED
#define ONVIF_ReplayConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ReplayConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ReplayConfiguration
{
public:
	ReplayConfiguration();

	ReplayConfiguration(const std::string& sessionTimeout);

	virtual ~ReplayConfiguration();

	const std::string& getSessionTimeout() const;

	void setSessionTimeout(const std::string& val);

	void setSessionTimeout(std::string&& val);

private:
	//@ name=SessionTimeout
	//@ order=0
	std::string _sessionTimeout;

};


inline const std::string& ReplayConfiguration::getSessionTimeout() const
{
	return _sessionTimeout;
}


inline void ReplayConfiguration::setSessionTimeout(const std::string& val)
{
	_sessionTimeout = val;
}


inline void ReplayConfiguration::setSessionTimeout(std::string&& val)
{
	_sessionTimeout = std::move(val);
}


} // ONVIF


#endif // ONVIF_ReplayConfiguration_INCLUDED
