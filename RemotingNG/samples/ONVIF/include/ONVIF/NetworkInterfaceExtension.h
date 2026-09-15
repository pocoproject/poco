// 
// NetworkInterfaceExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkInterfaceExtension_INCLUDED
#define ONVIF_NetworkInterfaceExtension_INCLUDED


#include "ONVIF/Dot11Configuration.h"
#include "ONVIF/Dot3Configuration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkInterfaceExtension2;
} 


namespace ONVIF {


//@ name=NetworkInterfaceExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkInterfaceExtension
{
public:
	NetworkInterfaceExtension();

	NetworkInterfaceExtension(
		const int& interfaceType, 
		const std::vector<Dot3Configuration>& dot3, 
		const std::vector<Dot11Configuration>& dot11, 
		const Poco::SharedPtr<NetworkInterfaceExtension2>& extension);

	virtual ~NetworkInterfaceExtension();

	const std::vector<Dot11Configuration>& getDot11() const;

	std::vector<Dot11Configuration>& getDot11();

	const std::vector<Dot3Configuration>& getDot3() const;

	std::vector<Dot3Configuration>& getDot3();

	const Poco::SharedPtr<NetworkInterfaceExtension2>& getExtension() const;

	const int& getInterfaceType() const;

	void setDot11(const std::vector<Dot11Configuration>& val);

	void setDot11(std::vector<Dot11Configuration>&& val);

	void setDot3(const std::vector<Dot3Configuration>& val);

	void setDot3(std::vector<Dot3Configuration>&& val);

	void setExtension(const Poco::SharedPtr<NetworkInterfaceExtension2>& val);

	void setExtension(Poco::SharedPtr<NetworkInterfaceExtension2>&& val);

	void setInterfaceType(const int& val);

	void setInterfaceType(int&& val);

private:
	//@ name=InterfaceType
	//@ order=0
	int _interfaceType;

	//@ mandatory=false
	//@ name=Dot3
	//@ order=1
	std::vector<Dot3Configuration> _dot3;

	//@ mandatory=false
	//@ name=Dot11
	//@ order=2
	std::vector<Dot11Configuration> _dot11;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<NetworkInterfaceExtension2> _extension;

};


inline const std::vector<Dot11Configuration>& NetworkInterfaceExtension::getDot11() const
{
	return _dot11;
}


inline std::vector<Dot11Configuration>& NetworkInterfaceExtension::getDot11()
{
	return _dot11;
}


inline const std::vector<Dot3Configuration>& NetworkInterfaceExtension::getDot3() const
{
	return _dot3;
}


inline std::vector<Dot3Configuration>& NetworkInterfaceExtension::getDot3()
{
	return _dot3;
}


inline const Poco::SharedPtr<NetworkInterfaceExtension2>& NetworkInterfaceExtension::getExtension() const
{
	return _extension;
}


inline const int& NetworkInterfaceExtension::getInterfaceType() const
{
	return _interfaceType;
}


inline void NetworkInterfaceExtension::setDot11(const std::vector<Dot11Configuration>& val)
{
	_dot11 = val;
}


inline void NetworkInterfaceExtension::setDot11(std::vector<Dot11Configuration>&& val)
{
	_dot11 = std::move(val);
}


inline void NetworkInterfaceExtension::setDot3(const std::vector<Dot3Configuration>& val)
{
	_dot3 = val;
}


inline void NetworkInterfaceExtension::setDot3(std::vector<Dot3Configuration>&& val)
{
	_dot3 = std::move(val);
}


inline void NetworkInterfaceExtension::setExtension(const Poco::SharedPtr<NetworkInterfaceExtension2>& val)
{
	_extension = val;
}


inline void NetworkInterfaceExtension::setExtension(Poco::SharedPtr<NetworkInterfaceExtension2>&& val)
{
	_extension = std::move(val);
}


inline void NetworkInterfaceExtension::setInterfaceType(const int& val)
{
	_interfaceType = val;
}


inline void NetworkInterfaceExtension::setInterfaceType(int&& val)
{
	_interfaceType = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkInterfaceExtension2.h"


#endif // ONVIF_NetworkInterfaceExtension_INCLUDED
