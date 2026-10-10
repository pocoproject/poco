// 
// Vector1D.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Vector1D_INCLUDED
#define ONVIF_Vector1D_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Vector1D
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Vector1D
{
public:
	Vector1D();

	Vector1D(
		float x, 
		const Poco::Optional<Poco::URI>& space);

	virtual ~Vector1D();

	const Poco::Optional<Poco::URI>& getSpace() const;

	float getX() const;

	void setSpace(const Poco::Optional<Poco::URI>& val);

	void setSpace(Poco::Optional<Poco::URI>&& val);

	void setX(float val);

private:
	//@ name=x
	//@ order=0
	//@ type=attr
	float _x;

	//@ mandatory=false
	//@ name=space
	//@ order=1
	//@ type=attr
	Poco::Optional<Poco::URI> _space;

};


inline const Poco::Optional<Poco::URI>& Vector1D::getSpace() const
{
	return _space;
}


inline float Vector1D::getX() const
{
	return _x;
}


inline void Vector1D::setSpace(const Poco::Optional<Poco::URI>& val)
{
	_space = val;
}


inline void Vector1D::setSpace(Poco::Optional<Poco::URI>&& val)
{
	_space = std::move(val);
}


inline void Vector1D::setX(float val)
{
	_x = val;
}


} // ONVIF


#endif // ONVIF_Vector1D_INCLUDED
