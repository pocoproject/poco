// 
// Message.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Message_INCLUDED
#define ONVIF_Message_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class ItemList;
class MessageExtension;
} 


namespace ONVIF {


//@ name="#Message"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Message
{
public:
	Message();

	Message(
		const Poco::DateTime& utcTime, 
		const Poco::Optional<std::string>& propertyOperation, 
		const Poco::SharedPtr<ItemList>& source, 
		const Poco::SharedPtr<ItemList>& key, 
		const Poco::SharedPtr<ItemList>& data, 
		const Poco::SharedPtr<MessageExtension>& extension);

	virtual ~Message();

	const Poco::SharedPtr<ItemList>& getData() const;

	const Poco::SharedPtr<MessageExtension>& getExtension() const;

	const Poco::SharedPtr<ItemList>& getKey() const;

	const Poco::Optional<std::string>& getPropertyOperation() const;

	const Poco::SharedPtr<ItemList>& getSource() const;

	//@ return={xsdType=dateTime}
	const Poco::DateTime& getUtcTime() const;

	void setData(const Poco::SharedPtr<ItemList>& val);

	void setData(Poco::SharedPtr<ItemList>&& val);

	void setExtension(const Poco::SharedPtr<MessageExtension>& val);

	void setExtension(Poco::SharedPtr<MessageExtension>&& val);

	void setKey(const Poco::SharedPtr<ItemList>& val);

	void setKey(Poco::SharedPtr<ItemList>&& val);

	void setPropertyOperation(const Poco::Optional<std::string>& val);

	void setPropertyOperation(Poco::Optional<std::string>&& val);

	void setSource(const Poco::SharedPtr<ItemList>& val);

	void setSource(Poco::SharedPtr<ItemList>&& val);

	//@ $val={xsdType=dateTime}
	void setUtcTime(const Poco::DateTime& val);

	//@ $val={xsdType=dateTime}
	void setUtcTime(Poco::DateTime&& val);

private:
	//@ name=UtcTime
	//@ order=0
	//@ type=attr
	//@ xsdType=dateTime
	Poco::DateTime _utcTime;

	//@ mandatory=false
	//@ name=PropertyOperation
	//@ order=1
	//@ type=attr
	Poco::Optional<std::string> _propertyOperation;

	//@ mandatory=false
	//@ name=Source
	//@ order=2
	Poco::SharedPtr<ItemList> _source;

	//@ mandatory=false
	//@ name=Key
	//@ order=3
	Poco::SharedPtr<ItemList> _key;

	//@ mandatory=false
	//@ name=Data
	//@ order=4
	Poco::SharedPtr<ItemList> _data;

	//@ mandatory=false
	//@ name=Extension
	//@ order=5
	Poco::SharedPtr<MessageExtension> _extension;

};


inline const Poco::SharedPtr<ItemList>& Message::getData() const
{
	return _data;
}


inline const Poco::SharedPtr<MessageExtension>& Message::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<ItemList>& Message::getKey() const
{
	return _key;
}


inline const Poco::Optional<std::string>& Message::getPropertyOperation() const
{
	return _propertyOperation;
}


inline const Poco::SharedPtr<ItemList>& Message::getSource() const
{
	return _source;
}


inline const Poco::DateTime& Message::getUtcTime() const
{
	return _utcTime;
}


inline void Message::setData(const Poco::SharedPtr<ItemList>& val)
{
	_data = val;
}


inline void Message::setData(Poco::SharedPtr<ItemList>&& val)
{
	_data = std::move(val);
}


inline void Message::setExtension(const Poco::SharedPtr<MessageExtension>& val)
{
	_extension = val;
}


inline void Message::setExtension(Poco::SharedPtr<MessageExtension>&& val)
{
	_extension = std::move(val);
}


inline void Message::setKey(const Poco::SharedPtr<ItemList>& val)
{
	_key = val;
}


inline void Message::setKey(Poco::SharedPtr<ItemList>&& val)
{
	_key = std::move(val);
}


inline void Message::setPropertyOperation(const Poco::Optional<std::string>& val)
{
	_propertyOperation = val;
}


inline void Message::setPropertyOperation(Poco::Optional<std::string>&& val)
{
	_propertyOperation = std::move(val);
}


inline void Message::setSource(const Poco::SharedPtr<ItemList>& val)
{
	_source = val;
}


inline void Message::setSource(Poco::SharedPtr<ItemList>&& val)
{
	_source = std::move(val);
}


inline void Message::setUtcTime(const Poco::DateTime& val)
{
	_utcTime = val;
}


inline void Message::setUtcTime(Poco::DateTime&& val)
{
	_utcTime = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/ItemList.h"
#include "ONVIF/MessageExtension.h"


#endif // ONVIF_Message_INCLUDED
