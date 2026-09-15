// 
// FloatItems.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_FloatItems_INCLUDED
#define ONVIF_FloatItems_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=FloatItems
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API FloatItems
{
public:
	FloatItems();

	FloatItems(const std::vector<float>& items);

	virtual ~FloatItems();

	const std::vector<float>& getItems() const;

	std::vector<float>& getItems();

	void setItems(const std::vector<float>& val);

private:
	//@ mandatory=false
	//@ name=Items
	//@ order=0
	std::vector<float> _items;

};


inline const std::vector<float>& FloatItems::getItems() const
{
	return _items;
}


inline std::vector<float>& FloatItems::getItems()
{
	return _items;
}


inline void FloatItems::setItems(const std::vector<float>& val)
{
	_items = val;
}


} // ONVIF


#endif // ONVIF_FloatItems_INCLUDED
