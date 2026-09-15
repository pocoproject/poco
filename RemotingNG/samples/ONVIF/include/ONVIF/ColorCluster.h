// 
// ColorCluster.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ColorCluster_INCLUDED
#define ONVIF_ColorCluster_INCLUDED


#include "ONVIF/Color.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ColorCovariance;
} 


namespace ONVIF {


//@ name="#ColorCluster"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ColorCluster
{
public:
	ColorCluster();

	ColorCluster(
		const Color& color, 
		Poco::Optional<float> weight, 
		const Poco::SharedPtr<ColorCovariance>& covariance);

	virtual ~ColorCluster();

	const Color& getColor() const;

	const Poco::SharedPtr<ColorCovariance>& getCovariance() const;

	Poco::Optional<float> getWeight() const;

	void setColor(const Color& val);

	void setColor(Color&& val);

	void setCovariance(const Poco::SharedPtr<ColorCovariance>& val);

	void setCovariance(Poco::SharedPtr<ColorCovariance>&& val);

	void setWeight(Poco::Optional<float> val);

private:
	//@ name=Color
	//@ order=0
	Color _color;

	//@ mandatory=false
	//@ name=Weight
	//@ order=1
	Poco::Optional<float> _weight;

	//@ mandatory=false
	//@ name=Covariance
	//@ order=2
	Poco::SharedPtr<ColorCovariance> _covariance;

};


inline const Color& ColorCluster::getColor() const
{
	return _color;
}


inline const Poco::SharedPtr<ColorCovariance>& ColorCluster::getCovariance() const
{
	return _covariance;
}


inline Poco::Optional<float> ColorCluster::getWeight() const
{
	return _weight;
}


inline void ColorCluster::setColor(const Color& val)
{
	_color = val;
}


inline void ColorCluster::setColor(Color&& val)
{
	_color = std::move(val);
}


inline void ColorCluster::setCovariance(const Poco::SharedPtr<ColorCovariance>& val)
{
	_covariance = val;
}


inline void ColorCluster::setCovariance(Poco::SharedPtr<ColorCovariance>&& val)
{
	_covariance = std::move(val);
}


inline void ColorCluster::setWeight(Poco::Optional<float> val)
{
	_weight = val;
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ColorCovariance.h"


#endif // ONVIF_ColorCluster_INCLUDED
