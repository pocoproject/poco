// 
// NetworkZeroConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkZeroConfiguration_INCLUDED
#define ONVIF_NetworkZeroConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkZeroConfigurationExtension;
} 


namespace ONVIF {


//@ name=NetworkZeroConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkZeroConfiguration
{
public:
	NetworkZeroConfiguration();

	NetworkZeroConfiguration(
		const std::string& interfaceToken, 
		bool enabled, 
		const std::vector<std::string>& addresses, 
		const Poco::SharedPtr<NetworkZeroConfigurationExtension>& extension);

	virtual ~NetworkZeroConfiguration();

	const std::vector<std::string>& getAddresses() const;

	std::vector<std::string>& getAddresses();

	bool getEnabled() const;

	const Poco::SharedPtr<NetworkZeroConfigurationExtension>& getExtension() const;

	const std::string& getInterfaceToken() const;

	void setAddresses(const std::vector<std::string>& val);

	void setAddresses(std::vector<std::string>&& val);

	void setEnabled(bool val);

	void setExtension(const Poco::SharedPtr<NetworkZeroConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<NetworkZeroConfigurationExtension>&& val);

	void setInterfaceToken(const std::string& val);

	void setInterfaceToken(std::string&& val);

private:
	//@ name=InterfaceToken
	//@ order=0
	std::string _interfaceToken;

	//@ name=Enabled
	//@ order=1
	bool _enabled;

	//@ mandatory=false
	//@ name=Addresses
	//@ order=2
	std::vector<std::string> _addresses;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<NetworkZeroConfigurationExtension> _extension;

};


inline const std::vector<std::string>& NetworkZeroConfiguration::getAddresses() const
{
	return _addresses;
}


inline std::vector<std::string>& NetworkZeroConfiguration::getAddresses()
{
	return _addresses;
}


inline bool NetworkZeroConfiguration::getEnabled() const
{
	return _enabled;
}


inline const Poco::SharedPtr<NetworkZeroConfigurationExtension>& NetworkZeroConfiguration::getExtension() const
{
	return _extension;
}


inline const std::string& NetworkZeroConfiguration::getInterfaceToken() const
{
	return _interfaceToken;
}


inline void NetworkZeroConfiguration::setAddresses(const std::vector<std::string>& val)
{
	_addresses = val;
}


inline void NetworkZeroConfiguration::setAddresses(std::vector<std::string>&& val)
{
	_addresses = std::move(val);
}


inline void NetworkZeroConfiguration::setEnabled(bool val)
{
	_enabled = val;
}


inline void NetworkZeroConfiguration::setExtension(const Poco::SharedPtr<NetworkZeroConfigurationExtension>& val)
{
	_extension = val;
}


inline void NetworkZeroConfiguration::setExtension(Poco::SharedPtr<NetworkZeroConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void NetworkZeroConfiguration::setInterfaceToken(const std::string& val)
{
	_interfaceToken = val;
}


inline void NetworkZeroConfiguration::setInterfaceToken(std::string&& val)
{
	_interfaceToken = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkZeroConfigurationExtension.h"


#endif // ONVIF_NetworkZeroConfiguration_INCLUDED
