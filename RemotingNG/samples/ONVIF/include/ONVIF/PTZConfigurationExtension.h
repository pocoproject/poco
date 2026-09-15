// 
// PTZConfigurationExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZConfigurationExtension_INCLUDED
#define ONVIF_PTZConfigurationExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTControlDirection;
class PTZConfigurationExtension2;
} 


namespace ONVIF {


//@ name=PTZConfigurationExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZConfigurationExtension
{
public:
	PTZConfigurationExtension();

	PTZConfigurationExtension(
		const Poco::SharedPtr<PTControlDirection>& pTControlDirection, 
		const Poco::SharedPtr<PTZConfigurationExtension2>& extension);

	virtual ~PTZConfigurationExtension();

	const Poco::SharedPtr<PTZConfigurationExtension2>& getExtension() const;

	const Poco::SharedPtr<PTControlDirection>& getPTControlDirection() const;

	void setExtension(const Poco::SharedPtr<PTZConfigurationExtension2>& val);

	void setExtension(Poco::SharedPtr<PTZConfigurationExtension2>&& val);

	void setPTControlDirection(const Poco::SharedPtr<PTControlDirection>& val);

	void setPTControlDirection(Poco::SharedPtr<PTControlDirection>&& val);

private:
	//@ mandatory=false
	//@ name=PTControlDirection
	//@ order=0
	Poco::SharedPtr<PTControlDirection> _pTControlDirection;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<PTZConfigurationExtension2> _extension;

};


inline const Poco::SharedPtr<PTZConfigurationExtension2>& PTZConfigurationExtension::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<PTControlDirection>& PTZConfigurationExtension::getPTControlDirection() const
{
	return _pTControlDirection;
}


inline void PTZConfigurationExtension::setExtension(const Poco::SharedPtr<PTZConfigurationExtension2>& val)
{
	_extension = val;
}


inline void PTZConfigurationExtension::setExtension(Poco::SharedPtr<PTZConfigurationExtension2>&& val)
{
	_extension = std::move(val);
}


inline void PTZConfigurationExtension::setPTControlDirection(const Poco::SharedPtr<PTControlDirection>& val)
{
	_pTControlDirection = val;
}


inline void PTZConfigurationExtension::setPTControlDirection(Poco::SharedPtr<PTControlDirection>&& val)
{
	_pTControlDirection = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTControlDirection.h"
#include "ONVIF/PTZConfigurationExtension2.h"


#endif // ONVIF_PTZConfigurationExtension_INCLUDED
