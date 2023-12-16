//
// Binder.h
//
// Library: Data/MySQL
// Package: MySQL
// Module:  Binder
//
// Definition of the Binder class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_MySQL_Binder_INCLUDED
#define Data_MySQL_Binder_INCLUDED


#include "Poco/Data/MySQL/MySQL.h"
#include "Poco/Data/AbstractBinder.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include <mysql/mysql.h>


namespace Poco {
namespace Data {
namespace MySQL {


class MySQL_API Binder: public Poco::Data::AbstractBinder
	/// Binds placeholders in the sql query to the provided values. Performs data types mapping.
{
public:
	typedef SharedPtr<Binder> Ptr;

	Binder();
		/// Creates the Binder.

	~Binder() override;
		/// Destroys the Binder.

	void bind(std::size_t pos, const Poco::Int8& val, Direction dir) override;
		/// Binds an Int8.

	void bind(std::size_t pos, const Poco::UInt8& val, Direction dir) override;
		/// Binds an UInt8.

	void bind(std::size_t pos, const Poco::Int16& val, Direction dir) override;
		/// Binds an Int16.

	void bind(std::size_t pos, const Poco::UInt16& val, Direction dir) override;
		/// Binds an UInt16.

	void bind(std::size_t pos, const Poco::Int32& val, Direction dir) override;
		/// Binds an Int32.

	void bind(std::size_t pos, const Poco::UInt32& val, Direction dir) override;
		/// Binds an UInt32.

	void bind(std::size_t pos, const Poco::Int64& val, Direction dir) override;
		/// Binds an Int64.

	void bind(std::size_t pos, const Poco::UInt64& val, Direction dir) override;
		/// Binds an UInt64.

#ifndef POCO_INT64_IS_LONG
	virtual void bind(std::size_t pos, const long& val, Direction dir = PD_IN);
		/// Binds a long.

	virtual void bind(std::size_t pos, const unsigned long& val, Direction dir = PD_IN);
		/// Binds an unsigned long.
#endif

	void bind(std::size_t pos, const bool& val, Direction dir) override;
		/// Binds a boolean.

	void bind(std::size_t pos, const float& val, Direction dir) override;
		/// Binds a float.

	void bind(std::size_t pos, const double& val, Direction dir) override;
		/// Binds a double.

	void bind(std::size_t pos, const char& val, Direction dir) override;
		/// Binds a single character.

	void bind(std::size_t pos, const std::string& val, Direction dir) override;
		/// Binds a string.

	void bind(std::size_t pos, const Poco::Data::BLOB& val, Direction dir) override;
		/// Binds a BLOB.

	void bind(std::size_t pos, const Poco::Data::CLOB& val, Direction dir) override;
		/// Binds a CLOB.

	void bind(std::size_t pos, const DateTime& val, Direction dir) override;
		/// Binds a DateTime.

	void bind(std::size_t pos, const Date& val, Direction dir) override;
		/// Binds a Date.

	void bind(std::size_t pos, const Time& val, Direction dir) override;
		/// Binds a Time.

	void bind(std::size_t pos, const UUID& val, Direction dir) override;
		/// Binds a UUID.

	void bind(std::size_t pos, const NullData& val, Direction dir) override;
		/// Binds a null.

	void bind(std::size_t pos, const std::vector<Poco::Int8>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::Int8>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::Int8>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Poco::UInt8>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::UInt8>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::UInt8>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Poco::Int16>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::Int16>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::Int16>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Poco::UInt16>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::UInt16>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::UInt16>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Poco::Int32>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::Int32>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::Int32>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Poco::UInt32>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::UInt32>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::UInt32>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Poco::Int64>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::Int64>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::Int64>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Poco::UInt64>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Poco::UInt64>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Poco::UInt64>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<bool>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<bool>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<bool>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<float>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<float>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<float>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<double>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<double>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<double>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<char>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<char>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<char>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<BLOB>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<BLOB>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<BLOB>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<CLOB>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<CLOB>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<CLOB>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<DateTime>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<DateTime>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<DateTime>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Date>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Date>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Date>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<Time>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<Time>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<Time>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<NullData>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<NullData>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<NullData>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::vector<std::string>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::deque<std::string>& val, Direction dir = PD_IN) override;

	void bind(std::size_t pos, const std::list<std::string>& val, Direction dir = PD_IN) override;

	std::size_t size() const;
		/// Return count of binded parameters

	MYSQL_BIND* getBindArray() const;
		/// Return array

private:
	Binder(const Binder&);
		/// Don't copy the binder

	void bind(std::size_t, const char* const&, Direction) override
		/// Binds a const char ptr.
		/// This is a private no-op in this implementation
		/// due to security risk.
	{
	}

	void realBind(std::size_t pos, enum_field_types type, const void* buffer, int length, bool isUnsigned = false);
		/// Common bind implementation

private:
	std::vector<MYSQL_BIND> _bindArray;
	std::vector<MYSQL_TIME*> _dates;
};


} } } // namespace Poco::Data::MySQL


#endif // Data_MySQL_Binder_INCLUDED
