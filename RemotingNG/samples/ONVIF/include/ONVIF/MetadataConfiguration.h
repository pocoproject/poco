// 
// MetadataConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MetadataConfiguration_INCLUDED
#define ONVIF_MetadataConfiguration_INCLUDED


#include "ONVIF/ConfigurationEntity.h"
#include "ONVIF/MulticastConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AnalyticsEngineConfiguration;
class EventSubscription;
class MetadataConfigurationExtension;
class PTZFilter;
} 


namespace ONVIF {


//@ name=MetadataConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MetadataConfiguration: public ConfigurationEntity
{
public:
	MetadataConfiguration();

	MetadataConfiguration(
		const std::string& token, 
		const std::string& name, 
		int useCount, 
		const Poco::Optional<std::string>& compressionType, 
		Poco::Optional<bool> geoLocation, 
		Poco::Optional<bool> shapePolygon, 
		const Poco::SharedPtr<PTZFilter>& pTZStatus, 
		const Poco::SharedPtr<EventSubscription>& events, 
		Poco::Optional<bool> analytics, 
		const MulticastConfiguration& multicast, 
		const std::string& sessionTimeout, 
		const Poco::SharedPtr<AnalyticsEngineConfiguration>& analyticsEngineConfiguration, 
		const Poco::SharedPtr<MetadataConfigurationExtension>& extension);

	virtual ~MetadataConfiguration();

	Poco::Optional<bool> getAnalytics() const;

	const Poco::SharedPtr<AnalyticsEngineConfiguration>& getAnalyticsEngineConfiguration() const;

	const Poco::Optional<std::string>& getCompressionType() const;

	const Poco::SharedPtr<EventSubscription>& getEvents() const;

	const Poco::SharedPtr<MetadataConfigurationExtension>& getExtension() const;

	Poco::Optional<bool> getGeoLocation() const;

	const MulticastConfiguration& getMulticast() const;

	const Poco::SharedPtr<PTZFilter>& getPTZStatus() const;

	const std::string& getSessionTimeout() const;

	Poco::Optional<bool> getShapePolygon() const;

	void setAnalytics(Poco::Optional<bool> val);

	void setAnalyticsEngineConfiguration(const Poco::SharedPtr<AnalyticsEngineConfiguration>& val);

	void setAnalyticsEngineConfiguration(Poco::SharedPtr<AnalyticsEngineConfiguration>&& val);

	void setCompressionType(const Poco::Optional<std::string>& val);

	void setCompressionType(Poco::Optional<std::string>&& val);

	void setEvents(const Poco::SharedPtr<EventSubscription>& val);

	void setEvents(Poco::SharedPtr<EventSubscription>&& val);

	void setExtension(const Poco::SharedPtr<MetadataConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<MetadataConfigurationExtension>&& val);

	void setGeoLocation(Poco::Optional<bool> val);

	void setMulticast(const MulticastConfiguration& val);

	void setMulticast(MulticastConfiguration&& val);

	void setPTZStatus(const Poco::SharedPtr<PTZFilter>& val);

	void setPTZStatus(Poco::SharedPtr<PTZFilter>&& val);

	void setSessionTimeout(const std::string& val);

	void setSessionTimeout(std::string&& val);

	void setShapePolygon(Poco::Optional<bool> val);

private:
	//@ mandatory=false
	//@ name=CompressionType
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _compressionType;

	//@ mandatory=false
	//@ name=GeoLocation
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _geoLocation;

	//@ mandatory=false
	//@ name=ShapePolygon
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _shapePolygon;

	//@ mandatory=false
	//@ name=PTZStatus
	//@ order=3
	Poco::SharedPtr<PTZFilter> _pTZStatus;

	//@ mandatory=false
	//@ name=Events
	//@ order=4
	Poco::SharedPtr<EventSubscription> _events;

	//@ mandatory=false
	//@ name=Analytics
	//@ order=5
	Poco::Optional<bool> _analytics;

	//@ name=Multicast
	//@ order=6
	MulticastConfiguration _multicast;

	//@ name=SessionTimeout
	//@ order=7
	std::string _sessionTimeout;

	//@ mandatory=false
	//@ name=AnalyticsEngineConfiguration
	//@ order=8
	Poco::SharedPtr<AnalyticsEngineConfiguration> _analyticsEngineConfiguration;

	//@ mandatory=false
	//@ name=Extension
	//@ order=9
	Poco::SharedPtr<MetadataConfigurationExtension> _extension;

};


inline Poco::Optional<bool> MetadataConfiguration::getAnalytics() const
{
	return _analytics;
}


inline const Poco::SharedPtr<AnalyticsEngineConfiguration>& MetadataConfiguration::getAnalyticsEngineConfiguration() const
{
	return _analyticsEngineConfiguration;
}


inline const Poco::Optional<std::string>& MetadataConfiguration::getCompressionType() const
{
	return _compressionType;
}


inline const Poco::SharedPtr<EventSubscription>& MetadataConfiguration::getEvents() const
{
	return _events;
}


inline const Poco::SharedPtr<MetadataConfigurationExtension>& MetadataConfiguration::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> MetadataConfiguration::getGeoLocation() const
{
	return _geoLocation;
}


inline const MulticastConfiguration& MetadataConfiguration::getMulticast() const
{
	return _multicast;
}


inline const Poco::SharedPtr<PTZFilter>& MetadataConfiguration::getPTZStatus() const
{
	return _pTZStatus;
}


inline const std::string& MetadataConfiguration::getSessionTimeout() const
{
	return _sessionTimeout;
}


inline Poco::Optional<bool> MetadataConfiguration::getShapePolygon() const
{
	return _shapePolygon;
}


inline void MetadataConfiguration::setAnalytics(Poco::Optional<bool> val)
{
	_analytics = val;
}


inline void MetadataConfiguration::setAnalyticsEngineConfiguration(const Poco::SharedPtr<AnalyticsEngineConfiguration>& val)
{
	_analyticsEngineConfiguration = val;
}


inline void MetadataConfiguration::setAnalyticsEngineConfiguration(Poco::SharedPtr<AnalyticsEngineConfiguration>&& val)
{
	_analyticsEngineConfiguration = std::move(val);
}


inline void MetadataConfiguration::setCompressionType(const Poco::Optional<std::string>& val)
{
	_compressionType = val;
}


inline void MetadataConfiguration::setCompressionType(Poco::Optional<std::string>&& val)
{
	_compressionType = std::move(val);
}


inline void MetadataConfiguration::setEvents(const Poco::SharedPtr<EventSubscription>& val)
{
	_events = val;
}


inline void MetadataConfiguration::setEvents(Poco::SharedPtr<EventSubscription>&& val)
{
	_events = std::move(val);
}


inline void MetadataConfiguration::setExtension(const Poco::SharedPtr<MetadataConfigurationExtension>& val)
{
	_extension = val;
}


inline void MetadataConfiguration::setExtension(Poco::SharedPtr<MetadataConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void MetadataConfiguration::setGeoLocation(Poco::Optional<bool> val)
{
	_geoLocation = val;
}


inline void MetadataConfiguration::setMulticast(const MulticastConfiguration& val)
{
	_multicast = val;
}


inline void MetadataConfiguration::setMulticast(MulticastConfiguration&& val)
{
	_multicast = std::move(val);
}


inline void MetadataConfiguration::setPTZStatus(const Poco::SharedPtr<PTZFilter>& val)
{
	_pTZStatus = val;
}


inline void MetadataConfiguration::setPTZStatus(Poco::SharedPtr<PTZFilter>&& val)
{
	_pTZStatus = std::move(val);
}


inline void MetadataConfiguration::setSessionTimeout(const std::string& val)
{
	_sessionTimeout = val;
}


inline void MetadataConfiguration::setSessionTimeout(std::string&& val)
{
	_sessionTimeout = std::move(val);
}


inline void MetadataConfiguration::setShapePolygon(Poco::Optional<bool> val)
{
	_shapePolygon = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AnalyticsEngineConfiguration.h"
#include "ONVIF/EventSubscription.h"
#include "ONVIF/MetadataConfigurationExtension.h"
#include "ONVIF/PTZFilter.h"


#endif // ONVIF_MetadataConfiguration_INCLUDED
