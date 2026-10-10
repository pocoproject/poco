// 
// SendAuxiliaryCommand.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Device_SendAuxiliaryCommand_INCLUDED
#define ONVIF_Device_SendAuxiliaryCommand_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Device {


//@ name="#SendAuxiliaryCommand"
//@ namespace="http://www.onvif.org/ver10/device/wsdl"
//@ serialize
class ONVIF_API SendAuxiliaryCommand
{
public:
	SendAuxiliaryCommand();

	SendAuxiliaryCommand(const std::string& auxiliaryCommand);

	virtual ~SendAuxiliaryCommand();

	const std::string& getAuxiliaryCommand() const;

	void setAuxiliaryCommand(const std::string& val);

	void setAuxiliaryCommand(std::string&& val);

private:
	//@ name=AuxiliaryCommand
	//@ order=0
	std::string _auxiliaryCommand;

};


inline const std::string& SendAuxiliaryCommand::getAuxiliaryCommand() const
{
	return _auxiliaryCommand;
}


inline void SendAuxiliaryCommand::setAuxiliaryCommand(const std::string& val)
{
	_auxiliaryCommand = val;
}


inline void SendAuxiliaryCommand::setAuxiliaryCommand(std::string&& val)
{
	_auxiliaryCommand = std::move(val);
}


} } // ONVIF::Device


#endif // ONVIF_Device_SendAuxiliaryCommand_INCLUDED
