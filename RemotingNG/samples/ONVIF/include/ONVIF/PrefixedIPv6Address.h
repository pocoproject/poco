// 
// PrefixedIPv6Address.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PrefixedIPv6Address_INCLUDED
#define ONVIF_PrefixedIPv6Address_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PrefixedIPv6Address
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PrefixedIPv6Address
{
public:
	PrefixedIPv6Address();

	PrefixedIPv6Address(
		const std::string& address, 
		int prefixLength);

	virtual ~PrefixedIPv6Address();

	const std::string& getAddress() const;

	int getPrefixLength() const;

	void setAddress(const std::string& val);

	void setAddress(std::string&& val);

	void setPrefixLength(int val);

private:
	//@ name=Address
	//@ order=0
	std::string _address;

	//@ name=PrefixLength
	//@ order=1
	int _prefixLength;

};


inline const std::string& PrefixedIPv6Address::getAddress() const
{
	return _address;
}


inline int PrefixedIPv6Address::getPrefixLength() const
{
	return _prefixLength;
}


inline void PrefixedIPv6Address::setAddress(const std::string& val)
{
	_address = val;
}


inline void PrefixedIPv6Address::setAddress(std::string&& val)
{
	_address = std::move(val);
}


inline void PrefixedIPv6Address::setPrefixLength(int val)
{
	_prefixLength = val;
}


} // ONVIF


#endif // ONVIF_PrefixedIPv6Address_INCLUDED
