// 
// RelayOutputSettings.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RelayOutputSettings_INCLUDED
#define ONVIF_RelayOutputSettings_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RelayOutputSettings
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RelayOutputSettings
{
public:
	RelayOutputSettings();

	RelayOutputSettings(
		const std::string& mode, 
		const std::string& delayTime, 
		const std::string& idleState);

	virtual ~RelayOutputSettings();

	const std::string& getDelayTime() const;

	const std::string& getIdleState() const;

	const std::string& getMode() const;

	void setDelayTime(const std::string& val);

	void setDelayTime(std::string&& val);

	void setIdleState(const std::string& val);

	void setIdleState(std::string&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ name=DelayTime
	//@ order=1
	std::string _delayTime;

	//@ name=IdleState
	//@ order=2
	std::string _idleState;

};


inline const std::string& RelayOutputSettings::getDelayTime() const
{
	return _delayTime;
}


inline const std::string& RelayOutputSettings::getIdleState() const
{
	return _idleState;
}


inline const std::string& RelayOutputSettings::getMode() const
{
	return _mode;
}


inline void RelayOutputSettings::setDelayTime(const std::string& val)
{
	_delayTime = val;
}


inline void RelayOutputSettings::setDelayTime(std::string&& val)
{
	_delayTime = std::move(val);
}


inline void RelayOutputSettings::setIdleState(const std::string& val)
{
	_idleState = val;
}


inline void RelayOutputSettings::setIdleState(std::string&& val)
{
	_idleState = std::move(val);
}


inline void RelayOutputSettings::setMode(const std::string& val)
{
	_mode = val;
}


inline void RelayOutputSettings::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_RelayOutputSettings_INCLUDED
