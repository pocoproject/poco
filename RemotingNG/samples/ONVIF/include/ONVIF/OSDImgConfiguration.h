// 
// OSDImgConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDImgConfiguration_INCLUDED
#define ONVIF_OSDImgConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {
class OSDImgConfigurationExtension;
} 


namespace ONVIF {


//@ name=OSDImgConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDImgConfiguration
{
public:
	OSDImgConfiguration();

	OSDImgConfiguration(
		const Poco::URI& imgPath, 
		const Poco::SharedPtr<OSDImgConfigurationExtension>& extension);

	virtual ~OSDImgConfiguration();

	const Poco::SharedPtr<OSDImgConfigurationExtension>& getExtension() const;

	const Poco::URI& getImgPath() const;

	void setExtension(const Poco::SharedPtr<OSDImgConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<OSDImgConfigurationExtension>&& val);

	void setImgPath(const Poco::URI& val);

	void setImgPath(Poco::URI&& val);

private:
	//@ name=ImgPath
	//@ order=0
	Poco::URI _imgPath;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<OSDImgConfigurationExtension> _extension;

};


inline const Poco::SharedPtr<OSDImgConfigurationExtension>& OSDImgConfiguration::getExtension() const
{
	return _extension;
}


inline const Poco::URI& OSDImgConfiguration::getImgPath() const
{
	return _imgPath;
}


inline void OSDImgConfiguration::setExtension(const Poco::SharedPtr<OSDImgConfigurationExtension>& val)
{
	_extension = val;
}


inline void OSDImgConfiguration::setExtension(Poco::SharedPtr<OSDImgConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDImgConfiguration::setImgPath(const Poco::URI& val)
{
	_imgPath = val;
}


inline void OSDImgConfiguration::setImgPath(Poco::URI&& val)
{
	_imgPath = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/OSDImgConfigurationExtension.h"


#endif // ONVIF_OSDImgConfiguration_INCLUDED
