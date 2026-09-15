// 
// EAPMethodConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_EAPMethodConfiguration_INCLUDED
#define ONVIF_EAPMethodConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class EapMethodExtension;
class TLSConfiguration;
} 


namespace ONVIF {


//@ name=EAPMethodConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API EAPMethodConfiguration
{
public:
	EAPMethodConfiguration();

	EAPMethodConfiguration(
		const Poco::SharedPtr<TLSConfiguration>& tLSConfiguration, 
		const Poco::Optional<std::string>& password, 
		const Poco::SharedPtr<EapMethodExtension>& extension);

	virtual ~EAPMethodConfiguration();

	const Poco::SharedPtr<EapMethodExtension>& getExtension() const;

	const Poco::Optional<std::string>& getPassword() const;

	const Poco::SharedPtr<TLSConfiguration>& getTLSConfiguration() const;

	void setExtension(const Poco::SharedPtr<EapMethodExtension>& val);

	void setExtension(Poco::SharedPtr<EapMethodExtension>&& val);

	void setPassword(const Poco::Optional<std::string>& val);

	void setPassword(Poco::Optional<std::string>&& val);

	void setTLSConfiguration(const Poco::SharedPtr<TLSConfiguration>& val);

	void setTLSConfiguration(Poco::SharedPtr<TLSConfiguration>&& val);

private:
	//@ mandatory=false
	//@ name=TLSConfiguration
	//@ order=0
	Poco::SharedPtr<TLSConfiguration> _tLSConfiguration;

	//@ mandatory=false
	//@ name=Password
	//@ order=1
	Poco::Optional<std::string> _password;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<EapMethodExtension> _extension;

};


inline const Poco::SharedPtr<EapMethodExtension>& EAPMethodConfiguration::getExtension() const
{
	return _extension;
}


inline const Poco::Optional<std::string>& EAPMethodConfiguration::getPassword() const
{
	return _password;
}


inline const Poco::SharedPtr<TLSConfiguration>& EAPMethodConfiguration::getTLSConfiguration() const
{
	return _tLSConfiguration;
}


inline void EAPMethodConfiguration::setExtension(const Poco::SharedPtr<EapMethodExtension>& val)
{
	_extension = val;
}


inline void EAPMethodConfiguration::setExtension(Poco::SharedPtr<EapMethodExtension>&& val)
{
	_extension = std::move(val);
}


inline void EAPMethodConfiguration::setPassword(const Poco::Optional<std::string>& val)
{
	_password = val;
}


inline void EAPMethodConfiguration::setPassword(Poco::Optional<std::string>&& val)
{
	_password = std::move(val);
}


inline void EAPMethodConfiguration::setTLSConfiguration(const Poco::SharedPtr<TLSConfiguration>& val)
{
	_tLSConfiguration = val;
}


inline void EAPMethodConfiguration::setTLSConfiguration(Poco::SharedPtr<TLSConfiguration>&& val)
{
	_tLSConfiguration = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/EapMethodExtension.h"
#include "ONVIF/TLSConfiguration.h"


#endif // ONVIF_EAPMethodConfiguration_INCLUDED
