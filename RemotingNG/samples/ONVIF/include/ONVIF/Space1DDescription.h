// 
// Space1DDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Space1DDescription_INCLUDED
#define ONVIF_Space1DDescription_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Space1DDescription
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Space1DDescription
{
public:
	Space1DDescription();

	Space1DDescription(
		const Poco::URI& uRI, 
		const FloatRange& xRange);

	virtual ~Space1DDescription();

	const Poco::URI& getURI() const;

	const FloatRange& getXRange() const;

	void setURI(const Poco::URI& val);

	void setURI(Poco::URI&& val);

	void setXRange(const FloatRange& val);

	void setXRange(FloatRange&& val);

private:
	//@ name=URI
	//@ order=0
	Poco::URI _uRI;

	//@ name=XRange
	//@ order=1
	FloatRange _xRange;

};


inline const Poco::URI& Space1DDescription::getURI() const
{
	return _uRI;
}


inline const FloatRange& Space1DDescription::getXRange() const
{
	return _xRange;
}


inline void Space1DDescription::setURI(const Poco::URI& val)
{
	_uRI = val;
}


inline void Space1DDescription::setURI(Poco::URI&& val)
{
	_uRI = std::move(val);
}


inline void Space1DDescription::setXRange(const FloatRange& val)
{
	_xRange = val;
}


inline void Space1DDescription::setXRange(FloatRange&& val)
{
	_xRange = std::move(val);
}


} // ONVIF


#endif // ONVIF_Space1DDescription_INCLUDED
