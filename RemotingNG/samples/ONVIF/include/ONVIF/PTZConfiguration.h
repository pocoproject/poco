// 
// PTZConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZConfiguration_INCLUDED
#define ONVIF_PTZConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZConfigurationExtension;
class PTZSpeed;
class PanTiltLimits;
class ZoomLimits;
} 


namespace ONVIF {


//@ name=PTZConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZConfiguration: public ConfigurationEntity
{
public:
	PTZConfiguration();

	PTZConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		Poco::Optional<int> moveRamp, 
		Poco::Optional<int> presetRamp, 
		Poco::Optional<int> presetTourRamp, 
		const std::string& nodeToken, 
		const Poco::Optional<Poco::URI>& defaultAbsolutePantTiltPositionSpace, 
		const Poco::Optional<Poco::URI>& defaultAbsoluteZoomPositionSpace, 
		const Poco::Optional<Poco::URI>& defaultRelativePanTiltTranslationSpace, 
		const Poco::Optional<Poco::URI>& defaultRelativeZoomTranslationSpace, 
		const Poco::Optional<Poco::URI>& defaultContinuousPanTiltVelocitySpace, 
		const Poco::Optional<Poco::URI>& defaultContinuousZoomVelocitySpace, 
		const Poco::SharedPtr<PTZSpeed>& defaultPTZSpeed, 
		const Poco::Optional<std::string>& defaultPTZTimeout, 
		const Poco::SharedPtr<PanTiltLimits>& panTiltLimits, 
		const Poco::SharedPtr<ZoomLimits>& zoomLimits, 
		const Poco::SharedPtr<PTZConfigurationExtension>& extension);

	virtual ~PTZConfiguration();

	const Poco::Optional<Poco::URI>& getDefaultAbsolutePantTiltPositionSpace() const;

	const Poco::Optional<Poco::URI>& getDefaultAbsoluteZoomPositionSpace() const;

	const Poco::Optional<Poco::URI>& getDefaultContinuousPanTiltVelocitySpace() const;

	const Poco::Optional<Poco::URI>& getDefaultContinuousZoomVelocitySpace() const;

	const Poco::SharedPtr<PTZSpeed>& getDefaultPTZSpeed() const;

	const Poco::Optional<std::string>& getDefaultPTZTimeout() const;

	const Poco::Optional<Poco::URI>& getDefaultRelativePanTiltTranslationSpace() const;

	const Poco::Optional<Poco::URI>& getDefaultRelativeZoomTranslationSpace() const;

	const Poco::SharedPtr<PTZConfigurationExtension>& getExtension() const;

	Poco::Optional<int> getMoveRamp() const;

	const std::string& getNodeToken() const;

	const Poco::SharedPtr<PanTiltLimits>& getPanTiltLimits() const;

	Poco::Optional<int> getPresetRamp() const;

	Poco::Optional<int> getPresetTourRamp() const;

	const Poco::SharedPtr<ZoomLimits>& getZoomLimits() const;

	void setDefaultAbsolutePantTiltPositionSpace(const Poco::Optional<Poco::URI>& val);

	void setDefaultAbsolutePantTiltPositionSpace(Poco::Optional<Poco::URI>&& val);

	void setDefaultAbsoluteZoomPositionSpace(const Poco::Optional<Poco::URI>& val);

	void setDefaultAbsoluteZoomPositionSpace(Poco::Optional<Poco::URI>&& val);

	void setDefaultContinuousPanTiltVelocitySpace(const Poco::Optional<Poco::URI>& val);

	void setDefaultContinuousPanTiltVelocitySpace(Poco::Optional<Poco::URI>&& val);

	void setDefaultContinuousZoomVelocitySpace(const Poco::Optional<Poco::URI>& val);

	void setDefaultContinuousZoomVelocitySpace(Poco::Optional<Poco::URI>&& val);

	void setDefaultPTZSpeed(const Poco::SharedPtr<PTZSpeed>& val);

	void setDefaultPTZSpeed(Poco::SharedPtr<PTZSpeed>&& val);

	void setDefaultPTZTimeout(const Poco::Optional<std::string>& val);

	void setDefaultPTZTimeout(Poco::Optional<std::string>&& val);

	void setDefaultRelativePanTiltTranslationSpace(const Poco::Optional<Poco::URI>& val);

	void setDefaultRelativePanTiltTranslationSpace(Poco::Optional<Poco::URI>&& val);

	void setDefaultRelativeZoomTranslationSpace(const Poco::Optional<Poco::URI>& val);

	void setDefaultRelativeZoomTranslationSpace(Poco::Optional<Poco::URI>&& val);

	void setExtension(const Poco::SharedPtr<PTZConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<PTZConfigurationExtension>&& val);

	void setMoveRamp(Poco::Optional<int> val);

	void setNodeToken(const std::string& val);

	void setNodeToken(std::string&& val);

	void setPanTiltLimits(const Poco::SharedPtr<PanTiltLimits>& val);

	void setPanTiltLimits(Poco::SharedPtr<PanTiltLimits>&& val);

	void setPresetRamp(Poco::Optional<int> val);

	void setPresetTourRamp(Poco::Optional<int> val);

	void setZoomLimits(const Poco::SharedPtr<ZoomLimits>& val);

	void setZoomLimits(Poco::SharedPtr<ZoomLimits>&& val);

private:
	//@ mandatory=false
	//@ name=MoveRamp
	//@ order=0
	//@ type=attr
	Poco::Optional<int> _moveRamp;

	//@ mandatory=false
	//@ name=PresetRamp
	//@ order=1
	//@ type=attr
	Poco::Optional<int> _presetRamp;

	//@ mandatory=false
	//@ name=PresetTourRamp
	//@ order=2
	//@ type=attr
	Poco::Optional<int> _presetTourRamp;

	//@ name=NodeToken
	//@ order=3
	std::string _nodeToken;

	//@ mandatory=false
	//@ name=DefaultAbsolutePantTiltPositionSpace
	//@ order=4
	Poco::Optional<Poco::URI> _defaultAbsolutePantTiltPositionSpace;

	//@ mandatory=false
	//@ name=DefaultAbsoluteZoomPositionSpace
	//@ order=5
	Poco::Optional<Poco::URI> _defaultAbsoluteZoomPositionSpace;

	//@ mandatory=false
	//@ name=DefaultRelativePanTiltTranslationSpace
	//@ order=6
	Poco::Optional<Poco::URI> _defaultRelativePanTiltTranslationSpace;

	//@ mandatory=false
	//@ name=DefaultRelativeZoomTranslationSpace
	//@ order=7
	Poco::Optional<Poco::URI> _defaultRelativeZoomTranslationSpace;

	//@ mandatory=false
	//@ name=DefaultContinuousPanTiltVelocitySpace
	//@ order=8
	Poco::Optional<Poco::URI> _defaultContinuousPanTiltVelocitySpace;

	//@ mandatory=false
	//@ name=DefaultContinuousZoomVelocitySpace
	//@ order=9
	Poco::Optional<Poco::URI> _defaultContinuousZoomVelocitySpace;

	//@ mandatory=false
	//@ name=DefaultPTZSpeed
	//@ order=10
	Poco::SharedPtr<PTZSpeed> _defaultPTZSpeed;

	//@ mandatory=false
	//@ name=DefaultPTZTimeout
	//@ order=11
	Poco::Optional<std::string> _defaultPTZTimeout;

	//@ mandatory=false
	//@ name=PanTiltLimits
	//@ order=12
	Poco::SharedPtr<PanTiltLimits> _panTiltLimits;

	//@ mandatory=false
	//@ name=ZoomLimits
	//@ order=13
	Poco::SharedPtr<ZoomLimits> _zoomLimits;

	//@ mandatory=false
	//@ name=Extension
	//@ order=14
	Poco::SharedPtr<PTZConfigurationExtension> _extension;

};


inline const Poco::Optional<Poco::URI>& PTZConfiguration::getDefaultAbsolutePantTiltPositionSpace() const
{
	return _defaultAbsolutePantTiltPositionSpace;
}


inline const Poco::Optional<Poco::URI>& PTZConfiguration::getDefaultAbsoluteZoomPositionSpace() const
{
	return _defaultAbsoluteZoomPositionSpace;
}


inline const Poco::Optional<Poco::URI>& PTZConfiguration::getDefaultContinuousPanTiltVelocitySpace() const
{
	return _defaultContinuousPanTiltVelocitySpace;
}


inline const Poco::Optional<Poco::URI>& PTZConfiguration::getDefaultContinuousZoomVelocitySpace() const
{
	return _defaultContinuousZoomVelocitySpace;
}


inline const Poco::SharedPtr<PTZSpeed>& PTZConfiguration::getDefaultPTZSpeed() const
{
	return _defaultPTZSpeed;
}


inline const Poco::Optional<std::string>& PTZConfiguration::getDefaultPTZTimeout() const
{
	return _defaultPTZTimeout;
}


inline const Poco::Optional<Poco::URI>& PTZConfiguration::getDefaultRelativePanTiltTranslationSpace() const
{
	return _defaultRelativePanTiltTranslationSpace;
}


inline const Poco::Optional<Poco::URI>& PTZConfiguration::getDefaultRelativeZoomTranslationSpace() const
{
	return _defaultRelativeZoomTranslationSpace;
}


inline const Poco::SharedPtr<PTZConfigurationExtension>& PTZConfiguration::getExtension() const
{
	return _extension;
}


inline Poco::Optional<int> PTZConfiguration::getMoveRamp() const
{
	return _moveRamp;
}


inline const std::string& PTZConfiguration::getNodeToken() const
{
	return _nodeToken;
}


inline const Poco::SharedPtr<PanTiltLimits>& PTZConfiguration::getPanTiltLimits() const
{
	return _panTiltLimits;
}


inline Poco::Optional<int> PTZConfiguration::getPresetRamp() const
{
	return _presetRamp;
}


inline Poco::Optional<int> PTZConfiguration::getPresetTourRamp() const
{
	return _presetTourRamp;
}


inline const Poco::SharedPtr<ZoomLimits>& PTZConfiguration::getZoomLimits() const
{
	return _zoomLimits;
}


inline void PTZConfiguration::setDefaultAbsolutePantTiltPositionSpace(const Poco::Optional<Poco::URI>& val)
{
	_defaultAbsolutePantTiltPositionSpace = val;
}


inline void PTZConfiguration::setDefaultAbsolutePantTiltPositionSpace(Poco::Optional<Poco::URI>&& val)
{
	_defaultAbsolutePantTiltPositionSpace = std::move(val);
}


inline void PTZConfiguration::setDefaultAbsoluteZoomPositionSpace(const Poco::Optional<Poco::URI>& val)
{
	_defaultAbsoluteZoomPositionSpace = val;
}


inline void PTZConfiguration::setDefaultAbsoluteZoomPositionSpace(Poco::Optional<Poco::URI>&& val)
{
	_defaultAbsoluteZoomPositionSpace = std::move(val);
}


inline void PTZConfiguration::setDefaultContinuousPanTiltVelocitySpace(const Poco::Optional<Poco::URI>& val)
{
	_defaultContinuousPanTiltVelocitySpace = val;
}


inline void PTZConfiguration::setDefaultContinuousPanTiltVelocitySpace(Poco::Optional<Poco::URI>&& val)
{
	_defaultContinuousPanTiltVelocitySpace = std::move(val);
}


inline void PTZConfiguration::setDefaultContinuousZoomVelocitySpace(const Poco::Optional<Poco::URI>& val)
{
	_defaultContinuousZoomVelocitySpace = val;
}


inline void PTZConfiguration::setDefaultContinuousZoomVelocitySpace(Poco::Optional<Poco::URI>&& val)
{
	_defaultContinuousZoomVelocitySpace = std::move(val);
}


inline void PTZConfiguration::setDefaultPTZSpeed(const Poco::SharedPtr<PTZSpeed>& val)
{
	_defaultPTZSpeed = val;
}


inline void PTZConfiguration::setDefaultPTZSpeed(Poco::SharedPtr<PTZSpeed>&& val)
{
	_defaultPTZSpeed = std::move(val);
}


inline void PTZConfiguration::setDefaultPTZTimeout(const Poco::Optional<std::string>& val)
{
	_defaultPTZTimeout = val;
}


inline void PTZConfiguration::setDefaultPTZTimeout(Poco::Optional<std::string>&& val)
{
	_defaultPTZTimeout = std::move(val);
}


inline void PTZConfiguration::setDefaultRelativePanTiltTranslationSpace(const Poco::Optional<Poco::URI>& val)
{
	_defaultRelativePanTiltTranslationSpace = val;
}


inline void PTZConfiguration::setDefaultRelativePanTiltTranslationSpace(Poco::Optional<Poco::URI>&& val)
{
	_defaultRelativePanTiltTranslationSpace = std::move(val);
}


inline void PTZConfiguration::setDefaultRelativeZoomTranslationSpace(const Poco::Optional<Poco::URI>& val)
{
	_defaultRelativeZoomTranslationSpace = val;
}


inline void PTZConfiguration::setDefaultRelativeZoomTranslationSpace(Poco::Optional<Poco::URI>&& val)
{
	_defaultRelativeZoomTranslationSpace = std::move(val);
}


inline void PTZConfiguration::setExtension(const Poco::SharedPtr<PTZConfigurationExtension>& val)
{
	_extension = val;
}


inline void PTZConfiguration::setExtension(Poco::SharedPtr<PTZConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZConfiguration::setMoveRamp(Poco::Optional<int> val)
{
	_moveRamp = val;
}


inline void PTZConfiguration::setNodeToken(const std::string& val)
{
	_nodeToken = val;
}


inline void PTZConfiguration::setNodeToken(std::string&& val)
{
	_nodeToken = std::move(val);
}


inline void PTZConfiguration::setPanTiltLimits(const Poco::SharedPtr<PanTiltLimits>& val)
{
	_panTiltLimits = val;
}


inline void PTZConfiguration::setPanTiltLimits(Poco::SharedPtr<PanTiltLimits>&& val)
{
	_panTiltLimits = std::move(val);
}


inline void PTZConfiguration::setPresetRamp(Poco::Optional<int> val)
{
	_presetRamp = val;
}


inline void PTZConfiguration::setPresetTourRamp(Poco::Optional<int> val)
{
	_presetTourRamp = val;
}


inline void PTZConfiguration::setZoomLimits(const Poco::SharedPtr<ZoomLimits>& val)
{
	_zoomLimits = val;
}


inline void PTZConfiguration::setZoomLimits(Poco::SharedPtr<ZoomLimits>&& val)
{
	_zoomLimits = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZConfigurationExtension.h"
#include "ONVIF/PTZSpeed.h"
#include "ONVIF/PanTiltLimits.h"
#include "ONVIF/ZoomLimits.h"


#endif // ONVIF_PTZConfiguration_INCLUDED
