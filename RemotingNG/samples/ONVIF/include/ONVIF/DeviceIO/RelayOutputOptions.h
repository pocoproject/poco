// 
// RelayOutputOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DeviceIO_RelayOutputOptions_INCLUDED
#define ONVIF_DeviceIO_RelayOutputOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace DeviceIO {
class RelayOutputOptionsExtension;
} } 


namespace ONVIF {
namespace DeviceIO {


//@ name=RelayOutputOptions
//@ namespace="http://www.onvif.org/ver10/deviceIO/wsdl"
//@ serialize
class ONVIF_API RelayOutputOptions
{
public:
	RelayOutputOptions();

	RelayOutputOptions(
		const std::string& token, 
		const std::vector<std::string>& mode, 
		const Poco::Optional<std::vector<float>>& delayTimes, 
		Poco::Optional<bool> discrete, 
		const Poco::SharedPtr<RelayOutputOptionsExtension>& extension);

	virtual ~RelayOutputOptions();

	const Poco::Optional<std::vector<float>>& getDelayTimes() const;

	Poco::Optional<bool> getDiscrete() const;

	const Poco::SharedPtr<RelayOutputOptionsExtension>& getExtension() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	const std::string& getToken() const;

	void setDelayTimes(const Poco::Optional<std::vector<float>>& val);

	void setDelayTimes(Poco::Optional<std::vector<float>>&& val);

	void setDiscrete(Poco::Optional<bool> val);

	void setExtension(const Poco::SharedPtr<RelayOutputOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<RelayOutputOptionsExtension>&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=token
	//@ order=0
	//@ type=attr
	std::string _token;

	//@ name=Mode
	//@ order=1
	std::vector<std::string> _mode;

	//@ mandatory=false
	//@ name=DelayTimes
	//@ order=2
	Poco::Optional<std::vector<float>> _delayTimes;

	//@ mandatory=false
	//@ name=Discrete
	//@ order=3
	Poco::Optional<bool> _discrete;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<RelayOutputOptionsExtension> _extension;

};


inline const Poco::Optional<std::vector<float>>& RelayOutputOptions::getDelayTimes() const
{
	return _delayTimes;
}


inline Poco::Optional<bool> RelayOutputOptions::getDiscrete() const
{
	return _discrete;
}


inline const Poco::SharedPtr<RelayOutputOptionsExtension>& RelayOutputOptions::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& RelayOutputOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& RelayOutputOptions::getMode()
{
	return _mode;
}


inline const std::string& RelayOutputOptions::getToken() const
{
	return _token;
}


inline void RelayOutputOptions::setDelayTimes(const Poco::Optional<std::vector<float>>& val)
{
	_delayTimes = val;
}


inline void RelayOutputOptions::setDelayTimes(Poco::Optional<std::vector<float>>&& val)
{
	_delayTimes = std::move(val);
}


inline void RelayOutputOptions::setDiscrete(Poco::Optional<bool> val)
{
	_discrete = val;
}


inline void RelayOutputOptions::setExtension(const Poco::SharedPtr<RelayOutputOptionsExtension>& val)
{
	_extension = val;
}


inline void RelayOutputOptions::setExtension(Poco::SharedPtr<RelayOutputOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void RelayOutputOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void RelayOutputOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


inline void RelayOutputOptions::setToken(const std::string& val)
{
	_token = val;
}


inline void RelayOutputOptions::setToken(std::string&& val)
{
	_token = std::move(val);
}


} } // ONVIF::DeviceIO


// The following headers are required for template instantiation.
#include "ONVIF/DeviceIO/RelayOutputOptionsExtension.h"


#endif // ONVIF_DeviceIO_RelayOutputOptions_INCLUDED
