// 
// IPv6NetworkInterface.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPv6NetworkInterface_INCLUDED
#define ONVIF_IPv6NetworkInterface_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class IPv6Configuration;
} 


namespace ONVIF {


//@ name=IPv6NetworkInterface
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPv6NetworkInterface
{
public:
	IPv6NetworkInterface();

	IPv6NetworkInterface(
		bool enabled, 
		const Poco::SharedPtr<IPv6Configuration>& config);

	virtual ~IPv6NetworkInterface();

	const Poco::SharedPtr<IPv6Configuration>& getConfig() const;

	bool getEnabled() const;

	void setConfig(const Poco::SharedPtr<IPv6Configuration>& val);

	void setConfig(Poco::SharedPtr<IPv6Configuration>&& val);

	void setEnabled(bool val);

private:
	//@ name=Enabled
	//@ order=0
	bool _enabled;

	//@ mandatory=false
	//@ name=Config
	//@ order=1
	Poco::SharedPtr<IPv6Configuration> _config;

};


inline const Poco::SharedPtr<IPv6Configuration>& IPv6NetworkInterface::getConfig() const
{
	return _config;
}


inline bool IPv6NetworkInterface::getEnabled() const
{
	return _enabled;
}


inline void IPv6NetworkInterface::setConfig(const Poco::SharedPtr<IPv6Configuration>& val)
{
	_config = val;
}


inline void IPv6NetworkInterface::setConfig(Poco::SharedPtr<IPv6Configuration>&& val)
{
	_config = std::move(val);
}


inline void IPv6NetworkInterface::setEnabled(bool val)
{
	_enabled = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/IPv6Configuration.h"


#endif // ONVIF_IPv6NetworkInterface_INCLUDED
