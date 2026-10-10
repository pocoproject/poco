// 
// PTZFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZFilter_INCLUDED
#define ONVIF_PTZFilter_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PTZFilter
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZFilter
{
public:
	PTZFilter();

	PTZFilter(
		bool status, 
		bool position);

	virtual ~PTZFilter();

	bool getPosition() const;

	bool getStatus() const;

	void setPosition(bool val);

	void setStatus(bool val);

private:
	//@ name=Status
	//@ order=0
	bool _status;

	//@ name=Position
	//@ order=1
	bool _position;

};


inline bool PTZFilter::getPosition() const
{
	return _position;
}


inline bool PTZFilter::getStatus() const
{
	return _status;
}


inline void PTZFilter::setPosition(bool val)
{
	_position = val;
}


inline void PTZFilter::setStatus(bool val)
{
	_status = val;
}


} // ONVIF


#endif // ONVIF_PTZFilter_INCLUDED
