// 
// GetCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_GetCapabilities_INCLUDED
#define ONVIF_Device_GetCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#GetCapabilities"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API GetCapabilities
{
public:
	GetCapabilities();

	GetCapabilities(const std::vector<std::string>& category);

	virtual ~GetCapabilities();

	const std::vector<std::string>& getCategory() const;

	std::vector<std::string>& getCategory();

	void setCategory(const std::vector<std::string>& val);

	void setCategory(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Category
	//@ order=0
	std::vector<std::string> _category;

};


inline const std::vector<std::string>& GetCapabilities::getCategory() const
{
	return _category;
}


inline std::vector<std::string>& GetCapabilities::getCategory()
{
	return _category;
}


inline void GetCapabilities::setCategory(const std::vector<std::string>& val)
{
	_category = val;
}


inline void GetCapabilities::setCategory(std::vector<std::string>&& val)
{
	_category = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_GetCapabilities_INCLUDED
