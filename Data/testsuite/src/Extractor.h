//
// Extractor.h
//
// Definition of the Extractor class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Test_Extractor_INCLUDED
#define Data_Test_Extractor_INCLUDED


#include "Poco/Data/AbstractExtractor.h"


namespace Poco {
namespace Data {
namespace Test {


class Extractor: public Poco::Data::AbstractExtractor
	/// A no-op implementation of AbstractExtractor for testing.
{
public:
	Extractor(Poco::TextEncoding::Ptr pDBEncoding = nullptr);
		/// Creates the Extractor.

	~Extractor();
		/// Destroys the Extractor.

	// Int8
	bool extract(std::size_t pos, Poco::Int8& val) override;
		/// Extracts an Int8.

	bool extract(std::size_t pos, std::vector<Poco::Int8>& val) override;
		/// Extracts a std::vector<Int8>.

	bool extract(std::size_t pos, std::deque<Poco::Int8>& val) override;
		/// Extracts a std::deque<Int8>.

	bool extract(std::size_t pos, std::list<Poco::Int8>& val) override;
		/// Extracts a std::list<Int8>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val) override;
		/// Extracts a Nullable<Int8>.

	// UInt8
	bool extract(std::size_t pos, Poco::UInt8& val) override;
		/// Extracts an UInt8.

	bool extract(std::size_t pos, std::vector<Poco::UInt8>& val) override;
		/// Extracts a std::vector<UInt8>.

	bool extract(std::size_t pos, std::deque<Poco::UInt8>& val) override;
		/// Extracts a std::deque<UInt8>.

	bool extract(std::size_t pos, std::list<Poco::UInt8>& val) override;
		/// Extracts a std::list<UInt8>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val) override;
		/// Extracts a Nullable<UInt8>.

	// Int16
	bool extract(std::size_t pos, Poco::Int16& val) override;
		/// Extracts an Int16.

	bool extract(std::size_t pos, std::vector<Poco::Int16>& val) override;
		/// Extracts a std::vector<Int16>.

	bool extract(std::size_t pos, std::deque<Poco::Int16>& val) override;
		/// Extracts a std::deque<Int16>.

	bool extract(std::size_t pos, std::list<Poco::Int16>& val) override;
		/// Extracts a std::list<Int16>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val) override;
		/// Extracts a Nullable<Int16>.


	// UInt16
	bool extract(std::size_t pos, Poco::UInt16& val) override;
		/// Extracts an UInt16.

	bool extract(std::size_t pos, std::vector<Poco::UInt16>& val) override;
		/// Extracts a std::vector<UInt16>.

	bool extract(std::size_t pos, std::deque<Poco::UInt16>& val) override;
		/// Extracts a std::deque<UInt16>.

	bool extract(std::size_t pos, std::list<Poco::UInt16>& val) override;
		/// Extracts a std::list<UInt16>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val) override;
		/// Extracts a Nullable<UInt16>.

	// Int32
	bool extract(std::size_t pos, Poco::Int32& val) override;
		/// Extracts an Int32.

	bool extract(std::size_t pos, std::vector<Poco::Int32>& val) override;
		/// Extracts a std::vector<Int32>.

	bool extract(std::size_t pos, std::deque<Poco::Int32>& val) override;
		/// Extracts a std::deque<Int32>.

	bool extract(std::size_t pos, std::list<Poco::Int32>& val) override;
		/// Extracts a std::list<Int32>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val) override;
		/// Extracts a Nullable<Int32>.

	// UInt32
	bool extract(std::size_t pos, Poco::UInt32& val) override;
		/// Extracts an UInt32.

	bool extract(std::size_t pos, std::vector<Poco::UInt32>& val) override;
		/// Extracts a std::vector<UInt32>.

	bool extract(std::size_t pos, std::deque<Poco::UInt32>& val) override;
		/// Extracts a std::deque<UInt32>.

	bool extract(std::size_t pos, std::list<Poco::UInt32>& val) override;
		/// Extracts a std::list<UInt32>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val) override;
		/// Extracts a Nullable<UInt32>.

	// Int64
	bool extract(std::size_t pos, Poco::Int64& val) override;
		/// Extracts an Int64.

	bool extract(std::size_t pos, std::vector<Poco::Int64>& val) override;
		/// Extracts a std::vector<Int64>.

	bool extract(std::size_t pos, std::deque<Poco::Int64>& val) override;
		/// Extracts a std::deque<Int64>.

	bool extract(std::size_t pos, std::list<Poco::Int64>& val) override;
		/// Extracts a std::list<Int64>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val) override;
		/// Extracts a Nullable<Int64>.

	// UInt64
	bool extract(std::size_t pos, Poco::UInt64& val) override;
		/// Extracts an UInt64.

	bool extract(std::size_t pos, std::vector<Poco::UInt64>& val) override;
		/// Extracts a std::vector<UInt64>.

	bool extract(std::size_t pos, std::deque<Poco::UInt64>& val) override;
		/// Extracts a std::deque<UInt64>.

	bool extract(std::size_t pos, std::list<Poco::UInt64>& val) override;
		/// Extracts a std::list<UInt64>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val) override;
		/// Extracts a Nullable<UInt64>.

	// Any
	bool extract(std::size_t pos, Poco::Any& val) override;
		/// Extracts an Any.

	bool extract(std::size_t pos, std::vector<Poco::Any>& val) override;
		/// Extracts a std::vector<Poco::Any>.

	bool extract(std::size_t pos, std::deque<Poco::Any>& val) override;
		/// Extracts a std::deque<Poco::Any>.

	bool extract(std::size_t pos, std::list<Poco::Any>& val) override;
		/// Extracts a std::list<Poco::Any>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Any>& val) override;
		/// Extracts a Nullable<Poco::Any>.

	// Var
	bool extract(std::size_t pos, Poco::Dynamic::Var& val) override;
		/// Extracts a Var.

	bool extract(std::size_t pos, std::vector<Poco::Dynamic::Var>& val) override;
		/// Extracts a std::vector<Poco::Dynamic::Var>.

	bool extract(std::size_t pos, std::deque<Poco::Dynamic::Var>& val) override;
		/// Extracts a std::deque<Poco::Dynamic::Var>.

	bool extract(std::size_t pos, std::list<Poco::Dynamic::Var>& val) override;
		/// Extracts a std::list<Poco::Dynamic::Var>.

	bool extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val) override;
		/// Extracts a Nullable<Poco::Dynamic::Var>.

#ifndef POCO_INT64_IS_LONG
	// long
	bool extract(std::size_t pos, long& val) override;
		/// Extracts a long.

	bool extract(std::size_t pos, std::vector<long>& val) override;
		/// Extracts a std::vector<long>.

	bool extract(std::size_t pos, std::deque<long>& val) override;
		/// Extracts a std::deque<long>.

	bool extract(std::size_t pos, std::list<long>& val) override;
		/// Extracts a std::list<long>.

	bool extract(std::size_t pos, Poco::Nullable<long>& val) override;
		/// Extracts a Nullable<long>.

	// unsigned long
	bool extract(std::size_t pos, unsigned long& val) override;
		/// Extracts an unsigned unsigned long.

	bool extract(std::size_t pos, std::vector<unsigned long>& val) override;
		/// Extracts a std::vector<unsigned long>.

	bool extract(std::size_t pos, std::deque<unsigned long>& val) override;
		/// Extracts a std::deque<unsigned long>.

	bool extract(std::size_t pos, std::list<unsigned long>& val) override;
		/// Extracts a std::list<unsigned long>.

	bool extract(std::size_t pos, Poco::Nullable<unsigned long>& val) override;
		/// Extracts a Nullable<unsigned long>.
#endif

	// bool
	bool extract(std::size_t pos, bool& val) override;
		/// Extracts a boolean.

	bool extract(std::size_t pos, std::vector<bool>& val) override;
		/// Extracts a std::vector<bool>.

	bool extract(std::size_t pos, std::deque<bool>& val) override;
		/// Extracts a std::deque<bool>.

	bool extract(std::size_t pos, std::list<bool>& val) override;
		/// Extracts a std::list<bool>.

	bool extract(std::size_t pos, Poco::Nullable<bool>& val) override;
		/// Extracts a Nullable<bool>.

	// float
	bool extract(std::size_t pos, float& val) override;
		/// Extracts a float.

	bool extract(std::size_t pos, std::vector<float>& val) override;
		/// Extracts a std::vector<float>.

	bool extract(std::size_t pos, std::deque<float>& val) override;
		/// Extracts a std::deque<float>.

	bool extract(std::size_t pos, std::list<float>& val) override;
		/// Extracts a std::list<float>.

	bool extract(std::size_t pos, Poco::Nullable<float>& val) override;
		/// Extracts a Nullable<float>.

	// double
	bool extract(std::size_t pos, double& val) override;
		/// Extracts a double.

	bool extract(std::size_t pos, std::vector<double>& val) override;
		/// Extracts a std::vector<double>.

	bool extract(std::size_t pos, std::deque<double>& val) override;
		/// Extracts a std::deque<double>.

	bool extract(std::size_t pos, std::list<double>& val) override;
		/// Extracts a std::list<double>.

	bool extract(std::size_t pos, Poco::Nullable<double>& val) override;
		/// Extracts a Nullable<double>.

	// char
	bool extract(std::size_t pos, char& val) override;
		/// Extracts a single character.

	bool extract(std::size_t pos, std::vector<char>& val) override;
		/// Extracts a std::vector<char>.

	bool extract(std::size_t pos, std::deque<char>& val) override;
		/// Extracts a std::deque<char>.

	bool extract(std::size_t pos, std::list<char>& val) override;
		/// Extracts a std::list<char>.

	bool extract(std::size_t pos, Poco::Nullable<char>& val) override;
		/// Extracts a Nullable<char>.

	// string
	bool extract(std::size_t pos, std::string& val) override;
		/// Extracts a string.

	bool extract(std::size_t pos, std::vector<std::string>& val) override;
		/// Extracts a std::vector<std::string>.

	bool extract(std::size_t pos, std::deque<std::string>& val) override;
		/// Extracts a std::deque<std::string>.

	bool extract(std::size_t pos, std::list<std::string>& val) override;
		/// Extracts a std::list<std::string>.

	bool extract(std::size_t pos, Poco::Nullable<std::string>& val) override;
		/// Extracts a Nullable<std::string>.

	// UTF16String
	bool extract(std::size_t pos, Poco::UTF16String& val) override;
		/// Extracts a UTF16String.

	bool extract(std::size_t pos, std::vector<UTF16String>& val) override;
		/// Extracts a std::vector<UTF16String>.

	bool extract(std::size_t pos, std::deque<UTF16String>& val) override;
		/// Extracts a std::deque<UTF16String>.

	bool extract(std::size_t pos, std::list<UTF16String>& val) override;
		/// Extracts a std::list<UTF16String>.

	bool extract(std::size_t pos, Poco::Nullable<UTF16String>& val) override;
		/// Extracts a Nullable<UTF16String>.

	// BLOB
	bool extract(std::size_t pos, Poco::Data::BLOB& val) override;
		/// Extracts a BLOB.

	bool extract(std::size_t pos, std::vector<BLOB>& val) override;
		/// Extracts a std::vector<BLOB>.

	bool extract(std::size_t pos, std::deque<BLOB>& val) override;
		/// Extracts a std::deque<BLOB>.

	bool extract(std::size_t pos, std::list<BLOB>& val) override;
		/// Extracts a std::list<BLOB>.

	bool extract(std::size_t pos, Poco::Nullable<BLOB>& val) override;
		/// Extracts a Nullable<BLOB>.

	// CLOB
	bool extract(std::size_t pos, Poco::Data::CLOB& val) override;
		/// Extracts a CLOB.

	bool extract(std::size_t pos, std::vector<CLOB>& val) override;
		/// Extracts a std::vector<CLOB>.

	bool extract(std::size_t pos, std::deque<CLOB>& val) override;
		/// Extracts a std::deque<CLOB>.

	bool extract(std::size_t pos, std::list<CLOB>& val) override;
		/// Extracts a std::list<CLOB>.

	bool extract(std::size_t pos, Poco::Nullable<CLOB>& val) override;
		/// Extracts a Nullable<CLOB>.

	// Date
	bool extract(std::size_t pos, Date& val) override;
		/// Extracts a Date.

	bool extract(std::size_t pos, std::vector<Date>& val) override;
		/// Extracts a std::vector<Date>.

	bool extract(std::size_t pos, std::deque<Date>& val) override;
		/// Extracts a std::deque<Date>.

	bool extract(std::size_t pos, std::list<Date>& val) override;
		/// Extracts a std::list<Date>.

	bool extract(std::size_t pos, Poco::Nullable<Date>& val) override;
		/// Extracts a Nullable<Date>.

	// Time
	bool extract(std::size_t pos, Time& val) override;
		/// Extracts a Time.

	bool extract(std::size_t pos, std::vector<Time>& val) override;
		/// Extracts a std::vector<Time>.

	bool extract(std::size_t pos, std::deque<Time>& val) override;
		/// Extracts a std::deque<Time>.

	bool extract(std::size_t pos, std::list<Time>& val) override;
		/// Extracts a std::list<Time>.

	bool extract(std::size_t pos, Poco::Nullable<Time>& val) override;
		/// Extracts a Nullable<Time>.

	// DateTime
	bool extract(std::size_t pos, DateTime& val) override;
		/// Extracts a DateTime.

	bool extract(std::size_t pos, std::vector<DateTime>& val) override;
		/// Extracts a std::vector<DateTime>.

	bool extract(std::size_t pos, std::deque<DateTime>& val) override;
		/// Extracts a std::deque<DateTime>.

	bool extract(std::size_t pos, std::list<DateTime>& val) override;
		/// Extracts a std::list<DateTime>.

	bool extract(std::size_t pos, Poco::Nullable<DateTime>& val) override;
		/// Extracts a Nullable<DateTime>.

	// UUID
	bool extract(std::size_t pos, UUID& val) override;
		/// Extracts a UUID.

	bool extract(std::size_t pos, std::vector<UUID>& val) override;
		/// Extracts a std::vector<UUID>.

	bool extract(std::size_t pos, std::deque<UUID>& val) override;
		/// Extracts a std::deque<UUID>.

	bool extract(std::size_t pos, std::list<UUID>& val) override;
		/// Extracts a std::list<UUID>.

	bool extract(std::size_t pos, Poco::Nullable<UUID>& val) override;
		/// Extracts a Nullable<UUID>.

	bool isNull(std::size_t col, std::size_t row = -1) override;
		/// Returns true if the current row value at pos column is null.

	void reset() override;

	void setString(const std::string& str)
	{
		_stringValue = str;
	}

private:
	std::string _stringValue;
};


inline void Extractor::reset()
{
}


inline bool Extractor::isNull(std::size_t col, std::size_t row)
{
	return false;
}


} } } // namespace Poco::Data::Test


#endif // Data_Test_Extractor_INCLUDED
