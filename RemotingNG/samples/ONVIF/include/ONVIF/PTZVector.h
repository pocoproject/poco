// 
// PTZVector.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZVector_INCLUDED
#define ONVIF_PTZVector_INCLUDED


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


//@ name=PTZVector
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZVector
{
public:
	PTZVector();

	PTZVector(
		const Poco::SharedPtr<Vector2D>& panTilt, 
		const Poco::SharedPtr<Vector1D>& zoom);

	virtual ~PTZVector();

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


inline const Poco::SharedPtr<Vector2D>& PTZVector::getPanTilt() const
{
	return _panTilt;
}


inline const Poco::SharedPtr<Vector1D>& PTZVector::getZoom() const
{
	return _zoom;
}


inline void PTZVector::setPanTilt(const Poco::SharedPtr<Vector2D>& val)
{
	_panTilt = val;
}


inline void PTZVector::setPanTilt(Poco::SharedPtr<Vector2D>&& val)
{
	_panTilt = std::move(val);
}


inline void PTZVector::setZoom(const Poco::SharedPtr<Vector1D>& val)
{
	_zoom = val;
}


inline void PTZVector::setZoom(Poco::SharedPtr<Vector1D>&& val)
{
	_zoom = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Vector1D.h"
#include "ONVIF/Vector2D.h"


#endif // ONVIF_PTZVector_INCLUDED
