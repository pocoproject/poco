// 
// ImagingSettingsExtension20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingSettingsExtension20_INCLUDED
#define ONVIF_ImagingSettingsExtension20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ImageStabilization;
class ImagingSettingsExtension202;
} 


namespace ONVIF {


//@ name=ImagingSettingsExtension20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingSettingsExtension20
{
public:
	ImagingSettingsExtension20();

	ImagingSettingsExtension20(
		const Poco::SharedPtr<ImageStabilization>& imageStabilization, 
		const Poco::SharedPtr<ImagingSettingsExtension202>& extension);

	virtual ~ImagingSettingsExtension20();

	const Poco::SharedPtr<ImagingSettingsExtension202>& getExtension() const;

	const Poco::SharedPtr<ImageStabilization>& getImageStabilization() const;

	void setExtension(const Poco::SharedPtr<ImagingSettingsExtension202>& val);

	void setExtension(Poco::SharedPtr<ImagingSettingsExtension202>&& val);

	void setImageStabilization(const Poco::SharedPtr<ImageStabilization>& val);

	void setImageStabilization(Poco::SharedPtr<ImageStabilization>&& val);

private:
	//@ mandatory=false
	//@ name=ImageStabilization
	//@ order=0
	Poco::SharedPtr<ImageStabilization> _imageStabilization;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ImagingSettingsExtension202> _extension;

};


inline const Poco::SharedPtr<ImagingSettingsExtension202>& ImagingSettingsExtension20::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<ImageStabilization>& ImagingSettingsExtension20::getImageStabilization() const
{
	return _imageStabilization;
}


inline void ImagingSettingsExtension20::setExtension(const Poco::SharedPtr<ImagingSettingsExtension202>& val)
{
	_extension = val;
}


inline void ImagingSettingsExtension20::setExtension(Poco::SharedPtr<ImagingSettingsExtension202>&& val)
{
	_extension = std::move(val);
}


inline void ImagingSettingsExtension20::setImageStabilization(const Poco::SharedPtr<ImageStabilization>& val)
{
	_imageStabilization = val;
}


inline void ImagingSettingsExtension20::setImageStabilization(Poco::SharedPtr<ImageStabilization>&& val)
{
	_imageStabilization = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ImageStabilization.h"
#include "ONVIF/ImagingSettingsExtension202.h"


#endif // ONVIF_ImagingSettingsExtension20_INCLUDED
