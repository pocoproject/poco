// 
// GetRelayOutputOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_GetRelayOutputOptions_INCLUDED
#define ONVIF_DeviceIO_GetRelayOutputOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {


//@ name="#GetRelayOutputOptions"
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API GetRelayOutputOptions
{
public:
	GetRelayOutputOptions();

	GetRelayOutputOptions(const Poco::Optional<std::string>& relayOutputToken);

	virtual ~GetRelayOutputOptions();

	const Poco::Optional<std::string>& getRelayOutputToken() const;

	void setRelayOutputToken(const Poco::Optional<std::string>& val);

	void setRelayOutputToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=RelayOutputToken
	//@ order=0
	Poco::Optional<std::string> _relayOutputToken;

};


inline const Poco::Optional<std::string>& GetRelayOutputOptions::getRelayOutputToken() const
{
	return _relayOutputToken;
}


inline void GetRelayOutputOptions::setRelayOutputToken(const Poco::Optional<std::string>& val)
{
	_relayOutputToken = val;
}


inline void GetRelayOutputOptions::setRelayOutputToken(Poco::Optional<std::string>&& val)
{
	_relayOutputToken = std::move(val);
}


} } // ONVIF::DeviceIO


#endif // ONVIF_DeviceIO_GetRelayOutputOptions_INCLUDED
