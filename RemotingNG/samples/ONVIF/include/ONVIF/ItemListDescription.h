// 
// ItemListDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_ItemListDescription_INCLUDED
#define ONVIF_ItemListDescription_INCLUDED


#include "ONVIF/ElementItemDescription.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/SimpleItemDescription.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ItemListDescriptionExtension;
} 


namespace ONVIF {


//@ name=ItemListDescription
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API ItemListDescription
{
public:
	ItemListDescription();

	ItemListDescription(
		const std::vector<SimpleItemDescription>& simpleItemDescription, 
		const std::vector<ElementItemDescription>& elementItemDescription, 
		const Poco::SharedPtr<ItemListDescriptionExtension>& extension);

	virtual ~ItemListDescription();

	const std::vector<ElementItemDescription>& getElementItemDescription() const;

	std::vector<ElementItemDescription>& getElementItemDescription();

	const Poco::SharedPtr<ItemListDescriptionExtension>& getExtension() const;

	const std::vector<SimpleItemDescription>& getSimpleItemDescription() const;

	std::vector<SimpleItemDescription>& getSimpleItemDescription();

	void setElementItemDescription(const std::vector<ElementItemDescription>& val);

	void setElementItemDescription(std::vector<ElementItemDescription>&& val);

	void setExtension(const Poco::SharedPtr<ItemListDescriptionExtension>& val);

	void setExtension(Poco::SharedPtr<ItemListDescriptionExtension>&& val);

	void setSimpleItemDescription(const std::vector<SimpleItemDescription>& val);

	void setSimpleItemDescription(std::vector<SimpleItemDescription>&& val);

private:
	//@ mandatory=false
	//@ name=SimpleItemDescription
	//@ order=0
	std::vector<SimpleItemDescription> _simpleItemDescription;

	//@ mandatory=false
	//@ name=ElementItemDescription
	//@ order=1
	std::vector<ElementItemDescription> _elementItemDescription;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<ItemListDescriptionExtension> _extension;

};


inline const std::vector<ElementItemDescription>& ItemListDescription::getElementItemDescription() const
{
	return _elementItemDescription;
}


inline std::vector<ElementItemDescription>& ItemListDescription::getElementItemDescription()
{
	return _elementItemDescription;
}


inline const Poco::SharedPtr<ItemListDescriptionExtension>& ItemListDescription::getExtension() const
{
	return _extension;
}


inline const std::vector<SimpleItemDescription>& ItemListDescription::getSimpleItemDescription() const
{
	return _simpleItemDescription;
}


inline std::vector<SimpleItemDescription>& ItemListDescription::getSimpleItemDescription()
{
	return _simpleItemDescription;
}


inline void ItemListDescription::setElementItemDescription(const std::vector<ElementItemDescription>& val)
{
	_elementItemDescription = val;
}


inline void ItemListDescription::setElementItemDescription(std::vector<ElementItemDescription>&& val)
{
	_elementItemDescription = std::move(val);
}


inline void ItemListDescription::setExtension(const Poco::SharedPtr<ItemListDescriptionExtension>& val)
{
	_extension = val;
}


inline void ItemListDescription::setExtension(Poco::SharedPtr<ItemListDescriptionExtension>&& val)
{
	_extension = std::move(val);
}


inline void ItemListDescription::setSimpleItemDescription(const std::vector<SimpleItemDescription>& val)
{
	_simpleItemDescription = val;
}


inline void ItemListDescription::setSimpleItemDescription(std::vector<SimpleItemDescription>&& val)
{
	_simpleItemDescription = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ItemListDescriptionExtension.h"


#endif // ONVIF_ItemListDescription_INCLUDED
