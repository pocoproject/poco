// 
// PTZCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZCapabilities_INCLUDED
#define ONVIF_PTZCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=PTZCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZCapabilities
{
public:
	PTZCapabilities();

	PTZCapabilities(const Poco::URI& xAddr);

	virtual ~PTZCapabilities();

	const Poco::URI& getXAddr() const;

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

};


inline const Poco::URI& PTZCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void PTZCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void PTZCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_PTZCapabilities_INCLUDED
