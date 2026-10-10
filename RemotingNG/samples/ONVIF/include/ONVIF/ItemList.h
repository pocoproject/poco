// 
// ItemList.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ItemList_INCLUDED
#define ONVIF_ItemList_INCLUDED


#include "ONVIF/ElementItem.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/SimpleItem.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ItemListExtension;
} 


namespace ONVIF {


//@ name=ItemList
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ItemList
{
public:
	ItemList();

	ItemList(
		const std::vector<SimpleItem>& simpleItem, 
		const std::vector<ElementItem>& elementItem, 
		const Poco::SharedPtr<ItemListExtension>& extension);

	virtual ~ItemList();

	const std::vector<ElementItem>& getElementItem() const;

	std::vector<ElementItem>& getElementItem();

	const Poco::SharedPtr<ItemListExtension>& getExtension() const;

	const std::vector<SimpleItem>& getSimpleItem() const;

	std::vector<SimpleItem>& getSimpleItem();

	void setElementItem(const std::vector<ElementItem>& val);

	void setElementItem(std::vector<ElementItem>&& val);

	void setExtension(const Poco::SharedPtr<ItemListExtension>& val);

	void setExtension(Poco::SharedPtr<ItemListExtension>&& val);

	void setSimpleItem(const std::vector<SimpleItem>& val);

	void setSimpleItem(std::vector<SimpleItem>&& val);

private:
	//@ mandatory=false
	//@ name=SimpleItem
	//@ order=0
	std::vector<SimpleItem> _simpleItem;

	//@ mandatory=false
	//@ name=ElementItem
	//@ order=1
	std::vector<ElementItem> _elementItem;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<ItemListExtension> _extension;

};


inline const std::vector<ElementItem>& ItemList::getElementItem() const
{
	return _elementItem;
}


inline std::vector<ElementItem>& ItemList::getElementItem()
{
	return _elementItem;
}


inline const Poco::SharedPtr<ItemListExtension>& ItemList::getExtension() const
{
	return _extension;
}


inline const std::vector<SimpleItem>& ItemList::getSimpleItem() const
{
	return _simpleItem;
}


inline std::vector<SimpleItem>& ItemList::getSimpleItem()
{
	return _simpleItem;
}


inline void ItemList::setElementItem(const std::vector<ElementItem>& val)
{
	_elementItem = val;
}


inline void ItemList::setElementItem(std::vector<ElementItem>&& val)
{
	_elementItem = std::move(val);
}


inline void ItemList::setExtension(const Poco::SharedPtr<ItemListExtension>& val)
{
	_extension = val;
}


inline void ItemList::setExtension(Poco::SharedPtr<ItemListExtension>&& val)
{
	_extension = std::move(val);
}


inline void ItemList::setSimpleItem(const std::vector<SimpleItem>& val)
{
	_simpleItem = val;
}


inline void ItemList::setSimpleItem(std::vector<SimpleItem>&& val)
{
	_simpleItem = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ItemListExtension.h"


#endif // ONVIF_ItemList_INCLUDED
