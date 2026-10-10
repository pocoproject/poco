// 
// Exposure20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Exposure20_INCLUDED
#define ONVIF_Exposure20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class Rectangle;
} 


namespace ONVIF {


//@ name=Exposure20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Exposure20
{
public:
	Exposure20();

	Exposure20(
		const std::string& mode, 
		const Poco::Optional<std::string>& priority, 
		const Poco::SharedPtr<Rectangle>& window, 
		Poco::Optional<float> minExposureTime, 
		Poco::Optional<float> maxExposureTime, 
		Poco::Optional<float> minGain, 
		Poco::Optional<float> maxGain, 
		Poco::Optional<float> minIris, 
		Poco::Optional<float> maxIris, 
		Poco::Optional<float> exposureTime, 
		Poco::Optional<float> gain, 
		Poco::Optional<float> iris);

	virtual ~Exposure20();

	Poco::Optional<float> getExposureTime() const;

	Poco::Optional<float> getGain() const;

	Poco::Optional<float> getIris() const;

	Poco::Optional<float> getMaxExposureTime() const;

	Poco::Optional<float> getMaxGain() const;

	Poco::Optional<float> getMaxIris() const;

	Poco::Optional<float> getMinExposureTime() const;

	Poco::Optional<float> getMinGain() const;

	Poco::Optional<float> getMinIris() const;

	const std::string& getMode() const;

	const Poco::Optional<std::string>& getPriority() const;

	const Poco::SharedPtr<Rectangle>& getWindow() const;

	void setExposureTime(Poco::Optional<float> val);

	void setGain(Poco::Optional<float> val);

	void setIris(Poco::Optional<float> val);

	void setMaxExposureTime(Poco::Optional<float> val);

	void setMaxGain(Poco::Optional<float> val);

	void setMaxIris(Poco::Optional<float> val);

	void setMinExposureTime(Poco::Optional<float> val);

	void setMinGain(Poco::Optional<float> val);

	void setMinIris(Poco::Optional<float> val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setPriority(const Poco::Optional<std::string>& val);

	void setPriority(Poco::Optional<std::string>&& val);

	void setWindow(const Poco::SharedPtr<Rectangle>& val);

	void setWindow(Poco::SharedPtr<Rectangle>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=Priority
	//@ order=1
	Poco::Optional<std::string> _priority;

	//@ mandatory=false
	//@ name=Window
	//@ order=2
	Poco::SharedPtr<Rectangle> _window;

	//@ mandatory=false
	//@ name=MinExposureTime
	//@ order=3
	Poco::Optional<float> _minExposureTime;

	//@ mandatory=false
	//@ name=MaxExposureTime
	//@ order=4
	Poco::Optional<float> _maxExposureTime;

	//@ mandatory=false
	//@ name=MinGain
	//@ order=5
	Poco::Optional<float> _minGain;

	//@ mandatory=false
	//@ name=MaxGain
	//@ order=6
	Poco::Optional<float> _maxGain;

	//@ mandatory=false
	//@ name=MinIris
	//@ order=7
	Poco::Optional<float> _minIris;

	//@ mandatory=false
	//@ name=MaxIris
	//@ order=8
	Poco::Optional<float> _maxIris;

	//@ mandatory=false
	//@ name=ExposureTime
	//@ order=9
	Poco::Optional<float> _exposureTime;

	//@ mandatory=false
	//@ name=Gain
	//@ order=10
	Poco::Optional<float> _gain;

	//@ mandatory=false
	//@ name=Iris
	//@ order=11
	Poco::Optional<float> _iris;

};


inline Poco::Optional<float> Exposure20::getExposureTime() const
{
	return _exposureTime;
}


inline Poco::Optional<float> Exposure20::getGain() const
{
	return _gain;
}


inline Poco::Optional<float> Exposure20::getIris() const
{
	return _iris;
}


inline Poco::Optional<float> Exposure20::getMaxExposureTime() const
{
	return _maxExposureTime;
}


inline Poco::Optional<float> Exposure20::getMaxGain() const
{
	return _maxGain;
}


inline Poco::Optional<float> Exposure20::getMaxIris() const
{
	return _maxIris;
}


inline Poco::Optional<float> Exposure20::getMinExposureTime() const
{
	return _minExposureTime;
}


inline Poco::Optional<float> Exposure20::getMinGain() const
{
	return _minGain;
}


inline Poco::Optional<float> Exposure20::getMinIris() const
{
	return _minIris;
}


inline const std::string& Exposure20::getMode() const
{
	return _mode;
}


inline const Poco::Optional<std::string>& Exposure20::getPriority() const
{
	return _priority;
}


inline const Poco::SharedPtr<Rectangle>& Exposure20::getWindow() const
{
	return _window;
}


inline void Exposure20::setExposureTime(Poco::Optional<float> val)
{
	_exposureTime = val;
}


inline void Exposure20::setGain(Poco::Optional<float> val)
{
	_gain = val;
}


inline void Exposure20::setIris(Poco::Optional<float> val)
{
	_iris = val;
}


inline void Exposure20::setMaxExposureTime(Poco::Optional<float> val)
{
	_maxExposureTime = val;
}


inline void Exposure20::setMaxGain(Poco::Optional<float> val)
{
	_maxGain = val;
}


inline void Exposure20::setMaxIris(Poco::Optional<float> val)
{
	_maxIris = val;
}


inline void Exposure20::setMinExposureTime(Poco::Optional<float> val)
{
	_minExposureTime = val;
}


inline void Exposure20::setMinGain(Poco::Optional<float> val)
{
	_minGain = val;
}


inline void Exposure20::setMinIris(Poco::Optional<float> val)
{
	_minIris = val;
}


inline void Exposure20::setMode(const std::string& val)
{
	_mode = val;
}


inline void Exposure20::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void Exposure20::setPriority(const Poco::Optional<std::string>& val)
{
	_priority = val;
}


inline void Exposure20::setPriority(Poco::Optional<std::string>&& val)
{
	_priority = std::move(val);
}


inline void Exposure20::setWindow(const Poco::SharedPtr<Rectangle>& val)
{
	_window = val;
}


inline void Exposure20::setWindow(Poco::SharedPtr<Rectangle>&& val)
{
	_window = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/Rectangle.h"


#endif // ONVIF_Exposure20_INCLUDED
