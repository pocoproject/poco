// 
// PTZPresetTourPresetDetailOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED
#define ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZPresetTourPresetDetailOptionsExtension;
class Space1DDescription;
class Space2DDescription;
} 


namespace ONVIF {


//@ name=PTZPresetTourPresetDetailOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZPresetTourPresetDetailOptions
{
public:
	PTZPresetTourPresetDetailOptions();

	PTZPresetTourPresetDetailOptions(
		const std::vector<std::string>& presetToken, 
		Poco::Optional<bool> home, 
		const Poco::SharedPtr<Space2DDescription>& panTiltPositionSpace, 
		const Poco::SharedPtr<Space1DDescription>& zoomPositionSpace, 
		const Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>& extension);

	virtual ~PTZPresetTourPresetDetailOptions();

	const Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>& getExtension() const;

	Poco::Optional<bool> getHome() const;

	const Poco::SharedPtr<Space2DDescription>& getPanTiltPositionSpace() const;

	const std::vector<std::string>& getPresetToken() const;

	std::vector<std::string>& getPresetToken();

	const Poco::SharedPtr<Space1DDescription>& getZoomPositionSpace() const;

	void setExtension(const Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>&& val);

	void setHome(Poco::Optional<bool> val);

	void setPanTiltPositionSpace(const Poco::SharedPtr<Space2DDescription>& val);

	void setPanTiltPositionSpace(Poco::SharedPtr<Space2DDescription>&& val);

	void setPresetToken(const std::vector<std::string>& val);

	void setPresetToken(std::vector<std::string>&& val);

	void setZoomPositionSpace(const Poco::SharedPtr<Space1DDescription>& val);

	void setZoomPositionSpace(Poco::SharedPtr<Space1DDescription>&& val);

private:
	//@ mandatory=false
	//@ name=PresetToken
	//@ order=0
	std::vector<std::string> _presetToken;

	//@ mandatory=false
	//@ name=Home
	//@ order=1
	Poco::Optional<bool> _home;

	//@ mandatory=false
	//@ name=PanTiltPositionSpace
	//@ order=2
	Poco::SharedPtr<Space2DDescription> _panTiltPositionSpace;

	//@ mandatory=false
	//@ name=ZoomPositionSpace
	//@ order=3
	Poco::SharedPtr<Space1DDescription> _zoomPositionSpace;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension> _extension;

};


inline const Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>& PTZPresetTourPresetDetailOptions::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> PTZPresetTourPresetDetailOptions::getHome() const
{
	return _home;
}


inline const Poco::SharedPtr<Space2DDescription>& PTZPresetTourPresetDetailOptions::getPanTiltPositionSpace() const
{
	return _panTiltPositionSpace;
}


inline const std::vector<std::string>& PTZPresetTourPresetDetailOptions::getPresetToken() const
{
	return _presetToken;
}


inline std::vector<std::string>& PTZPresetTourPresetDetailOptions::getPresetToken()
{
	return _presetToken;
}


inline const Poco::SharedPtr<Space1DDescription>& PTZPresetTourPresetDetailOptions::getZoomPositionSpace() const
{
	return _zoomPositionSpace;
}


inline void PTZPresetTourPresetDetailOptions::setExtension(const Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>& val)
{
	_extension = val;
}


inline void PTZPresetTourPresetDetailOptions::setExtension(Poco::SharedPtr<PTZPresetTourPresetDetailOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZPresetTourPresetDetailOptions::setHome(Poco::Optional<bool> val)
{
	_home = val;
}


inline void PTZPresetTourPresetDetailOptions::setPanTiltPositionSpace(const Poco::SharedPtr<Space2DDescription>& val)
{
	_panTiltPositionSpace = val;
}


inline void PTZPresetTourPresetDetailOptions::setPanTiltPositionSpace(Poco::SharedPtr<Space2DDescription>&& val)
{
	_panTiltPositionSpace = std::move(val);
}


inline void PTZPresetTourPresetDetailOptions::setPresetToken(const std::vector<std::string>& val)
{
	_presetToken = val;
}


inline void PTZPresetTourPresetDetailOptions::setPresetToken(std::vector<std::string>&& val)
{
	_presetToken = std::move(val);
}


inline void PTZPresetTourPresetDetailOptions::setZoomPositionSpace(const Poco::SharedPtr<Space1DDescription>& val)
{
	_zoomPositionSpace = val;
}


inline void PTZPresetTourPresetDetailOptions::setZoomPositionSpace(Poco::SharedPtr<Space1DDescription>&& val)
{
	_zoomPositionSpace = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZPresetTourPresetDetailOptionsExtension.h"
#include "ONVIF/Space1DDescription.h"
#include "ONVIF/Space2DDescription.h"


#endif // ONVIF_PTZPresetTourPresetDetailOptions_INCLUDED
