// 
// SetHostname.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SetHostname_INCLUDED
#define ONVIF_Device_SetHostname_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SetHostname"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SetHostname
{
public:
	SetHostname();

	SetHostname(const std::string& name);

	virtual ~SetHostname();

	const std::string& getName() const;

	void setName(const std::string& val);

	void setName(std::string&& val);

private:
	//@ name=Name
	//@ order=0
	std::string _name;

};


inline const std::string& SetHostname::getName() const
{
	return _name;
}


inline void SetHostname::setName(const std::string& val)
{
	_name = val;
}


inline void SetHostname::setName(std::string&& val)
{
	_name = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SetHostname_INCLUDED
