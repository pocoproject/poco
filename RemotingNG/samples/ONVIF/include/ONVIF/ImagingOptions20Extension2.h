// 
// ImagingOptions20Extension2.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingOptions20Extension2_INCLUDED
#define ONVIF_ImagingOptions20Extension2_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ImagingOptions20Extension3;
class IrCutFilterAutoAdjustmentOptions;
} 


namespace ONVIF {


//@ name=ImagingOptions20Extension2
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingOptions20Extension2
{
public:
	ImagingOptions20Extension2();

	ImagingOptions20Extension2(
		const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>& irCutFilterAutoAdjustment, 
		const Poco::SharedPtr<ImagingOptions20Extension3>& extension);

	virtual ~ImagingOptions20Extension2();

	const Poco::SharedPtr<ImagingOptions20Extension3>& getExtension() const;

	const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>& getIrCutFilterAutoAdjustment() const;

	void setExtension(const Poco::SharedPtr<ImagingOptions20Extension3>& val);

	void setExtension(Poco::SharedPtr<ImagingOptions20Extension3>&& val);

	void setIrCutFilterAutoAdjustment(const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>& val);

	void setIrCutFilterAutoAdjustment(Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>&& val);

private:
	//@ mandatory=false
	//@ name=IrCutFilterAutoAdjustment
	//@ order=0
	Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions> _irCutFilterAutoAdjustment;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<ImagingOptions20Extension3> _extension;

};


inline const Poco::SharedPtr<ImagingOptions20Extension3>& ImagingOptions20Extension2::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>& ImagingOptions20Extension2::getIrCutFilterAutoAdjustment() const
{
	return _irCutFilterAutoAdjustment;
}


inline void ImagingOptions20Extension2::setExtension(const Poco::SharedPtr<ImagingOptions20Extension3>& val)
{
	_extension = val;
}


inline void ImagingOptions20Extension2::setExtension(Poco::SharedPtr<ImagingOptions20Extension3>&& val)
{
	_extension = std::move(val);
}


inline void ImagingOptions20Extension2::setIrCutFilterAutoAdjustment(const Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>& val)
{
	_irCutFilterAutoAdjustment = val;
}


inline void ImagingOptions20Extension2::setIrCutFilterAutoAdjustment(Poco::SharedPtr<IrCutFilterAutoAdjustmentOptions>&& val)
{
	_irCutFilterAutoAdjustment = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ImagingOptions20Extension3.h"
#include "ONVIF/IrCutFilterAutoAdjustmentOptions.h"


#endif // ONVIF_ImagingOptions20Extension2_INCLUDED
