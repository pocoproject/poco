// 
// VideoSourceConfigurationExtension2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSourceConfigurationExtension2_INCLUDED
#define ONVIF_VideoSourceConfigurationExtension2_INCLUDED


#include "ONVIF/LensDescription.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class SceneOrientation;
} 


namespace ONVIF {


//@ name=VideoSourceConfigurationExtension2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSourceConfigurationExtension2
{
public:
	VideoSourceConfigurationExtension2();

	VideoSourceConfigurationExtension2(
		const std::vector<LensDescription>& lensDescription, 
		const Poco::SharedPtr<SceneOrientation>& sceneOrientation);

	virtual ~VideoSourceConfigurationExtension2();

	const std::vector<LensDescription>& getLensDescription() const;

	std::vector<LensDescription>& getLensDescription();

	const Poco::SharedPtr<SceneOrientation>& getSceneOrientation() const;

	void setLensDescription(const std::vector<LensDescription>& val);

	void setLensDescription(std::vector<LensDescription>&& val);

	void setSceneOrientation(const Poco::SharedPtr<SceneOrientation>& val);

	void setSceneOrientation(Poco::SharedPtr<SceneOrientation>&& val);

private:
	//@ mandatory=false
	//@ name=LensDescription
	//@ order=0
	std::vector<LensDescription> _lensDescription;

	//@ mandatory=false
	//@ name=SceneOrientation
	//@ order=1
	Poco::SharedPtr<SceneOrientation> _sceneOrientation;

};


inline const std::vector<LensDescription>& VideoSourceConfigurationExtension2::getLensDescription() const
{
	return _lensDescription;
}


inline std::vector<LensDescription>& VideoSourceConfigurationExtension2::getLensDescription()
{
	return _lensDescription;
}


inline const Poco::SharedPtr<SceneOrientation>& VideoSourceConfigurationExtension2::getSceneOrientation() const
{
	return _sceneOrientation;
}


inline void VideoSourceConfigurationExtension2::setLensDescription(const std::vector<LensDescription>& val)
{
	_lensDescription = val;
}


inline void VideoSourceConfigurationExtension2::setLensDescription(std::vector<LensDescription>&& val)
{
	_lensDescription = std::move(val);
}


inline void VideoSourceConfigurationExtension2::setSceneOrientation(const Poco::SharedPtr<SceneOrientation>& val)
{
	_sceneOrientation = val;
}


inline void VideoSourceConfigurationExtension2::setSceneOrientation(Poco::SharedPtr<SceneOrientation>&& val)
{
	_sceneOrientation = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SceneOrientation.h"


#endif // ONVIF_VideoSourceConfigurationExtension2_INCLUDED
