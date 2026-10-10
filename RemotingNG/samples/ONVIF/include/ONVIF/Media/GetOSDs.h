// 
// GetOSDs.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_GetOSDs_INCLUDED
#define ONVIF_Media_GetOSDs_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#GetOSDs"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API GetOSDs
{
public:
	GetOSDs();

	GetOSDs(const Poco::Optional<std::string>& configurationToken);

	virtual ~GetOSDs();

	const Poco::Optional<std::string>& getConfigurationToken() const;

	void setConfigurationToken(const Poco::Optional<std::string>& val);

	void setConfigurationToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=ConfigurationToken
	//@ order=0
	Poco::Optional<std::string> _configurationToken;

};


inline const Poco::Optional<std::string>& GetOSDs::getConfigurationToken() const
{
	return _configurationToken;
}


inline void GetOSDs::setConfigurationToken(const Poco::Optional<std::string>& val)
{
	_configurationToken = val;
}


inline void GetOSDs::setConfigurationToken(Poco::Optional<std::string>&& val)
{
	_configurationToken = std::move(val);
}


} } // ONVIF::Media


#endif // ONVIF_Media_GetOSDs_INCLUDED
