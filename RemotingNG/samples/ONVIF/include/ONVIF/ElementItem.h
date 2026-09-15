// 
// ElementItem.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ElementItem_INCLUDED
#define ONVIF_ElementItem_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name="#ElementItem"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ElementItem
{
public:
	ElementItem();

	ElementItem(const std::string& name);

	virtual ~ElementItem();

	const std::string& getName() const;

	void setName(const std::string& val);

	void setName(std::string&& val);

private:
	//@ name=Name
	//@ order=0
	//@ type=attr
	std::string _name;

};


inline const std::string& ElementItem::getName() const
{
	return _name;
}


inline void ElementItem::setName(const std::string& val)
{
	_name = val;
}


inline void ElementItem::setName(std::string&& val)
{
	_name = std::move(val);
}


} // ONVIF


#endif // ONVIF_ElementItem_INCLUDED
