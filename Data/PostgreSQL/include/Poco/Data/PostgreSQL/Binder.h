//
// Binder.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Binder
//
// Definition of the Binder class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PostgreSQL_Binder_INCLUDED
#define SQL_PostgreSQL_Binder_INCLUDED


#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/Data/PostgreSQL/PostgreSQLException.h"
#include "Poco/Data/AbstractBinder.h"
#include "Poco/Data/MetaColumn.h"
#include "Poco/Data/LOB.h"
#include "Poco/Types.h"
#include <libpq-fe.h>


namespace Poco {
namespace Data {
namespace PostgreSQL {


class PostgreSQL_API Binder: public Poco::Data::AbstractBinder
	/// Binds INPUT (only) placeholders in the sql query to the provided values.
	/// Allows data type mapping at statement execution time.
{
public:
	using Ptr = SharedPtr<Binder>;

	Binder();
		/// Creates the Binder.

	virtual ~Binder();
		/// Destroys the Binder.

	virtual void bind(std::size_t pos, const Poco::Int8& val, Direction dir = PD_IN);
		/// Binds an Int8.

	virtual void bind(std::size_t pos, const Poco::UInt8& val, Direction dir = PD_IN);
		/// Binds an UInt8.

	virtual void bind(std::size_t pos, const Poco::Int16& val, Direction dir = PD_IN);
		/// Binds an Int16.

	virtual void bind(std::size_t pos, const Poco::UInt16& val, Direction dir = PD_IN);
		/// Binds an UInt16.

	virtual void bind(std::size_t pos, const Poco::Int32& val, Direction dir = PD_IN);
		/// Binds an Int32.

	virtual void bind(std::size_t pos, const Poco::UInt32& val, Direction dir = PD_IN);
		/// Binds an UInt32.

	virtual void bind(std::size_t pos, const Poco::Int64& val, Direction dir = PD_IN);
		/// Binds an Int64.

	virtual void bind(std::size_t pos, const Poco::UInt64& val, Direction dir = PD_IN);
		/// Binds an UInt64.

#ifndef POCO_INT64_IS_LONG
	virtual void bind(std::size_t pos, const long& val, Direction dir = PD_IN);
		/// Binds a long.

	virtual void bind(std::size_t pos, const unsigned long& val, Direction dir = PD_IN);
		/// Binds an unsigned long.
#endif

	virtual void bind(std::size_t pos, const bool& val, Direction dir = PD_IN);
		/// Binds a boolean.

	virtual void bind(std::size_t pos, const float& val, Direction dir = PD_IN);
		/// Binds a float.

	virtual void bind(std::size_t pos, const double& val, Direction dir = PD_IN);
		/// Binds a double.

	virtual void bind(std::size_t pos, const char& val, Direction dir = PD_IN);
		/// Binds a single character.

	virtual void bind(std::size_t pos, const std::string& val, Direction dir = PD_IN);
		/// Binds a string.

	virtual void bind(std::size_t pos, const Poco::Data::BLOB& val, Direction dir = PD_IN);
		/// Binds a BLOB.

	virtual void bind(std::size_t pos, const Poco::Data::CLOB& val, Direction dir = PD_IN);
		/// Binds a CLOB.

	virtual void bind(std::size_t pos, const DateTime& val, Direction dir = PD_IN);
		/// Binds a DateTime.

	virtual void bind(std::size_t pos, const Date& val, Direction dir = PD_IN);
		/// Binds a Date.

	virtual void bind(std::size_t pos, const Time& val, Direction dir = PD_IN);
		/// Binds a Time.

	virtual void bind(std::size_t pos, const UUID& val, Direction dir = PD_IN);
		/// Binds a UUID.

	virtual void bind(std::size_t pos, const NullData& val, Direction dir = PD_IN);
		/// Binds a null.

	virtual void bind(std::size_t pos, const std::vector<Poco::Int8>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::Int8>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::Int8>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt8>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt8>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::UInt8>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Poco::Int16>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::Int16>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::Int16>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt16>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt16>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::UInt16>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Poco::Int32>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::Int32>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::Int32>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt32>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt32>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::UInt32>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Poco::Int64>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::Int64>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::Int64>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt64>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt64>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Poco::UInt64>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<bool>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<bool>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<bool>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<float>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<float>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<float>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<double>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<double>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<double>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<char>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<char>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<char>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<BLOB>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<BLOB>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<BLOB>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<CLOB>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<CLOB>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<CLOB>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<DateTime>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<DateTime>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<DateTime>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Date>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Date>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Date>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<Time>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<Time>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<Time>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<NullData>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<NullData>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<NullData>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::vector<std::string>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::deque<std::string>& val, Direction dir = PD_IN);

	virtual void bind(std::size_t pos, const std::list<std::string>& val, Direction dir = PD_IN);

	std::size_t size() const;
		/// Return count of bound parameters

	InputParameterVector bindVector() const;
		/// Return the vector of bound parameters.

	void updateBindVectorToCurrentValues();
		/// obtain the current version of the bound data and update the internal representation

private:
	Binder(const Binder&);
		/// Don't copy the binder

	virtual void bind(std::size_t, const char* const&, Direction)
		/// Binds a const char ptr.
		/// This is a private no-op in this implementation
		/// due to security risk.
	{
	}

	void realBind(std::size_t aPosition, Poco::Data::MetaColumn::ColumnDataType aFieldType, const void* aBufferPtr, std::size_t aLength);
		/// Common bind implementation

private:
	InputParameterVector _bindVector;
};


} } } // namespace Poco::Data::PostgreSQL


#endif // Data_PostgreSQL_Binder_INCLUDED
