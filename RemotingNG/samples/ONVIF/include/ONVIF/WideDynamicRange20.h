// 
// WideDynamicRange20.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_WideDynamicRange20_INCLUDED
#define ONVIF_WideDynamicRange20_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=WideDynamicRange20
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API WideDynamicRange20
{
public:
	WideDynamicRange20();

	WideDynamicRange20(
		const std::string& mode, 
		Poco::Optional<float> level);

	virtual ~WideDynamicRange20();

	Poco::Optional<float> getLevel() const;

	const std::string& getMode() const;

	void setLevel(Poco::Optional<float> val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ mandatory=false
	//@ name=Level
	//@ order=1
	Poco::Optional<float> _level;

};


inline Poco::Optional<float> WideDynamicRange20::getLevel() const
{
	return _level;
}


inline const std::string& WideDynamicRange20::getMode() const
{
	return _mode;
}


inline void WideDynamicRange20::setLevel(Poco::Optional<float> val)
{
	_level = val;
}


inline void WideDynamicRange20::setMode(const std::string& val)
{
	_mode = val;
}


inline void WideDynamicRange20::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_WideDynamicRange20_INCLUDED
