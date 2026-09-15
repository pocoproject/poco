// 
// MetadataInput.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MetadataInput_INCLUDED
#define ONVIF_MetadataInput_INCLUDED


#include "ONVIF/Config.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class MetadataInputExtension;
} 


namespace ONVIF {


//@ name=MetadataInput
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MetadataInput
{
public:
	MetadataInput();

	MetadataInput(
		const std::vector<Config>& metadataConfig, 
		const Poco::SharedPtr<MetadataInputExtension>& extension);

	virtual ~MetadataInput();

	const Poco::SharedPtr<MetadataInputExtension>& getExtension() const;

	const std::vector<Config>& getMetadataConfig() const;

	std::vector<Config>& getMetadataConfig();

	void setExtension(const Poco::SharedPtr<MetadataInputExtension>& val);

	void setExtension(Poco::SharedPtr<MetadataInputExtension>&& val);

	void setMetadataConfig(const std::vector<Config>& val);

	void setMetadataConfig(std::vector<Config>&& val);

private:
	//@ mandatory=false
	//@ name=MetadataConfig
	//@ order=0
	std::vector<Config> _metadataConfig;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<MetadataInputExtension> _extension;

};


inline const Poco::SharedPtr<MetadataInputExtension>& MetadataInput::getExtension() const
{
	return _extension;
}


inline const std::vector<Config>& MetadataInput::getMetadataConfig() const
{
	return _metadataConfig;
}


inline std::vector<Config>& MetadataInput::getMetadataConfig()
{
	return _metadataConfig;
}


inline void MetadataInput::setExtension(const Poco::SharedPtr<MetadataInputExtension>& val)
{
	_extension = val;
}


inline void MetadataInput::setExtension(Poco::SharedPtr<MetadataInputExtension>&& val)
{
	_extension = std::move(val);
}


inline void MetadataInput::setMetadataConfig(const std::vector<Config>& val)
{
	_metadataConfig = val;
}


inline void MetadataInput::setMetadataConfig(std::vector<Config>&& val)
{
	_metadataConfig = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/MetadataInputExtension.h"


#endif // ONVIF_MetadataInput_INCLUDED
