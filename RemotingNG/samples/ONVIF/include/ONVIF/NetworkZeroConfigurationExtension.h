// 
// NetworkZeroConfigurationExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkZeroConfigurationExtension_INCLUDED
#define ONVIF_NetworkZeroConfigurationExtension_INCLUDED


#include "ONVIF/NetworkZeroConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class NetworkZeroConfigurationExtension2;
} 


namespace ONVIF {


//@ name=NetworkZeroConfigurationExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkZeroConfigurationExtension
{
public:
	NetworkZeroConfigurationExtension();

	NetworkZeroConfigurationExtension(
		const std::vector<NetworkZeroConfiguration>& additional, 
		const Poco::SharedPtr<NetworkZeroConfigurationExtension2>& extension);

	virtual ~NetworkZeroConfigurationExtension();

	const std::vector<NetworkZeroConfiguration>& getAdditional() const;

	std::vector<NetworkZeroConfiguration>& getAdditional();

	const Poco::SharedPtr<NetworkZeroConfigurationExtension2>& getExtension() const;

	void setAdditional(const std::vector<NetworkZeroConfiguration>& val);

	void setAdditional(std::vector<NetworkZeroConfiguration>&& val);

	void setExtension(const Poco::SharedPtr<NetworkZeroConfigurationExtension2>& val);

	void setExtension(Poco::SharedPtr<NetworkZeroConfigurationExtension2>&& val);

private:
	//@ mandatory=false
	//@ name=Additional
	//@ order=0
	std::vector<NetworkZeroConfiguration> _additional;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<NetworkZeroConfigurationExtension2> _extension;

};


inline const std::vector<NetworkZeroConfiguration>& NetworkZeroConfigurationExtension::getAdditional() const
{
	return _additional;
}


inline std::vector<NetworkZeroConfiguration>& NetworkZeroConfigurationExtension::getAdditional()
{
	return _additional;
}


inline const Poco::SharedPtr<NetworkZeroConfigurationExtension2>& NetworkZeroConfigurationExtension::getExtension() const
{
	return _extension;
}


inline void NetworkZeroConfigurationExtension::setAdditional(const std::vector<NetworkZeroConfiguration>& val)
{
	_additional = val;
}


inline void NetworkZeroConfigurationExtension::setAdditional(std::vector<NetworkZeroConfiguration>&& val)
{
	_additional = std::move(val);
}


inline void NetworkZeroConfigurationExtension::setExtension(const Poco::SharedPtr<NetworkZeroConfigurationExtension2>& val)
{
	_extension = val;
}


inline void NetworkZeroConfigurationExtension::setExtension(Poco::SharedPtr<NetworkZeroConfigurationExtension2>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/NetworkZeroConfigurationExtension2.h"


#endif // ONVIF_NetworkZeroConfigurationExtension_INCLUDED
