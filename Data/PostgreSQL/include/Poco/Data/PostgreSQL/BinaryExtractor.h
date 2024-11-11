//
// BinaryExtractor.h
//
// Library: Data/PostgreSQL
// Package: PostgreSQL
// Module:  Extractor
//
// Definition of the BinaryExtractor class.
//
// Copyright (c) 2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_PostgreSQL_BinaryExtractor_INCLUDED
#define SQL_PostgreSQL_BinaryExtractor_INCLUDED


#include "Poco/Data/PostgreSQL/PostgreSQL.h"
#include "Poco/Data/PostgreSQL/PostgreSQLTypes.h"
#include "Poco/Data/PostgreSQL/StatementExecutor.h"
#include "Poco/Data/AbstractExtractor.h"
#include "Poco/Data/LOB.h"
#include "Poco/Types.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Dynamic/Var.h"


namespace Poco {
namespace Data {
namespace PostgreSQL {


class PostgreSQL_API BinaryExtractor: public Poco::Data::AbstractExtractor
	/// Extracts and converts data values from the result row returned by PostgreSQL.
	/// If NULL is received, the incoming val value is not changed and false is returned
{
public:
	using Ptr = SharedPtr<BinaryExtractor>;

	BinaryExtractor(StatementExecutor& st);
		/// Creates the Extractor.

	virtual ~BinaryExtractor();
		/// Destroys the Extractor.

	bool extract(std::size_t pos, Poco::Int8& val) override;
		/// Extracts an Int8.

	bool extract(std::size_t pos, Poco::UInt8& val) override;
		/// Extracts an UInt8.

	bool extract(std::size_t pos, Poco::Int16& val) override;
		/// Extracts an Int16.

	bool extract(std::size_t pos, Poco::UInt16& val) override;
		/// Extracts an UInt16.

	bool extract(std::size_t pos, Poco::Int32& val) override;
		/// Extracts an Int32.

	bool extract(std::size_t pos, Poco::UInt32& val) override;
		/// Extracts an UInt32.

	bool extract(std::size_t pos, Poco::Int64& val) override;
		/// Extracts an Int64.

	bool extract(std::size_t pos, Poco::UInt64& val) override;
		/// Extracts an UInt64.

#ifndef POCO_INT64_IS_LONG
	bool extract(std::size_t pos, long& val) override;
		/// Extracts a long. Returns false if null was received.

	bool extract(std::size_t pos, unsigned long& val) override;
		/// Extracts an unsigned long. Returns false if null was received.
#endif

	bool extract(std::size_t pos, bool& val) override;
		/// Extracts a boolean.

	bool extract(std::size_t pos, float& val) override;
		/// Extracts a float.

	bool extract(std::size_t pos, double& val) override;
		/// Extracts a double.

	bool extract(std::size_t pos, char& val) override;
		/// Extracts a single character.

	bool extract(std::size_t pos, std::string& val) override;
		/// Extracts a string.

	bool extract(std::size_t pos, Poco::Data::BLOB& val) override;
		/// Extracts a BLOB.

	bool extract(std::size_t pos, Poco::Data::CLOB& val) override;
		/// Extracts a CLOB.

	bool extract(std::size_t pos, DateTime& val) override;
		/// Extracts a DateTime. Returns false if null was received.

	bool extract(std::size_t pos, Date& val) override;
		/// Extracts a Date. Returns false if null was received.

	bool extract(std::size_t pos, Time& val) override;
		/// Extracts a Time. Returns false if null was received.

	bool extract(std::size_t pos, UUID& val) override;
		/// Extracts a UUID. Returns false if null was received.

	bool extract(std::size_t pos, Any& val) override;
		/// Extracts an Any. Returns false if null was received.

	bool extract(std::size_t pos, Dynamic::Var& val) override;
		/// Extracts a Dynamic::Var. Returns false if null was received.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val) override;
		/// Extracts a nullable nullable Int8.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val) override;
		/// Extracts a nullable nullable UInt8.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val) override;
		/// Extracts a nullable nullable Int16.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val) override;
		/// Extracts a nullable nullable UInt16.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val) override;
		/// Extracts a nullable nullable Int32.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val) override;
		/// Extracts a nullable nullable UInt32.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val) override;
		/// Extracts a nullable nullable Int64.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val) override;
		/// Extracts a nullable nullable UInt64.

#ifndef POCO_INT64_IS_LONG
	bool extract(std::size_t pos, Poco::Nullable<long>& val) override;
		/// Extracts a nullable long.

	bool extract(std::size_t pos, Poco::Nullable<unsigned long>& val) override;
		/// Extracts a nullable nullable unsigned long.
#endif

	bool extract(std::size_t pos, Poco::Nullable<bool>& val) override;
		/// Extracts a nullable boolean.

	bool extract(std::size_t pos, Poco::Nullable<float>& val) override;
		/// Extracts a nullable float.

	bool extract(std::size_t pos, Poco::Nullable<double>& val) override;
		/// Extracts a nullable double.

	bool extract(std::size_t pos, Poco::Nullable<char>& val) override;
		/// Extracts a nullable single character.

	bool extract(std::size_t pos, Poco::Nullable<std::string>& val) override;
		/// Extracts a nullable string.

	bool extract(std::size_t pos, Poco::Nullable<UTF16String>& val) override;
		/// Extracts a nullable UTF16String.

	bool extract(std::size_t pos, Poco::Nullable<BLOB>& val) override;
		/// Extracts a nullable BLOB.

	bool extract(std::size_t pos, Poco::Nullable<CLOB>& val) override;
		/// Extracts a nullable CLOB.

	bool extract(std::size_t pos, Poco::Nullable<DateTime>& val) override;
		/// Extracts a nullable DateTime.

	bool extract(std::size_t pos, Poco::Nullable<Date>& val) override;
		/// Extracts a nullable Date.

	bool extract(std::size_t pos, Poco::Nullable<Time>& val) override;
		/// Extracts a nullable Time.

	bool extract(std::size_t pos, Poco::Nullable<UUID>& val) override;
		/// Extracts a nullable UUID.

	bool extract(std::size_t pos, Poco::Nullable<Any>& val) override;
		/// Extracts a nullable nullable Any.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val) override;
		/// Extracts a nullable Var.

	bool isNull(std::size_t col, std::size_t row) override;
		/// Returns true if the value at [col,row] position is null.

	void reset() override;
		/// Resets any information internally cached by the extractor.

	////////////
	// Not implemented extract functions
	////////////

	bool extract(std::size_t pos, std::vector<Poco::Int8>& val) override;
		/// Extracts an Int8 vector.

	bool extract(std::size_t pos, std::deque<Poco::Int8>& val) override;
		/// Extracts an Int8 deque.

	bool extract(std::size_t pos, std::list<Poco::Int8>& val) override;
		/// Extracts an Int8 list.

	bool extract(std::size_t pos, std::vector<Poco::UInt8>& val) override;
		/// Extracts an UInt8 vector.

	bool extract(std::size_t pos, std::deque<Poco::UInt8>& val) override;
		/// Extracts an UInt8 deque.

	bool extract(std::size_t pos, std::list<Poco::UInt8>& val) override;
		/// Extracts an UInt8 list.

	bool extract(std::size_t pos, std::vector<Poco::Int16>& val) override;
		/// Extracts an Int16 vector.

	bool extract(std::size_t pos, std::deque<Poco::Int16>& val) override;
		/// Extracts an Int16 deque.

	bool extract(std::size_t pos, std::list<Poco::Int16>& val) override;
		/// Extracts an Int16 list.

	bool extract(std::size_t pos, std::vector<Poco::UInt16>& val) override;
		/// Extracts an UInt16 vector.

	bool extract(std::size_t pos, std::deque<Poco::UInt16>& val) override;
		/// Extracts an UInt16 deque.

	bool extract(std::size_t pos, std::list<Poco::UInt16>& val) override;
		/// Extracts an UInt16 list.

	bool extract(std::size_t pos, std::vector<Poco::Int32>& val) override;
		/// Extracts an Int32 vector.

	bool extract(std::size_t pos, std::deque<Poco::Int32>& val) override;
		/// Extracts an Int32 deque.

	bool extract(std::size_t pos, std::list<Poco::Int32>& val) override;
		/// Extracts an Int32 list.

	bool extract(std::size_t pos, std::vector<Poco::UInt32>& val) override;
		/// Extracts an UInt32 vector.

	bool extract(std::size_t pos, std::deque<Poco::UInt32>& val) override;
		/// Extracts an UInt32 deque.

	bool extract(std::size_t pos, std::list<Poco::UInt32>& val) override;
		/// Extracts an UInt32 list.

	bool extract(std::size_t pos, std::vector<Poco::Int64>& val) override;
		/// Extracts an Int64 vector.

	bool extract(std::size_t pos, std::deque<Poco::Int64>& val) override;
		/// Extracts an Int64 deque.

	bool extract(std::size_t pos, std::list<Poco::Int64>& val) override;
		/// Extracts an Int64 list.

	bool extract(std::size_t pos, std::vector<Poco::UInt64>& val) override;
		/// Extracts an UInt64 vector.

	bool extract(std::size_t pos, std::deque<Poco::UInt64>& val) override;
		/// Extracts an UInt64 deque.

	bool extract(std::size_t pos, std::list<Poco::UInt64>& val) override;
		/// Extracts an UInt64 list.

#ifndef POCO_INT64_IS_LONG
	bool extract(std::size_t pos, std::vector<long>& val) override;
		/// Extracts a long vector.

	bool extract(std::size_t pos, std::deque<long>& val) override;
		/// Extracts a long deque.

	bool extract(std::size_t pos, std::list<long>& val) override;
		/// Extracts a long list.

	bool extract(std::size_t pos, std::vector<unsigned long>& val) override;
		/// Extracts a long vector.

	bool extract(std::size_t pos, std::deque<unsigned long>& val) override;
		/// Extracts a long deque.

	bool extract(std::size_t pos, std::list<unsigned long>& val) override;
		/// Extracts a long list.
#endif

	bool extract(std::size_t pos, std::vector<bool>& val) override;
		/// Extracts a boolean vector.

	bool extract(std::size_t pos, std::deque<bool>& val) override;
		/// Extracts a boolean deque.

	bool extract(std::size_t pos, std::list<bool>& val) override;
		/// Extracts a boolean list.

	bool extract(std::size_t pos, std::vector<float>& val) override;
		/// Extracts a float vector.

	bool extract(std::size_t pos, std::deque<float>& val) override;
		/// Extracts a float deque.

	bool extract(std::size_t pos, std::list<float>& val) override;
		/// Extracts a float list.

	bool extract(std::size_t pos, std::vector<double>& val) override;
		/// Extracts a double vector.

	bool extract(std::size_t pos, std::deque<double>& val) override;
		/// Extracts a double deque.

	bool extract(std::size_t pos, std::list<double>& val) override;
		/// Extracts a double list.

	bool extract(std::size_t pos, std::vector<char>& val) override;
		/// Extracts a character vector.

	bool extract(std::size_t pos, std::deque<char>& val) override;
		/// Extracts a character deque.

	bool extract(std::size_t pos, std::list<char>& val) override;
		/// Extracts a character list.

	bool extract(std::size_t pos, std::vector<std::string>& val) override;
		/// Extracts a string vector.

	bool extract(std::size_t pos, std::deque<std::string>& val) override;
		/// Extracts a string deque.

	bool extract(std::size_t pos, std::list<std::string>& val) override;
		/// Extracts a string list.

	bool extract(std::size_t pos, std::vector<BLOB>& val) override;
		/// Extracts a BLOB vector.

	bool extract(std::size_t pos, std::deque<BLOB>& val) override;
		/// Extracts a BLOB deque.

	bool extract(std::size_t pos, std::list<BLOB>& val) override;
		/// Extracts a BLOB list.

	bool extract(std::size_t pos, std::vector<CLOB>& val) override;
		/// Extracts a CLOB vector.

	bool extract(std::size_t pos, std::deque<CLOB>& val) override;
		/// Extracts a CLOB deque.

	bool extract(std::size_t pos, std::list<CLOB>& val) override;
		/// Extracts a CLOB list.

	bool extract(std::size_t pos, std::vector<DateTime>& val) override;
		/// Extracts a DateTime vector.

	bool extract(std::size_t pos, std::deque<DateTime>& val) override;
		/// Extracts a DateTime deque.

	bool extract(std::size_t pos, std::list<DateTime>& val) override;
		/// Extracts a DateTime list.

	bool extract(std::size_t pos, std::vector<Date>& val) override;
		/// Extracts a Date vector.

	bool extract(std::size_t pos, std::deque<Date>& val) override;
		/// Extracts a Date deque.

	bool extract(std::size_t pos, std::list<Date>& val) override;
		/// Extracts a Date list.

	bool extract(std::size_t pos, std::vector<Time>& val) override;
		/// Extracts a Time vector.

	bool extract(std::size_t pos, std::deque<Time>& val) override;
		/// Extracts a Time deque.

	bool extract(std::size_t pos, std::list<Time>& val) override;
		/// Extracts a Time list.

	bool extract(std::size_t pos, std::vector<Any>& val) override;
		/// Extracts an Any vector.

	bool extract(std::size_t pos, std::deque<Any>& val) override;
		/// Extracts an Any deque.

	bool extract(std::size_t pos, std::list<Any>& val) override;
		/// Extracts an Any list.

	bool extract(std::size_t pos, std::vector<Dynamic::Var>& val) override;
		/// Extracts a Dynamic::Var vector.

	bool extract(std::size_t pos, std::deque<Dynamic::Var>& val) override;
		/// Extracts a Dynamic::Var deque.

	bool extract(std::size_t pos, std::list<Dynamic::Var>& val) override;
		/// Extracts a Dynamic::Var list.

private:
	const OutputParameter& extractPreamble(std::size_t aPosition) const;
	bool isColumnNull(const OutputParameter& anOutputParameter) const;

	// Prevent VC8 warning "operator= could not be generated"
	BinaryExtractor& operator = (const BinaryExtractor&);

private:
	template <typename T>
	bool extractNullable(std::size_t pos, Poco::Nullable<T>& val)
	{
		typename Poco::Nullable<T>::Type v;
		if (!extract(pos, v)) val.clear();
		else val = std::move(v);
		return true;
	}

	StatementExecutor& _statementExecutor;
};


//
// inlines
//
inline bool BinaryExtractor::isColumnNull(const OutputParameter& anOutputParameter) const
{
	return anOutputParameter.isNull() || 0 == anOutputParameter.pData();
}


} } } // namespace Poco::Data::PostgreSQL


#endif // SQL_PostgreSQL_BinaryExtractor_INCLUDED
