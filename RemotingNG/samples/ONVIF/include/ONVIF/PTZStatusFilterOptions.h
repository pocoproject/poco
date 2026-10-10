// 
// PTZStatusFilterOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZStatusFilterOptions_INCLUDED
#define ONVIF_PTZStatusFilterOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZStatusFilterOptionsExtension;
} 


namespace ONVIF {


//@ name=PTZStatusFilterOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZStatusFilterOptions
{
public:
	PTZStatusFilterOptions();

	PTZStatusFilterOptions(
		bool panTiltStatusSupported, 
		bool zoomStatusSupported, 
		Poco::Optional<bool> panTiltPositionSupported, 
		Poco::Optional<bool> zoomPositionSupported, 
		const Poco::SharedPtr<PTZStatusFilterOptionsExtension>& extension);

	virtual ~PTZStatusFilterOptions();

	const Poco::SharedPtr<PTZStatusFilterOptionsExtension>& getExtension() const;

	Poco::Optional<bool> getPanTiltPositionSupported() const;

	bool getPanTiltStatusSupported() const;

	Poco::Optional<bool> getZoomPositionSupported() const;

	bool getZoomStatusSupported() const;

	void setExtension(const Poco::SharedPtr<PTZStatusFilterOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<PTZStatusFilterOptionsExtension>&& val);

	void setPanTiltPositionSupported(Poco::Optional<bool> val);

	void setPanTiltStatusSupported(bool val);

	void setZoomPositionSupported(Poco::Optional<bool> val);

	void setZoomStatusSupported(bool val);

private:
	//@ name=PanTiltStatusSupported
	//@ order=0
	bool _panTiltStatusSupported;

	//@ name=ZoomStatusSupported
	//@ order=1
	bool _zoomStatusSupported;

	//@ mandatory=false
	//@ name=PanTiltPositionSupported
	//@ order=2
	Poco::Optional<bool> _panTiltPositionSupported;

	//@ mandatory=false
	//@ name=ZoomPositionSupported
	//@ order=3
	Poco::Optional<bool> _zoomPositionSupported;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<PTZStatusFilterOptionsExtension> _extension;

};


inline const Poco::SharedPtr<PTZStatusFilterOptionsExtension>& PTZStatusFilterOptions::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> PTZStatusFilterOptions::getPanTiltPositionSupported() const
{
	return _panTiltPositionSupported;
}


inline bool PTZStatusFilterOptions::getPanTiltStatusSupported() const
{
	return _panTiltStatusSupported;
}


inline Poco::Optional<bool> PTZStatusFilterOptions::getZoomPositionSupported() const
{
	return _zoomPositionSupported;
}


inline bool PTZStatusFilterOptions::getZoomStatusSupported() const
{
	return _zoomStatusSupported;
}


inline void PTZStatusFilterOptions::setExtension(const Poco::SharedPtr<PTZStatusFilterOptionsExtension>& val)
{
	_extension = val;
}


inline void PTZStatusFilterOptions::setExtension(Poco::SharedPtr<PTZStatusFilterOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZStatusFilterOptions::setPanTiltPositionSupported(Poco::Optional<bool> val)
{
	_panTiltPositionSupported = val;
}


inline void PTZStatusFilterOptions::setPanTiltStatusSupported(bool val)
{
	_panTiltStatusSupported = val;
}


inline void PTZStatusFilterOptions::setZoomPositionSupported(Poco::Optional<bool> val)
{
	_zoomPositionSupported = val;
}


inline void PTZStatusFilterOptions::setZoomStatusSupported(bool val)
{
	_zoomStatusSupported = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZStatusFilterOptionsExtension.h"


#endif // ONVIF_PTZStatusFilterOptions_INCLUDED
