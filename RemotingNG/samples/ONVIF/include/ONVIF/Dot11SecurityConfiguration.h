// 
// Dot11SecurityConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Dot11SecurityConfiguration_INCLUDED
#define ONVIF_Dot11SecurityConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Dot11PSKSet;
class Dot11SecurityConfigurationExtension;
} 


namespace ONVIF {


//@ name=Dot11SecurityConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Dot11SecurityConfiguration
{
public:
	Dot11SecurityConfiguration();

	Dot11SecurityConfiguration(
		const std::string& mode, 
		const Poco::Optional<std::string>& algorithm, 
		const Poco::SharedPtr<Dot11PSKSet>& pSK, 
		const Poco::Optional<std::string>& dot1X, 
		const Poco::SharedPtr<Dot11SecurityConfigurationExtension>& extension);

	virtual ~Dot11SecurityConfiguration();

	const Poco::Optional<std::string>& getAlgorithm() const;

	const Poco::Optional<std::string>& getDot1X() const;

	const Poco::SharedPtr<Dot11SecurityConfigurationExtension>& getExtension() const;

	const std::string& getMode() const;

	const Poco::SharedPtr<Dot11PSKSet>& getPSK() const;

	void setAlgorithm(const Poco::Optional<std::string>& val);

	void setAlgorithm(Poco::Optional<std::string>&& val);

	void setDot1X(const Poco::Optional<std::string>& val);

	void setDot1X(Poco::Optional<std::string>&& val);

	void setExtension(const Poco::SharedPtr<Dot11SecurityConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<Dot11SecurityConfigurationExtension>&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setPSK(const Poco::SharedPtr<Dot11PSKSet>& val);

	void setPSK(Poco::SharedPtr<Dot11PSKSet>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=Algorithm
	//@ order=1
	Poco::Optional<std::string> _algorithm;

	//@ mandatory=false
	//@ name=PSK
	//@ order=2
	Poco::SharedPtr<Dot11PSKSet> _pSK;

	//@ mandatory=false
	//@ name=Dot1X
	//@ order=3
	Poco::Optional<std::string> _dot1X;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<Dot11SecurityConfigurationExtension> _extension;

};


inline const Poco::Optional<std::string>& Dot11SecurityConfiguration::getAlgorithm() const
{
	return _algorithm;
}


inline const Poco::Optional<std::string>& Dot11SecurityConfiguration::getDot1X() const
{
	return _dot1X;
}


inline const Poco::SharedPtr<Dot11SecurityConfigurationExtension>& Dot11SecurityConfiguration::getExtension() const
{
	return _extension;
}


inline const std::string& Dot11SecurityConfiguration::getMode() const
{
	return _mode;
}


inline const Poco::SharedPtr<Dot11PSKSet>& Dot11SecurityConfiguration::getPSK() const
{
	return _pSK;
}


inline void Dot11SecurityConfiguration::setAlgorithm(const Poco::Optional<std::string>& val)
{
	_algorithm = val;
}


inline void Dot11SecurityConfiguration::setAlgorithm(Poco::Optional<std::string>&& val)
{
	_algorithm = std::move(val);
}


inline void Dot11SecurityConfiguration::setDot1X(const Poco::Optional<std::string>& val)
{
	_dot1X = val;
}


inline void Dot11SecurityConfiguration::setDot1X(Poco::Optional<std::string>&& val)
{
	_dot1X = std::move(val);
}


inline void Dot11SecurityConfiguration::setExtension(const Poco::SharedPtr<Dot11SecurityConfigurationExtension>& val)
{
	_extension = val;
}


inline void Dot11SecurityConfiguration::setExtension(Poco::SharedPtr<Dot11SecurityConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void Dot11SecurityConfiguration::setMode(const std::string& val)
{
	_mode = val;
}


inline void Dot11SecurityConfiguration::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void Dot11SecurityConfiguration::setPSK(const Poco::SharedPtr<Dot11PSKSet>& val)
{
	_pSK = val;
}


inline void Dot11SecurityConfiguration::setPSK(Poco::SharedPtr<Dot11PSKSet>&& val)
{
	_pSK = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Dot11PSKSet.h"
#include "ONVIF/Dot11SecurityConfigurationExtension.h"


#endif // ONVIF_Dot11SecurityConfiguration_INCLUDED
