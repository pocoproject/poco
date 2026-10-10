// 
// WhiteBalanceOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_WhiteBalanceOptions_INCLUDED
#define ONVIF_WhiteBalanceOptions_INCLUDED


#include "ONVIF/FloatRange.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=WhiteBalanceOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API WhiteBalanceOptions
{
public:
	WhiteBalanceOptions();

	WhiteBalanceOptions(
		const std::vector<std::string>& mode, 
		const FloatRange& yrGain, 
		const FloatRange& ybGain);

	virtual ~WhiteBalanceOptions();

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	const FloatRange& getYbGain() const;

	const FloatRange& getYrGain() const;

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

	void setYbGain(const FloatRange& val);

	void setYbGain(FloatRange&& val);

	void setYrGain(const FloatRange& val);

	void setYrGain(FloatRange&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ name=YrGain
	//@ order=1
	FloatRange _yrGain;

	//@ name=YbGain
	//@ order=2
	FloatRange _ybGain;

};


inline const std::vector<std::string>& WhiteBalanceOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& WhiteBalanceOptions::getMode()
{
	return _mode;
}


inline const FloatRange& WhiteBalanceOptions::getYbGain() const
{
	return _ybGain;
}


inline const FloatRange& WhiteBalanceOptions::getYrGain() const
{
	return _yrGain;
}


inline void WhiteBalanceOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void WhiteBalanceOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


inline void WhiteBalanceOptions::setYbGain(const FloatRange& val)
{
	_ybGain = val;
}


inline void WhiteBalanceOptions::setYbGain(FloatRange&& val)
{
	_ybGain = std::move(val);
}


inline void WhiteBalanceOptions::setYrGain(const FloatRange& val)
{
	_yrGain = val;
}


inline void WhiteBalanceOptions::setYrGain(FloatRange&& val)
{
	_yrGain = std::move(val);
}


} // ONVIF


#endif // ONVIF_WhiteBalanceOptions_INCLUDED
