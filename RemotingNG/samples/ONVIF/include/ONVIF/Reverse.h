// 
// Reverse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Reverse_INCLUDED
#define ONVIF_Reverse_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Reverse
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Reverse
{
public:
	Reverse();

	Reverse(const std::string& mode);

	virtual ~Reverse();

	const std::string& getMode() const;

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

};


inline const std::string& Reverse::getMode() const
{
	return _mode;
}


inline void Reverse::setMode(const std::string& val)
{
	_mode = val;
}


inline void Reverse::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_Reverse_INCLUDED
