// 
// ReplayCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ReplayCapabilities_INCLUDED
#define ONVIF_ReplayCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=ReplayCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ReplayCapabilities
{
public:
	ReplayCapabilities();

	ReplayCapabilities(const Poco::URI& xAddr);

	virtual ~ReplayCapabilities();

	const Poco::URI& getXAddr() const;

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

};


inline const Poco::URI& ReplayCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void ReplayCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void ReplayCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_ReplayCapabilities_INCLUDED
