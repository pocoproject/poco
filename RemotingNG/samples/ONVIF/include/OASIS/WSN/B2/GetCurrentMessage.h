// 
// GetCurrentMessage.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_GetCurrentMessage_INCLUDED
#define OASIS_WSN_B2_GetCurrentMessage_INCLUDED


#include "OASIS/WSN/B2/TopicExpression.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name="#GetCurrentMessage"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API GetCurrentMessage
{
public:
	GetCurrentMessage();

	GetCurrentMessage(const TopicExpression& topic);

	virtual ~GetCurrentMessage();

	const TopicExpression& getTopic() const;

	void setTopic(const TopicExpression& val);

	void setTopic(TopicExpression&& val);

private:
	//@ name=Topic
	//@ order=0
	TopicExpression _topic;

};


inline const TopicExpression& GetCurrentMessage::getTopic() const
{
	return _topic;
}


inline void GetCurrentMessage::setTopic(const TopicExpression& val)
{
	_topic = val;
}


inline void GetCurrentMessage::setTopic(TopicExpression&& val)
{
	_topic = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_GetCurrentMessage_INCLUDED
