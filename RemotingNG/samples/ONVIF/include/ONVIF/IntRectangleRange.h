// 
// IntRectangleRange.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IntRectangleRange_INCLUDED
#define ONVIF_IntRectangleRange_INCLUDED


#include "ONVIF/IntRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IntRectangleRange
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IntRectangleRange
{
public:
	IntRectangleRange();

	IntRectangleRange(
		const IntRange& xRange, 
		const IntRange& yRange, 
		const IntRange& widthRange, 
		const IntRange& heightRange);

	virtual ~IntRectangleRange();

	const IntRange& getHeightRange() const;

	const IntRange& getWidthRange() const;

	const IntRange& getXRange() const;

	const IntRange& getYRange() const;

	void setHeightRange(const IntRange& val);

	void setHeightRange(IntRange&& val);

	void setWidthRange(const IntRange& val);

	void setWidthRange(IntRange&& val);

	void setXRange(const IntRange& val);

	void setXRange(IntRange&& val);

	void setYRange(const IntRange& val);

	void setYRange(IntRange&& val);

private:
	//@ name=XRange
	//@ order=0
	IntRange _xRange;

	//@ name=YRange
	//@ order=1
	IntRange _yRange;

	//@ name=WidthRange
	//@ order=2
	IntRange _widthRange;

	//@ name=HeightRange
	//@ order=3
	IntRange _heightRange;

};


inline const IntRange& IntRectangleRange::getHeightRange() const
{
	return _heightRange;
}


inline const IntRange& IntRectangleRange::getWidthRange() const
{
	return _widthRange;
}


inline const IntRange& IntRectangleRange::getXRange() const
{
	return _xRange;
}


inline const IntRange& IntRectangleRange::getYRange() const
{
	return _yRange;
}


inline void IntRectangleRange::setHeightRange(const IntRange& val)
{
	_heightRange = val;
}


inline void IntRectangleRange::setHeightRange(IntRange&& val)
{
	_heightRange = std::move(val);
}


inline void IntRectangleRange::setWidthRange(const IntRange& val)
{
	_widthRange = val;
}


inline void IntRectangleRange::setWidthRange(IntRange&& val)
{
	_widthRange = std::move(val);
}


inline void IntRectangleRange::setXRange(const IntRange& val)
{
	_xRange = val;
}


inline void IntRectangleRange::setXRange(IntRange&& val)
{
	_xRange = std::move(val);
}


inline void IntRectangleRange::setYRange(const IntRange& val)
{
	_yRange = val;
}


inline void IntRectangleRange::setYRange(IntRange&& val)
{
	_yRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_IntRectangleRange_INCLUDED
