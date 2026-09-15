// 
// Dot11Configuration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Dot11Configuration_INCLUDED
#define ONVIF_Dot11Configuration_INCLUDED


#include "ONVIF/Dot11SecurityConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Dot11Configuration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Dot11Configuration
{
public:
	Dot11Configuration();

	Dot11Configuration(
		const std::string& sSID, 
		const std::string& mode, 
		const std::string& alias, 
		const int& priority, 
		const Dot11SecurityConfiguration& security);

	virtual ~Dot11Configuration();

	const std::string& getAlias() const;

	const std::string& getMode() const;

	const int& getPriority() const;

	const std::string& getSSID() const;

	const Dot11SecurityConfiguration& getSecurity() const;

	void setAlias(const std::string& val);

	void setAlias(std::string&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setPriority(const int& val);

	void setPriority(int&& val);

	void setSSID(const std::string& val);

	void setSSID(std::string&& val);

	void setSecurity(const Dot11SecurityConfiguration& val);

	void setSecurity(Dot11SecurityConfiguration&& val);

private:
	//@ name=SSID
	//@ order=0
	std::string _sSID;

	//@ name=Mode
	//@ order=1
	std::string _mode;

	//@ name=Alias
	//@ order=2
	std::string _alias;

	//@ name=Priority
	//@ order=3
	int _priority;

	//@ name=Security
	//@ order=4
	Dot11SecurityConfiguration _security;

};


inline const std::string& Dot11Configuration::getAlias() const
{
	return _alias;
}


inline const std::string& Dot11Configuration::getMode() const
{
	return _mode;
}


inline const int& Dot11Configuration::getPriority() const
{
	return _priority;
}


inline const std::string& Dot11Configuration::getSSID() const
{
	return _sSID;
}


inline const Dot11SecurityConfiguration& Dot11Configuration::getSecurity() const
{
	return _security;
}


inline void Dot11Configuration::setAlias(const std::string& val)
{
	_alias = val;
}


inline void Dot11Configuration::setAlias(std::string&& val)
{
	_alias = std::move(val);
}


inline void Dot11Configuration::setMode(const std::string& val)
{
	_mode = val;
}


inline void Dot11Configuration::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void Dot11Configuration::setPriority(const int& val)
{
	_priority = val;
}


inline void Dot11Configuration::setPriority(int&& val)
{
	_priority = std::move(val);
}


inline void Dot11Configuration::setSSID(const std::string& val)
{
	_sSID = val;
}


inline void Dot11Configuration::setSSID(std::string&& val)
{
	_sSID = std::move(val);
}


inline void Dot11Configuration::setSecurity(const Dot11SecurityConfiguration& val)
{
	_security = val;
}


inline void Dot11Configuration::setSecurity(Dot11SecurityConfiguration&& val)
{
	_security = std::move(val);
}


} // ONVIF


#endif // ONVIF_Dot11Configuration_INCLUDED
