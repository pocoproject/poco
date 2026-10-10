// 
// ImagingStatus20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingStatus20_INCLUDED
#define ONVIF_ImagingStatus20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FocusStatus20;
class ImagingStatus20Extension;
} 


namespace ONVIF {


//@ name=ImagingStatus20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingStatus20
{
public:
	ImagingStatus20();

	ImagingStatus20(
		const Poco::SharedPtr<FocusStatus20>& focusStatus20, 
		const Poco::SharedPtr<ImagingStatus20Extension>& extension);

	virtual ~ImagingStatus20();

	const Poco::SharedPtr<ImagingStatus20Extension>& getExtension() const;

	const Poco::SharedPtr<FocusStatus20>& getFocusStatus20() const;

	void setExtension(const Poco::SharedPtr<ImagingStatus20Extension>& val);

	void setExtension(Poco::SharedPtr<ImagingStatus20Extension>&& val);

	void setFocusStatus20(const Poco::SharedPtr<FocusStatus20>& val);

	void setFocusStatus20(Poco::SharedPtr<FocusStatus20>&& val);

private:
	//@ mandatory=false
	//@ name=FocusStatus20
	//@ order=0
	Poco::SharedPtr<FocusStatus20> _focusStatus20;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ImagingStatus20Extension> _extension;

};


inline const Poco::SharedPtr<ImagingStatus20Extension>& ImagingStatus20::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<FocusStatus20>& ImagingStatus20::getFocusStatus20() const
{
	return _focusStatus20;
}


inline void ImagingStatus20::setExtension(const Poco::SharedPtr<ImagingStatus20Extension>& val)
{
	_extension = val;
}


inline void ImagingStatus20::setExtension(Poco::SharedPtr<ImagingStatus20Extension>&& val)
{
	_extension = std::move(val);
}


inline void ImagingStatus20::setFocusStatus20(const Poco::SharedPtr<FocusStatus20>& val)
{
	_focusStatus20 = val;
}


inline void ImagingStatus20::setFocusStatus20(Poco::SharedPtr<FocusStatus20>&& val)
{
	_focusStatus20 = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FocusStatus20.h"
#include "ONVIF/ImagingStatus20Extension.h"


#endif // ONVIF_ImagingStatus20_INCLUDED
