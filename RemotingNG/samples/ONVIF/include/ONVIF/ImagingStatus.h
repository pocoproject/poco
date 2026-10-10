// 
// ImagingStatus.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingStatus_INCLUDED
#define ONVIF_ImagingStatus_INCLUDED


#include "ONVIF/FocusStatus.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ImagingStatus
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingStatus
{
public:
	ImagingStatus();

	ImagingStatus(const FocusStatus& focusStatus);

	virtual ~ImagingStatus();

	const FocusStatus& getFocusStatus() const;

	void setFocusStatus(const FocusStatus& val);

	void setFocusStatus(FocusStatus&& val);

private:
	//@ name=FocusStatus
	//@ order=0
	FocusStatus _focusStatus;

};


inline const FocusStatus& ImagingStatus::getFocusStatus() const
{
	return _focusStatus;
}


inline void ImagingStatus::setFocusStatus(const FocusStatus& val)
{
	_focusStatus = val;
}


inline void ImagingStatus::setFocusStatus(FocusStatus&& val)
{
	_focusStatus = std::move(val);
}


} // ONVIF


#endif // ONVIF_ImagingStatus_INCLUDED
