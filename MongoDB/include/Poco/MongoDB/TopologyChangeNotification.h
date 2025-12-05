//
// TopologyChangeNotification.h
//
// Library: MongoDB
// Package: MongoDB
// Module:  TopologyChangeNotification
//
// Definition of the TopologyChangeNotification class.
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef MongoDB_TopologyChangeNotification_INCLUDED
#define MongoDB_TopologyChangeNotification_INCLUDED


#include "Poco/MongoDB/MongoDB.h"
#include "Poco/Notification.h"
#include "Poco/Dynamic/Struct.h"
#include <string>


namespace Poco {
namespace MongoDB {


class MongoDB_API TopologyChangeNotification : public Notification
	/// Notification sent when MongoDB replica set topology changes.
	///
	/// This notification is posted to Poco::NotificationCenter::defaultCenter()
	/// whenever a topology change is detected during topology refresh.
	///
	/// The notification contains a Dynamic::Struct with the following members:
	/// - replicaSet (std::string): The replica set name
	/// - timestamp (Poco::Int64): Timestamp in seconds since Unix epoch
	/// - topologyType (std::string): Human-readable topology type
	///   (e.g., "Replica Set (with Primary)", "Single Server", etc.)
	/// - changeDescription (std::string): Brief description of what changed
	///   (e.g., "Primary elected: mongo1:27017", "Servers: 2 -> 3")
	///
	/// Example usage:
	///     class MyClass
	///     {
	///     public:
	///         MyClass()
	///         {
	///             // Register observer
	///             NotificationCenter::defaultCenter().addNObserver(
	///                 *this,
	///                 &MyClass::handleTopologyChange
	///             );
	///         }
	///
	///         ~MyClass()
	///         {
	///             // Unregister observer
	///             NotificationCenter::defaultCenter().removeNObserver(
	///                 *this,
	///                 &MyClass::handleTopologyChange
	///             );
	///         }
	///
	///         void handleTopologyChange(const AutoPtr<TopologyChangeNotification>& pNf)
	///         {
	///             const auto& data = pNf->data();
	///
	///             std::string rsName = data["replicaSet"];
	///             Poco::Int64 timestamp = data["timestamp"];
	///             std::string topologyType = data["topologyType"];
	///             std::string changeDesc = data["changeDescription"];
	///
	///             // Handle topology change...
	///         }
	///     };
{
public:
	using Ptr = AutoPtr<TopologyChangeNotification>;

	TopologyChangeNotification(const Dynamic::Struct<std::string>& data);
		/// Creates a TopologyChangeNotification with the given data.

	const Dynamic::Struct<std::string>& data() const;
		/// Returns the topology change data.

	std::string name() const override;
		/// Returns the notification name.

private:
	Dynamic::Struct<std::string> _data;
};


//
// inlines
//


inline TopologyChangeNotification::TopologyChangeNotification(const Dynamic::Struct<std::string>& data):
	_data(data)
{
}


inline const Dynamic::Struct<std::string>& TopologyChangeNotification::data() const
{
	return _data;
}


inline std::string TopologyChangeNotification::name() const
{
	return "TopologyChangeNotification";
}


} } // namespace Poco::MongoDB


#endif // MongoDB_TopologyChangeNotification_INCLUDED
