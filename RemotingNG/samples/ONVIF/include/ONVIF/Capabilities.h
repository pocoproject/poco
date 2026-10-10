// 
// Capabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Capabilities_INCLUDED
#define ONVIF_Capabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AnalyticsCapabilities;
class CapabilitiesExtension;
class DeviceCapabilities;
class EventCapabilities;
class ImagingCapabilities;
class MediaCapabilities;
class PTZCapabilities;
} 


namespace ONVIF {


//@ name=Capabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Capabilities
{
public:
	Capabilities();

	Capabilities(
		const Poco::SharedPtr<AnalyticsCapabilities>& analytics, 
		const Poco::SharedPtr<DeviceCapabilities>& device, 
		const Poco::SharedPtr<EventCapabilities>& events, 
		const Poco::SharedPtr<ImagingCapabilities>& imaging, 
		const Poco::SharedPtr<MediaCapabilities>& media, 
		const Poco::SharedPtr<PTZCapabilities>& pTZ, 
		const Poco::SharedPtr<CapabilitiesExtension>& extension);

	virtual ~Capabilities();

	const Poco::SharedPtr<AnalyticsCapabilities>& getAnalytics() const;

	const Poco::SharedPtr<DeviceCapabilities>& getDevice() const;

	const Poco::SharedPtr<EventCapabilities>& getEvents() const;

	const Poco::SharedPtr<CapabilitiesExtension>& getExtension() const;

	const Poco::SharedPtr<ImagingCapabilities>& getImaging() const;

	const Poco::SharedPtr<MediaCapabilities>& getMedia() const;

	const Poco::SharedPtr<PTZCapabilities>& getPTZ() const;

	void setAnalytics(const Poco::SharedPtr<AnalyticsCapabilities>& val);

	void setAnalytics(Poco::SharedPtr<AnalyticsCapabilities>&& val);

	void setDevice(const Poco::SharedPtr<DeviceCapabilities>& val);

	void setDevice(Poco::SharedPtr<DeviceCapabilities>&& val);

	void setEvents(const Poco::SharedPtr<EventCapabilities>& val);

	void setEvents(Poco::SharedPtr<EventCapabilities>&& val);

	void setExtension(const Poco::SharedPtr<CapabilitiesExtension>& val);

	void setExtension(Poco::SharedPtr<CapabilitiesExtension>&& val);

	void setImaging(const Poco::SharedPtr<ImagingCapabilities>& val);

	void setImaging(Poco::SharedPtr<ImagingCapabilities>&& val);

	void setMedia(const Poco::SharedPtr<MediaCapabilities>& val);

	void setMedia(Poco::SharedPtr<MediaCapabilities>&& val);

	void setPTZ(const Poco::SharedPtr<PTZCapabilities>& val);

	void setPTZ(Poco::SharedPtr<PTZCapabilities>&& val);

private:
	//@ mandatory=false
	//@ name=Analytics
	//@ order=0
	Poco::SharedPtr<AnalyticsCapabilities> _analytics;

	//@ mandatory=false
	//@ name=Device
	//@ order=1
	Poco::SharedPtr<DeviceCapabilities> _device;

	//@ mandatory=false
	//@ name=Events
	//@ order=2
	Poco::SharedPtr<EventCapabilities> _events;

	//@ mandatory=false
	//@ name=Imaging
	//@ order=3
	Poco::SharedPtr<ImagingCapabilities> _imaging;

	//@ mandatory=false
	//@ name=Media
	//@ order=4
	Poco::SharedPtr<MediaCapabilities> _media;

	//@ mandatory=false
	//@ name=PTZ
	//@ order=5
	Poco::SharedPtr<PTZCapabilities> _pTZ;

	//@ mandatory=false
	//@ name=Extension
	//@ order=6
	Poco::SharedPtr<CapabilitiesExtension> _extension;

};


inline const Poco::SharedPtr<AnalyticsCapabilities>& Capabilities::getAnalytics() const
{
	return _analytics;
}


inline const Poco::SharedPtr<DeviceCapabilities>& Capabilities::getDevice() const
{
	return _device;
}


inline const Poco::SharedPtr<EventCapabilities>& Capabilities::getEvents() const
{
	return _events;
}


inline const Poco::SharedPtr<CapabilitiesExtension>& Capabilities::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<ImagingCapabilities>& Capabilities::getImaging() const
{
	return _imaging;
}


inline const Poco::SharedPtr<MediaCapabilities>& Capabilities::getMedia() const
{
	return _media;
}


inline const Poco::SharedPtr<PTZCapabilities>& Capabilities::getPTZ() const
{
	return _pTZ;
}


inline void Capabilities::setAnalytics(const Poco::SharedPtr<AnalyticsCapabilities>& val)
{
	_analytics = val;
}


inline void Capabilities::setAnalytics(Poco::SharedPtr<AnalyticsCapabilities>&& val)
{
	_analytics = std::move(val);
}


inline void Capabilities::setDevice(const Poco::SharedPtr<DeviceCapabilities>& val)
{
	_device = val;
}


inline void Capabilities::setDevice(Poco::SharedPtr<DeviceCapabilities>&& val)
{
	_device = std::move(val);
}


inline void Capabilities::setEvents(const Poco::SharedPtr<EventCapabilities>& val)
{
	_events = val;
}


inline void Capabilities::setEvents(Poco::SharedPtr<EventCapabilities>&& val)
{
	_events = std::move(val);
}


inline void Capabilities::setExtension(const Poco::SharedPtr<CapabilitiesExtension>& val)
{
	_extension = val;
}


inline void Capabilities::setExtension(Poco::SharedPtr<CapabilitiesExtension>&& val)
{
	_extension = std::move(val);
}


inline void Capabilities::setImaging(const Poco::SharedPtr<ImagingCapabilities>& val)
{
	_imaging = val;
}


inline void Capabilities::setImaging(Poco::SharedPtr<ImagingCapabilities>&& val)
{
	_imaging = std::move(val);
}


inline void Capabilities::setMedia(const Poco::SharedPtr<MediaCapabilities>& val)
{
	_media = val;
}


inline void Capabilities::setMedia(Poco::SharedPtr<MediaCapabilities>&& val)
{
	_media = std::move(val);
}


inline void Capabilities::setPTZ(const Poco::SharedPtr<PTZCapabilities>& val)
{
	_pTZ = val;
}


inline void Capabilities::setPTZ(Poco::SharedPtr<PTZCapabilities>&& val)
{
	_pTZ = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AnalyticsCapabilities.h"
#include "ONVIF/CapabilitiesExtension.h"
#include "ONVIF/DeviceCapabilities.h"
#include "ONVIF/EventCapabilities.h"
#include "ONVIF/ImagingCapabilities.h"
#include "ONVIF/MediaCapabilities.h"
#include "ONVIF/PTZCapabilities.h"


#endif // ONVIF_Capabilities_INCLUDED
