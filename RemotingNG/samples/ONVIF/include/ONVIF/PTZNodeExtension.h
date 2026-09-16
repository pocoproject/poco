// 
// PTZNodeExtension.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZNodeExtension_INCLUDED
#define ONVIF_PTZNodeExtension_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZNodeExtension2;
class PTZPresetTourSupported;
} 


namespace ONVIF {


//@ name=PTZNodeExtension
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZNodeExtension
{
public:
	PTZNodeExtension();

	PTZNodeExtension(
		const Poco::SharedPtr<PTZPresetTourSupported>& supportedPresetTour, 
		const Poco::SharedPtr<PTZNodeExtension2>& extension);

	virtual ~PTZNodeExtension();

	const Poco::SharedPtr<PTZNodeExtension2>& getExtension() const;

	const Poco::SharedPtr<PTZPresetTourSupported>& getSupportedPresetTour() const;

	void setExtension(const Poco::SharedPtr<PTZNodeExtension2>& val);

	void setExtension(Poco::SharedPtr<PTZNodeExtension2>&& val);

	void setSupportedPresetTour(const Poco::SharedPtr<PTZPresetTourSupported>& val);

	void setSupportedPresetTour(Poco::SharedPtr<PTZPresetTourSupported>&& val);

private:
	//@ mandatory=false
	//@ name=SupportedPresetTour
	//@ order=0
	Poco::SharedPtr<PTZPresetTourSupported> _supportedPresetTour;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<PTZNodeExtension2> _extension;

};


inline const Poco::SharedPtr<PTZNodeExtension2>& PTZNodeExtension::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<PTZPresetTourSupported>& PTZNodeExtension::getSupportedPresetTour() const
{
	return _supportedPresetTour;
}


inline void PTZNodeExtension::setExtension(const Poco::SharedPtr<PTZNodeExtension2>& val)
{
	_extension = val;
}


inline void PTZNodeExtension::setExtension(Poco::SharedPtr<PTZNodeExtension2>&& val)
{
	_extension = std::move(val);
}


inline void PTZNodeExtension::setSupportedPresetTour(const Poco::SharedPtr<PTZPresetTourSupported>& val)
{
	_supportedPresetTour = val;
}


inline void PTZNodeExtension::setSupportedPresetTour(Poco::SharedPtr<PTZPresetTourSupported>&& val)
{
	_supportedPresetTour = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZNodeExtension2.h"
#include "ONVIF/PTZPresetTourSupported.h"


#endif // ONVIF_PTZNodeExtension_INCLUDED
