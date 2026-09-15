// 
// OSDPosConfiguration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_OSDPosConfiguration_INCLUDED
#define ONVIF_OSDPosConfiguration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class OSDPosConfigurationExtension;
class Vector;
} 


namespace ONVIF {


//@ name=OSDPosConfiguration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API OSDPosConfiguration
{
public:
	OSDPosConfiguration();

	OSDPosConfiguration(
		const std::string& type, 
		const Poco::SharedPtr<Vector>& pos, 
		const Poco::SharedPtr<OSDPosConfigurationExtension>& extension);

	virtual ~OSDPosConfiguration();

	const Poco::SharedPtr<OSDPosConfigurationExtension>& getExtension() const;

	const Poco::SharedPtr<Vector>& getPos() const;

	const std::string& getType() const;

	void setExtension(const Poco::SharedPtr<OSDPosConfigurationExtension>& val);

	void setExtension(Poco::SharedPtr<OSDPosConfigurationExtension>&& val);

	void setPos(const Poco::SharedPtr<Vector>& val);

	void setPos(Poco::SharedPtr<Vector>&& val);

	void setType(const std::string& val);

	void setType(std::string&& val);

private:
	//@ name=Type
	//@ order=0
	std::string _type;

	//@ mandatory=false
	//@ name=Pos
	//@ order=1
	Poco::SharedPtr<Vector> _pos;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<OSDPosConfigurationExtension> _extension;

};


inline const Poco::SharedPtr<OSDPosConfigurationExtension>& OSDPosConfiguration::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<Vector>& OSDPosConfiguration::getPos() const
{
	return _pos;
}


inline const std::string& OSDPosConfiguration::getType() const
{
	return _type;
}


inline void OSDPosConfiguration::setExtension(const Poco::SharedPtr<OSDPosConfigurationExtension>& val)
{
	_extension = val;
}


inline void OSDPosConfiguration::setExtension(Poco::SharedPtr<OSDPosConfigurationExtension>&& val)
{
	_extension = std::move(val);
}


inline void OSDPosConfiguration::setPos(const Poco::SharedPtr<Vector>& val)
{
	_pos = val;
}


inline void OSDPosConfiguration::setPos(Poco::SharedPtr<Vector>&& val)
{
	_pos = std::move(val);
}


inline void OSDPosConfiguration::setType(const std::string& val)
{
	_type = val;
}


inline void OSDPosConfiguration::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/OSDPosConfigurationExtension.h"
#include "ONVIF/Vector.h"


#endif // ONVIF_OSDPosConfiguration_INCLUDED
