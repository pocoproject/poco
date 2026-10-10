// 
// PTZStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZStatus_INCLUDED
#define ONVIF_PTZStatus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZMoveStatus;
class PTZVector;
} 


namespace ONVIF {


//@ name=PTZStatus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZStatus
{
public:
	PTZStatus();

	PTZStatus(
		const Poco::SharedPtr<PTZVector>& position, 
		const Poco::SharedPtr<PTZMoveStatus>& moveStatus, 
		const Poco::Optional<std::string>& error, 
		const Poco::DateTime& utcTime);

	virtual ~PTZStatus();

	const Poco::Optional<std::string>& getError() const;

	const Poco::SharedPtr<PTZMoveStatus>& getMoveStatus() const;

	const Poco::SharedPtr<PTZVector>& getPosition() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getUtcTime() const;

	void setError(const Poco::Optional<std::string>& val);

	void setError(Poco::Optional<std::string>&& val);

	void setMoveStatus(const Poco::SharedPtr<PTZMoveStatus>& val);

	void setMoveStatus(Poco::SharedPtr<PTZMoveStatus>&& val);

	void setPosition(const Poco::SharedPtr<PTZVector>& val);

	void setPosition(Poco::SharedPtr<PTZVector>&& val);

	//@ $val={xsdType=dateTime}
	void setUtcTime(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setUtcTime(Poco::DateTime&& val);

private:
	//@ mandatory=false
	//@ name=Position
	//@ order=0
	Poco::SharedPtr<PTZVector> _position;

	//@ mandatory=false
	//@ name=MoveStatus
	//@ order=1
	Poco::SharedPtr<PTZMoveStatus> _moveStatus;

	//@ mandatory=false
	//@ name=Error
	//@ order=2
	Poco::Optional<std::string> _error;

	//@ name=UtcTime
	//@ order=3
	//@ xsdType=dateTime
	Poco::DateTime _utcTime;

};


inline const Poco::Optional<std::string>& PTZStatus::getError() const
{
	return _error;
}


inline const Poco::SharedPtr<PTZMoveStatus>& PTZStatus::getMoveStatus() const
{
	return _moveStatus;
}


inline const Poco::SharedPtr<PTZVector>& PTZStatus::getPosition() const
{
	return _position;
}


inline const Poco::DateTime& PTZStatus::getUtcTime() const
{
	return _utcTime;
}


inline void PTZStatus::setError(const Poco::Optional<std::string>& val)
{
	_error = val;
}


inline void PTZStatus::setError(Poco::Optional<std::string>&& val)
{
	_error = std::move(val);
}


inline void PTZStatus::setMoveStatus(const Poco::SharedPtr<PTZMoveStatus>& val)
{
	_moveStatus = val;
}


inline void PTZStatus::setMoveStatus(Poco::SharedPtr<PTZMoveStatus>&& val)
{
	_moveStatus = std::move(val);
}


inline void PTZStatus::setPosition(const Poco::SharedPtr<PTZVector>& val)
{
	_position = val;
}


inline void PTZStatus::setPosition(Poco::SharedPtr<PTZVector>&& val)
{
	_position = std::move(val);
}


inline void PTZStatus::setUtcTime(const Poco::DateTime& val)
{
	_utcTime = val;
}


inline void PTZStatus::setUtcTime(Poco::DateTime&& val)
{
	_utcTime = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZMoveStatus.h"
#include "ONVIF/PTZVector.h"


#endif // ONVIF_PTZStatus_INCLUDED
