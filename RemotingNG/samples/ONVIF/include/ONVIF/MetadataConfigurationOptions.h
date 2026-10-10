// 
// MetadataConfigurationOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MetadataConfigurationOptions_INCLUDED
#define ONVIF_MetadataConfigurationOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZStatusFilterOptions.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class MetadataConfigurationOptionsExtension;
} 


namespace ONVIF {


//@ name=MetadataConfigurationOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MetadataConfigurationOptions
{
public:
	MetadataConfigurationOptions();

	MetadataConfigurationOptions(
		Poco::Optional<bool> geoLocation, 
		Poco::Optional<int> maxContentFilterSize, 
		const PTZStatusFilterOptions& pTZStatusFilterOptions, 
		const Poco::SharedPtr<MetadataConfigurationOptionsExtension>& extension);

	virtual ~MetadataConfigurationOptions();

	const Poco::SharedPtr<MetadataConfigurationOptionsExtension>& getExtension() const;

	Poco::Optional<bool> getGeoLocation() const;

	Poco::Optional<int> getMaxContentFilterSize() const;

	const PTZStatusFilterOptions& getPTZStatusFilterOptions() const;

	void setExtension(const Poco::SharedPtr<MetadataConfigurationOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<MetadataConfigurationOptionsExtension>&& val);

	void setGeoLocation(Poco::Optional<bool> val);

	void setMaxContentFilterSize(Poco::Optional<int> val);

	void setPTZStatusFilterOptions(const PTZStatusFilterOptions& val);

	void setPTZStatusFilterOptions(PTZStatusFilterOptions&& val);

private:
	//@ mandatory=false
	//@ name=GeoLocation
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _geoLocation;

	//@ mandatory=false
	//@ name=MaxContentFilterSize
	//@ order=1
	//@ type=attr
	Poco::Optional<int> _maxContentFilterSize;

	//@ name=PTZStatusFilterOptions
	//@ order=2
	PTZStatusFilterOptions _pTZStatusFilterOptions;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<MetadataConfigurationOptionsExtension> _extension;

};


inline const Poco::SharedPtr<MetadataConfigurationOptionsExtension>& MetadataConfigurationOptions::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> MetadataConfigurationOptions::getGeoLocation() const
{
	return _geoLocation;
}


inline Poco::Optional<int> MetadataConfigurationOptions::getMaxContentFilterSize() const
{
	return _maxContentFilterSize;
}


inline const PTZStatusFilterOptions& MetadataConfigurationOptions::getPTZStatusFilterOptions() const
{
	return _pTZStatusFilterOptions;
}


inline void MetadataConfigurationOptions::setExtension(const Poco::SharedPtr<MetadataConfigurationOptionsExtension>& val)
{
	_extension = val;
}


inline void MetadataConfigurationOptions::setExtension(Poco::SharedPtr<MetadataConfigurationOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void MetadataConfigurationOptions::setGeoLocation(Poco::Optional<bool> val)
{
	_geoLocation = val;
}


inline void MetadataConfigurationOptions::setMaxContentFilterSize(Poco::Optional<int> val)
{
	_maxContentFilterSize = val;
}


inline void MetadataConfigurationOptions::setPTZStatusFilterOptions(const PTZStatusFilterOptions& val)
{
	_pTZStatusFilterOptions = val;
}


inline void MetadataConfigurationOptions::setPTZStatusFilterOptions(PTZStatusFilterOptions&& val)
{
	_pTZStatusFilterOptions = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/MetadataConfigurationOptionsExtension.h"


#endif // ONVIF_MetadataConfigurationOptions_INCLUDED
