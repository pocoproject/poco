// 
// EFlip.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_EFlip_INCLUDED
#define ONVIF_EFlip_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=EFlip
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API EFlip
{
public:
	EFlip();

	EFlip(const std::string& mode);

	virtual ~EFlip();

	const std::string& getMode() const;

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

};


inline const std::string& EFlip::getMode() const
{
	return _mode;
}


inline void EFlip::setMode(const std::string& val)
{
	_mode = val;
}


inline void EFlip::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_EFlip_INCLUDED
