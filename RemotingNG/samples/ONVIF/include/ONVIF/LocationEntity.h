// 
// LocationEntity.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_LocationEntity_INCLUDED
#define ONVIF_LocationEntity_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class GeoLocation;
class GeoOrientation;
class LocalLocation;
class LocalOrientation;
} 


namespace ONVIF {


//@ name=LocationEntity
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API LocationEntity
{
public:
	LocationEntity();

	LocationEntity(
		const Poco::Optional<std::string>& entity, 
		const Poco::Optional<std::string>& token, 
		Poco::Optional<bool> fixed, 
		const Poco::Optional<Poco::URI>& geoSource, 
		Poco::Optional<bool> autoGeo, 
		const Poco::SharedPtr<GeoLocation>& geoLocation, 
		const Poco::SharedPtr<GeoOrientation>& geoOrientation, 
		const Poco::SharedPtr<LocalLocation>& localLocation, 
		const Poco::SharedPtr<LocalOrientation>& localOrientation);

	virtual ~LocationEntity();

	Poco::Optional<bool> getAutoGeo() const;

	const Poco::Optional<std::string>& getEntity() const;

	Poco::Optional<bool> getFixed() const;

	const Poco::SharedPtr<GeoLocation>& getGeoLocation() const;

	const Poco::SharedPtr<GeoOrientation>& getGeoOrientation() const;

	const Poco::Optional<Poco::URI>& getGeoSource() const;

	const Poco::SharedPtr<LocalLocation>& getLocalLocation() const;

	const Poco::SharedPtr<LocalOrientation>& getLocalOrientation() const;

	const Poco::Optional<std::string>& getToken() const;

	void setAutoGeo(Poco::Optional<bool> val);

	void setEntity(const Poco::Optional<std::string>& val);

	void setEntity(Poco::Optional<std::string>&& val);

	void setFixed(Poco::Optional<bool> val);

	void setGeoLocation(const Poco::SharedPtr<GeoLocation>& val);

	void setGeoLocation(Poco::SharedPtr<GeoLocation>&& val);

	void setGeoOrientation(const Poco::SharedPtr<GeoOrientation>& val);

	void setGeoOrientation(Poco::SharedPtr<GeoOrientation>&& val);

	void setGeoSource(const Poco::Optional<Poco::URI>& val);

	void setGeoSource(Poco::Optional<Poco::URI>&& val);

	void setLocalLocation(const Poco::SharedPtr<LocalLocation>& val);

	void setLocalLocation(Poco::SharedPtr<LocalLocation>&& val);

	void setLocalOrientation(const Poco::SharedPtr<LocalOrientation>& val);

	void setLocalOrientation(Poco::SharedPtr<LocalOrientation>&& val);

	void setToken(const Poco::Optional<std::string>& val);

	void setToken(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=Entity
	//@ order=0
	//@ type=attr
	Poco::Optional<std::string> _entity;

	//@ mandatory=false
	//@ name=Token
	//@ order=1
	//@ type=attr
	Poco::Optional<std::string> _token;

	//@ mandatory=false
	//@ name=Fixed
	//@ order=2
	//@ type=attr
	Poco::Optional<bool> _fixed;

	//@ mandatory=false
	//@ name=GeoSource
	//@ order=3
	//@ type=attr
	Poco::Optional<Poco::URI> _geoSource;

	//@ mandatory=false
	//@ name=AutoGeo
	//@ order=4
	//@ type=attr
	Poco::Optional<bool> _autoGeo;

	//@ mandatory=false
	//@ name=GeoLocation
	//@ order=5
	Poco::SharedPtr<GeoLocation> _geoLocation;

	//@ mandatory=false
	//@ name=GeoOrientation
	//@ order=6
	Poco::SharedPtr<GeoOrientation> _geoOrientation;

	//@ mandatory=false
	//@ name=LocalLocation
	//@ order=7
	Poco::SharedPtr<LocalLocation> _localLocation;

	//@ mandatory=false
	//@ name=LocalOrientation
	//@ order=8
	Poco::SharedPtr<LocalOrientation> _localOrientation;

};


inline Poco::Optional<bool> LocationEntity::getAutoGeo() const
{
	return _autoGeo;
}


inline const Poco::Optional<std::string>& LocationEntity::getEntity() const
{
	return _entity;
}


inline Poco::Optional<bool> LocationEntity::getFixed() const
{
	return _fixed;
}


inline const Poco::SharedPtr<GeoLocation>& LocationEntity::getGeoLocation() const
{
	return _geoLocation;
}


inline const Poco::SharedPtr<GeoOrientation>& LocationEntity::getGeoOrientation() const
{
	return _geoOrientation;
}


inline const Poco::Optional<Poco::URI>& LocationEntity::getGeoSource() const
{
	return _geoSource;
}


inline const Poco::SharedPtr<LocalLocation>& LocationEntity::getLocalLocation() const
{
	return _localLocation;
}


inline const Poco::SharedPtr<LocalOrientation>& LocationEntity::getLocalOrientation() const
{
	return _localOrientation;
}


inline const Poco::Optional<std::string>& LocationEntity::getToken() const
{
	return _token;
}


inline void LocationEntity::setAutoGeo(Poco::Optional<bool> val)
{
	_autoGeo = val;
}


inline void LocationEntity::setEntity(const Poco::Optional<std::string>& val)
{
	_entity = val;
}


inline void LocationEntity::setEntity(Poco::Optional<std::string>&& val)
{
	_entity = std::move(val);
}


inline void LocationEntity::setFixed(Poco::Optional<bool> val)
{
	_fixed = val;
}


inline void LocationEntity::setGeoLocation(const Poco::SharedPtr<GeoLocation>& val)
{
	_geoLocation = val;
}


inline void LocationEntity::setGeoLocation(Poco::SharedPtr<GeoLocation>&& val)
{
	_geoLocation = std::move(val);
}


inline void LocationEntity::setGeoOrientation(const Poco::SharedPtr<GeoOrientation>& val)
{
	_geoOrientation = val;
}


inline void LocationEntity::setGeoOrientation(Poco::SharedPtr<GeoOrientation>&& val)
{
	_geoOrientation = std::move(val);
}


inline void LocationEntity::setGeoSource(const Poco::Optional<Poco::URI>& val)
{
	_geoSource = val;
}


inline void LocationEntity::setGeoSource(Poco::Optional<Poco::URI>&& val)
{
	_geoSource = std::move(val);
}


inline void LocationEntity::setLocalLocation(const Poco::SharedPtr<LocalLocation>& val)
{
	_localLocation = val;
}


inline void LocationEntity::setLocalLocation(Poco::SharedPtr<LocalLocation>&& val)
{
	_localLocation = std::move(val);
}


inline void LocationEntity::setLocalOrientation(const Poco::SharedPtr<LocalOrientation>& val)
{
	_localOrientation = val;
}


inline void LocationEntity::setLocalOrientation(Poco::SharedPtr<LocalOrientation>&& val)
{
	_localOrientation = std::move(val);
}


inline void LocationEntity::setToken(const Poco::Optional<std::string>& val)
{
	_token = val;
}


inline void LocationEntity::setToken(Poco::Optional<std::string>&& val)
{
	_token = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/GeoLocation.h"
#include "ONVIF/GeoOrientation.h"
#include "ONVIF/LocalLocation.h"
#include "ONVIF/LocalOrientation.h"


#endif // ONVIF_LocationEntity_INCLUDED
