//
// AbstractExtractor.h
//
// Library: Data
// Package: DataCore
// Module:  AbstractExtractor
//
// Definition of the AbstractExtractor class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_AbstractExtractor_INCLUDED
#define Data_AbstractExtractor_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/Data/Constants.h"
#include "Poco/Data/LOB.h"
#include "Poco/Nullable.h"
#include "Poco/UUID.h"
#include "Poco/UTFString.h"
#include "Poco/TextEncoding.h"
#include <memory>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <cstddef>


namespace Poco {

class DateTime;
class Any;

namespace Dynamic {
class Var;
}

namespace Data {


class Date;
class Time;
class Transcoder;


class Data_API AbstractExtractor
	/// Interface used to extract data from a single result row.
	/// If an extractor receives null it is not allowed to change val!
{
public:
	using Ptr = SharedPtr<AbstractExtractor>;

	AbstractExtractor(Poco::TextEncoding::Ptr pDBEncoding = nullptr,
		Poco::TextEncoding::Ptr pToEncoding = nullptr);
		/// Creates the AbstractExtractor.

	virtual ~AbstractExtractor();
		/// Destroys the AbstractExtractor.

	virtual bool extract(std::size_t pos, Poco::Int8& val) = 0;
		/// Extracts an Int8. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::Int8>& val) = 0;
		/// Extracts an Int8 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::Int8>& val) = 0;
		/// Extracts an Int8 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::Int8>& val) = 0;
		/// Extracts an Int8 list.

	virtual bool extract(std::size_t pos, Poco::UInt8& val) = 0;
		/// Extracts an UInt8. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::UInt8>& val) = 0;
		/// Extracts an UInt8 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::UInt8>& val) = 0;
		/// Extracts an UInt8 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::UInt8>& val) = 0;
		/// Extracts an UInt8 list.

	virtual bool extract(std::size_t pos, Poco::Int16& val) = 0;
		/// Extracts an Int16. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::Int16>& val) = 0;
		/// Extracts an Int16 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::Int16>& val) = 0;
		/// Extracts an Int16 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::Int16>& val) = 0;
		/// Extracts an Int16 list.

	virtual bool extract(std::size_t pos, Poco::UInt16& val) = 0;
		/// Extracts an UInt16. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::UInt16>& val) = 0;
		/// Extracts an UInt16 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::UInt16>& val) = 0;
		/// Extracts an UInt16 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::UInt16>& val) = 0;
		/// Extracts an UInt16 list.

	virtual bool extract(std::size_t pos, Poco::Int32& val) = 0;
		/// Extracts an Int32. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::Int32>& val) = 0;
		/// Extracts an Int32 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::Int32>& val) = 0;
		/// Extracts an Int32 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::Int32>& val) = 0;
		/// Extracts an Int32 list.

	virtual bool extract(std::size_t pos, Poco::UInt32& val) = 0;
		/// Extracts an UInt32. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::UInt32>& val) = 0;
		/// Extracts an UInt32 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::UInt32>& val) = 0;
		/// Extracts an UInt32 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::UInt32>& val) = 0;
		/// Extracts an UInt32 list.

	virtual bool extract(std::size_t pos, Poco::Int64& val) = 0;
		/// Extracts an Int64. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::Int64>& val) = 0;
		/// Extracts an Int64 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::Int64>& val) = 0;
		/// Extracts an Int64 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::Int64>& val) = 0;
		/// Extracts an Int64 list.

	virtual bool extract(std::size_t pos, Poco::UInt64& val) = 0;
		/// Extracts an UInt64. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::UInt64>& val) = 0;
		/// Extracts an UInt64 vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::UInt64>& val) = 0;
		/// Extracts an UInt64 deque.

	virtual bool extract(std::size_t pos, std::list<Poco::UInt64>& val) = 0;
		/// Extracts an UInt64 list.

#ifndef POCO_INT64_IS_LONG
	virtual bool extract(std::size_t pos, long& val) = 0;
		/// Extracts a long. Returns false if null was received.

	virtual bool extract(std::size_t pos, unsigned long& val) = 0;
		/// Extracts an unsigned long. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<long>& val) = 0;
		/// Extracts a long vector.

	virtual bool extract(std::size_t pos, std::deque<long>& val) = 0;
		/// Extracts a long deque.

	virtual bool extract(std::size_t pos, std::list<long>& val) = 0;
		/// Extracts a long list.

	virtual bool extract(std::size_t pos, std::vector<unsigned long>& val) = 0;
		/// Extracts an unsigned long vector.

	virtual bool extract(std::size_t pos, std::deque<unsigned long>& val) = 0;
		/// Extracts an unsigned long deque.

	virtual bool extract(std::size_t pos, std::list<unsigned long>& val) = 0;
		/// Extracts an unsigned long list.
#endif

	virtual bool extract(std::size_t pos, bool& val) = 0;
		/// Extracts a boolean. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<bool>& val) = 0;
		/// Extracts a boolean vector.

	virtual bool extract(std::size_t pos, std::deque<bool>& val) = 0;
		/// Extracts a boolean deque.

	virtual bool extract(std::size_t pos, std::list<bool>& val) = 0;
		/// Extracts a boolean list.

	virtual bool extract(std::size_t pos, float& val) = 0;
		/// Extracts a float. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<float>& val) = 0;
		/// Extracts a float vector.

	virtual bool extract(std::size_t pos, std::deque<float>& val) = 0;
		/// Extracts a float deque.

	virtual bool extract(std::size_t pos, std::list<float>& val) = 0;
		/// Extracts a float list.

	virtual bool extract(std::size_t pos, double& val) = 0;
		/// Extracts a double. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<double>& val) = 0;
		/// Extracts a double vector.

	virtual bool extract(std::size_t pos, std::deque<double>& val) = 0;
		/// Extracts a double deque.

	virtual bool extract(std::size_t pos, std::list<double>& val) = 0;
		/// Extracts a double list.

	virtual bool extract(std::size_t pos, char& val) = 0;
		/// Extracts a single character. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<char>& val) = 0;
		/// Extracts a character vector.

	virtual bool extract(std::size_t pos, std::deque<char>& val) = 0;
		/// Extracts a character deque.

	virtual bool extract(std::size_t pos, std::list<char>& val) = 0;
		/// Extracts a character list.

	virtual bool extract(std::size_t pos, std::string& val) = 0;
		/// Extracts a string. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<std::string>& val) = 0;
		/// Extracts a string vector.

	virtual bool extract(std::size_t pos, std::deque<std::string>& val) = 0;
		/// Extracts a string deque.

	virtual bool extract(std::size_t pos, std::list<std::string>& val) = 0;
		/// Extracts a string list.

	virtual bool extract(std::size_t pos, UTF16String& val);
		/// Extracts a UTF16String. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<UTF16String>& val);
		/// Extracts a UTF16String vector.

	virtual bool extract(std::size_t pos, std::deque<UTF16String>& val);
		/// Extracts a UTF16String deque.

	virtual bool extract(std::size_t pos, std::list<UTF16String>& val);
		/// Extracts a UTF16String list.

	virtual bool extract(std::size_t pos, BLOB& val) = 0;
		/// Extracts a BLOB. Returns false if null was received.

	virtual bool extract(std::size_t pos, CLOB& val) = 0;
		/// Extracts a CLOB. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<BLOB>& val) = 0;
		/// Extracts a BLOB vector.

	virtual bool extract(std::size_t pos, std::deque<BLOB>& val) = 0;
		/// Extracts a BLOB deque.

	virtual bool extract(std::size_t pos, std::list<BLOB>& val) = 0;
		/// Extracts a BLOB list.

	virtual bool extract(std::size_t pos, std::vector<CLOB>& val) = 0;
		/// Extracts a CLOB vector.

	virtual bool extract(std::size_t pos, std::deque<CLOB>& val) = 0;
		/// Extracts a CLOB deque.

	virtual bool extract(std::size_t pos, std::list<CLOB>& val) = 0;
		/// Extracts a CLOB list.

	virtual bool extract(std::size_t pos, DateTime& val) = 0;
		/// Extracts a DateTime. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<DateTime>& val) = 0;
		/// Extracts a DateTime vector.

	virtual bool extract(std::size_t pos, std::deque<DateTime>& val) = 0;
		/// Extracts a DateTime deque.

	virtual bool extract(std::size_t pos, std::list<DateTime>& val) = 0;
		/// Extracts a DateTime list.

	virtual bool extract(std::size_t pos, Date& val) = 0;
		/// Extracts a Date. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Date>& val) = 0;
		/// Extracts a Date vector.

	virtual bool extract(std::size_t pos, std::deque<Date>& val) = 0;
		/// Extracts a Date deque.

	virtual bool extract(std::size_t pos, std::list<Date>& val) = 0;
		/// Extracts a Date list.

	virtual bool extract(std::size_t pos, Time& val) = 0;
		/// Extracts a Time. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Time>& val) = 0;
		/// Extracts a Time vector.

	virtual bool extract(std::size_t pos, std::deque<Time>& val) = 0;
		/// Extracts a Time deque.

	virtual bool extract(std::size_t pos, std::list<Time>& val) = 0;
		/// Extracts a Time list.

	virtual bool extract(std::size_t pos, UUID& val) = 0;
		/// Extracts a UUID. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<UUID>& val);
		/// Extracts a UUID vector.

	virtual bool extract(std::size_t pos, std::deque<UUID>& val);
		/// Extracts a UUID deque.

	virtual bool extract(std::size_t pos, std::list<UUID>& val);
		/// Extracts a UUID list.

	virtual bool extract(std::size_t pos, Any& val) = 0;
		/// Extracts an Any. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Any>& val) = 0;
		/// Extracts an Any vector.

	virtual bool extract(std::size_t pos, std::deque<Any>& val) = 0;
		/// Extracts an Any deque.

	virtual bool extract(std::size_t pos, std::list<Any>& val) = 0;
		/// Extracts an Any list.

	virtual bool extract(std::size_t pos, Poco::Dynamic::Var& val) = 0;
		/// Extracts a Var. Returns false if null was received.

	virtual bool extract(std::size_t pos, std::vector<Poco::Dynamic::Var>& val) = 0;
		/// Extracts a Var vector.

	virtual bool extract(std::size_t pos, std::deque<Poco::Dynamic::Var>& val) = 0;
		/// Extracts a Var deque.

	virtual bool extract(std::size_t pos, std::list<Poco::Dynamic::Var>& val) = 0;
		/// Extracts a Var list.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::Int8>& val);
		/// Extracts a nullable nullable Int8.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::UInt8>& val);
		/// Extracts a nullable nullable UInt8.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::Int16>& val);
		/// Extracts a nullable nullable Int16.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::UInt16>& val);
		/// Extracts a nullable nullable UInt16.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::Int32>& val);
		/// Extracts a nullable nullable Int32.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::UInt32>& val);
		/// Extracts a nullable nullable UInt32.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::Int64>& val);
		/// Extracts a nullable nullable Int64.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::UInt64>& val);
		/// Extracts a nullable nullable UInt64.

#ifndef POCO_INT64_IS_LONG
	virtual bool extract(std::size_t pos, Poco::Nullable<long>& val);
		/// Extracts a nullable long.

	virtual bool extract(std::size_t pos, Poco::Nullable<unsigned long>& val);
		/// Extracts a nullable nullable unsigned long.
#endif

	virtual bool extract(std::size_t pos, Poco::Nullable<bool>& val);
		/// Extracts a nullable boolean.

	virtual bool extract(std::size_t pos, Poco::Nullable<float>& val);
		/// Extracts a nullable float.

	virtual bool extract(std::size_t pos, Poco::Nullable<double>& val);
		/// Extracts a nullable double.

	virtual bool extract(std::size_t pos, Poco::Nullable<char>& val);
		/// Extracts a nullable single character.

	virtual bool extract(std::size_t pos, Poco::Nullable<std::string>& val);
		/// Extracts a nullable string.

	virtual bool extract(std::size_t pos, Poco::Nullable<UTF16String>& val);
		/// Extracts a nullable UTF16String.

	virtual bool extract(std::size_t pos, Poco::Nullable<BLOB>& val);
		/// Extracts a nullable BLOB.

	virtual bool extract(std::size_t pos, Poco::Nullable<CLOB>& val);
		/// Extracts a nullable CLOB.

	virtual bool extract(std::size_t pos, Poco::Nullable<DateTime>& val);
		/// Extracts a nullable DateTime.

	virtual bool extract(std::size_t pos, Poco::Nullable<Date>& val);
		/// Extracts a nullable Date.

	virtual bool extract(std::size_t pos, Poco::Nullable<Time>& val);
		/// Extracts a nullable Time.

	virtual bool extract(std::size_t pos, Poco::Nullable<UUID>& val);
		/// Extracts a nullable UUID.

	virtual bool extract(std::size_t pos, Poco::Nullable<Any>& val);
		/// Extracts a nullable nullable Any.

	virtual bool extract(std::size_t pos, Poco::Nullable<Poco::Dynamic::Var>& val);
		/// Extracts a nullable Var.

	virtual bool isNull(std::size_t col, std::size_t row = POCO_DATA_INVALID_ROW) = 0;
		/// Returns true if the value at [col,row] position is null.

	virtual void reset();
		/// Resets any information internally cached by the extractor.

protected:
	bool transcodeRequired() const;
	void transcode(const std::string& from, std::string& to);
	void reverseTranscode(const std::string& from, std::string& to);

private:
	std::unique_ptr<Transcoder> _pTranscoder;
};


///
/// inlines
///

inline void AbstractExtractor::reset()
{
	//default no-op
}


inline bool AbstractExtractor::transcodeRequired() const
{
	return _pTranscoder.operator bool();
}


} } // namespace Poco::Data


#endif // Data_AbstractExtractor_INCLUDED
