// 
// WhiteBalanceOptions20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_WhiteBalanceOptions20_INCLUDED
#define ONVIF_WhiteBalanceOptions20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class FloatRange;
class WhiteBalanceOptions20Extension;
} 


namespace ONVIF {


//@ name=WhiteBalanceOptions20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API WhiteBalanceOptions20
{
public:
	WhiteBalanceOptions20();

	WhiteBalanceOptions20(
		const std::vector<std::string>& mode, 
		const Poco::SharedPtr<FloatRange>& yrGain, 
		const Poco::SharedPtr<FloatRange>& ybGain, 
		const Poco::SharedPtr<WhiteBalanceOptions20Extension>& extension);

	virtual ~WhiteBalanceOptions20();

	const Poco::SharedPtr<WhiteBalanceOptions20Extension>& getExtension() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	const Poco::SharedPtr<FloatRange>& getYbGain() const;

	const Poco::SharedPtr<FloatRange>& getYrGain() const;

	void setExtension(const Poco::SharedPtr<WhiteBalanceOptions20Extension>& val);

	void setExtension(Poco::SharedPtr<WhiteBalanceOptions20Extension>&& val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

	void setYbGain(const Poco::SharedPtr<FloatRange>& val);

	void setYbGain(Poco::SharedPtr<FloatRange>&& val);

	void setYrGain(const Poco::SharedPtr<FloatRange>& val);

	void setYrGain(Poco::SharedPtr<FloatRange>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ mandatory=false
	//@ name=YrGain
	//@ order=1
	Poco::SharedPtr<FloatRange> _yrGain;

	//@ mandatory=false
	//@ name=YbGain
	//@ order=2
	Poco::SharedPtr<FloatRange> _ybGain;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<WhiteBalanceOptions20Extension> _extension;

};


inline const Poco::SharedPtr<WhiteBalanceOptions20Extension>& WhiteBalanceOptions20::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& WhiteBalanceOptions20::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& WhiteBalanceOptions20::getMode()
{
	return _mode;
}


inline const Poco::SharedPtr<FloatRange>& WhiteBalanceOptions20::getYbGain() const
{
	return _ybGain;
}


inline const Poco::SharedPtr<FloatRange>& WhiteBalanceOptions20::getYrGain() const
{
	return _yrGain;
}


inline void WhiteBalanceOptions20::setExtension(const Poco::SharedPtr<WhiteBalanceOptions20Extension>& val)
{
	_extension = val;
}


inline void WhiteBalanceOptions20::setExtension(Poco::SharedPtr<WhiteBalanceOptions20Extension>&& val)
{
	_extension = std::move(val);
}


inline void WhiteBalanceOptions20::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void WhiteBalanceOptions20::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


inline void WhiteBalanceOptions20::setYbGain(const Poco::SharedPtr<FloatRange>& val)
{
	_ybGain = val;
}


inline void WhiteBalanceOptions20::setYbGain(Poco::SharedPtr<FloatRange>&& val)
{
	_ybGain = std::move(val);
}


inline void WhiteBalanceOptions20::setYrGain(const Poco::SharedPtr<FloatRange>& val)
{
	_yrGain = val;
}


inline void WhiteBalanceOptions20::setYrGain(Poco::SharedPtr<FloatRange>&& val)
{
	_yrGain = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/FloatRange.h"
#include "ONVIF/WhiteBalanceOptions20Extension.h"


#endif // ONVIF_WhiteBalanceOptions20_INCLUDED
