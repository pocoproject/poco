// 
// IOCapabilitiesExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IOCapabilitiesExtension_INCLUDED
#define ONVIF_IOCapabilitiesExtension_INCLUDED


#include "ONVIF/IOCapabilitiesExtension2.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IOCapabilitiesExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IOCapabilitiesExtension
{
public:
	IOCapabilitiesExtension();

	IOCapabilitiesExtension(
		Poco::Optional<bool> auxiliary, 
		const std::vector<std::string>& auxiliaryCommands, 
		const IOCapabilitiesExtension2& extension);

	virtual ~IOCapabilitiesExtension();

	Poco::Optional<bool> getAuxiliary() const;

	const std::vector<std::string>& getAuxiliaryCommands() const;

	std::vector<std::string>& getAuxiliaryCommands();

	const IOCapabilitiesExtension2& getExtension() const;

	void setAuxiliary(Poco::Optional<bool> val);

	void setAuxiliaryCommands(const std::vector<std::string>& val);

	void setAuxiliaryCommands(std::vector<std::string>&& val);

	void setExtension(const IOCapabilitiesExtension2& val);

	void setExtension(IOCapabilitiesExtension2&& val);

private:
	//@ mandatory=false
	//@ name=Auxiliary
	//@ order=0
	Poco::Optional<bool> _auxiliary;

	//@ mandatory=false
	//@ name=AuxiliaryCommands
	//@ order=1
	std::vector<std::string> _auxiliaryCommands;

	//@ name=Extension
	//@ order=2
	IOCapabilitiesExtension2 _extension;

};


inline Poco::Optional<bool> IOCapabilitiesExtension::getAuxiliary() const
{
	return _auxiliary;
}


inline const std::vector<std::string>& IOCapabilitiesExtension::getAuxiliaryCommands() const
{
	return _auxiliaryCommands;
}


inline std::vector<std::string>& IOCapabilitiesExtension::getAuxiliaryCommands()
{
	return _auxiliaryCommands;
}


inline const IOCapabilitiesExtension2& IOCapabilitiesExtension::getExtension() const
{
	return _extension;
}


inline void IOCapabilitiesExtension::setAuxiliary(Poco::Optional<bool> val)
{
	_auxiliary = val;
}


inline void IOCapabilitiesExtension::setAuxiliaryCommands(const std::vector<std::string>& val)
{
	_auxiliaryCommands = val;
}


inline void IOCapabilitiesExtension::setAuxiliaryCommands(std::vector<std::string>&& val)
{
	_auxiliaryCommands = std::move(val);
}


inline void IOCapabilitiesExtension::setExtension(const IOCapabilitiesExtension2& val)
{
	_extension = val;
}


inline void IOCapabilitiesExtension::setExtension(IOCapabilitiesExtension2&& val)
{
	_extension = std::move(val);
}


} // ONVIF


#endif // ONVIF_IOCapabilitiesExtension_INCLUDED
