// 
// PTZPositionFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPositionFilter_INCLUDED
#define ONVIF_PTZPositionFilter_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZVector.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PTZPositionFilter
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPositionFilter
{
public:
	PTZPositionFilter();

	PTZPositionFilter(
		const PTZVector& minPosition, 
		const PTZVector& maxPosition, 
		bool enterOrExit);

	virtual ~PTZPositionFilter();

	bool getEnterOrExit() const;

	const PTZVector& getMaxPosition() const;

	const PTZVector& getMinPosition() const;

	void setEnterOrExit(bool val);

	void setMaxPosition(const PTZVector& val);

	void setMaxPosition(PTZVector&& val);

	void setMinPosition(const PTZVector& val);

	void setMinPosition(PTZVector&& val);

private:
	//@ name=MinPosition
	//@ order=0
	PTZVector _minPosition;

	//@ name=MaxPosition
	//@ order=1
	PTZVector _maxPosition;

	//@ name=EnterOrExit
	//@ order=2
	bool _enterOrExit;

};


inline bool PTZPositionFilter::getEnterOrExit() const
{
	return _enterOrExit;
}


inline const PTZVector& PTZPositionFilter::getMaxPosition() const
{
	return _maxPosition;
}


inline const PTZVector& PTZPositionFilter::getMinPosition() const
{
	return _minPosition;
}


inline void PTZPositionFilter::setEnterOrExit(bool val)
{
	_enterOrExit = val;
}


inline void PTZPositionFilter::setMaxPosition(const PTZVector& val)
{
	_maxPosition = val;
}


inline void PTZPositionFilter::setMaxPosition(PTZVector&& val)
{
	_maxPosition = std::move(val);
}


inline void PTZPositionFilter::setMinPosition(const PTZVector& val)
{
	_minPosition = val;
}


inline void PTZPositionFilter::setMinPosition(PTZVector&& val)
{
	_minPosition = std::move(val);
}


} // ONVIF


#endif // ONVIF_PTZPositionFilter_INCLUDED
