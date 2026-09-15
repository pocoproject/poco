// 
// VideoSourceConfigurationExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSourceConfigurationExtension_INCLUDED
#define ONVIF_VideoSourceConfigurationExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Rotate;
class VideoSourceConfigurationExtension2;
} 


namespace ONVIF {


//@ name=VideoSourceConfigurationExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSourceConfigurationExtension
{
public:
	VideoSourceConfigurationExtension();

	VideoSourceConfigurationExtension(
		const Poco::SharedPtr<Rotate>& rotate, 
		const Poco::SharedPtr<VideoSourceConfigurationExtension2>& extension);

	virtual ~VideoSourceConfigurationExtension();

	const Poco::SharedPtr<VideoSourceConfigurationExtension2>& getExtension() const;

	const Poco::SharedPtr<Rotate>& getRotate() const;

	void setExtension(const Poco::SharedPtr<VideoSourceConfigurationExtension2>& val);

	void setExtension(Poco::SharedPtr<VideoSourceConfigurationExtension2>&& val);

	void setRotate(const Poco::SharedPtr<Rotate>& val);

	void setRotate(Poco::SharedPtr<Rotate>&& val);

private:
	//@ mandatory=false
	//@ name=Rotate
	//@ order=0
	Poco::SharedPtr<Rotate> _rotate;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<VideoSourceConfigurationExtension2> _extension;

};


inline const Poco::SharedPtr<VideoSourceConfigurationExtension2>& VideoSourceConfigurationExtension::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<Rotate>& VideoSourceConfigurationExtension::getRotate() const
{
	return _rotate;
}


inline void VideoSourceConfigurationExtension::setExtension(const Poco::SharedPtr<VideoSourceConfigurationExtension2>& val)
{
	_extension = val;
}


inline void VideoSourceConfigurationExtension::setExtension(Poco::SharedPtr<VideoSourceConfigurationExtension2>&& val)
{
	_extension = std::move(val);
}


inline void VideoSourceConfigurationExtension::setRotate(const Poco::SharedPtr<Rotate>& val)
{
	_rotate = val;
}


inline void VideoSourceConfigurationExtension::setRotate(Poco::SharedPtr<Rotate>&& val)
{
	_rotate = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Rotate.h"
#include "ONVIF/VideoSourceConfigurationExtension2.h"


#endif // ONVIF_VideoSourceConfigurationExtension_INCLUDED
