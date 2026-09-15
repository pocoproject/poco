// 
// LensDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_LensDescription_INCLUDED
#define ONVIF_LensDescription_INCLUDED


#include "ONVIF/LensOffset.h"
#include "ONVIF/LensProjection.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=LensDescription
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API LensDescription
{
public:
	LensDescription();

	LensDescription(
		Poco::Optional<float> focalLength, 
		const LensOffset& offset, 
		const std::vector<LensProjection>& projection, 
		float xFactor);

	virtual ~LensDescription();

	Poco::Optional<float> getFocalLength() const;

	const LensOffset& getOffset() const;

	const std::vector<LensProjection>& getProjection() const;

	std::vector<LensProjection>& getProjection();

	float getXFactor() const;

	void setFocalLength(Poco::Optional<float> val);

	void setOffset(const LensOffset& val);

	void setOffset(LensOffset&& val);

	void setProjection(const std::vector<LensProjection>& val);

	void setProjection(std::vector<LensProjection>&& val);

	void setXFactor(float val);

private:
	//@ mandatory=false
	//@ name=FocalLength
	//@ order=0
	//@ type=attr
	Poco::Optional<float> _focalLength;

	//@ name=Offset
	//@ order=1
	LensOffset _offset;

	//@ name=Projection
	//@ order=2
	std::vector<LensProjection> _projection;

	//@ name=XFactor
	//@ order=3
	float _xFactor;

};


inline Poco::Optional<float> LensDescription::getFocalLength() const
{
	return _focalLength;
}


inline const LensOffset& LensDescription::getOffset() const
{
	return _offset;
}


inline const std::vector<LensProjection>& LensDescription::getProjection() const
{
	return _projection;
}


inline std::vector<LensProjection>& LensDescription::getProjection()
{
	return _projection;
}


inline float LensDescription::getXFactor() const
{
	return _xFactor;
}


inline void LensDescription::setFocalLength(Poco::Optional<float> val)
{
	_focalLength = val;
}


inline void LensDescription::setOffset(const LensOffset& val)
{
	_offset = val;
}


inline void LensDescription::setOffset(LensOffset&& val)
{
	_offset = std::move(val);
}


inline void LensDescription::setProjection(const std::vector<LensProjection>& val)
{
	_projection = val;
}


inline void LensDescription::setProjection(std::vector<LensProjection>&& val)
{
	_projection = std::move(val);
}


inline void LensDescription::setXFactor(float val)
{
	_xFactor = val;
}


} // ONVIF


#endif // ONVIF_LensDescription_INCLUDED
