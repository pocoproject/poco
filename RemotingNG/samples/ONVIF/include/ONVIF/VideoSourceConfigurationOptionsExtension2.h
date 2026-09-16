// 
// VideoSourceConfigurationOptionsExtension2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_VideoSourceConfigurationOptionsExtension2_INCLUDED
#define ONVIF_VideoSourceConfigurationOptionsExtension2_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=VideoSourceConfigurationOptionsExtension2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API VideoSourceConfigurationOptionsExtension2
{
public:
	VideoSourceConfigurationOptionsExtension2();

	VideoSourceConfigurationOptionsExtension2(const std::vector<std::string>& sceneOrientationMode);

	virtual ~VideoSourceConfigurationOptionsExtension2();

	const std::vector<std::string>& getSceneOrientationMode() const;

	std::vector<std::string>& getSceneOrientationMode();

	void setSceneOrientationMode(const std::vector<std::string>& val);

	void setSceneOrientationMode(std::vector<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=SceneOrientationMode
	//@ order=0
	std::vector<std::string> _sceneOrientationMode;

};


inline const std::vector<std::string>& VideoSourceConfigurationOptionsExtension2::getSceneOrientationMode() const
{
	return _sceneOrientationMode;
}


inline std::vector<std::string>& VideoSourceConfigurationOptionsExtension2::getSceneOrientationMode()
{
	return _sceneOrientationMode;
}


inline void VideoSourceConfigurationOptionsExtension2::setSceneOrientationMode(const std::vector<std::string>& val)
{
	_sceneOrientationMode = val;
}


inline void VideoSourceConfigurationOptionsExtension2::setSceneOrientationMode(std::vector<std::string>&& val)
{
	_sceneOrientationMode = std::move(val);
}


} // ONVIF


#endif // ONVIF_VideoSourceConfigurationOptionsExtension2_INCLUDED
