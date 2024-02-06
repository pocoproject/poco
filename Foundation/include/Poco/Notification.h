//
// Notification.h
//
// Library: Foundation
// Package: Notifications
// Module:  Notification
//
// Definition of the Notification class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Notification_INCLUDED
#define Foundation_Notification_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Mutex.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"
#include <memory>


namespace Poco {


class Foundation_API Notification: public RefCountedObject
	/// The base class for all notification classes used
	/// with the NotificationCenter and the NotificationQueue
	/// classes.
	/// The Notification class can be used with the AutoPtr
	/// template class.
{
public:
	using Ptr = AutoPtr<Notification>;

	Notification(const std::string& name = ""s);
		/// Creates the notification.

	virtual std::string name() const;
		/// Returns the name of the notification.
		/// The default implementation returns the class name.

protected:
	virtual ~Notification();
	std::unique_ptr<std::string> _pName;
};


} // namespace Poco


#endif // Foundation_Notification_INCLUDED
