// 
// PTZSpaces.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZSpaces_INCLUDED
#define ONVIF_PTZSpaces_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Space1DDescription.h"
#include "ONVIF/Space2DDescription.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZSpacesExtension;
} 


namespace ONVIF {


//@ name=PTZSpaces
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZSpaces
{
public:
	PTZSpaces();

	PTZSpaces(
		const std::vector<Space2DDescription>& absolutePanTiltPositionSpace, 
		const std::vector<Space1DDescription>& absoluteZoomPositionSpace, 
		const std::vector<Space2DDescription>& relativePanTiltTranslationSpace, 
		const std::vector<Space1DDescription>& relativeZoomTranslationSpace, 
		const std::vector<Space2DDescription>& continuousPanTiltVelocitySpace, 
		const std::vector<Space1DDescription>& continuousZoomVelocitySpace, 
		const std::vector<Space1DDescription>& panTiltSpeedSpace, 
		const std::vector<Space1DDescription>& zoomSpeedSpace, 
		const Poco::SharedPtr<PTZSpacesExtension>& extension);

	virtual ~PTZSpaces();

	const std::vector<Space2DDescription>& getAbsolutePanTiltPositionSpace() const;

	std::vector<Space2DDescription>& getAbsolutePanTiltPositionSpace();

	const std::vector<Space1DDescription>& getAbsoluteZoomPositionSpace() const;

	std::vector<Space1DDescription>& getAbsoluteZoomPositionSpace();

	const std::vector<Space2DDescription>& getContinuousPanTiltVelocitySpace() const;

	std::vector<Space2DDescription>& getContinuousPanTiltVelocitySpace();

	const std::vector<Space1DDescription>& getContinuousZoomVelocitySpace() const;

	std::vector<Space1DDescription>& getContinuousZoomVelocitySpace();

	const Poco::SharedPtr<PTZSpacesExtension>& getExtension() const;

	const std::vector<Space1DDescription>& getPanTiltSpeedSpace() const;

	std::vector<Space1DDescription>& getPanTiltSpeedSpace();

	const std::vector<Space2DDescription>& getRelativePanTiltTranslationSpace() const;

	std::vector<Space2DDescription>& getRelativePanTiltTranslationSpace();

	const std::vector<Space1DDescription>& getRelativeZoomTranslationSpace() const;

	std::vector<Space1DDescription>& getRelativeZoomTranslationSpace();

	const std::vector<Space1DDescription>& getZoomSpeedSpace() const;

	std::vector<Space1DDescription>& getZoomSpeedSpace();

	void setAbsolutePanTiltPositionSpace(const std::vector<Space2DDescription>& val);

	void setAbsolutePanTiltPositionSpace(std::vector<Space2DDescription>&& val);

	void setAbsoluteZoomPositionSpace(const std::vector<Space1DDescription>& val);

	void setAbsoluteZoomPositionSpace(std::vector<Space1DDescription>&& val);

	void setContinuousPanTiltVelocitySpace(const std::vector<Space2DDescription>& val);

	void setContinuousPanTiltVelocitySpace(std::vector<Space2DDescription>&& val);

	void setContinuousZoomVelocitySpace(const std::vector<Space1DDescription>& val);

	void setContinuousZoomVelocitySpace(std::vector<Space1DDescription>&& val);

	void setExtension(const Poco::SharedPtr<PTZSpacesExtension>& val);

	void setExtension(Poco::SharedPtr<PTZSpacesExtension>&& val);

	void setPanTiltSpeedSpace(const std::vector<Space1DDescription>& val);

	void setPanTiltSpeedSpace(std::vector<Space1DDescription>&& val);

	void setRelativePanTiltTranslationSpace(const std::vector<Space2DDescription>& val);

	void setRelativePanTiltTranslationSpace(std::vector<Space2DDescription>&& val);

	void setRelativeZoomTranslationSpace(const std::vector<Space1DDescription>& val);

	void setRelativeZoomTranslationSpace(std::vector<Space1DDescription>&& val);

	void setZoomSpeedSpace(const std::vector<Space1DDescription>& val);

	void setZoomSpeedSpace(std::vector<Space1DDescription>&& val);

private:
	//@ mandatory=false
	//@ name=AbsolutePanTiltPositionSpace
	//@ order=0
	std::vector<Space2DDescription> _absolutePanTiltPositionSpace;

	//@ mandatory=false
	//@ name=AbsoluteZoomPositionSpace
	//@ order=1
	std::vector<Space1DDescription> _absoluteZoomPositionSpace;

	//@ mandatory=false
	//@ name=RelativePanTiltTranslationSpace
	//@ order=2
	std::vector<Space2DDescription> _relativePanTiltTranslationSpace;

	//@ mandatory=false
	//@ name=RelativeZoomTranslationSpace
	//@ order=3
	std::vector<Space1DDescription> _relativeZoomTranslationSpace;

	//@ mandatory=false
	//@ name=ContinuousPanTiltVelocitySpace
	//@ order=4
	std::vector<Space2DDescription> _continuousPanTiltVelocitySpace;

	//@ mandatory=false
	//@ name=ContinuousZoomVelocitySpace
	//@ order=5
	std::vector<Space1DDescription> _continuousZoomVelocitySpace;

	//@ mandatory=false
	//@ name=PanTiltSpeedSpace
	//@ order=6
	std::vector<Space1DDescription> _panTiltSpeedSpace;

	//@ mandatory=false
	//@ name=ZoomSpeedSpace
	//@ order=7
	std::vector<Space1DDescription> _zoomSpeedSpace;

	//@ mandatory=false
	//@ name=Extension
	//@ order=8
	Poco::SharedPtr<PTZSpacesExtension> _extension;

};


inline const std::vector<Space2DDescription>& PTZSpaces::getAbsolutePanTiltPositionSpace() const
{
	return _absolutePanTiltPositionSpace;
}


inline std::vector<Space2DDescription>& PTZSpaces::getAbsolutePanTiltPositionSpace()
{
	return _absolutePanTiltPositionSpace;
}


inline const std::vector<Space1DDescription>& PTZSpaces::getAbsoluteZoomPositionSpace() const
{
	return _absoluteZoomPositionSpace;
}


inline std::vector<Space1DDescription>& PTZSpaces::getAbsoluteZoomPositionSpace()
{
	return _absoluteZoomPositionSpace;
}


inline const std::vector<Space2DDescription>& PTZSpaces::getContinuousPanTiltVelocitySpace() const
{
	return _continuousPanTiltVelocitySpace;
}


inline std::vector<Space2DDescription>& PTZSpaces::getContinuousPanTiltVelocitySpace()
{
	return _continuousPanTiltVelocitySpace;
}


inline const std::vector<Space1DDescription>& PTZSpaces::getContinuousZoomVelocitySpace() const
{
	return _continuousZoomVelocitySpace;
}


inline std::vector<Space1DDescription>& PTZSpaces::getContinuousZoomVelocitySpace()
{
	return _continuousZoomVelocitySpace;
}


inline const Poco::SharedPtr<PTZSpacesExtension>& PTZSpaces::getExtension() const
{
	return _extension;
}


inline const std::vector<Space1DDescription>& PTZSpaces::getPanTiltSpeedSpace() const
{
	return _panTiltSpeedSpace;
}


inline std::vector<Space1DDescription>& PTZSpaces::getPanTiltSpeedSpace()
{
	return _panTiltSpeedSpace;
}


inline const std::vector<Space2DDescription>& PTZSpaces::getRelativePanTiltTranslationSpace() const
{
	return _relativePanTiltTranslationSpace;
}


inline std::vector<Space2DDescription>& PTZSpaces::getRelativePanTiltTranslationSpace()
{
	return _relativePanTiltTranslationSpace;
}


inline const std::vector<Space1DDescription>& PTZSpaces::getRelativeZoomTranslationSpace() const
{
	return _relativeZoomTranslationSpace;
}


inline std::vector<Space1DDescription>& PTZSpaces::getRelativeZoomTranslationSpace()
{
	return _relativeZoomTranslationSpace;
}


inline const std::vector<Space1DDescription>& PTZSpaces::getZoomSpeedSpace() const
{
	return _zoomSpeedSpace;
}


inline std::vector<Space1DDescription>& PTZSpaces::getZoomSpeedSpace()
{
	return _zoomSpeedSpace;
}


inline void PTZSpaces::setAbsolutePanTiltPositionSpace(const std::vector<Space2DDescription>& val)
{
	_absolutePanTiltPositionSpace = val;
}


inline void PTZSpaces::setAbsolutePanTiltPositionSpace(std::vector<Space2DDescription>&& val)
{
	_absolutePanTiltPositionSpace = std::move(val);
}


inline void PTZSpaces::setAbsoluteZoomPositionSpace(const std::vector<Space1DDescription>& val)
{
	_absoluteZoomPositionSpace = val;
}


inline void PTZSpaces::setAbsoluteZoomPositionSpace(std::vector<Space1DDescription>&& val)
{
	_absoluteZoomPositionSpace = std::move(val);
}


inline void PTZSpaces::setContinuousPanTiltVelocitySpace(const std::vector<Space2DDescription>& val)
{
	_continuousPanTiltVelocitySpace = val;
}


inline void PTZSpaces::setContinuousPanTiltVelocitySpace(std::vector<Space2DDescription>&& val)
{
	_continuousPanTiltVelocitySpace = std::move(val);
}


inline void PTZSpaces::setContinuousZoomVelocitySpace(const std::vector<Space1DDescription>& val)
{
	_continuousZoomVelocitySpace = val;
}


inline void PTZSpaces::setContinuousZoomVelocitySpace(std::vector<Space1DDescription>&& val)
{
	_continuousZoomVelocitySpace = std::move(val);
}


inline void PTZSpaces::setExtension(const Poco::SharedPtr<PTZSpacesExtension>& val)
{
	_extension = val;
}


inline void PTZSpaces::setExtension(Poco::SharedPtr<PTZSpacesExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZSpaces::setPanTiltSpeedSpace(const std::vector<Space1DDescription>& val)
{
	_panTiltSpeedSpace = val;
}


inline void PTZSpaces::setPanTiltSpeedSpace(std::vector<Space1DDescription>&& val)
{
	_panTiltSpeedSpace = std::move(val);
}


inline void PTZSpaces::setRelativePanTiltTranslationSpace(const std::vector<Space2DDescription>& val)
{
	_relativePanTiltTranslationSpace = val;
}


inline void PTZSpaces::setRelativePanTiltTranslationSpace(std::vector<Space2DDescription>&& val)
{
	_relativePanTiltTranslationSpace = std::move(val);
}


inline void PTZSpaces::setRelativeZoomTranslationSpace(const std::vector<Space1DDescription>& val)
{
	_relativeZoomTranslationSpace = val;
}


inline void PTZSpaces::setRelativeZoomTranslationSpace(std::vector<Space1DDescription>&& val)
{
	_relativeZoomTranslationSpace = std::move(val);
}


inline void PTZSpaces::setZoomSpeedSpace(const std::vector<Space1DDescription>& val)
{
	_zoomSpeedSpace = val;
}


inline void PTZSpaces::setZoomSpeedSpace(std::vector<Space1DDescription>&& val)
{
	_zoomSpeedSpace = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZSpacesExtension.h"


#endif // ONVIF_PTZSpaces_INCLUDED
