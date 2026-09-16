// 
// DisplayCapabilities.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DisplayCapabilities_INCLUDED
#define ONVIF_DisplayCapabilities_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DisplayCapabilities
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DisplayCapabilities
{
public:
	DisplayCapabilities();

	DisplayCapabilities(
		const Poco::URI& xAddr, 
		bool fixedLayout);

	virtual ~DisplayCapabilities();

	bool getFixedLayout() const;

	const Poco::URI& getXAddr() const;

	void setFixedLayout(bool val);

	void setXAddr(const Poco::URI& val);

	void setXAddr(Poco::URI&& val);

private:
	//@ name=XAddr
	//@ order=0
	Poco::URI _xAddr;

	//@ name=FixedLayout
	//@ order=1
	bool _fixedLayout;

};


inline bool DisplayCapabilities::getFixedLayout() const
{
	return _fixedLayout;
}


inline const Poco::URI& DisplayCapabilities::getXAddr() const
{
	return _xAddr;
}


inline void DisplayCapabilities::setFixedLayout(bool val)
{
	_fixedLayout = val;
}


inline void DisplayCapabilities::setXAddr(const Poco::URI& val)
{
	_xAddr = val;
}


inline void DisplayCapabilities::setXAddr(Poco::URI&& val)
{
	_xAddr = std::move(val);
}


} // ONVIF


#endif // ONVIF_DisplayCapabilities_INCLUDED
