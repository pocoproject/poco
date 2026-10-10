// 
// ImagingOptions20Extension3.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingOptions20Extension3_INCLUDED
#define ONVIF_ImagingOptions20Extension3_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class DefoggingOptions;
class ImagingOptions20Extension4;
class NoiseReductionOptions;
class ToneCompensationOptions;
} 


namespace ONVIF {


//@ name=ImagingOptions20Extension3
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingOptions20Extension3
{
public:
	ImagingOptions20Extension3();

	ImagingOptions20Extension3(
		const Poco::SharedPtr<ToneCompensationOptions>& toneCompensationOptions, 
		const Poco::SharedPtr<DefoggingOptions>& defoggingOptions, 
		const Poco::SharedPtr<NoiseReductionOptions>& noiseReductionOptions, 
		const Poco::SharedPtr<ImagingOptions20Extension4>& extension);

	virtual ~ImagingOptions20Extension3();

	const Poco::SharedPtr<DefoggingOptions>& getDefoggingOptions() const;

	const Poco::SharedPtr<ImagingOptions20Extension4>& getExtension() const;

	const Poco::SharedPtr<NoiseReductionOptions>& getNoiseReductionOptions() const;

	const Poco::SharedPtr<ToneCompensationOptions>& getToneCompensationOptions() const;

	void setDefoggingOptions(const Poco::SharedPtr<DefoggingOptions>& val);

	void setDefoggingOptions(Poco::SharedPtr<DefoggingOptions>&& val);

	void setExtension(const Poco::SharedPtr<ImagingOptions20Extension4>& val);

	void setExtension(Poco::SharedPtr<ImagingOptions20Extension4>&& val);

	void setNoiseReductionOptions(const Poco::SharedPtr<NoiseReductionOptions>& val);

	void setNoiseReductionOptions(Poco::SharedPtr<NoiseReductionOptions>&& val);

	void setToneCompensationOptions(const Poco::SharedPtr<ToneCompensationOptions>& val);

	void setToneCompensationOptions(Poco::SharedPtr<ToneCompensationOptions>&& val);

private:
	//@ mandatory=false
	//@ name=ToneCompensationOptions
	//@ order=0
	Poco::SharedPtr<ToneCompensationOptions> _toneCompensationOptions;

	//@ mandatory=false
	//@ name=DefoggingOptions
	//@ order=1
	Poco::SharedPtr<DefoggingOptions> _defoggingOptions;

	//@ mandatory=false
	//@ name=NoiseReductionOptions
	//@ order=2
	Poco::SharedPtr<NoiseReductionOptions> _noiseReductionOptions;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<ImagingOptions20Extension4> _extension;

};


inline const Poco::SharedPtr<DefoggingOptions>& ImagingOptions20Extension3::getDefoggingOptions() const
{
	return _defoggingOptions;
}


inline const Poco::SharedPtr<ImagingOptions20Extension4>& ImagingOptions20Extension3::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<NoiseReductionOptions>& ImagingOptions20Extension3::getNoiseReductionOptions() const
{
	return _noiseReductionOptions;
}


inline const Poco::SharedPtr<ToneCompensationOptions>& ImagingOptions20Extension3::getToneCompensationOptions() const
{
	return _toneCompensationOptions;
}


inline void ImagingOptions20Extension3::setDefoggingOptions(const Poco::SharedPtr<DefoggingOptions>& val)
{
	_defoggingOptions = val;
}


inline void ImagingOptions20Extension3::setDefoggingOptions(Poco::SharedPtr<DefoggingOptions>&& val)
{
	_defoggingOptions = std::move(val);
}


inline void ImagingOptions20Extension3::setExtension(const Poco::SharedPtr<ImagingOptions20Extension4>& val)
{
	_extension = val;
}


inline void ImagingOptions20Extension3::setExtension(Poco::SharedPtr<ImagingOptions20Extension4>&& val)
{
	_extension = std::move(val);
}


inline void ImagingOptions20Extension3::setNoiseReductionOptions(const Poco::SharedPtr<NoiseReductionOptions>& val)
{
	_noiseReductionOptions = val;
}


inline void ImagingOptions20Extension3::setNoiseReductionOptions(Poco::SharedPtr<NoiseReductionOptions>&& val)
{
	_noiseReductionOptions = std::move(val);
}


inline void ImagingOptions20Extension3::setToneCompensationOptions(const Poco::SharedPtr<ToneCompensationOptions>& val)
{
	_toneCompensationOptions = val;
}


inline void ImagingOptions20Extension3::setToneCompensationOptions(Poco::SharedPtr<ToneCompensationOptions>&& val)
{
	_toneCompensationOptions = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/DefoggingOptions.h"
#include "ONVIF/ImagingOptions20Extension4.h"
#include "ONVIF/NoiseReductionOptions.h"
#include "ONVIF/ToneCompensationOptions.h"


#endif // ONVIF_ImagingOptions20Extension3_INCLUDED
