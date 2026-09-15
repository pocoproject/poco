// 
// Dot11Status.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Dot11Status_INCLUDED
#define ONVIF_Dot11Status_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Dot11Status
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Dot11Status
{
public:
	Dot11Status();

	Dot11Status(
		const std::string& sSID, 
		const Poco::Optional<std::string>& bSSID, 
		const Poco::Optional<std::string>& pairCipher, 
		const Poco::Optional<std::string>& groupCipher, 
		const Poco::Optional<std::string>& signalStrength, 
		const std::string& activeConfigAlias);

	virtual ~Dot11Status();

	const std::string& getActiveConfigAlias() const;

	const Poco::Optional<std::string>& getBSSID() const;

	const Poco::Optional<std::string>& getGroupCipher() const;

	const Poco::Optional<std::string>& getPairCipher() const;

	const std::string& getSSID() const;

	const Poco::Optional<std::string>& getSignalStrength() const;

	void setActiveConfigAlias(const std::string& val);

	void setActiveConfigAlias(std::string&& val);

	void setBSSID(const Poco::Optional<std::string>& val);

	void setBSSID(Poco::Optional<std::string>&& val);

	void setGroupCipher(const Poco::Optional<std::string>& val);

	void setGroupCipher(Poco::Optional<std::string>&& val);

	void setPairCipher(const Poco::Optional<std::string>& val);

	void setPairCipher(Poco::Optional<std::string>&& val);

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
	//@ name=PairCipher
	//@ order=2
	Poco::Optional<std::string> _pairCipher;

	//@ mandatory=false
	//@ name=GroupCipher
	//@ order=3
	Poco::Optional<std::string> _groupCipher;

	//@ mandatory=false
	//@ name=SignalStrength
	//@ order=4
	Poco::Optional<std::string> _signalStrength;

	//@ name=ActiveConfigAlias
	//@ order=5
	std::string _activeConfigAlias;

};


inline const std::string& Dot11Status::getActiveConfigAlias() const
{
	return _activeConfigAlias;
}


inline const Poco::Optional<std::string>& Dot11Status::getBSSID() const
{
	return _bSSID;
}


inline const Poco::Optional<std::string>& Dot11Status::getGroupCipher() const
{
	return _groupCipher;
}


inline const Poco::Optional<std::string>& Dot11Status::getPairCipher() const
{
	return _pairCipher;
}


inline const std::string& Dot11Status::getSSID() const
{
	return _sSID;
}


inline const Poco::Optional<std::string>& Dot11Status::getSignalStrength() const
{
	return _signalStrength;
}


inline void Dot11Status::setActiveConfigAlias(const std::string& val)
{
	_activeConfigAlias = val;
}


inline void Dot11Status::setActiveConfigAlias(std::string&& val)
{
	_activeConfigAlias = std::move(val);
}


inline void Dot11Status::setBSSID(const Poco::Optional<std::string>& val)
{
	_bSSID = val;
}


inline void Dot11Status::setBSSID(Poco::Optional<std::string>&& val)
{
	_bSSID = std::move(val);
}


inline void Dot11Status::setGroupCipher(const Poco::Optional<std::string>& val)
{
	_groupCipher = val;
}


inline void Dot11Status::setGroupCipher(Poco::Optional<std::string>&& val)
{
	_groupCipher = std::move(val);
}


inline void Dot11Status::setPairCipher(const Poco::Optional<std::string>& val)
{
	_pairCipher = val;
}


inline void Dot11Status::setPairCipher(Poco::Optional<std::string>&& val)
{
	_pairCipher = std::move(val);
}


inline void Dot11Status::setSSID(const std::string& val)
{
	_sSID = val;
}


inline void Dot11Status::setSSID(std::string&& val)
{
	_sSID = std::move(val);
}


inline void Dot11Status::setSignalStrength(const Poco::Optional<std::string>& val)
{
	_signalStrength = val;
}


inline void Dot11Status::setSignalStrength(Poco::Optional<std::string>&& val)
{
	_signalStrength = std::move(val);
}


} // ONVIF


#endif // ONVIF_Dot11Status_INCLUDED
