// 
// MessageDescription.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_MessageDescription_INCLUDED
#define ONVIF_MessageDescription_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ItemListDescription;
class MessageDescriptionExtension;
} 


namespace ONVIF {


//@ name=MessageDescription
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API MessageDescription
{
public:
	MessageDescription();

	MessageDescription(
		Poco::Optional<bool> isProperty, 
		const Poco::SharedPtr<ItemListDescription>& source, 
		const Poco::SharedPtr<ItemListDescription>& key, 
		const Poco::SharedPtr<ItemListDescription>& data, 
		const Poco::SharedPtr<MessageDescriptionExtension>& extension);

	virtual ~MessageDescription();

	const Poco::SharedPtr<ItemListDescription>& getData() const;

	const Poco::SharedPtr<MessageDescriptionExtension>& getExtension() const;

	Poco::Optional<bool> getIsProperty() const;

	const Poco::SharedPtr<ItemListDescription>& getKey() const;

	const Poco::SharedPtr<ItemListDescription>& getSource() const;

	void setData(const Poco::SharedPtr<ItemListDescription>& val);

	void setData(Poco::SharedPtr<ItemListDescription>&& val);

	void setExtension(const Poco::SharedPtr<MessageDescriptionExtension>& val);

	void setExtension(Poco::SharedPtr<MessageDescriptionExtension>&& val);

	void setIsProperty(Poco::Optional<bool> val);

	void setKey(const Poco::SharedPtr<ItemListDescription>& val);

	void setKey(Poco::SharedPtr<ItemListDescription>&& val);

	void setSource(const Poco::SharedPtr<ItemListDescription>& val);

	void setSource(Poco::SharedPtr<ItemListDescription>&& val);

private:
	//@ mandatory=false
	//@ name=IsProperty
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _isProperty;

	//@ mandatory=false
	//@ name=Source
	//@ order=1
	Poco::SharedPtr<ItemListDescription> _source;

	//@ mandatory=false
	//@ name=Key
	//@ order=2
	Poco::SharedPtr<ItemListDescription> _key;

	//@ mandatory=false
	//@ name=Data
	//@ order=3
	Poco::SharedPtr<ItemListDescription> _data;

	//@ mandatory=false
	//@ name=Extension
	//@ order=4
	Poco::SharedPtr<MessageDescriptionExtension> _extension;

};


inline const Poco::SharedPtr<ItemListDescription>& MessageDescription::getData() const
{
	return _data;
}


inline const Poco::SharedPtr<MessageDescriptionExtension>& MessageDescription::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> MessageDescription::getIsProperty() const
{
	return _isProperty;
}


inline const Poco::SharedPtr<ItemListDescription>& MessageDescription::getKey() const
{
	return _key;
}


inline const Poco::SharedPtr<ItemListDescription>& MessageDescription::getSource() const
{
	return _source;
}


inline void MessageDescription::setData(const Poco::SharedPtr<ItemListDescription>& val)
{
	_data = val;
}


inline void MessageDescription::setData(Poco::SharedPtr<ItemListDescription>&& val)
{
	_data = std::move(val);
}


inline void MessageDescription::setExtension(const Poco::SharedPtr<MessageDescriptionExtension>& val)
{
	_extension = val;
}


inline void MessageDescription::setExtension(Poco::SharedPtr<MessageDescriptionExtension>&& val)
{
	_extension = std::move(val);
}


inline void MessageDescription::setIsProperty(Poco::Optional<bool> val)
{
	_isProperty = val;
}


inline void MessageDescription::setKey(const Poco::SharedPtr<ItemListDescription>& val)
{
	_key = val;
}


inline void MessageDescription::setKey(Poco::SharedPtr<ItemListDescription>&& val)
{
	_key = std::move(val);
}


inline void MessageDescription::setSource(const Poco::SharedPtr<ItemListDescription>& val)
{
	_source = val;
}


inline void MessageDescription::setSource(Poco::SharedPtr<ItemListDescription>&& val)
{
	_source = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ItemListDescription.h"
#include "ONVIF/MessageDescriptionExtension.h"


#endif // ONVIF_MessageDescription_INCLUDED
