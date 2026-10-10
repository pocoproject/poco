// 
// PanTiltLimits.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PanTiltLimits_INCLUDED
#define ONVIF_PanTiltLimits_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Space2DDescription.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PanTiltLimits
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PanTiltLimits
{
public:
	PanTiltLimits();

	PanTiltLimits(const Space2DDescription& range);

	virtual ~PanTiltLimits();

	const Space2DDescription& getRange() const;

	void setRange(const Space2DDescription& val);

	void setRange(Space2DDescription&& val);

private:
	//@ name=Range
	//@ order=0
	Space2DDescription _range;

};


inline const Space2DDescription& PanTiltLimits::getRange() const
{
	return _range;
}


inline void PanTiltLimits::setRange(const Space2DDescription& val)
{
	_range = val;
}


inline void PanTiltLimits::setRange(Space2DDescription&& val)
{
	_range = std::move(val);
}


} // ONVIF


#endif // ONVIF_PanTiltLimits_INCLUDED
