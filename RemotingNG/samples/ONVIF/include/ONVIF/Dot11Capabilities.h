// 
// Dot11Capabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Dot11Capabilities_INCLUDED
#define ONVIF_Dot11Capabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Dot11Capabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Dot11Capabilities
{
public:
	Dot11Capabilities();

	Dot11Capabilities(
		bool tKIP, 
		bool scanAvailableNetworks, 
		bool multipleConfiguration, 
		bool adHocStationMode, 
		bool wEP);

	virtual ~Dot11Capabilities();

	bool getAdHocStationMode() const;

	bool getMultipleConfiguration() const;

	bool getScanAvailableNetworks() const;

	bool getTKIP() const;

	bool getWEP() const;

	void setAdHocStationMode(bool val);

	void setMultipleConfiguration(bool val);

	void setScanAvailableNetworks(bool val);

	void setTKIP(bool val);

	void setWEP(bool val);

private:
	//@ name=TKIP
	//@ order=0
	bool _tKIP;

	//@ name=ScanAvailableNetworks
	//@ order=1
	bool _scanAvailableNetworks;

	//@ name=MultipleConfiguration
	//@ order=2
	bool _multipleConfiguration;

	//@ name=AdHocStationMode
	//@ order=3
	bool _adHocStationMode;

	//@ name=WEP
	//@ order=4
	bool _wEP;

};


inline bool Dot11Capabilities::getAdHocStationMode() const
{
	return _adHocStationMode;
}


inline bool Dot11Capabilities::getMultipleConfiguration() const
{
	return _multipleConfiguration;
}


inline bool Dot11Capabilities::getScanAvailableNetworks() const
{
	return _scanAvailableNetworks;
}


inline bool Dot11Capabilities::getTKIP() const
{
	return _tKIP;
}


inline bool Dot11Capabilities::getWEP() const
{
	return _wEP;
}


inline void Dot11Capabilities::setAdHocStationMode(bool val)
{
	_adHocStationMode = val;
}


inline void Dot11Capabilities::setMultipleConfiguration(bool val)
{
	_multipleConfiguration = val;
}


inline void Dot11Capabilities::setScanAvailableNetworks(bool val)
{
	_scanAvailableNetworks = val;
}


inline void Dot11Capabilities::setTKIP(bool val)
{
	_tKIP = val;
}


inline void Dot11Capabilities::setWEP(bool val)
{
	_wEP = val;
}


} // ONVIF


#endif // ONVIF_Dot11Capabilities_INCLUDED
