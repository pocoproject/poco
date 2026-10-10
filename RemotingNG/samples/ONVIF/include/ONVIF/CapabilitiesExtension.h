// 
// CapabilitiesExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CapabilitiesExtension_INCLUDED
#define ONVIF_CapabilitiesExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AnalyticsDeviceCapabilities;
class CapabilitiesExtension2;
class DeviceIOCapabilities;
class DisplayCapabilities;
class ReceiverCapabilities;
class RecordingCapabilities;
class ReplayCapabilities;
class SearchCapabilities;
} 


namespace ONVIF {


//@ name=CapabilitiesExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CapabilitiesExtension
{
public:
	CapabilitiesExtension();

	CapabilitiesExtension(
		const Poco::SharedPtr<DeviceIOCapabilities>& deviceIO, 
		const Poco::SharedPtr<DisplayCapabilities>& display, 
		const Poco::SharedPtr<RecordingCapabilities>& recording, 
		const Poco::SharedPtr<SearchCapabilities>& search, 
		const Poco::SharedPtr<ReplayCapabilities>& replay, 
		const Poco::SharedPtr<ReceiverCapabilities>& receiver, 
		const Poco::SharedPtr<AnalyticsDeviceCapabilities>& analyticsDevice, 
		const Poco::SharedPtr<CapabilitiesExtension2>& extensions);

	virtual ~CapabilitiesExtension();

	const Poco::SharedPtr<AnalyticsDeviceCapabilities>& getAnalyticsDevice() const;

	const Poco::SharedPtr<DeviceIOCapabilities>& getDeviceIO() const;

	const Poco::SharedPtr<DisplayCapabilities>& getDisplay() const;

	const Poco::SharedPtr<CapabilitiesExtension2>& getExtensions() const;

	const Poco::SharedPtr<ReceiverCapabilities>& getReceiver() const;

	const Poco::SharedPtr<RecordingCapabilities>& getRecording() const;

	const Poco::SharedPtr<ReplayCapabilities>& getReplay() const;

	const Poco::SharedPtr<SearchCapabilities>& getSearch() const;

	void setAnalyticsDevice(const Poco::SharedPtr<AnalyticsDeviceCapabilities>& val);

	void setAnalyticsDevice(Poco::SharedPtr<AnalyticsDeviceCapabilities>&& val);

	void setDeviceIO(const Poco::SharedPtr<DeviceIOCapabilities>& val);

	void setDeviceIO(Poco::SharedPtr<DeviceIOCapabilities>&& val);

	void setDisplay(const Poco::SharedPtr<DisplayCapabilities>& val);

	void setDisplay(Poco::SharedPtr<DisplayCapabilities>&& val);

	void setExtensions(const Poco::SharedPtr<CapabilitiesExtension2>& val);

	void setExtensions(Poco::SharedPtr<CapabilitiesExtension2>&& val);

	void setReceiver(const Poco::SharedPtr<ReceiverCapabilities>& val);

	void setReceiver(Poco::SharedPtr<ReceiverCapabilities>&& val);

	void setRecording(const Poco::SharedPtr<RecordingCapabilities>& val);

	void setRecording(Poco::SharedPtr<RecordingCapabilities>&& val);

	void setReplay(const Poco::SharedPtr<ReplayCapabilities>& val);

	void setReplay(Poco::SharedPtr<ReplayCapabilities>&& val);

	void setSearch(const Poco::SharedPtr<SearchCapabilities>& val);

	void setSearch(Poco::SharedPtr<SearchCapabilities>&& val);

private:
	//@ mandatory=false
	//@ name=DeviceIO
	//@ order=0
	Poco::SharedPtr<DeviceIOCapabilities> _deviceIO;

	//@ mandatory=false
	//@ name=Display
	//@ order=1
	Poco::SharedPtr<DisplayCapabilities> _display;

	//@ mandatory=false
	//@ name=Recording
	//@ order=2
	Poco::SharedPtr<RecordingCapabilities> _recording;

	//@ mandatory=false
	//@ name=Search
	//@ order=3
	Poco::SharedPtr<SearchCapabilities> _search;

	//@ mandatory=false
	//@ name=Replay
	//@ order=4
	Poco::SharedPtr<ReplayCapabilities> _replay;

	//@ mandatory=false
	//@ name=Receiver
	//@ order=5
	Poco::SharedPtr<ReceiverCapabilities> _receiver;

	//@ mandatory=false
	//@ name=AnalyticsDevice
	//@ order=6
	Poco::SharedPtr<AnalyticsDeviceCapabilities> _analyticsDevice;

	//@ mandatory=false
	//@ name=Extensions
	//@ order=7
	Poco::SharedPtr<CapabilitiesExtension2> _extensions;

};


inline const Poco::SharedPtr<AnalyticsDeviceCapabilities>& CapabilitiesExtension::getAnalyticsDevice() const
{
	return _analyticsDevice;
}


inline const Poco::SharedPtr<DeviceIOCapabilities>& CapabilitiesExtension::getDeviceIO() const
{
	return _deviceIO;
}


inline const Poco::SharedPtr<DisplayCapabilities>& CapabilitiesExtension::getDisplay() const
{
	return _display;
}


inline const Poco::SharedPtr<CapabilitiesExtension2>& CapabilitiesExtension::getExtensions() const
{
	return _extensions;
}


inline const Poco::SharedPtr<ReceiverCapabilities>& CapabilitiesExtension::getReceiver() const
{
	return _receiver;
}


inline const Poco::SharedPtr<RecordingCapabilities>& CapabilitiesExtension::getRecording() const
{
	return _recording;
}


inline const Poco::SharedPtr<ReplayCapabilities>& CapabilitiesExtension::getReplay() const
{
	return _replay;
}


inline const Poco::SharedPtr<SearchCapabilities>& CapabilitiesExtension::getSearch() const
{
	return _search;
}


inline void CapabilitiesExtension::setAnalyticsDevice(const Poco::SharedPtr<AnalyticsDeviceCapabilities>& val)
{
	_analyticsDevice = val;
}


inline void CapabilitiesExtension::setAnalyticsDevice(Poco::SharedPtr<AnalyticsDeviceCapabilities>&& val)
{
	_analyticsDevice = std::move(val);
}


inline void CapabilitiesExtension::setDeviceIO(const Poco::SharedPtr<DeviceIOCapabilities>& val)
{
	_deviceIO = val;
}


inline void CapabilitiesExtension::setDeviceIO(Poco::SharedPtr<DeviceIOCapabilities>&& val)
{
	_deviceIO = std::move(val);
}


inline void CapabilitiesExtension::setDisplay(const Poco::SharedPtr<DisplayCapabilities>& val)
{
	_display = val;
}


inline void CapabilitiesExtension::setDisplay(Poco::SharedPtr<DisplayCapabilities>&& val)
{
	_display = std::move(val);
}


inline void CapabilitiesExtension::setExtensions(const Poco::SharedPtr<CapabilitiesExtension2>& val)
{
	_extensions = val;
}


inline void CapabilitiesExtension::setExtensions(Poco::SharedPtr<CapabilitiesExtension2>&& val)
{
	_extensions = std::move(val);
}


inline void CapabilitiesExtension::setReceiver(const Poco::SharedPtr<ReceiverCapabilities>& val)
{
	_receiver = val;
}


inline void CapabilitiesExtension::setReceiver(Poco::SharedPtr<ReceiverCapabilities>&& val)
{
	_receiver = std::move(val);
}


inline void CapabilitiesExtension::setRecording(const Poco::SharedPtr<RecordingCapabilities>& val)
{
	_recording = val;
}


inline void CapabilitiesExtension::setRecording(Poco::SharedPtr<RecordingCapabilities>&& val)
{
	_recording = std::move(val);
}


inline void CapabilitiesExtension::setReplay(const Poco::SharedPtr<ReplayCapabilities>& val)
{
	_replay = val;
}


inline void CapabilitiesExtension::setReplay(Poco::SharedPtr<ReplayCapabilities>&& val)
{
	_replay = std::move(val);
}


inline void CapabilitiesExtension::setSearch(const Poco::SharedPtr<SearchCapabilities>& val)
{
	_search = val;
}


inline void CapabilitiesExtension::setSearch(Poco::SharedPtr<SearchCapabilities>&& val)
{
	_search = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AnalyticsDeviceCapabilities.h"
#include "ONVIF/CapabilitiesExtension2.h"
#include "ONVIF/DeviceIOCapabilities.h"
#include "ONVIF/DisplayCapabilities.h"
#include "ONVIF/ReceiverCapabilities.h"
#include "ONVIF/RecordingCapabilities.h"
#include "ONVIF/ReplayCapabilities.h"
#include "ONVIF/SearchCapabilities.h"


#endif // ONVIF_CapabilitiesExtension_INCLUDED
