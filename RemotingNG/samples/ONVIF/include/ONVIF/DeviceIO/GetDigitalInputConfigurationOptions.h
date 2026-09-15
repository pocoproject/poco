// 
// GetDigitalInputConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetDigitalInputConfigurationOptions_INCLUDED
#define ONVIF_DeviceIO_GetDigitalInputConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetDigitalInputConfigurationOptions"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetDigitalInputConfigurationOptions
{
public:
	GetDigitalInputConfigurationOptions();

	GetDigitalInputConfigurationOptions(const Poco::Optional<std::string>& token);

	virtual ~GetDigitalInputConfigurationOptions();

	const Poco::Optional<std::string>& getToken() const;

	void setToken(const Poco::Optional<std::string>& val);

	void setToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Token
	//@ order=0
	Poco::Optional<std::string> _token;

};


inline const Poco::Optional<std::string>& GetDigitalInputConfigurationOptions::getToken() const
{
	return _token;
}


inline void GetDigitalInputConfigurationOptions::setToken(const Poco::Optional<std::string>& val)
{
	_token = val;
}


inline void GetDigitalInputConfigurationOptions::setToken(Poco::Optional<std::string>&& val)
{
	_token = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetDigitalInputConfigurationOptions_INCLUDED
