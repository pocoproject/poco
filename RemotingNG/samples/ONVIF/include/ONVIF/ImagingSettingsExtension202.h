// 
// ImagingSettingsExtension202.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingSettingsExtension202_INCLUDED
#define ONVIF_ImagingSettingsExtension202_INCLUDED


#include "ONVIF/IrCutFilterAutoAdjustment.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ImagingSettingsExtension203;
} 


namespace ONVIF {


//@ name=ImagingSettingsExtension202
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingSettingsExtension202
{
public:
	ImagingSettingsExtension202();

	ImagingSettingsExtension202(
		const std::vector<IrCutFilterAutoAdjustment>& irCutFilterAutoAdjustment, 
		const Poco::SharedPtr<ImagingSettingsExtension203>& extension);

	virtual ~ImagingSettingsExtension202();

	const Poco::SharedPtr<ImagingSettingsExtension203>& getExtension() const;

	const std::vector<IrCutFilterAutoAdjustment>& getIrCutFilterAutoAdjustment() const;

	std::vector<IrCutFilterAutoAdjustment>& getIrCutFilterAutoAdjustment();

	void setExtension(const Poco::SharedPtr<ImagingSettingsExtension203>& val);

	void setExtension(Poco::SharedPtr<ImagingSettingsExtension203>&& val);

	void setIrCutFilterAutoAdjustment(const std::vector<IrCutFilterAutoAdjustment>& val);

	void setIrCutFilterAutoAdjustment(std::vector<IrCutFilterAutoAdjustment>&& val);

private:
	//@ mandatory=false
	//@ name=IrCutFilterAutoAdjustment
	//@ order=0
	std::vector<IrCutFilterAutoAdjustment> _irCutFilterAutoAdjustment;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ImagingSettingsExtension203> _extension;

};


inline const Poco::SharedPtr<ImagingSettingsExtension203>& ImagingSettingsExtension202::getExtension() const
{
	return _extension;
}


inline const std::vector<IrCutFilterAutoAdjustment>& ImagingSettingsExtension202::getIrCutFilterAutoAdjustment() const
{
	return _irCutFilterAutoAdjustment;
}


inline std::vector<IrCutFilterAutoAdjustment>& ImagingSettingsExtension202::getIrCutFilterAutoAdjustment()
{
	return _irCutFilterAutoAdjustment;
}


inline void ImagingSettingsExtension202::setExtension(const Poco::SharedPtr<ImagingSettingsExtension203>& val)
{
	_extension = val;
}


inline void ImagingSettingsExtension202::setExtension(Poco::SharedPtr<ImagingSettingsExtension203>&& val)
{
	_extension = std::move(val);
}


inline void ImagingSettingsExtension202::setIrCutFilterAutoAdjustment(const std::vector<IrCutFilterAutoAdjustment>& val)
{
	_irCutFilterAutoAdjustment = val;
}


inline void ImagingSettingsExtension202::setIrCutFilterAutoAdjustment(std::vector<IrCutFilterAutoAdjustment>&& val)
{
	_irCutFilterAutoAdjustment = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ImagingSettingsExtension203.h"


#endif // ONVIF_ImagingSettingsExtension202_INCLUDED
