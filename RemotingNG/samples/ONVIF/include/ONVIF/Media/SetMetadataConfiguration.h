// 
// SetMetadataConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Media_SetMetadataConfiguration_INCLUDED
#define ONVIF_Media_SetMetadataConfiguration_INCLUDED


#include "ONVIF/MetadataConfiguration.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
namespace Media {


//@ name="#SetMetadataConfiguration"
//@ namespace="http://www.onvif.org/ver10/media/wsdl"
//@ serialize
class ONVIF_API SetMetadataConfiguration
{
public:
	SetMetadataConfiguration();

	SetMetadataConfiguration(
		const ONVIF::MetadataConfiguration& configuration, 
		bool forcePersistence);

	virtual ~SetMetadataConfiguration();

	const ONVIF::MetadataConfiguration& getConfiguration() const;

	bool getForcePersistence() const;

	void setConfiguration(const ONVIF::MetadataConfiguration& val);

	void setConfiguration(ONVIF::MetadataConfiguration&& val);

	void setForcePersistence(bool val);

private:
	//@ name=Configuration
	//@ order=0
	ONVIF::MetadataConfiguration _configuration;

	//@ name=ForcePersistence
	//@ order=1
	bool _forcePersistence;

};


inline const ONVIF::MetadataConfiguration& SetMetadataConfiguration::getConfiguration() const
{
	return _configuration;
}


inline bool SetMetadataConfiguration::getForcePersistence() const
{
	return _forcePersistence;
}


inline void SetMetadataConfiguration::setConfiguration(const ONVIF::MetadataConfiguration& val)
{
	_configuration = val;
}


inline void SetMetadataConfiguration::setConfiguration(ONVIF::MetadataConfiguration&& val)
{
	_configuration = std::move(val);
}


inline void SetMetadataConfiguration::setForcePersistence(bool val)
{
	_forcePersistence = val;
}


} } // ONVIF::Media


#endif // ONVIF_Media_SetMetadataConfiguration_INCLUDED
