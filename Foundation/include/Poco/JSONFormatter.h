//
// JSONFormatter.h
//
// Library: Foundation
// Package: Logging
// Module:  JSONFormatter
//
// Definition of the JSONFormatter class.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_JSONFormatter_INCLUDED
#define Foundation_JSONFormatter_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Formatter.h"
#include "Poco/Message.h"
#include <vector>


namespace Poco {


class Foundation_API JSONFormatter: public Formatter
	/// This formatter formats log messages as compact 
	/// (no unnecessary whitespace) single-line JSON strings.
	///
	/// The following JSON schema is used:
	///     {
	///         "timestamp": "2024-09-26T13:41:23.324461Z",
	///         "source": "sample",
	///         "level": "information",
	///         "message": "This is a test message.",
	///         "thread": 12,
	///         "file": "source.cpp",
	///         "line": 456,
	///         "params": {
	///             "prop1": "value1"
	///         }
	///     }
	///
	/// The "file" and "line" properties will only be included if the log
	/// message contains a file name and line number.
	///
	/// The "params" object will only be included if custom parameters
	/// have been added to the Message.
{
public:
	using Ptr = AutoPtr<JSONFormatter>;

	JSONFormatter() = default;
		/// Creates a JSONFormatter.

	~JSONFormatter() override = default;
		/// Destroys the JSONFormatter.

	void format(const Message& msg, std::string& text) override;
		/// Formats the message as a JSON string.

	void setProperty(const std::string& name, const std::string& value) override;
		/// Sets the property with the given name to the given value.
		///
		/// The following properties are supported:
		///
		///     * times: Specifies whether times are adjusted for local time
		///       or taken as they are in UTC. Supported values are "local" and "UTC".
		///     * thread: Specifies the value given for the thread. Can be
		///       "none" (excluded), "name" (thread name), "id" (POCO thread ID) or "osid" 
		///       (operating system thread ID).
		///
		/// If any other property name is given, a PropertyNotSupported
		/// exception is thrown.

	std::string getProperty(const std::string& name) const override;
		/// Returns the value of the property with the given name or
		/// throws a PropertyNotSupported exception if the given
		/// name is not recognized.

	static const std::string PROP_TIMES;
	static const std::string PROP_THREAD;

protected:
	std::string getThread(const Message& message) const;
	static const std::string& getPriorityName(int prio);

	enum ThreadFormat
	{
		JSONF_THREAD_NONE = 0,
		JSONF_THREAD_NAME = 1,
		JSONF_THREAD_ID = 2,
		JSONF_THREAD_OS_ID = 3
	};

private:
	bool _localTime = false;
	ThreadFormat _threadFormat = JSONF_THREAD_ID;
};


} // namespace Poco


#endif // Foundation_JSONFormatter_INCLUDED
