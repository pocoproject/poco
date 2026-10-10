// 
// PTZMoveStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZMoveStatus_INCLUDED
#define ONVIF_PTZMoveStatus_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PTZMoveStatus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZMoveStatus
{
public:
	PTZMoveStatus();

	PTZMoveStatus(
		const Poco::Optional<std::string>& panTilt, 
		const Poco::Optional<std::string>& zoom);

	virtual ~PTZMoveStatus();

	const Poco::Optional<std::string>& getPanTilt() const;

	const Poco::Optional<std::string>& getZoom() const;

	void setPanTilt(const Poco::Optional<std::string>& val);

	void setPanTilt(Poco::Optional<std::string>&& val);

	void setZoom(const Poco::Optional<std::string>& val);

	void setZoom(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=PanTilt
	//@ order=0
	Poco::Optional<std::string> _panTilt;

	//@ mandatory=false
	//@ name=Zoom
	//@ order=1
	Poco::Optional<std::string> _zoom;

};


inline const Poco::Optional<std::string>& PTZMoveStatus::getPanTilt() const
{
	return _panTilt;
}


inline const Poco::Optional<std::string>& PTZMoveStatus::getZoom() const
{
	return _zoom;
}


inline void PTZMoveStatus::setPanTilt(const Poco::Optional<std::string>& val)
{
	_panTilt = val;
}


inline void PTZMoveStatus::setPanTilt(Poco::Optional<std::string>&& val)
{
	_panTilt = std::move(val);
}


inline void PTZMoveStatus::setZoom(const Poco::Optional<std::string>& val)
{
	_zoom = val;
}


inline void PTZMoveStatus::setZoom(Poco::Optional<std::string>&& val)
{
	_zoom = std::move(val);
}


} // ONVIF


#endif // ONVIF_PTZMoveStatus_INCLUDED
