// 
// IntItems.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_IntItems_INCLUDED
#define ONVIF_IntItems_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=IntItems
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API IntItems
{
public:
	IntItems();

	IntItems(const std::vector<int>& items);

	virtual ~IntItems();

	const std::vector<int>& getItems() const;

	std::vector<int>& getItems();

	void setItems(const std::vector<int>& val);

private:
	//@ mandatory=false
	//@ name=Items
	//@ order=0
	std::vector<int> _items;

};


inline const std::vector<int>& IntItems::getItems() const
{
	return _items;
}


inline std::vector<int>& IntItems::getItems()
{
	return _items;
}


inline void IntItems::setItems(const std::vector<int>& val)
{
	_items = val;
}


} // ONVIF


#endif // ONVIF_IntItems_INCLUDED
