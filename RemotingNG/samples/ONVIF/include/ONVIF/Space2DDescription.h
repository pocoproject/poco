// 
// Space2DDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Space2DDescription_INCLUDED
#define ONVIF_Space2DDescription_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Space2DDescription
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Space2DDescription
{
public:
	Space2DDescription();

	Space2DDescription(
		const Poco::URI& uRI, 
		const FloatRange& xRange, 
		const FloatRange& yRange);

	virtual ~Space2DDescription();

	const Poco::URI& getURI() const;

	const FloatRange& getXRange() const;

	const FloatRange& getYRange() const;

	void setURI(const Poco::URI& val);

	void setURI(Poco::URI&& val);

	void setXRange(const FloatRange& val);

	void setXRange(FloatRange&& val);

	void setYRange(const FloatRange& val);

	void setYRange(FloatRange&& val);

private:
	//@ name=URI
	//@ order=0
	Poco::URI _uRI;

	//@ name=XRange
	//@ order=1
	FloatRange _xRange;

	//@ name=YRange
	//@ order=2
	FloatRange _yRange;

};


inline const Poco::URI& Space2DDescription::getURI() const
{
	return _uRI;
}


inline const FloatRange& Space2DDescription::getXRange() const
{
	return _xRange;
}


inline const FloatRange& Space2DDescription::getYRange() const
{
	return _yRange;
}


inline void Space2DDescription::setURI(const Poco::URI& val)
{
	_uRI = val;
}


inline void Space2DDescription::setURI(Poco::URI&& val)
{
	_uRI = std::move(val);
}


inline void Space2DDescription::setXRange(const FloatRange& val)
{
	_xRange = val;
}


inline void Space2DDescription::setXRange(FloatRange&& val)
{
	_xRange = std::move(val);
}


inline void Space2DDescription::setYRange(const FloatRange& val)
{
	_yRange = val;
}


inline void Space2DDescription::setYRange(FloatRange&& val)
{
	_yRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_Space2DDescription_INCLUDED
