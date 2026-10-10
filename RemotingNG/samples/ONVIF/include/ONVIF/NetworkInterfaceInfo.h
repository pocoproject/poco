// 
// NetworkInterfaceInfo.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_NetworkInterfaceInfo_INCLUDED
#define ONVIF_NetworkInterfaceInfo_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=NetworkInterfaceInfo
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API NetworkInterfaceInfo
{
public:
	NetworkInterfaceInfo();

	NetworkInterfaceInfo(
		const Poco::Optional<std::string>& name, 
		const std::string& hwAddress, 
		Poco::Optional<int> mTU);

	virtual ~NetworkInterfaceInfo();

	const std::string& getHwAddress() const;

	Poco::Optional<int> getMTU() const;

	const Poco::Optional<std::string>& getName() const;

	void setHwAddress(const std::string& val);

	void setHwAddress(std::string&& val);

	void setMTU(Poco::Optional<int> val);

	void setName(const Poco::Optional<std::string>& val);

	void setName(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Name
	//@ order=0
	Poco::Optional<std::string> _name;

	//@ name=HwAddress
	//@ order=1
	std::string _hwAddress;

	//@ mandatory=false
	//@ name=MTU
	//@ order=2
	Poco::Optional<int> _mTU;

};


inline const std::string& NetworkInterfaceInfo::getHwAddress() const
{
	return _hwAddress;
}


inline Poco::Optional<int> NetworkInterfaceInfo::getMTU() const
{
	return _mTU;
}


inline const Poco::Optional<std::string>& NetworkInterfaceInfo::getName() const
{
	return _name;
}


inline void NetworkInterfaceInfo::setHwAddress(const std::string& val)
{
	_hwAddress = val;
}


inline void NetworkInterfaceInfo::setHwAddress(std::string&& val)
{
	_hwAddress = std::move(val);
}


inline void NetworkInterfaceInfo::setMTU(Poco::Optional<int> val)
{
	_mTU = val;
}


inline void NetworkInterfaceInfo::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void NetworkInterfaceInfo::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


} // ONVIF


#endif // ONVIF_NetworkInterfaceInfo_INCLUDED
