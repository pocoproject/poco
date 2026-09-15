// 
// ZoomLimits.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ZoomLimits_INCLUDED
#define ONVIF_ZoomLimits_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Space1DDescription.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ZoomLimits
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ZoomLimits
{
public:
	ZoomLimits();

	ZoomLimits(const Space1DDescription& range);

	virtual ~ZoomLimits();

	const Space1DDescription& getRange() const;

	void setRange(const Space1DDescription& val);

	void setRange(Space1DDescription&& val);

private:
	//@ name=Range
	//@ order=0
	Space1DDescription _range;

};


inline const Space1DDescription& ZoomLimits::getRange() const
{
	return _range;
}


inline void ZoomLimits::setRange(const Space1DDescription& val)
{
	_range = val;
}


inline void ZoomLimits::setRange(Space1DDescription&& val)
{
	_range = std::move(val);
}


} // ONVIF


#endif // ONVIF_ZoomLimits_INCLUDED
