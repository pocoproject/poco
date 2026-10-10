// 
// Vector2D.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Vector2D_INCLUDED
#define ONVIF_Vector2D_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Vector2D
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Vector2D
{
public:
	Vector2D();

	Vector2D(
		float x, 
		float y, 
		const Poco::Optional<Poco::URI>& space);

	virtual ~Vector2D();

	const Poco::Optional<Poco::URI>& getSpace() const;

	float getX() const;

	float getY() const;

	void setSpace(const Poco::Optional<Poco::URI>& val);

	void setSpace(Poco::Optional<Poco::URI>&& val);

	void setX(float val);

	void setY(float val);

private:
	//@ name=x
	//@ order=0
	//@ type=attr
	float _x;

	//@ name=y
	//@ order=1
	//@ type=attr
	float _y;

	//@ mandatory=false
	//@ name=space
	//@ order=2
	//@ type=attr
	Poco::Optional<Poco::URI> _space;

};


inline const Poco::Optional<Poco::URI>& Vector2D::getSpace() const
{
	return _space;
}


inline float Vector2D::getX() const
{
	return _x;
}


inline float Vector2D::getY() const
{
	return _y;
}


inline void Vector2D::setSpace(const Poco::Optional<Poco::URI>& val)
{
	_space = val;
}


inline void Vector2D::setSpace(Poco::Optional<Poco::URI>&& val)
{
	_space = std::move(val);
}


inline void Vector2D::setX(float val)
{
	_x = val;
}


inline void Vector2D::setY(float val)
{
	_y = val;
}


} // ONVIF


#endif // ONVIF_Vector2D_INCLUDED
