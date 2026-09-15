// 
// GetMessagesResponse.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_B2_GetMessagesResponse_INCLUDED
#define OASIS_WSN_B2_GetMessagesResponse_INCLUDED


#include "OASIS/WSN/B2/NotificationMessageHolder.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace B2 {


//@ name="#GetMessagesResponse"
//@ namespace="http://docs.oasis-open.org/wsn/b-2"
//@ serialize
class ONVIF_API GetMessagesResponse
{
public:
	GetMessagesResponse();

	GetMessagesResponse(const std::vector<NotificationMessageHolder>& notificationMessage);

	virtual ~GetMessagesResponse();

	const std::vector<NotificationMessageHolder>& getNotificationMessage() const;

	std::vector<NotificationMessageHolder>& getNotificationMessage();

	void setNotificationMessage(const std::vector<NotificationMessageHolder>& val);

	void setNotificationMessage(std::vector<NotificationMessageHolder>&& val);

private:
	//@ mandatory=false
	//@ name=NotificationMessage
	//@ order=0
	std::vector<NotificationMessageHolder> _notificationMessage;

};


inline const std::vector<NotificationMessageHolder>& GetMessagesResponse::getNotificationMessage() const
{
	return _notificationMessage;
}


inline std::vector<NotificationMessageHolder>& GetMessagesResponse::getNotificationMessage()
{
	return _notificationMessage;
}


inline void GetMessagesResponse::setNotificationMessage(const std::vector<NotificationMessageHolder>& val)
{
	_notificationMessage = val;
}


inline void GetMessagesResponse::setNotificationMessage(std::vector<NotificationMessageHolder>&& val)
{
	_notificationMessage = std::move(val);
}


} } } // OASIS::WSN::B2


#endif // OASIS_WSN_B2_GetMessagesResponse_INCLUDED
