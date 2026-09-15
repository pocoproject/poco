// 
// AnalyticsEngineControl.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AnalyticsEngineControl_INCLUDED
#define ONVIF_AnalyticsEngineControl_INCLUDED


#include "ONVIF/Config.h"
#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class MulticastConfiguration;
} 


namespace ONVIF {


//@ name=AnalyticsEngineControl
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AnalyticsEngineControl: public ConfigurationEntity
{
public:
	AnalyticsEngineControl();

	AnalyticsEngineControl(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const std::string& engineToken, 
		const std::string& engineConfigToken, 
		const std::vector<std::string>& inputToken, 
		const std::vector<std::string>& receiverToken, 
		const Poco::SharedPtr<MulticastConfiguration>& multicast, 
		const Config& subscription, 
		const std::string& mode);

	virtual ~AnalyticsEngineControl();

	const std::string& getEngineConfigToken() const;

	const std::string& getEngineToken() const;

	const std::vector<std::string>& getInputToken() const;

	std::vector<std::string>& getInputToken();

	const std::string& getMode() const;

	const Poco::SharedPtr<MulticastConfiguration>& getMulticast() const;

	const std::vector<std::string>& getReceiverToken() const;

	std::vector<std::string>& getReceiverToken();

	const Config& getSubscription() const;

	void setEngineConfigToken(const std::string& val);

	void setEngineConfigToken(std::string&& val);

	void setEngineToken(const std::string& val);

	void setEngineToken(std::string&& val);

	void setInputToken(const std::vector<std::string>& val);

	void setInputToken(std::vector<std::string>&& val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setMulticast(const Poco::SharedPtr<MulticastConfiguration>& val);

	void setMulticast(Poco::SharedPtr<MulticastConfiguration>&& val);

	void setReceiverToken(const std::vector<std::string>& val);

	void setReceiverToken(std::vector<std::string>&& val);

	void setSubscription(const Config& val);

	void setSubscription(Config&& val);

private:
	//@ name=EngineToken
	//@ order=0
	std::string _engineToken;

	//@ name=EngineConfigToken
	//@ order=1
	std::string _engineConfigToken;

	//@ name=InputToken
	//@ order=2
	std::vector<std::string> _inputToken;

	//@ name=ReceiverToken
	//@ order=3
	std::vector<std::string> _receiverToken;

	//@ mandatory=false
	//@ name=Multicast
	//@ order=4
	Poco::SharedPtr<MulticastConfiguration> _multicast;

	//@ name=Subscription
	//@ order=5
	Config _subscription;

	//@ name=Mode
	//@ order=6
	std::string _mode;

};


inline const std::string& AnalyticsEngineControl::getEngineConfigToken() const
{
	return _engineConfigToken;
}


inline const std::string& AnalyticsEngineControl::getEngineToken() const
{
	return _engineToken;
}


inline const std::vector<std::string>& AnalyticsEngineControl::getInputToken() const
{
	return _inputToken;
}


inline std::vector<std::string>& AnalyticsEngineControl::getInputToken()
{
	return _inputToken;
}


inline const std::string& AnalyticsEngineControl::getMode() const
{
	return _mode;
}


inline const Poco::SharedPtr<MulticastConfiguration>& AnalyticsEngineControl::getMulticast() const
{
	return _multicast;
}


inline const std::vector<std::string>& AnalyticsEngineControl::getReceiverToken() const
{
	return _receiverToken;
}


inline std::vector<std::string>& AnalyticsEngineControl::getReceiverToken()
{
	return _receiverToken;
}


inline const Config& AnalyticsEngineControl::getSubscription() const
{
	return _subscription;
}


inline void AnalyticsEngineControl::setEngineConfigToken(const std::string& val)
{
	_engineConfigToken = val;
}


inline void AnalyticsEngineControl::setEngineConfigToken(std::string&& val)
{
	_engineConfigToken = std::move(val);
}


inline void AnalyticsEngineControl::setEngineToken(const std::string& val)
{
	_engineToken = val;
}


inline void AnalyticsEngineControl::setEngineToken(std::string&& val)
{
	_engineToken = std::move(val);
}


inline void AnalyticsEngineControl::setInputToken(const std::vector<std::string>& val)
{
	_inputToken = val;
}


inline void AnalyticsEngineControl::setInputToken(std::vector<std::string>&& val)
{
	_inputToken = std::move(val);
}


inline void AnalyticsEngineControl::setMode(const std::string& val)
{
	_mode = val;
}


inline void AnalyticsEngineControl::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void AnalyticsEngineControl::setMulticast(const Poco::SharedPtr<MulticastConfiguration>& val)
{
	_multicast = val;
}


inline void AnalyticsEngineControl::setMulticast(Poco::SharedPtr<MulticastConfiguration>&& val)
{
	_multicast = std::move(val);
}


inline void AnalyticsEngineControl::setReceiverToken(const std::vector<std::string>& val)
{
	_receiverToken = val;
}


inline void AnalyticsEngineControl::setReceiverToken(std::vector<std::string>&& val)
{
	_receiverToken = std::move(val);
}


inline void AnalyticsEngineControl::setSubscription(const Config& val)
{
	_subscription = val;
}


inline void AnalyticsEngineControl::setSubscription(Config&& val)
{
	_subscription = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/MulticastConfiguration.h"


#endif // ONVIF_AnalyticsEngineControl_INCLUDED
