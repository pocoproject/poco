// 
// MulticastConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MulticastConfiguration_INCLUDED
#define ONVIF_MulticastConfiguration_INCLUDED


#include "ONVIF/IPAddress.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=MulticastConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MulticastConfiguration
{
public:
	MulticastConfiguration();

	MulticastConfiguration(
		const IPAddress& address, 
		int port, 
		int tTL, 
		bool autoStart);

	virtual ~MulticastConfiguration();

	const IPAddress& getAddress() const;

	bool getAutoStart() const;

	int getPort() const;

	int getTTL() const;

	void setAddress(const IPAddress& val);

	void setAddress(IPAddress&& val);

	void setAutoStart(bool val);

	void setPort(int val);

	void setTTL(int val);

private:
	//@ name=Address
	//@ order=0
	IPAddress _address;

	//@ name=Port
	//@ order=1
	int _port;

	//@ name=TTL
	//@ order=2
	int _tTL;

	//@ name=AutoStart
	//@ order=3
	bool _autoStart;

};


inline const IPAddress& MulticastConfiguration::getAddress() const
{
	return _address;
}


inline bool MulticastConfiguration::getAutoStart() const
{
	return _autoStart;
}


inline int MulticastConfiguration::getPort() const
{
	return _port;
}


inline int MulticastConfiguration::getTTL() const
{
	return _tTL;
}


inline void MulticastConfiguration::setAddress(const IPAddress& val)
{
	_address = val;
}


inline void MulticastConfiguration::setAddress(IPAddress&& val)
{
	_address = std::move(val);
}


inline void MulticastConfiguration::setAutoStart(bool val)
{
	_autoStart = val;
}


inline void MulticastConfiguration::setPort(int val)
{
	_port = val;
}


inline void MulticastConfiguration::setTTL(int val)
{
	_tTL = val;
}


} // ONVIF


#endif // ONVIF_MulticastConfiguration_INCLUDED
