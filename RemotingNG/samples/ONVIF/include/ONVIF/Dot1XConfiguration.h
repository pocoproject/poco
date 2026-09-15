// 
// Dot1XConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Dot1XConfiguration_INCLUDED
#define ONVIF_Dot1XConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Dot1XConfigurationExtension;
class EAPMethodConfiguration;
} 


namespace ONVIF {


//@ name=Dot1XConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Dot1XConfiguration
{
public:
	Dot1XConfiguration();

	Dot1XConfiguration(
		const std::string& dot1XConfigurationToken, 
		const std::string& identity, 
		const Poco::Optional<std::string>& anonymousID, 
		int eAPMethod, 
		const std::vector<std::string>& cACertificateID, 
		const Poco::SharedPtr<EAPMethodConfiguration>& eAPMethodConfiguration, 
		const Poco::SharedPtr<Dot1XConfigurationExtension>& extension);

	virtual ~Dot1XConfiguration();

	const Poco::Optional<std::string>& getAnonymousID() const;

	const std::vector<std::string>& getCACertificateID() const;

	std::vector<std::string>& getCACertificateID();

	const std::string& getDot1XConfigurationToken() const;

	int getEAPMethod() const;

	const Poco::SharedPtr<EAPMethodConfiguration>& getEAPMethodConfiguration() const;

	const Poco::SharedPtr<Dot1XConfigurationExtension>& getExtension() const;

	const std::string& getIdentity() const;

	void setAnonymousID(const Poco::Optional<std::string>& val);

	void setAnonymousID(Poco::Optional<std::string>&& val);

	void setCACertificateID(const std::vector<std::string>& val);

	void setCACertificateID(std::vector<std::string>&& val);

	void setDot1XConfigurationToken(const std::string& val);

	void setDot1XConfigurationToken(std::string&& val);

	void setEAPMethod(int val);

	void setEAPMethodConfiguration(const Poco::SharedPtr<EAPMethodConfiguration>& val);

	void setEAPMethodConfiguration(Poco::SharedPtr<EAPMethodConfiguration>&& val);

	void setExtension(const Poco::SharedPtr<Dot1XConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<Dot1XConfigurationExtension>&& val);

	void setIdentity(const std::string& val);

	void setIdentity(std::string&& val);

private:
	//@ name=Dot1XConfigurationToken
	//@ order=0
	std::string _dot1XConfigurationToken;

	//@ name=Identity
	//@ order=1
	std::string _identity;

	//@ mandatory=false
	//@ name=AnonymousID
	//@ order=2
	Poco::Optional<std::string> _anonymousID;

	//@ name=EAPMethod
	//@ order=3
	int _eAPMethod;

	//@ mandatory=false
	//@ name=CACertificateID
	//@ order=4
	std::vector<std::string> _cACertificateID;

	//@ mandatory=false
	//@ name=EAPMethodConfiguration
	//@ order=5
	Poco::SharedPtr<EAPMethodConfiguration> _eAPMethodConfiguration;

	//@ mandatory=false
	//@ name=Extension
	//@ order=6
	Poco::SharedPtr<Dot1XConfigurationExtension> _extension;

};


inline const Poco::Optional<std::string>& Dot1XConfiguration::getAnonymousID() const
{
	return _anonymousID;
}


inline const std::vector<std::string>& Dot1XConfiguration::getCACertificateID() const
{
	return _cACertificateID;
}


inline std::vector<std::string>& Dot1XConfiguration::getCACertificateID()
{
	return _cACertificateID;
}


inline const std::string& Dot1XConfiguration::getDot1XConfigurationToken() const
{
	return _dot1XConfigurationToken;
}


inline int Dot1XConfiguration::getEAPMethod() const
{
	return _eAPMethod;
}


inline const Poco::SharedPtr<EAPMethodConfiguration>& Dot1XConfiguration::getEAPMethodConfiguration() const
{
	return _eAPMethodConfiguration;
}


inline const Poco::SharedPtr<Dot1XConfigurationExtension>& Dot1XConfiguration::getExtension() const
{
	return _extension;
}


inline const std::string& Dot1XConfiguration::getIdentity() const
{
	return _identity;
}


inline void Dot1XConfiguration::setAnonymousID(const Poco::Optional<std::string>& val)
{
	_anonymousID = val;
}


inline void Dot1XConfiguration::setAnonymousID(Poco::Optional<std::string>&& val)
{
	_anonymousID = std::move(val);
}


inline void Dot1XConfiguration::setCACertificateID(const std::vector<std::string>& val)
{
	_cACertificateID = val;
}


inline void Dot1XConfiguration::setCACertificateID(std::vector<std::string>&& val)
{
	_cACertificateID = std::move(val);
}


inline void Dot1XConfiguration::setDot1XConfigurationToken(const std::string& val)
{
	_dot1XConfigurationToken = val;
}


inline void Dot1XConfiguration::setDot1XConfigurationToken(std::string&& val)
{
	_dot1XConfigurationToken = std::move(val);
}


inline void Dot1XConfiguration::setEAPMethod(int val)
{
	_eAPMethod = val;
}


inline void Dot1XConfiguration::setEAPMethodConfiguration(const Poco::SharedPtr<EAPMethodConfiguration>& val)
{
	_eAPMethodConfiguration = val;
}


inline void Dot1XConfiguration::setEAPMethodConfiguration(Poco::SharedPtr<EAPMethodConfiguration>&& val)
{
	_eAPMethodConfiguration = std::move(val);
}


inline void Dot1XConfiguration::setExtension(const Poco::SharedPtr<Dot1XConfigurationExtension>& val)
{
	_extension = val;
}


inline void Dot1XConfiguration::setExtension(Poco::SharedPtr<Dot1XConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void Dot1XConfiguration::setIdentity(const std::string& val)
{
	_identity = val;
}


inline void Dot1XConfiguration::setIdentity(std::string&& val)
{
	_identity = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Dot1XConfigurationExtension.h"
#include "ONVIF/EAPMethodConfiguration.h"


#endif // ONVIF_Dot1XConfiguration_INCLUDED
