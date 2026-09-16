// 
// NetworkCapabilitiesExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkCapabilitiesExtension_INCLUDED
#define ONVIF_NetworkCapabilitiesExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkCapabilitiesExtension2;
} 


namespace ONVIF {


//@ name=NetworkCapabilitiesExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkCapabilitiesExtension
{
public:
	NetworkCapabilitiesExtension();

	NetworkCapabilitiesExtension(
		Poco::Optional<bool> dot11Configuration, 
		const Poco::SharedPtr<NetworkCapabilitiesExtension2>& extension);

	virtual ~NetworkCapabilitiesExtension();

	Poco::Optional<bool> getDot11Configuration() const;

	const Poco::SharedPtr<NetworkCapabilitiesExtension2>& getExtension() const;

	void setDot11Configuration(Poco::Optional<bool> val);

	void setExtension(const Poco::SharedPtr<NetworkCapabilitiesExtension2>& val);

	void setExtension(Poco::SharedPtr<NetworkCapabilitiesExtension2>&& val);

private:
	//@ mandatory=false
	//@ name=Dot11Configuration
	//@ order=0
	Poco::Optional<bool> _dot11Configuration;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<NetworkCapabilitiesExtension2> _extension;

};


inline Poco::Optional<bool> NetworkCapabilitiesExtension::getDot11Configuration() const
{
	return _dot11Configuration;
}


inline const Poco::SharedPtr<NetworkCapabilitiesExtension2>& NetworkCapabilitiesExtension::getExtension() const
{
	return _extension;
}


inline void NetworkCapabilitiesExtension::setDot11Configuration(Poco::Optional<bool> val)
{
	_dot11Configuration = val;
}


inline void NetworkCapabilitiesExtension::setExtension(const Poco::SharedPtr<NetworkCapabilitiesExtension2>& val)
{
	_extension = val;
}


inline void NetworkCapabilitiesExtension::setExtension(Poco::SharedPtr<NetworkCapabilitiesExtension2>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkCapabilitiesExtension2.h"


#endif // ONVIF_NetworkCapabilitiesExtension_INCLUDED
