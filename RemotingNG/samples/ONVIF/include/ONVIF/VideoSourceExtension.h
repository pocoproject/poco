// 
// VideoSourceExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSourceExtension_INCLUDED
#define ONVIF_VideoSourceExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ImagingSettings20;
class VideoSourceExtension2;
} 


namespace ONVIF {


//@ name=VideoSourceExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSourceExtension
{
public:
	VideoSourceExtension();

	VideoSourceExtension(
		const Poco::SharedPtr<ImagingSettings20>& imaging, 
		const Poco::SharedPtr<VideoSourceExtension2>& extension);

	virtual ~VideoSourceExtension();

	const Poco::SharedPtr<VideoSourceExtension2>& getExtension() const;

	const Poco::SharedPtr<ImagingSettings20>& getImaging() const;

	void setExtension(const Poco::SharedPtr<VideoSourceExtension2>& val);

	void setExtension(Poco::SharedPtr<VideoSourceExtension2>&& val);

	void setImaging(const Poco::SharedPtr<ImagingSettings20>& val);

	void setImaging(Poco::SharedPtr<ImagingSettings20>&& val);

private:
	//@ mandatory=false
	//@ name=Imaging
	//@ order=0
	Poco::SharedPtr<ImagingSettings20> _imaging;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<VideoSourceExtension2> _extension;

};


inline const Poco::SharedPtr<VideoSourceExtension2>& VideoSourceExtension::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<ImagingSettings20>& VideoSourceExtension::getImaging() const
{
	return _imaging;
}


inline void VideoSourceExtension::setExtension(const Poco::SharedPtr<VideoSourceExtension2>& val)
{
	_extension = val;
}


inline void VideoSourceExtension::setExtension(Poco::SharedPtr<VideoSourceExtension2>&& val)
{
	_extension = std::move(val);
}


inline void VideoSourceExtension::setImaging(const Poco::SharedPtr<ImagingSettings20>& val)
{
	_imaging = val;
}


inline void VideoSourceExtension::setImaging(Poco::SharedPtr<ImagingSettings20>&& val)
{
	_imaging = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ImagingSettings20.h"
#include "ONVIF/VideoSourceExtension2.h"


#endif // ONVIF_VideoSourceExtension_INCLUDED
