// 
// Include.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef W3_XOP_Include_INCLUDED
#define W3_XOP_Include_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include "Poco/URI.h"
#include <string>
#include <vector>


namespace W3 {
namespace XOP {


//@ name=Include
//@ namespace="http://www.w3.org/2004/08/xop/include"
//@ serialize
class ONVIF_API Include
{
public:
	Include();

	Include(const Poco::URI& href);

	virtual ~Include();

	const Poco::URI& getHref() const;

	void setHref(const Poco::URI& val);

	void setHref(Poco::URI&& val);

private:
	//@ name=href
	//@ order=0
	//@ type=attr
	Poco::URI _href;

};


inline const Poco::URI& Include::getHref() const
{
	return _href;
}


inline void Include::setHref(const Poco::URI& val)
{
	_href = val;
}


inline void Include::setHref(Poco::URI&& val)
{
	_href = std::move(val);
}


} } // W3::XOP


#endif // W3_XOP_Include_INCLUDED
