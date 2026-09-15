// 
// VideoSourceConfigurationOptionsExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED
#define ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class RotateOptions;
class VideoSourceConfigurationOptionsExtension2;
} 


namespace ONVIF {


//@ name=VideoSourceConfigurationOptionsExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSourceConfigurationOptionsExtension
{
public:
	VideoSourceConfigurationOptionsExtension();

	VideoSourceConfigurationOptionsExtension(
		const Poco::SharedPtr<RotateOptions>& rotate, 
		const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>& extension);

	virtual ~VideoSourceConfigurationOptionsExtension();

	const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>& getExtension() const;

	const Poco::SharedPtr<RotateOptions>& getRotate() const;

	void setExtension(const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>& val);

	void setExtension(Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>&& val);

	void setRotate(const Poco::SharedPtr<RotateOptions>& val);

	void setRotate(Poco::SharedPtr<RotateOptions>&& val);

private:
	//@ mandatory=false
	//@ name=Rotate
	//@ order=0
	Poco::SharedPtr<RotateOptions> _rotate;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2> _extension;

};


inline const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>& VideoSourceConfigurationOptionsExtension::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<RotateOptions>& VideoSourceConfigurationOptionsExtension::getRotate() const
{
	return _rotate;
}


inline void VideoSourceConfigurationOptionsExtension::setExtension(const Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>& val)
{
	_extension = val;
}


inline void VideoSourceConfigurationOptionsExtension::setExtension(Poco::SharedPtr<VideoSourceConfigurationOptionsExtension2>&& val)
{
	_extension = std::move(val);
}


inline void VideoSourceConfigurationOptionsExtension::setRotate(const Poco::SharedPtr<RotateOptions>& val)
{
	_rotate = val;
}


inline void VideoSourceConfigurationOptionsExtension::setRotate(Poco::SharedPtr<RotateOptions>&& val)
{
	_rotate = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/RotateOptions.h"
#include "ONVIF/VideoSourceConfigurationOptionsExtension2.h"


#endif // ONVIF_VideoSourceConfigurationOptionsExtension_INCLUDED
