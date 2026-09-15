// 
// NetworkInterfaceSetConfigurationExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED
#define ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED


#include "ONVIF/Dot11Configuration.h"
#include "ONVIF/Dot3Configuration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkInterfaceSetConfigurationExtension2;
} 


namespace ONVIF {


//@ name=NetworkInterfaceSetConfigurationExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkInterfaceSetConfigurationExtension
{
public:
	NetworkInterfaceSetConfigurationExtension();

	NetworkInterfaceSetConfigurationExtension(
		const std::vector<Dot3Configuration>& dot3, 
		const std::vector<Dot11Configuration>& dot11, 
		const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>& extension);

	virtual ~NetworkInterfaceSetConfigurationExtension();

	const std::vector<Dot11Configuration>& getDot11() const;

	std::vector<Dot11Configuration>& getDot11();

	const std::vector<Dot3Configuration>& getDot3() const;

	std::vector<Dot3Configuration>& getDot3();

	const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>& getExtension() const;

	void setDot11(const std::vector<Dot11Configuration>& val);

	void setDot11(std::vector<Dot11Configuration>&& val);

	void setDot3(const std::vector<Dot3Configuration>& val);

	void setDot3(std::vector<Dot3Configuration>&& val);

	void setExtension(const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>& val);

	void setExtension(Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>&& val);

private:
	//@ mandatory=false
	//@ name=Dot3
	//@ order=0
	std::vector<Dot3Configuration> _dot3;

	//@ mandatory=false
	//@ name=Dot11
	//@ order=1
	std::vector<Dot11Configuration> _dot11;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2> _extension;

};


inline const std::vector<Dot11Configuration>& NetworkInterfaceSetConfigurationExtension::getDot11() const
{
	return _dot11;
}


inline std::vector<Dot11Configuration>& NetworkInterfaceSetConfigurationExtension::getDot11()
{
	return _dot11;
}


inline const std::vector<Dot3Configuration>& NetworkInterfaceSetConfigurationExtension::getDot3() const
{
	return _dot3;
}


inline std::vector<Dot3Configuration>& NetworkInterfaceSetConfigurationExtension::getDot3()
{
	return _dot3;
}


inline const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>& NetworkInterfaceSetConfigurationExtension::getExtension() const
{
	return _extension;
}


inline void NetworkInterfaceSetConfigurationExtension::setDot11(const std::vector<Dot11Configuration>& val)
{
	_dot11 = val;
}


inline void NetworkInterfaceSetConfigurationExtension::setDot11(std::vector<Dot11Configuration>&& val)
{
	_dot11 = std::move(val);
}


inline void NetworkInterfaceSetConfigurationExtension::setDot3(const std::vector<Dot3Configuration>& val)
{
	_dot3 = val;
}


inline void NetworkInterfaceSetConfigurationExtension::setDot3(std::vector<Dot3Configuration>&& val)
{
	_dot3 = std::move(val);
}


inline void NetworkInterfaceSetConfigurationExtension::setExtension(const Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>& val)
{
	_extension = val;
}


inline void NetworkInterfaceSetConfigurationExtension::setExtension(Poco::SharedPtr<NetworkInterfaceSetConfigurationExtension2>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkInterfaceSetConfigurationExtension2.h"


#endif // ONVIF_NetworkInterfaceSetConfigurationExtension_INCLUDED
