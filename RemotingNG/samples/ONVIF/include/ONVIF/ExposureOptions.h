// 
// ExposureOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ExposureOptions_INCLUDED
#define ONVIF_ExposureOptions_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ExposureOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ExposureOptions
{
public:
	ExposureOptions();

	ExposureOptions(
		const std::vector<std::string>& mode, 
		const std::vector<std::string>& priority, 
		const FloatRange& minExposureTime, 
		const FloatRange& maxExposureTime, 
		const FloatRange& minGain, 
		const FloatRange& maxGain, 
		const FloatRange& minIris, 
		const FloatRange& maxIris, 
		const FloatRange& exposureTime, 
		const FloatRange& gain, 
		const FloatRange& iris);

	virtual ~ExposureOptions();

	const FloatRange& getExposureTime() const;

	const FloatRange& getGain() const;

	const FloatRange& getIris() const;

	const FloatRange& getMaxExposureTime() const;

	const FloatRange& getMaxGain() const;

	const FloatRange& getMaxIris() const;

	const FloatRange& getMinExposureTime() const;

	const FloatRange& getMinGain() const;

	const FloatRange& getMinIris() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	const std::vector<std::string>& getPriority() const;

	std::vector<std::string>& getPriority();

	void setExposureTime(const FloatRange& val);

	void setExposureTime(FloatRange&& val);

	void setGain(const FloatRange& val);

	void setGain(FloatRange&& val);

	void setIris(const FloatRange& val);

	void setIris(FloatRange&& val);

	void setMaxExposureTime(const FloatRange& val);

	void setMaxExposureTime(FloatRange&& val);

	void setMaxGain(const FloatRange& val);

	void setMaxGain(FloatRange&& val);

	void setMaxIris(const FloatRange& val);

	void setMaxIris(FloatRange&& val);

	void setMinExposureTime(const FloatRange& val);

	void setMinExposureTime(FloatRange&& val);

	void setMinGain(const FloatRange& val);

	void setMinGain(FloatRange&& val);

	void setMinIris(const FloatRange& val);

	void setMinIris(FloatRange&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

	void setPriority(const std::vector<std::string>& val);

	void setPriority(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ name=Priority
	//@ order=1
	std::vector<std::string> _priority;

	//@ name=MinExposureTime
	//@ order=2
	FloatRange _minExposureTime;

	//@ name=MaxExposureTime
	//@ order=3
	FloatRange _maxExposureTime;

	//@ name=MinGain
	//@ order=4
	FloatRange _minGain;

	//@ name=MaxGain
	//@ order=5
	FloatRange _maxGain;

	//@ name=MinIris
	//@ order=6
	FloatRange _minIris;

	//@ name=MaxIris
	//@ order=7
	FloatRange _maxIris;

	//@ name=ExposureTime
	//@ order=8
	FloatRange _exposureTime;

	//@ name=Gain
	//@ order=9
	FloatRange _gain;

	//@ name=Iris
	//@ order=10
	FloatRange _iris;

};


inline const FloatRange& ExposureOptions::getExposureTime() const
{
	return _exposureTime;
}


inline const FloatRange& ExposureOptions::getGain() const
{
	return _gain;
}


inline const FloatRange& ExposureOptions::getIris() const
{
	return _iris;
}


inline const FloatRange& ExposureOptions::getMaxExposureTime() const
{
	return _maxExposureTime;
}


inline const FloatRange& ExposureOptions::getMaxGain() const
{
	return _maxGain;
}


inline const FloatRange& ExposureOptions::getMaxIris() const
{
	return _maxIris;
}


inline const FloatRange& ExposureOptions::getMinExposureTime() const
{
	return _minExposureTime;
}


inline const FloatRange& ExposureOptions::getMinGain() const
{
	return _minGain;
}


inline const FloatRange& ExposureOptions::getMinIris() const
{
	return _minIris;
}


inline const std::vector<std::string>& ExposureOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& ExposureOptions::getMode()
{
	return _mode;
}


inline const std::vector<std::string>& ExposureOptions::getPriority() const
{
	return _priority;
}


inline std::vector<std::string>& ExposureOptions::getPriority()
{
	return _priority;
}


inline void ExposureOptions::setExposureTime(const FloatRange& val)
{
	_exposureTime = val;
}


inline void ExposureOptions::setExposureTime(FloatRange&& val)
{
	_exposureTime = std::move(val);
}


inline void ExposureOptions::setGain(const FloatRange& val)
{
	_gain = val;
}


inline void ExposureOptions::setGain(FloatRange&& val)
{
	_gain = std::move(val);
}


inline void ExposureOptions::setIris(const FloatRange& val)
{
	_iris = val;
}


inline void ExposureOptions::setIris(FloatRange&& val)
{
	_iris = std::move(val);
}


inline void ExposureOptions::setMaxExposureTime(const FloatRange& val)
{
	_maxExposureTime = val;
}


inline void ExposureOptions::setMaxExposureTime(FloatRange&& val)
{
	_maxExposureTime = std::move(val);
}


inline void ExposureOptions::setMaxGain(const FloatRange& val)
{
	_maxGain = val;
}


inline void ExposureOptions::setMaxGain(FloatRange&& val)
{
	_maxGain = std::move(val);
}


inline void ExposureOptions::setMaxIris(const FloatRange& val)
{
	_maxIris = val;
}


inline void ExposureOptions::setMaxIris(FloatRange&& val)
{
	_maxIris = std::move(val);
}


inline void ExposureOptions::setMinExposureTime(const FloatRange& val)
{
	_minExposureTime = val;
}


inline void ExposureOptions::setMinExposureTime(FloatRange&& val)
{
	_minExposureTime = std::move(val);
}


inline void ExposureOptions::setMinGain(const FloatRange& val)
{
	_minGain = val;
}


inline void ExposureOptions::setMinGain(FloatRange&& val)
{
	_minGain = std::move(val);
}


inline void ExposureOptions::setMinIris(const FloatRange& val)
{
	_minIris = val;
}


inline void ExposureOptions::setMinIris(FloatRange&& val)
{
	_minIris = std::move(val);
}


inline void ExposureOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void ExposureOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


inline void ExposureOptions::setPriority(const std::vector<std::string>& val)
{
	_priority = val;
}


inline void ExposureOptions::setPriority(std::vector<std::string>&& val)
{
	_priority = std::move(val);
}


} // ONVIF


#endif // ONVIF_ExposureOptions_INCLUDED
