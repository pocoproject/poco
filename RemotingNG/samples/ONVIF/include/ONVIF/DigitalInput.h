// 
// DigitalInput.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DigitalInput_INCLUDED
#define ONVIF_DigitalInput_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DigitalInput
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DigitalInput: public DeviceEntity
{
public:
	DigitalInput();

	DigitalInput(
		const std::string& token, 
		const Poco::Optional<std::string>& idleState);

	virtual ~DigitalInput();

	const Poco::Optional<std::string>& getIdleState() const;

	void setIdleState(const Poco::Optional<std::string>& val);

	void setIdleState(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=IdleState
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _idleState;

};


inline const Poco::Optional<std::string>& DigitalInput::getIdleState() const
{
	return _idleState;
}


inline void DigitalInput::setIdleState(const Poco::Optional<std::string>& val)
{
	_idleState = val;
}


inline void DigitalInput::setIdleState(Poco::Optional<std::string>&& val)
{
	_idleState = std::move(val);
}


} // ONVIF


#endif // ONVIF_DigitalInput_INCLUDED
