// 
// Dot11AvailableNetworks.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Dot11AvailableNetworks_INCLUDED
#define ONVIF_Dot11AvailableNetworks_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Dot11AvailableNetworksExtension;
} 


namespace ONVIF {


//@ name=Dot11AvailableNetworks
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Dot11AvailableNetworks
{
public:
	Dot11AvailableNetworks();

	Dot11AvailableNetworks(
		const std::string& sSID, 
		const Poco::Optional<std::string>& bSSID, 
		const std::vector<std::string>& authAndMangementSuite, 
		const std::vector<std::string>& pairCipher, 
		const std::vector<std::string>& groupCipher, 
		const Poco::Optional<std::string>& signalStrength, 
		const Poco::SharedPtr<Dot11AvailableNetworksExtension>& extension);

	virtual ~Dot11AvailableNetworks();

	const std::vector<std::string>& getAuthAndMangementSuite() const;

	std::vector<std::string>& getAuthAndMangementSuite();

	const Poco::Optional<std::string>& getBSSID() const;

	const Poco::SharedPtr<Dot11AvailableNetworksExtension>& getExtension() const;

	const std::vector<std::string>& getGroupCipher() const;

	std::vector<std::string>& getGroupCipher();

	const std::vector<std::string>& getPairCipher() const;

	std::vector<std::string>& getPairCipher();

	const std::string& getSSID() const;

	const Poco::Optional<std::string>& getSignalStrength() const;

	void setAuthAndMangementSuite(const std::vector<std::string>& val);

	void setAuthAndMangementSuite(std::vector<std::string>&& val);

	void setBSSID(const Poco::Optional<std::string>& val);

	void setBSSID(Poco::Optional<std::string>&& val);

	void setExtension(const Poco::SharedPtr<Dot11AvailableNetworksExtension>& val);

	void setExtension(Poco::SharedPtr<Dot11AvailableNetworksExtension>&& val);

	void setGroupCipher(const std::vector<std::string>& val);

	void setGroupCipher(std::vector<std::string>&& val);

	void setPairCipher(const std::vector<std::string>& val);

	void setPairCipher(std::vector<std::string>&& val);

	void setSSID(const std::string& val);

	void setSSID(std::string&& val);

	void setSignalStrength(const Poco::Optional<std::string>& val);

	void setSignalStrength(Poco::Optional<std::string>&& val);

private:
	//@ name=SSID
	//@ order=0
	std::string _sSID;

	//@ mandatory=false
	//@ name=BSSID
	//@ order=1
	Poco::Optional<std::string> _bSSID;

	//@ mandatory=false
	//@ name=AuthAndMangementSuite
	//@ order=2
	std::vector<std::string> _authAndMangementSuite;

	//@ mandatory=false
	//@ name=PairCipher
	//@ order=3
	std::vector<std::string> _pairCipher;

	//@ mandatory=false
	//@ name=GroupCipher
	//@ order=4
	std::vector<std::string> _groupCipher;

	//@ mandatory=false
	//@ name=SignalStrength
	//@ order=5
	Poco::Optional<std::string> _signalStrength;

	//@ mandatory=false
	//@ name=Extension
	//@ order=6
	Poco::SharedPtr<Dot11AvailableNetworksExtension> _extension;

};


inline const std::vector<std::string>& Dot11AvailableNetworks::getAuthAndMangementSuite() const
{
	return _authAndMangementSuite;
}


inline std::vector<std::string>& Dot11AvailableNetworks::getAuthAndMangementSuite()
{
	return _authAndMangementSuite;
}


inline const Poco::Optional<std::string>& Dot11AvailableNetworks::getBSSID() const
{
	return _bSSID;
}


inline const Poco::SharedPtr<Dot11AvailableNetworksExtension>& Dot11AvailableNetworks::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& Dot11AvailableNetworks::getGroupCipher() const
{
	return _groupCipher;
}


inline std::vector<std::string>& Dot11AvailableNetworks::getGroupCipher()
{
	return _groupCipher;
}


inline const std::vector<std::string>& Dot11AvailableNetworks::getPairCipher() const
{
	return _pairCipher;
}


inline std::vector<std::string>& Dot11AvailableNetworks::getPairCipher()
{
	return _pairCipher;
}


inline const std::string& Dot11AvailableNetworks::getSSID() const
{
	return _sSID;
}


inline const Poco::Optional<std::string>& Dot11AvailableNetworks::getSignalStrength() const
{
	return _signalStrength;
}


inline void Dot11AvailableNetworks::setAuthAndMangementSuite(const std::vector<std::string>& val)
{
	_authAndMangementSuite = val;
}


inline void Dot11AvailableNetworks::setAuthAndMangementSuite(std::vector<std::string>&& val)
{
	_authAndMangementSuite = std::move(val);
}


inline void Dot11AvailableNetworks::setBSSID(const Poco::Optional<std::string>& val)
{
	_bSSID = val;
}


inline void Dot11AvailableNetworks::setBSSID(Poco::Optional<std::string>&& val)
{
	_bSSID = std::move(val);
}


inline void Dot11AvailableNetworks::setExtension(const Poco::SharedPtr<Dot11AvailableNetworksExtension>& val)
{
	_extension = val;
}


inline void Dot11AvailableNetworks::setExtension(Poco::SharedPtr<Dot11AvailableNetworksExtension>&& val)
{
	_extension = std::move(val);
}


inline void Dot11AvailableNetworks::setGroupCipher(const std::vector<std::string>& val)
{
	_groupCipher = val;
}


inline void Dot11AvailableNetworks::setGroupCipher(std::vector<std::string>&& val)
{
	_groupCipher = std::move(val);
}


inline void Dot11AvailableNetworks::setPairCipher(const std::vector<std::string>& val)
{
	_pairCipher = val;
}


inline void Dot11AvailableNetworks::setPairCipher(std::vector<std::string>&& val)
{
	_pairCipher = std::move(val);
}


inline void Dot11AvailableNetworks::setSSID(const std::string& val)
{
	_sSID = val;
}


inline void Dot11AvailableNetworks::setSSID(std::string&& val)
{
	_sSID = std::move(val);
}


inline void Dot11AvailableNetworks::setSignalStrength(const Poco::Optional<std::string>& val)
{
	_signalStrength = val;
}


inline void Dot11AvailableNetworks::setSignalStrength(Poco::Optional<std::string>&& val)
{
	_signalStrength = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Dot11AvailableNetworksExtension.h"


#endif // ONVIF_Dot11AvailableNetworks_INCLUDED
