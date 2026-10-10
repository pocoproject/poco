// 
// NetworkInterfaceConnectionSetting.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkInterfaceConnectionSetting_INCLUDED
#define ONVIF_NetworkInterfaceConnectionSetting_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=NetworkInterfaceConnectionSetting
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkInterfaceConnectionSetting
{
public:
	NetworkInterfaceConnectionSetting();

	NetworkInterfaceConnectionSetting(
		bool autoNegotiation, 
		int speed, 
		const std::string& duplex);

	virtual ~NetworkInterfaceConnectionSetting();

	bool getAutoNegotiation() const;

	const std::string& getDuplex() const;

	int getSpeed() const;

	void setAutoNegotiation(bool val);

	void setDuplex(const std::string& val);

	void setDuplex(std::string&& val);

	void setSpeed(int val);

private:
	//@ name=AutoNegotiation
	//@ order=0
	bool _autoNegotiation;

	//@ name=Speed
	//@ order=1
	int _speed;

	//@ name=Duplex
	//@ order=2
	std::string _duplex;

};


inline bool NetworkInterfaceConnectionSetting::getAutoNegotiation() const
{
	return _autoNegotiation;
}


inline const std::string& NetworkInterfaceConnectionSetting::getDuplex() const
{
	return _duplex;
}


inline int NetworkInterfaceConnectionSetting::getSpeed() const
{
	return _speed;
}


inline void NetworkInterfaceConnectionSetting::setAutoNegotiation(bool val)
{
	_autoNegotiation = val;
}


inline void NetworkInterfaceConnectionSetting::setDuplex(const std::string& val)
{
	_duplex = val;
}


inline void NetworkInterfaceConnectionSetting::setDuplex(std::string&& val)
{
	_duplex = std::move(val);
}


inline void NetworkInterfaceConnectionSetting::setSpeed(int val)
{
	_speed = val;
}


} // ONVIF


#endif // ONVIF_NetworkInterfaceConnectionSetting_INCLUDED
