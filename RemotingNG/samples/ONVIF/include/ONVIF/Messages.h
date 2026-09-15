// 
// Messages.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Messages_INCLUDED
#define ONVIF_Messages_INCLUDED


#include "ONVIF/MessageDescription.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name="#Messages"
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Messages: public MessageDescription
{
public:
	Messages();

	Messages(
		Poco::Optional<bool> isProperty, 
		const Poco::SharedPtr<ItemListDescription>& source, 
		const Poco::SharedPtr<ItemListDescription>& key, 
		const Poco::SharedPtr<ItemListDescription>& data, 
		const Poco::SharedPtr<MessageDescriptionExtension>& extension, 
		const std::string& parentTopic);

	virtual ~Messages();

	const std::string& getParentTopic() const;

	void setParentTopic(const std::string& val);

	void setParentTopic(std::string&& val);

private:
	//@ name=ParentTopic
	//@ order=0
	std::string _parentTopic;

};


inline const std::string& Messages::getParentTopic() const
{
	return _parentTopic;
}


inline void Messages::setParentTopic(const std::string& val)
{
	_parentTopic = val;
}


inline void Messages::setParentTopic(std::string&& val)
{
	_parentTopic = std::move(val);
}


} // ONVIF


#endif // ONVIF_Messages_INCLUDED
