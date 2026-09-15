// 
// SystemCapabilitiesExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SystemCapabilitiesExtension_INCLUDED
#define ONVIF_SystemCapabilitiesExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class SystemCapabilitiesExtension2;
} 


namespace ONVIF {


//@ name=SystemCapabilitiesExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SystemCapabilitiesExtension
{
public:
	SystemCapabilitiesExtension();

	SystemCapabilitiesExtension(
		Poco::Optional<bool> httpFirmwareUpgrade, 
		Poco::Optional<bool> httpSystemBackup, 
		Poco::Optional<bool> httpSystemLogging, 
		Poco::Optional<bool> httpSupportInformation, 
		const Poco::SharedPtr<SystemCapabilitiesExtension2>& extension);

	virtual ~SystemCapabilitiesExtension();

	const Poco::SharedPtr<SystemCapabilitiesExtension2>& getExtension() const;

	Poco::Optional<bool> getHttpFirmwareUpgrade() const;

	Poco::Optional<bool> getHttpSupportInformation() const;

	Poco::Optional<bool> getHttpSystemBackup() const;

	Poco::Optional<bool> getHttpSystemLogging() const;

	void setExtension(const Poco::SharedPtr<SystemCapabilitiesExtension2>& val);

	void setExtension(Poco::SharedPtr<SystemCapabilitiesExtension2>&& val);

	void setHttpFirmwareUpgrade(Poco::Optional<bool> val);

	void setHttpSupportInformation(Poco::Optional<bool> val);

	void setHttpSystemBackup(Poco::Optional<bool> val);

	void setHttpSystemLogging(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=HttpFirmwareUpgrade
	//@ order=0
	Poco::Optional<bool> _httpFirmwareUpgrade;

	//@ mandatory=false
	//@ name=HttpSystemBackup
	//@ order=1
	Poco::Optional<bool> _httpSystemBackup;

	//@ mandatory=false
	//@ name=HttpSystemLogging
	//@ order=2
	Poco::Optional<bool> _httpSystemLogging;

	//@ mandatory=false
	//@ name=HttpSupportInformation
	//@ order=3
	Poco::Optional<bool> _httpSupportInformation;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<SystemCapabilitiesExtension2> _extension;

};


inline const Poco::SharedPtr<SystemCapabilitiesExtension2>& SystemCapabilitiesExtension::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> SystemCapabilitiesExtension::getHttpFirmwareUpgrade() const
{
	return _httpFirmwareUpgrade;
}


inline Poco::Optional<bool> SystemCapabilitiesExtension::getHttpSupportInformation() const
{
	return _httpSupportInformation;
}


inline Poco::Optional<bool> SystemCapabilitiesExtension::getHttpSystemBackup() const
{
	return _httpSystemBackup;
}


inline Poco::Optional<bool> SystemCapabilitiesExtension::getHttpSystemLogging() const
{
	return _httpSystemLogging;
}


inline void SystemCapabilitiesExtension::setExtension(const Poco::SharedPtr<SystemCapabilitiesExtension2>& val)
{
	_extension = val;
}


inline void SystemCapabilitiesExtension::setExtension(Poco::SharedPtr<SystemCapabilitiesExtension2>&& val)
{
	_extension = std::move(val);
}


inline void SystemCapabilitiesExtension::setHttpFirmwareUpgrade(Poco::Optional<bool> val)
{
	_httpFirmwareUpgrade = val;
}


inline void SystemCapabilitiesExtension::setHttpSupportInformation(Poco::Optional<bool> val)
{
	_httpSupportInformation = val;
}


inline void SystemCapabilitiesExtension::setHttpSystemBackup(Poco::Optional<bool> val)
{
	_httpSystemBackup = val;
}


inline void SystemCapabilitiesExtension::setHttpSystemLogging(Poco::Optional<bool> val)
{
	_httpSystemLogging = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SystemCapabilitiesExtension2.h"


#endif // ONVIF_SystemCapabilitiesExtension_INCLUDED
