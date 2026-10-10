// 
// FocusStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusStatus_INCLUDED
#define ONVIF_FocusStatus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FocusStatus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusStatus
{
public:
	FocusStatus();

	FocusStatus(
		float position, 
		const std::string& moveStatus, 
		const std::string& error);

	virtual ~FocusStatus();

	const std::string& getError() const;

	const std::string& getMoveStatus() const;

	float getPosition() const;

	void setError(const std::string& val);

	void setError(std::string&& val);

	void setMoveStatus(const std::string& val);

	void setMoveStatus(std::string&& val);

	void setPosition(float val);

private:
	//@ name=Position
	//@ order=0
	float _position;

	//@ name=MoveStatus
	//@ order=1
	std::string _moveStatus;

	//@ name=Error
	//@ order=2
	std::string _error;

};


inline const std::string& FocusStatus::getError() const
{
	return _error;
}


inline const std::string& FocusStatus::getMoveStatus() const
{
	return _moveStatus;
}


inline float FocusStatus::getPosition() const
{
	return _position;
}


inline void FocusStatus::setError(const std::string& val)
{
	_error = val;
}


inline void FocusStatus::setError(std::string&& val)
{
	_error = std::move(val);
}


inline void FocusStatus::setMoveStatus(const std::string& val)
{
	_moveStatus = val;
}


inline void FocusStatus::setMoveStatus(std::string&& val)
{
	_moveStatus = std::move(val);
}


inline void FocusStatus::setPosition(float val)
{
	_position = val;
}


} // ONVIF


#endif // ONVIF_FocusStatus_INCLUDED
