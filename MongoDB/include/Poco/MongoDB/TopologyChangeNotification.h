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
	/// - timestamp (Poco::Int64): Timestamp in seconds since epoch
	/// - topologyType (std::string): Human-readable topology type
	///   (e.g., "Replica Set (with Primary)", "Single Server", etc.)
	///
	/// Example usage:
	///     void MyClass::handleTopologyChange(const AutoPtr<TopologyChangeNotification>& pNf)
	///     {
	///         const auto& data = pNf->data();
	///
	///         std::string rsName = data["replicaSet"];
	///         Poco::Int64 timestamp = data["timestamp"];
	///         std::string topologyType = data["topologyType"];
	///
	///         // Handle topology change...
	///     }
	///
	///     // Register observer using NObserver:
	///     NotificationCenter::defaultCenter().addNObserver(
	///         *this,
	///         &MyClass::handleTopologyChange
	///     );
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
