// 
// RelayOutput.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_RelayOutput_INCLUDED
#define ONVIF_RelayOutput_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/RelayOutputSettings.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=RelayOutput
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API RelayOutput: public DeviceEntity
{
public:
	RelayOutput();

	RelayOutput(
		const std::string& token, 
		const RelayOutputSettings& properties);

	virtual ~RelayOutput();

	const RelayOutputSettings& getProperties() const;

	void setProperties(const RelayOutputSettings& val);

	void setProperties(RelayOutputSettings&& val);

private:
	//@ name=Properties
	//@ order=0
	RelayOutputSettings _properties;

};


inline const RelayOutputSettings& RelayOutput::getProperties() const
{
	return _properties;
}


inline void RelayOutput::setProperties(const RelayOutputSettings& val)
{
	_properties = val;
}


inline void RelayOutput::setProperties(RelayOutputSettings&& val)
{
	_properties = std::move(val);
}


} // ONVIF


#endif // ONVIF_RelayOutput_INCLUDED
