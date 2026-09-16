// 
// MiscCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_MiscCapabilities_INCLUDED
#define ONVIF_Device_MiscCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name=MiscCapabilities
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API MiscCapabilities
{
public:
	MiscCapabilities();

	MiscCapabilities(const std::vector<std::vector<std::string>>& auxiliaryCommands);

	virtual ~MiscCapabilities();

	const std::vector<std::vector<std::string>>& getAuxiliaryCommands() const;

	std::vector<std::vector<std::string>>& getAuxiliaryCommands();

	void setAuxiliaryCommands(const std::vector<std::vector<std::string>>& val);

	void setAuxiliaryCommands(std::vector<std::vector<std::string>>&& val);

private:
	//@ mandatory=false
	//@ name=AuxiliaryCommands
	//@ order=0
	//@ type=attr
	std::vector<std::vector<std::string>> _auxiliaryCommands;

};


inline const std::vector<std::vector<std::string>>& MiscCapabilities::getAuxiliaryCommands() const
{
	return _auxiliaryCommands;
}


inline std::vector<std::vector<std::string>>& MiscCapabilities::getAuxiliaryCommands()
{
	return _auxiliaryCommands;
}


inline void MiscCapabilities::setAuxiliaryCommands(const std::vector<std::vector<std::string>>& val)
{
	_auxiliaryCommands = val;
}


inline void MiscCapabilities::setAuxiliaryCommands(std::vector<std::vector<std::string>>&& val)
{
	_auxiliaryCommands = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_MiscCapabilities_INCLUDED
