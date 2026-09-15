// 
// PTZSpeed.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZSpeed_INCLUDED
#define ONVIF_PTZSpeed_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Vector1D;
class Vector2D;
} 


namespace ONVIF {


//@ name=PTZSpeed
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZSpeed
{
public:
	PTZSpeed();

	PTZSpeed(
		const Poco::SharedPtr<Vector2D>& panTilt, 
		const Poco::SharedPtr<Vector1D>& zoom);

	virtual ~PTZSpeed();

	const Poco::SharedPtr<Vector2D>& getPanTilt() const;

	const Poco::SharedPtr<Vector1D>& getZoom() const;

	void setPanTilt(const Poco::SharedPtr<Vector2D>& val);

	void setPanTilt(Poco::SharedPtr<Vector2D>&& val);

	void setZoom(const Poco::SharedPtr<Vector1D>& val);

	void setZoom(Poco::SharedPtr<Vector1D>&& val);

private:
	//@ mandatory=false
	//@ name=PanTilt
	//@ order=0
	Poco::SharedPtr<Vector2D> _panTilt;

	//@ mandatory=false
	//@ name=Zoom
	//@ order=1
	Poco::SharedPtr<Vector1D> _zoom;

};


inline const Poco::SharedPtr<Vector2D>& PTZSpeed::getPanTilt() const
{
	return _panTilt;
}


inline const Poco::SharedPtr<Vector1D>& PTZSpeed::getZoom() const
{
	return _zoom;
}


inline void PTZSpeed::setPanTilt(const Poco::SharedPtr<Vector2D>& val)
{
	_panTilt = val;
}


inline void PTZSpeed::setPanTilt(Poco::SharedPtr<Vector2D>&& val)
{
	_panTilt = std::move(val);
}


inline void PTZSpeed::setZoom(const Poco::SharedPtr<Vector1D>& val)
{
	_zoom = val;
}


inline void PTZSpeed::setZoom(Poco::SharedPtr<Vector1D>&& val)
{
	_zoom = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Vector1D.h"
#include "ONVIF/Vector2D.h"


#endif // ONVIF_PTZSpeed_INCLUDED
