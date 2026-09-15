// 
// IPv4NetworkInterface.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IPv4NetworkInterface_INCLUDED
#define ONVIF_IPv4NetworkInterface_INCLUDED


#include "ONVIF/IPv4Configuration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IPv4NetworkInterface
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IPv4NetworkInterface
{
public:
	IPv4NetworkInterface();

	IPv4NetworkInterface(
		bool enabled, 
		const IPv4Configuration& config);

	virtual ~IPv4NetworkInterface();

	const IPv4Configuration& getConfig() const;

	bool getEnabled() const;

	void setConfig(const IPv4Configuration& val);

	void setConfig(IPv4Configuration&& val);

	void setEnabled(bool val);

private:
	//@ name=Enabled
	//@ order=0
	bool _enabled;

	//@ name=Config
	//@ order=1
	IPv4Configuration _config;

};


inline const IPv4Configuration& IPv4NetworkInterface::getConfig() const
{
	return _config;
}


inline bool IPv4NetworkInterface::getEnabled() const
{
	return _enabled;
}


inline void IPv4NetworkInterface::setConfig(const IPv4Configuration& val)
{
	_config = val;
}


inline void IPv4NetworkInterface::setConfig(IPv4Configuration&& val)
{
	_config = std::move(val);
}


inline void IPv4NetworkInterface::setEnabled(bool val)
{
	_enabled = val;
}


} // ONVIF


#endif // ONVIF_IPv4NetworkInterface_INCLUDED
