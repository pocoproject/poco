// 
// Receiver.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Receiver_INCLUDED
#define ONVIF_Receiver_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/ReceiverConfiguration.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Receiver
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Receiver
{
public:
	Receiver();

	Receiver(
		const std::string& token, 
		const ReceiverConfiguration& configuration);

	virtual ~Receiver();

	const ReceiverConfiguration& getConfiguration() const;

	const std::string& getToken() const;

	void setConfiguration(const ReceiverConfiguration& val);

	void setConfiguration(ReceiverConfiguration&& val);

	void setToken(const std::string& val);

	void setToken(std::string&& val);

private:
	//@ name=Token
	//@ order=0
	std::string _token;

	//@ name=Configuration
	//@ order=1
	ReceiverConfiguration _configuration;

};


inline const ReceiverConfiguration& Receiver::getConfiguration() const
{
	return _configuration;
}


inline const std::string& Receiver::getToken() const
{
	return _token;
}


inline void Receiver::setConfiguration(const ReceiverConfiguration& val)
{
	_configuration = val;
}


inline void Receiver::setConfiguration(ReceiverConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void Receiver::setToken(const std::string& val)
{
	_token = val;
}


inline void Receiver::setToken(std::string&& val)
{
	_token = std::move(val);
}


} // ONVIF


#endif // ONVIF_Receiver_INCLUDED
