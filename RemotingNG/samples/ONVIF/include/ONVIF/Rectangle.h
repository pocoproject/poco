// 
// Rectangle.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Rectangle_INCLUDED
#define ONVIF_Rectangle_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Rectangle
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Rectangle
{
public:
	Rectangle();

	Rectangle(
		Poco::Optional<float> bottom, 
		Poco::Optional<float> top, 
		Poco::Optional<float> right, 
		Poco::Optional<float> left);

	virtual ~Rectangle();

	Poco::Optional<float> getBottom() const;

	Poco::Optional<float> getLeft() const;

	Poco::Optional<float> getRight() const;

	Poco::Optional<float> getTop() const;

	void setBottom(Poco::Optional<float> val);

	void setLeft(Poco::Optional<float> val);

	void setRight(Poco::Optional<float> val);

	void setTop(Poco::Optional<float> val);

private:
	//@ mandatory=false
	//@ name=bottom
	//@ order=0
	//@ type=attr
	Poco::Optional<float> _bottom;

	//@ mandatory=false
	//@ name=top
	//@ order=1
	//@ type=attr
	Poco::Optional<float> _top;

	//@ mandatory=false
	//@ name=right
	//@ order=2
	//@ type=attr
	Poco::Optional<float> _right;

	//@ mandatory=false
	//@ name=left
	//@ order=3
	//@ type=attr
	Poco::Optional<float> _left;

};


inline Poco::Optional<float> Rectangle::getBottom() const
{
	return _bottom;
}


inline Poco::Optional<float> Rectangle::getLeft() const
{
	return _left;
}


inline Poco::Optional<float> Rectangle::getRight() const
{
	return _right;
}


inline Poco::Optional<float> Rectangle::getTop() const
{
	return _top;
}


inline void Rectangle::setBottom(Poco::Optional<float> val)
{
	_bottom = val;
}


inline void Rectangle::setLeft(Poco::Optional<float> val)
{
	_left = val;
}


inline void Rectangle::setRight(Poco::Optional<float> val)
{
	_right = val;
}


inline void Rectangle::setTop(Poco::Optional<float> val)
{
	_top = val;
}


} // ONVIF


#endif // ONVIF_Rectangle_INCLUDED
