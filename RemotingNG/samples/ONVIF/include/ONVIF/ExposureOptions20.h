// 
// ExposureOptions20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ExposureOptions20_INCLUDED
#define ONVIF_ExposureOptions20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FloatRange;
} 


namespace ONVIF {


//@ name=ExposureOptions20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ExposureOptions20
{
public:
	ExposureOptions20();

	ExposureOptions20(
		const std::vector<std::string>& mode, 
		const std::vector<std::string>& priority, 
		const Poco::SharedPtr<FloatRange>& minExposureTime, 
		const Poco::SharedPtr<FloatRange>& maxExposureTime, 
		const Poco::SharedPtr<FloatRange>& minGain, 
		const Poco::SharedPtr<FloatRange>& maxGain, 
		const Poco::SharedPtr<FloatRange>& minIris, 
		const Poco::SharedPtr<FloatRange>& maxIris, 
		const Poco::SharedPtr<FloatRange>& exposureTime, 
		const Poco::SharedPtr<FloatRange>& gain, 
		const Poco::SharedPtr<FloatRange>& iris);

	virtual ~ExposureOptions20();

	const Poco::SharedPtr<FloatRange>& getExposureTime() const;

	const Poco::SharedPtr<FloatRange>& getGain() const;

	const Poco::SharedPtr<FloatRange>& getIris() const;

	const Poco::SharedPtr<FloatRange>& getMaxExposureTime() const;

	const Poco::SharedPtr<FloatRange>& getMaxGain() const;

	const Poco::SharedPtr<FloatRange>& getMaxIris() const;

	const Poco::SharedPtr<FloatRange>& getMinExposureTime() const;

	const Poco::SharedPtr<FloatRange>& getMinGain() const;

	const Poco::SharedPtr<FloatRange>& getMinIris() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	const std::vector<std::string>& getPriority() const;

	std::vector<std::string>& getPriority();

	void setExposureTime(const Poco::SharedPtr<FloatRange>& val);

	void setExposureTime(Poco::SharedPtr<FloatRange>&& val);

	void setGain(const Poco::SharedPtr<FloatRange>& val);

	void setGain(Poco::SharedPtr<FloatRange>&& val);

	void setIris(const Poco::SharedPtr<FloatRange>& val);

	void setIris(Poco::SharedPtr<FloatRange>&& val);

	void setMaxExposureTime(const Poco::SharedPtr<FloatRange>& val);

	void setMaxExposureTime(Poco::SharedPtr<FloatRange>&& val);

	void setMaxGain(const Poco::SharedPtr<FloatRange>& val);

	void setMaxGain(Poco::SharedPtr<FloatRange>&& val);

	void setMaxIris(const Poco::SharedPtr<FloatRange>& val);

	void setMaxIris(Poco::SharedPtr<FloatRange>&& val);

	void setMinExposureTime(const Poco::SharedPtr<FloatRange>& val);

	void setMinExposureTime(Poco::SharedPtr<FloatRange>&& val);

	void setMinGain(const Poco::SharedPtr<FloatRange>& val);

	void setMinGain(Poco::SharedPtr<FloatRange>&& val);

	void setMinIris(const Poco::SharedPtr<FloatRange>& val);

	void setMinIris(Poco::SharedPtr<FloatRange>&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

	void setPriority(const std::vector<std::string>& val);

	void setPriority(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ mandatory=false
	//@ name=Priority
	//@ order=1
	std::vector<std::string> _priority;

	//@ mandatory=false
	//@ name=MinExposureTime
	//@ order=2
	Poco::SharedPtr<FloatRange> _minExposureTime;

	//@ mandatory=false
	//@ name=MaxExposureTime
	//@ order=3
	Poco::SharedPtr<FloatRange> _maxExposureTime;

	//@ mandatory=false
	//@ name=MinGain
	//@ order=4
	Poco::SharedPtr<FloatRange> _minGain;

	//@ mandatory=false
	//@ name=MaxGain
	//@ order=5
	Poco::SharedPtr<FloatRange> _maxGain;

	//@ mandatory=false
	//@ name=MinIris
	//@ order=6
	Poco::SharedPtr<FloatRange> _minIris;

	//@ mandatory=false
	//@ name=MaxIris
	//@ order=7
	Poco::SharedPtr<FloatRange> _maxIris;

	//@ mandatory=false
	//@ name=ExposureTime
	//@ order=8
	Poco::SharedPtr<FloatRange> _exposureTime;

	//@ mandatory=false
	//@ name=Gain
	//@ order=9
	Poco::SharedPtr<FloatRange> _gain;

	//@ mandatory=false
	//@ name=Iris
	//@ order=10
	Poco::SharedPtr<FloatRange> _iris;

};


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getExposureTime() const
{
	return _exposureTime;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getGain() const
{
	return _gain;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getIris() const
{
	return _iris;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getMaxExposureTime() const
{
	return _maxExposureTime;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getMaxGain() const
{
	return _maxGain;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getMaxIris() const
{
	return _maxIris;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getMinExposureTime() const
{
	return _minExposureTime;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getMinGain() const
{
	return _minGain;
}


inline const Poco::SharedPtr<FloatRange>& ExposureOptions20::getMinIris() const
{
	return _minIris;
}


inline const std::vector<std::string>& ExposureOptions20::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& ExposureOptions20::getMode()
{
	return _mode;
}


inline const std::vector<std::string>& ExposureOptions20::getPriority() const
{
	return _priority;
}


inline std::vector<std::string>& ExposureOptions20::getPriority()
{
	return _priority;
}


inline void ExposureOptions20::setExposureTime(const Poco::SharedPtr<FloatRange>& val)
{
	_exposureTime = val;
}


inline void ExposureOptions20::setExposureTime(Poco::SharedPtr<FloatRange>&& val)
{
	_exposureTime = std::move(val);
}


inline void ExposureOptions20::setGain(const Poco::SharedPtr<FloatRange>& val)
{
	_gain = val;
}


inline void ExposureOptions20::setGain(Poco::SharedPtr<FloatRange>&& val)
{
	_gain = std::move(val);
}


inline void ExposureOptions20::setIris(const Poco::SharedPtr<FloatRange>& val)
{
	_iris = val;
}


inline void ExposureOptions20::setIris(Poco::SharedPtr<FloatRange>&& val)
{
	_iris = std::move(val);
}


inline void ExposureOptions20::setMaxExposureTime(const Poco::SharedPtr<FloatRange>& val)
{
	_maxExposureTime = val;
}


inline void ExposureOptions20::setMaxExposureTime(Poco::SharedPtr<FloatRange>&& val)
{
	_maxExposureTime = std::move(val);
}


inline void ExposureOptions20::setMaxGain(const Poco::SharedPtr<FloatRange>& val)
{
	_maxGain = val;
}


inline void ExposureOptions20::setMaxGain(Poco::SharedPtr<FloatRange>&& val)
{
	_maxGain = std::move(val);
}


inline void ExposureOptions20::setMaxIris(const Poco::SharedPtr<FloatRange>& val)
{
	_maxIris = val;
}


inline void ExposureOptions20::setMaxIris(Poco::SharedPtr<FloatRange>&& val)
{
	_maxIris = std::move(val);
}


inline void ExposureOptions20::setMinExposureTime(const Poco::SharedPtr<FloatRange>& val)
{
	_minExposureTime = val;
}


inline void ExposureOptions20::setMinExposureTime(Poco::SharedPtr<FloatRange>&& val)
{
	_minExposureTime = std::move(val);
}


inline void ExposureOptions20::setMinGain(const Poco::SharedPtr<FloatRange>& val)
{
	_minGain = val;
}


inline void ExposureOptions20::setMinGain(Poco::SharedPtr<FloatRange>&& val)
{
	_minGain = std::move(val);
}


inline void ExposureOptions20::setMinIris(const Poco::SharedPtr<FloatRange>& val)
{
	_minIris = val;
}


inline void ExposureOptions20::setMinIris(Poco::SharedPtr<FloatRange>&& val)
{
	_minIris = std::move(val);
}


inline void ExposureOptions20::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void ExposureOptions20::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


inline void ExposureOptions20::setPriority(const std::vector<std::string>& val)
{
	_priority = val;
}


inline void ExposureOptions20::setPriority(std::vector<std::string>&& val)
{
	_priority = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FloatRange.h"


#endif // ONVIF_ExposureOptions20_INCLUDED
