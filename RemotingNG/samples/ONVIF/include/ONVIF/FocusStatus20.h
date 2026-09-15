// 
// FocusStatus20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FocusStatus20_INCLUDED
#define ONVIF_FocusStatus20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FocusStatus20Extension;
} 


namespace ONVIF {


//@ name=FocusStatus20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FocusStatus20
{
public:
	FocusStatus20();

	FocusStatus20(
		float position, 
		const std::string& moveStatus, 
		const Poco::Optional<std::string>& error, 
		const Poco::SharedPtr<FocusStatus20Extension>& extension);

	virtual ~FocusStatus20();

	const Poco::Optional<std::string>& getError() const;

	const Poco::SharedPtr<FocusStatus20Extension>& getExtension() const;

	const std::string& getMoveStatus() const;

	float getPosition() const;

	void setError(const Poco::Optional<std::string>& val);

	void setError(Poco::Optional<std::string>&& val);

	void setExtension(const Poco::SharedPtr<FocusStatus20Extension>& val);

	void setExtension(Poco::SharedPtr<FocusStatus20Extension>&& val);

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

	//@ mandatory=false
	//@ name=Error
	//@ order=2
	Poco::Optional<std::string> _error;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<FocusStatus20Extension> _extension;

};


inline const Poco::Optional<std::string>& FocusStatus20::getError() const
{
	return _error;
}


inline const Poco::SharedPtr<FocusStatus20Extension>& FocusStatus20::getExtension() const
{
	return _extension;
}


inline const std::string& FocusStatus20::getMoveStatus() const
{
	return _moveStatus;
}


inline float FocusStatus20::getPosition() const
{
	return _position;
}


inline void FocusStatus20::setError(const Poco::Optional<std::string>& val)
{
	_error = val;
}


inline void FocusStatus20::setError(Poco::Optional<std::string>&& val)
{
	_error = std::move(val);
}


inline void FocusStatus20::setExtension(const Poco::SharedPtr<FocusStatus20Extension>& val)
{
	_extension = val;
}


inline void FocusStatus20::setExtension(Poco::SharedPtr<FocusStatus20Extension>&& val)
{
	_extension = std::move(val);
}


inline void FocusStatus20::setMoveStatus(const std::string& val)
{
	_moveStatus = val;
}


inline void FocusStatus20::setMoveStatus(std::string&& val)
{
	_moveStatus = std::move(val);
}


inline void FocusStatus20::setPosition(float val)
{
	_position = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FocusStatus20Extension.h"


#endif // ONVIF_FocusStatus20_INCLUDED
