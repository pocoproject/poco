// 
// IntRectangle.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IntRectangle_INCLUDED
#define ONVIF_IntRectangle_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IntRectangle
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IntRectangle
{
public:
	IntRectangle();

	IntRectangle(
		int x, 
		int y, 
		int width, 
		int height);

	virtual ~IntRectangle();

	int getHeight() const;

	int getWidth() const;

	int getX() const;

	int getY() const;

	void setHeight(int val);

	void setWidth(int val);

	void setX(int val);

	void setY(int val);

private:
	//@ name=x
	//@ order=0
	//@ type=attr
	int _x;

	//@ name=y
	//@ order=1
	//@ type=attr
	int _y;

	//@ name=width
	//@ order=2
	//@ type=attr
	int _width;

	//@ name=height
	//@ order=3
	//@ type=attr
	int _height;

};


inline int IntRectangle::getHeight() const
{
	return _height;
}


inline int IntRectangle::getWidth() const
{
	return _width;
}


inline int IntRectangle::getX() const
{
	return _x;
}


inline int IntRectangle::getY() const
{
	return _y;
}


inline void IntRectangle::setHeight(int val)
{
	_height = val;
}


inline void IntRectangle::setWidth(int val)
{
	_width = val;
}


inline void IntRectangle::setX(int val)
{
	_x = val;
}


inline void IntRectangle::setY(int val)
{
	_y = val;
}


} // ONVIF


#endif // ONVIF_IntRectangle_INCLUDED
