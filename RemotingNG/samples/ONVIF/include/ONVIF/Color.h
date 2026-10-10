// 
// Color.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Color_INCLUDED
#define ONVIF_Color_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Color
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Color
{
public:
	Color();

	Color(
		float x, 
		float y, 
		float z, 
		const Poco::Optional<Poco::URI>& colorspace, 
		Poco::Optional<float> likelihood);

	virtual ~Color();

	const Poco::Optional<Poco::URI>& getColorspace() const;

	Poco::Optional<float> getLikelihood() const;

	float getX() const;

	float getY() const;

	float getZ() const;

	void setColorspace(const Poco::Optional<Poco::URI>& val);

	void setColorspace(Poco::Optional<Poco::URI>&& val);

	void setLikelihood(Poco::Optional<float> val);

	void setX(float val);

	void setY(float val);

	void setZ(float val);

private:
	//@ name=X
	//@ order=0
	//@ type=attr
	float _x;

	//@ name=Y
	//@ order=1
	//@ type=attr
	float _y;

	//@ name=Z
	//@ order=2
	//@ type=attr
	float _z;

	//@ mandatory=false
	//@ name=Colorspace
	//@ order=3
	//@ type=attr
	Poco::Optional<Poco::URI> _colorspace;

	//@ mandatory=false
	//@ name=Likelihood
	//@ order=4
	//@ type=attr
	Poco::Optional<float> _likelihood;

};


inline const Poco::Optional<Poco::URI>& Color::getColorspace() const
{
	return _colorspace;
}


inline Poco::Optional<float> Color::getLikelihood() const
{
	return _likelihood;
}


inline float Color::getX() const
{
	return _x;
}


inline float Color::getY() const
{
	return _y;
}


inline float Color::getZ() const
{
	return _z;
}


inline void Color::setColorspace(const Poco::Optional<Poco::URI>& val)
{
	_colorspace = val;
}


inline void Color::setColorspace(Poco::Optional<Poco::URI>&& val)
{
	_colorspace = std::move(val);
}


inline void Color::setLikelihood(Poco::Optional<float> val)
{
	_likelihood = val;
}


inline void Color::setX(float val)
{
	_x = val;
}


inline void Color::setY(float val)
{
	_y = val;
}


inline void Color::setZ(float val)
{
	_z = val;
}


} // ONVIF


#endif // ONVIF_Color_INCLUDED
