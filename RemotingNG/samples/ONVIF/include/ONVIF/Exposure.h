// 
// Exposure.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Exposure_INCLUDED
#define ONVIF_Exposure_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Rectangle.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Exposure
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Exposure
{
public:
	Exposure();

	Exposure(
		const std::string& mode, 
		const std::string& priority, 
		const Rectangle& window, 
		float minExposureTime, 
		float maxExposureTime, 
		float minGain, 
		float maxGain, 
		float minIris, 
		float maxIris, 
		float exposureTime, 
		float gain, 
		float iris);

	virtual ~Exposure();

	float getExposureTime() const;

	float getGain() const;

	float getIris() const;

	float getMaxExposureTime() const;

	float getMaxGain() const;

	float getMaxIris() const;

	float getMinExposureTime() const;

	float getMinGain() const;

	float getMinIris() const;

	const std::string& getMode() const;

	const std::string& getPriority() const;

	const Rectangle& getWindow() const;

	void setExposureTime(float val);

	void setGain(float val);

	void setIris(float val);

	void setMaxExposureTime(float val);

	void setMaxGain(float val);

	void setMaxIris(float val);

	void setMinExposureTime(float val);

	void setMinGain(float val);

	void setMinIris(float val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

	void setPriority(const std::string& val);

	void setPriority(std::string&& val);

	void setWindow(const Rectangle& val);

	void setWindow(Rectangle&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ name=Priority
	//@ order=1
	std::string _priority;

	//@ name=Window
	//@ order=2
	Rectangle _window;

	//@ name=MinExposureTime
	//@ order=3
	float _minExposureTime;

	//@ name=MaxExposureTime
	//@ order=4
	float _maxExposureTime;

	//@ name=MinGain
	//@ order=5
	float _minGain;

	//@ name=MaxGain
	//@ order=6
	float _maxGain;

	//@ name=MinIris
	//@ order=7
	float _minIris;

	//@ name=MaxIris
	//@ order=8
	float _maxIris;

	//@ name=ExposureTime
	//@ order=9
	float _exposureTime;

	//@ name=Gain
	//@ order=10
	float _gain;

	//@ name=Iris
	//@ order=11
	float _iris;

};


inline float Exposure::getExposureTime() const
{
	return _exposureTime;
}


inline float Exposure::getGain() const
{
	return _gain;
}


inline float Exposure::getIris() const
{
	return _iris;
}


inline float Exposure::getMaxExposureTime() const
{
	return _maxExposureTime;
}


inline float Exposure::getMaxGain() const
{
	return _maxGain;
}


inline float Exposure::getMaxIris() const
{
	return _maxIris;
}


inline float Exposure::getMinExposureTime() const
{
	return _minExposureTime;
}


inline float Exposure::getMinGain() const
{
	return _minGain;
}


inline float Exposure::getMinIris() const
{
	return _minIris;
}


inline const std::string& Exposure::getMode() const
{
	return _mode;
}


inline const std::string& Exposure::getPriority() const
{
	return _priority;
}


inline const Rectangle& Exposure::getWindow() const
{
	return _window;
}


inline void Exposure::setExposureTime(float val)
{
	_exposureTime = val;
}


inline void Exposure::setGain(float val)
{
	_gain = val;
}


inline void Exposure::setIris(float val)
{
	_iris = val;
}


inline void Exposure::setMaxExposureTime(float val)
{
	_maxExposureTime = val;
}


inline void Exposure::setMaxGain(float val)
{
	_maxGain = val;
}


inline void Exposure::setMaxIris(float val)
{
	_maxIris = val;
}


inline void Exposure::setMinExposureTime(float val)
{
	_minExposureTime = val;
}


inline void Exposure::setMinGain(float val)
{
	_minGain = val;
}


inline void Exposure::setMinIris(float val)
{
	_minIris = val;
}


inline void Exposure::setMode(const std::string& val)
{
	_mode = val;
}


inline void Exposure::setMode(std::string&& val)
{
	_mode = std::move(val);
}


inline void Exposure::setPriority(const std::string& val)
{
	_priority = val;
}


inline void Exposure::setPriority(std::string&& val)
{
	_priority = std::move(val);
}


inline void Exposure::setWindow(const Rectangle& val)
{
	_window = val;
}


inline void Exposure::setWindow(Rectangle&& val)
{
	_window = std::move(val);
}


} // ONVIF


#endif // ONVIF_Exposure_INCLUDED
