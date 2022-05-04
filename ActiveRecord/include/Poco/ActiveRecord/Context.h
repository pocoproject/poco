//
// Context.h
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  Context
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecord_Context_INCLUDED
#define ActiveRecord_Context_INCLUDED


#include "Poco/ActiveRecord/ActiveRecordLib.h"
#include "Poco/ActiveRecord/StatementPlaceholderProvider.h"
#include "Poco/Data/Session.h"
#include "Poco/RefCountedObject.h"
#include "Poco/AutoPtr.h"


namespace Poco {
namespace ActiveRecord {


class ActiveRecordLib_API Context: public Poco::RefCountedObject
	/// Context information for ActiveRecord objects.
{
public:
	using Ptr = Poco::AutoPtr<Context>;

	explicit Context(const Poco::Data::Session& session);
		/// Creates the Context from an existing Poco::Data::Session.

	Context(const std::string& connector, const std::string& connectionString);
		/// Creates the Context from a connector name and connection string.

	~Context() = default;
		/// Destroys the Context.

	Poco::Data::Session& session();
		/// Returns the database session.

	StatementPlaceholderProvider::Ptr statementPlaceholderProvider() const;
		/// Returns a new StatementPlaceholderProvider.

private:
	Context() = delete;
	Context(const Context&) = delete;
	Context& operator = (const Context&) = delete;

	Poco::Data::Session _session;
};


//
// inlines
//


inline Poco::Data::Session& Context::session()
{
	return _session;
}


} } // namespace Poco::ActiveRecord


#endif // ActiveRecord_Context_INCLUDED
