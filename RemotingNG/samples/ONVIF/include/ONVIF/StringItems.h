// 
// StringItems.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_StringItems_INCLUDED
#define ONVIF_StringItems_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name="#StringItems"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API StringItems
{
public:
	StringItems();

	StringItems(const std::vector<std::string>& item);

	virtual ~StringItems();

	const std::vector<std::string>& getItem() const;

	std::vector<std::string>& getItem();

	void setItem(const std::vector<std::string>& val);

	void setItem(std::vector<std::string>&& val);

private:
	//@ name=Item
	//@ order=0
	std::vector<std::string> _item;

};


inline const std::vector<std::string>& StringItems::getItem() const
{
	return _item;
}


inline std::vector<std::string>& StringItems::getItem()
{
	return _item;
}


inline void StringItems::setItem(const std::vector<std::string>& val)
{
	_item = val;
}


inline void StringItems::setItem(std::vector<std::string>&& val)
{
	_item = std::move(val);
}


} // ONVIF


#endif // ONVIF_StringItems_INCLUDED
