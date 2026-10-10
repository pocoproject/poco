// 
// ColorCovariance.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ColorCovariance_INCLUDED
#define ONVIF_ColorCovariance_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ColorCovariance
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ColorCovariance
{
public:
	ColorCovariance();

	ColorCovariance(
		float xX, 
		float yY, 
		float zZ, 
		Poco::Optional<float> xY, 
		Poco::Optional<float> xZ, 
		Poco::Optional<float> yZ, 
		const Poco::Optional<Poco::URI>& colorspace);

	virtual ~ColorCovariance();

	const Poco::Optional<Poco::URI>& getColorspace() const;

	float getXX() const;

	Poco::Optional<float> getXY() const;

	Poco::Optional<float> getXZ() const;

	float getYY() const;

	Poco::Optional<float> getYZ() const;

	float getZZ() const;

	void setColorspace(const Poco::Optional<Poco::URI>& val);

	void setColorspace(Poco::Optional<Poco::URI>&& val);

	void setXX(float val);

	void setXY(Poco::Optional<float> val);

	void setXZ(Poco::Optional<float> val);

	void setYY(float val);

	void setYZ(Poco::Optional<float> val);

	void setZZ(float val);

private:
	//@ name=XX
	//@ order=0
	//@ type=attr
	float _xX;

	//@ name=YY
	//@ order=1
	//@ type=attr
	float _yY;

	//@ name=ZZ
	//@ order=2
	//@ type=attr
	float _zZ;

	//@ mandatory=false
	//@ name=XY
	//@ order=3
	//@ type=attr
	Poco::Optional<float> _xY;

	//@ mandatory=false
	//@ name=XZ
	//@ order=4
	//@ type=attr
	Poco::Optional<float> _xZ;

	//@ mandatory=false
	//@ name=YZ
	//@ order=5
	//@ type=attr
	Poco::Optional<float> _yZ;

	//@ mandatory=false
	//@ name=Colorspace
	//@ order=6
	//@ type=attr
	Poco::Optional<Poco::URI> _colorspace;

};


inline const Poco::Optional<Poco::URI>& ColorCovariance::getColorspace() const
{
	return _colorspace;
}


inline float ColorCovariance::getXX() const
{
	return _xX;
}


inline Poco::Optional<float> ColorCovariance::getXY() const
{
	return _xY;
}


inline Poco::Optional<float> ColorCovariance::getXZ() const
{
	return _xZ;
}


inline float ColorCovariance::getYY() const
{
	return _yY;
}


inline Poco::Optional<float> ColorCovariance::getYZ() const
{
	return _yZ;
}


inline float ColorCovariance::getZZ() const
{
	return _zZ;
}


inline void ColorCovariance::setColorspace(const Poco::Optional<Poco::URI>& val)
{
	_colorspace = val;
}


inline void ColorCovariance::setColorspace(Poco::Optional<Poco::URI>&& val)
{
	_colorspace = std::move(val);
}


inline void ColorCovariance::setXX(float val)
{
	_xX = val;
}


inline void ColorCovariance::setXY(Poco::Optional<float> val)
{
	_xY = val;
}


inline void ColorCovariance::setXZ(Poco::Optional<float> val)
{
	_xZ = val;
}


inline void ColorCovariance::setYY(float val)
{
	_yY = val;
}


inline void ColorCovariance::setYZ(Poco::Optional<float> val)
{
	_yZ = val;
}


inline void ColorCovariance::setZZ(float val)
{
	_zZ = val;
}


} // ONVIF


#endif // ONVIF_ColorCovariance_INCLUDED
