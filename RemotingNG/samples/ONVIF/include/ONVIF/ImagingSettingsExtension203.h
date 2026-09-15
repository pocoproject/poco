// 
// ImagingSettingsExtension203.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ImagingSettingsExtension203_INCLUDED
#define ONVIF_ImagingSettingsExtension203_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Defogging;
class ImagingSettingsExtension204;
class NoiseReduction;
class ToneCompensation;
} 


namespace ONVIF {


//@ name=ImagingSettingsExtension203
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ImagingSettingsExtension203
{
public:
	ImagingSettingsExtension203();

	ImagingSettingsExtension203(
		const Poco::SharedPtr<ToneCompensation>& toneCompensation, 
		const Poco::SharedPtr<Defogging>& defogging, 
		const Poco::SharedPtr<NoiseReduction>& noiseReduction, 
		const Poco::SharedPtr<ImagingSettingsExtension204>& extension);

	virtual ~ImagingSettingsExtension203();

	const Poco::SharedPtr<Defogging>& getDefogging() const;

	const Poco::SharedPtr<ImagingSettingsExtension204>& getExtension() const;

	const Poco::SharedPtr<NoiseReduction>& getNoiseReduction() const;

	const Poco::SharedPtr<ToneCompensation>& getToneCompensation() const;

	void setDefogging(const Poco::SharedPtr<Defogging>& val);

	void setDefogging(Poco::SharedPtr<Defogging>&& val);

	void setExtension(const Poco::SharedPtr<ImagingSettingsExtension204>& val);

	void setExtension(Poco::SharedPtr<ImagingSettingsExtension204>&& val);

	void setNoiseReduction(const Poco::SharedPtr<NoiseReduction>& val);

	void setNoiseReduction(Poco::SharedPtr<NoiseReduction>&& val);

	void setToneCompensation(const Poco::SharedPtr<ToneCompensation>& val);

	void setToneCompensation(Poco::SharedPtr<ToneCompensation>&& val);

private:
	//@ mandatory=false
	//@ name=ToneCompensation
	//@ order=0
	Poco::SharedPtr<ToneCompensation> _toneCompensation;

	//@ mandatory=false
	//@ name=Defogging
	//@ order=1
	Poco::SharedPtr<Defogging> _defogging;

	//@ mandatory=false
	//@ name=NoiseReduction
	//@ order=2
	Poco::SharedPtr<NoiseReduction> _noiseReduction;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<ImagingSettingsExtension204> _extension;

};


inline const Poco::SharedPtr<Defogging>& ImagingSettingsExtension203::getDefogging() const
{
	return _defogging;
}


inline const Poco::SharedPtr<ImagingSettingsExtension204>& ImagingSettingsExtension203::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<NoiseReduction>& ImagingSettingsExtension203::getNoiseReduction() const
{
	return _noiseReduction;
}


inline const Poco::SharedPtr<ToneCompensation>& ImagingSettingsExtension203::getToneCompensation() const
{
	return _toneCompensation;
}


inline void ImagingSettingsExtension203::setDefogging(const Poco::SharedPtr<Defogging>& val)
{
	_defogging = val;
}


inline void ImagingSettingsExtension203::setDefogging(Poco::SharedPtr<Defogging>&& val)
{
	_defogging = std::move(val);
}


inline void ImagingSettingsExtension203::setExtension(const Poco::SharedPtr<ImagingSettingsExtension204>& val)
{
	_extension = val;
}


inline void ImagingSettingsExtension203::setExtension(Poco::SharedPtr<ImagingSettingsExtension204>&& val)
{
	_extension = std::move(val);
}


inline void ImagingSettingsExtension203::setNoiseReduction(const Poco::SharedPtr<NoiseReduction>& val)
{
	_noiseReduction = val;
}


inline void ImagingSettingsExtension203::setNoiseReduction(Poco::SharedPtr<NoiseReduction>&& val)
{
	_noiseReduction = std::move(val);
}


inline void ImagingSettingsExtension203::setToneCompensation(const Poco::SharedPtr<ToneCompensation>& val)
{
	_toneCompensation = val;
}


inline void ImagingSettingsExtension203::setToneCompensation(Poco::SharedPtr<ToneCompensation>&& val)
{
	_toneCompensation = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Defogging.h"
#include "ONVIF/ImagingSettingsExtension204.h"
#include "ONVIF/NoiseReduction.h"
#include "ONVIF/ToneCompensation.h"


#endif // ONVIF_ImagingSettingsExtension203_INCLUDED
