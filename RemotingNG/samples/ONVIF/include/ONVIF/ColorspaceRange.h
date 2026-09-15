// 
// ColorspaceRange.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ColorspaceRange_INCLUDED
#define ONVIF_ColorspaceRange_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ColorspaceRange
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ColorspaceRange
{
public:
	ColorspaceRange();

	ColorspaceRange(
		const FloatRange& x, 
		const FloatRange& y, 
		const FloatRange& z, 
		const Poco::URI& colorspace);

	virtual ~ColorspaceRange();

	const Poco::URI& getColorspace() const;

	const FloatRange& getX() const;

	const FloatRange& getY() const;

	const FloatRange& getZ() const;

	void setColorspace(const Poco::URI& val);

	void setColorspace(Poco::URI&& val);

	void setX(const FloatRange& val);

	void setX(FloatRange&& val);

	void setY(const FloatRange& val);

	void setY(FloatRange&& val);

	void setZ(const FloatRange& val);

	void setZ(FloatRange&& val);

private:
	//@ name=X
	//@ order=0
	FloatRange _x;

	//@ name=Y
	//@ order=1
	FloatRange _y;

	//@ name=Z
	//@ order=2
	FloatRange _z;

	//@ name=Colorspace
	//@ order=3
	Poco::URI _colorspace;

};


inline const Poco::URI& ColorspaceRange::getColorspace() const
{
	return _colorspace;
}


inline const FloatRange& ColorspaceRange::getX() const
{
	return _x;
}


inline const FloatRange& ColorspaceRange::getY() const
{
	return _y;
}


inline const FloatRange& ColorspaceRange::getZ() const
{
	return _z;
}


inline void ColorspaceRange::setColorspace(const Poco::URI& val)
{
	_colorspace = val;
}


inline void ColorspaceRange::setColorspace(Poco::URI&& val)
{
	_colorspace = std::move(val);
}


inline void ColorspaceRange::setX(const FloatRange& val)
{
	_x = val;
}


inline void ColorspaceRange::setX(FloatRange&& val)
{
	_x = std::move(val);
}


inline void ColorspaceRange::setY(const FloatRange& val)
{
	_y = val;
}


inline void ColorspaceRange::setY(FloatRange&& val)
{
	_y = std::move(val);
}


inline void ColorspaceRange::setZ(const FloatRange& val)
{
	_z = val;
}


inline void ColorspaceRange::setZ(FloatRange&& val)
{
	_z = std::move(val);
}


} // ONVIF


#endif // ONVIF_ColorspaceRange_INCLUDED
