// 
// ImagingOptions20Extension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingOptions20Extension_INCLUDED
#define ONVIF_ImagingOptions20Extension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ImageStabilizationOptions;
class ImagingOptions20Extension2;
} 


namespace ONVIF {


//@ name=ImagingOptions20Extension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingOptions20Extension
{
public:
	ImagingOptions20Extension();

	ImagingOptions20Extension(
		const Poco::SharedPtr<ImageStabilizationOptions>& imageStabilization, 
		const Poco::SharedPtr<ImagingOptions20Extension2>& extension);

	virtual ~ImagingOptions20Extension();

	const Poco::SharedPtr<ImagingOptions20Extension2>& getExtension() const;

	const Poco::SharedPtr<ImageStabilizationOptions>& getImageStabilization() const;

	void setExtension(const Poco::SharedPtr<ImagingOptions20Extension2>& val);

	void setExtension(Poco::SharedPtr<ImagingOptions20Extension2>&& val);

	void setImageStabilization(const Poco::SharedPtr<ImageStabilizationOptions>& val);

	void setImageStabilization(Poco::SharedPtr<ImageStabilizationOptions>&& val);

private:
	//@ mandatory=false
	//@ name=ImageStabilization
	//@ order=0
	Poco::SharedPtr<ImageStabilizationOptions> _imageStabilization;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ImagingOptions20Extension2> _extension;

};


inline const Poco::SharedPtr<ImagingOptions20Extension2>& ImagingOptions20Extension::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<ImageStabilizationOptions>& ImagingOptions20Extension::getImageStabilization() const
{
	return _imageStabilization;
}


inline void ImagingOptions20Extension::setExtension(const Poco::SharedPtr<ImagingOptions20Extension2>& val)
{
	_extension = val;
}


inline void ImagingOptions20Extension::setExtension(Poco::SharedPtr<ImagingOptions20Extension2>&& val)
{
	_extension = std::move(val);
}


inline void ImagingOptions20Extension::setImageStabilization(const Poco::SharedPtr<ImageStabilizationOptions>& val)
{
	_imageStabilization = val;
}


inline void ImagingOptions20Extension::setImageStabilization(Poco::SharedPtr<ImageStabilizationOptions>&& val)
{
	_imageStabilization = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ImageStabilizationOptions.h"
#include "ONVIF/ImagingOptions20Extension2.h"


#endif // ONVIF_ImagingOptions20Extension_INCLUDED
