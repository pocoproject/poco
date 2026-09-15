// 
// SimpleItem.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SimpleItem_INCLUDED
#define ONVIF_SimpleItem_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name="#SimpleItem"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SimpleItem
{
public:
	SimpleItem();

	SimpleItem(
		const std::string& name, 
		const std::string& value);

	virtual ~SimpleItem();

	const std::string& getName() const;

	const std::string& getValue() const;

	void setName(const std::string& val);

	void setName(std::string&& val);

	void setValue(const std::string& val);

	void setValue(std::string&& val);

private:
	//@ name=Name
	//@ order=0
	//@ type=attr
	std::string _name;

	//@ name=Value
	//@ order=1
	//@ type=attr
	std::string _value;

};


inline const std::string& SimpleItem::getName() const
{
	return _name;
}


inline const std::string& SimpleItem::getValue() const
{
	return _value;
}


inline void SimpleItem::setName(const std::string& val)
{
	_name = val;
}


inline void SimpleItem::setName(std::string&& val)
{
	_name = std::move(val);
}


inline void SimpleItem::setValue(const std::string& val)
{
	_value = val;
}


inline void SimpleItem::setValue(std::string&& val)
{
	_value = std::move(val);
}


} // ONVIF


#endif // ONVIF_SimpleItem_INCLUDED
