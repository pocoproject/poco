//
// AbstractBinder.h
//
// Library: Data
// Package: DataCore
// Module:  AbstractBinder
//
// Definition of the AbstractBinder class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_AbstractBinder_INCLUDED
#define SQL_AbstractBinder_INCLUDED


#include "Poco/SQL/SQL.h"
#include "Poco/SQL/Date.h"
#include "Poco/SQL/Time.h"
#include "Poco/SQL/LOB.h"
#include "Poco/DateTime.h"
#include "Poco/Nullable.h"
#include "Poco/Any.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/UTFString.h"
#include <vector>
#include <deque>
#include <list>
#include <cstddef>


namespace Poco {
namespace SQL {


enum NullData
{
	NULL_GENERIC = Poco::NULL_GENERIC,
	DATA_NULL_INTEGER = 1,
	DATA_NULL_STRING = 2,
	DATA_NULL_DATE = 3,
	DATA_NULL_TIME = 4,
	DATA_NULL_DATETIME = 5,
	DATA_NULL_BLOB = 6,
	DATA_NULL_FLOAT = 7
};


struct NullValue
{

	NullValue()
	{}

	template <typename T>
	operator Poco::Nullable<T>() const
	{
		return Poco::Nullable<T>();
	}

	template <typename T>
	static NullData nullCode()
	{
		return SQL::NULL_GENERIC;
	}

};


namespace Keywords {

static const NullValue null;

} // namespace Keywords


template <typename T>
inline bool operator==(const NullValue& nv, const Nullable<T>& n)
{
	return n.isNull();
}


template <typename T>
inline bool operator!=(const NullValue& nv, const Nullable<T>& n)
{
	return !n.isNull();
}


class Poco_SQL_API AbstractBinder
	/// Interface for Binding data types to placeholders.
{
public:
	typedef SharedPtr<AbstractBinder> Ptr;

	struct WhenNullCb
	{
		WhenNullCb() :_func(NULL)
		{}

		inline bool defined() const
		{
			return (_func != NULL);
		}

		inline void onNull()
		{
			if (_func) _func(_data);
		}
	protected:
		void* _data;
		void (*_func)(void*);
	};

	enum Direction
		/// Binding direction for a parameter.
	{
		PD_IN,
		PD_OUT,
		PD_IN_OUT
	};

	AbstractBinder();
		/// Creates the AbstractBinder.

	virtual ~AbstractBinder();
		/// Destroys the AbstractBinder.

	virtual void bind(std::size_t pos, const Poco::Int8& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an Int8.

	virtual void bind(std::size_t pos, const std::vector<Poco::Int8>& val, Direction dir = PD_IN);
		/// Binds an Int8 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::Int8>& val, Direction dir = PD_IN);
		/// Binds an Int8 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::Int8>& val, Direction dir = PD_IN);
		/// Binds an Int8 list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::Int8> >& val, Direction dir = PD_IN);
		/// Binds an Int8 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::Int8> >& val, Direction dir = PD_IN);
		/// Binds an Int8 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::Int8> >& val, Direction dir = PD_IN);
		/// Binds an Int8 list.
		
	virtual void bind(std::size_t pos, const Poco::UInt8& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an UInt8.

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt8>& val, Direction dir = PD_IN);
		/// Binds an UInt8 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt8>& val, Direction dir = PD_IN);
		/// Binds an UInt8 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::UInt8>& val, Direction dir = PD_IN);
		/// Binds an UInt8 list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt8> >& val, Direction dir = PD_IN);
		/// Binds an UInt8 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt8> >& val, Direction dir = PD_IN);
		/// Binds an UInt8 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::UInt8> >& val, Direction dir = PD_IN);
		/// Binds an UInt8 list.

	virtual void bind(std::size_t pos, const Poco::Int16& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an Int16.

	virtual void bind(std::size_t pos, const std::vector<Poco::Int16>& val, Direction dir = PD_IN);
		/// Binds an Int16 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::Int16>& val, Direction dir = PD_IN);
		/// Binds an Int16 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::Int16>& val, Direction dir = PD_IN);
		/// Binds an Int16 list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::Int16> >& val, Direction dir = PD_IN);
		/// Binds an Int16 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::Int16> >& val, Direction dir = PD_IN);
		/// Binds an Int16 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::Int16> >& val, Direction dir = PD_IN);
		/// Binds an Int16 list.

	virtual void bind(std::size_t pos, const Poco::UInt16& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an UInt16.

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt16>& val, Direction dir = PD_IN);
		/// Binds an UInt16 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt16>& val, Direction dir = PD_IN);
		/// Binds an UInt16 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::UInt16>& val, Direction dir = PD_IN);
		/// Binds an UInt16 list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt16> >& val, Direction dir = PD_IN);
		/// Binds an UInt16 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt16> >& val, Direction dir = PD_IN);
		/// Binds an UInt16 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::UInt16> >& val, Direction dir = PD_IN);
		/// Binds an UInt16 list.

	virtual void bind(std::size_t pos, const Poco::Int32& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an Int32.

	virtual void bind(std::size_t pos, const std::vector<Poco::Int32>& val, Direction dir = PD_IN);
		/// Binds an Int32 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::Int32>& val, Direction dir = PD_IN);
		/// Binds an Int32 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::Int32>& val, Direction dir = PD_IN);
		/// Binds an Int32 list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::Int32> >& val, Direction dir = PD_IN);
		/// Binds an Int32 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::Int32> >& val, Direction dir = PD_IN);
		/// Binds an Int32 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::Int32> >& val, Direction dir = PD_IN);
		/// Binds an Int32 list.

	virtual void bind(std::size_t pos, const Poco::UInt32& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an UInt32.

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt32>& val, Direction dir = PD_IN);
		/// Binds an UInt32 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt32>& val, Direction dir = PD_IN);
		/// Binds an UInt32 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::UInt32>& val, Direction dir = PD_IN);
		/// Binds an UInt32 list.
		
	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt32> >& val, Direction dir = PD_IN);
		/// Binds an UInt32 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt32> >& val, Direction dir = PD_IN);
		/// Binds an UInt32 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::UInt32> >& val, Direction dir = PD_IN);
		/// Binds an UInt32 list.

	virtual void bind(std::size_t pos, const Poco::Int64& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an Int64.

	virtual void bind(std::size_t pos, const std::vector<Poco::Int64>& val, Direction dir = PD_IN);
		/// Binds an Int64 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::Int64>& val, Direction dir = PD_IN);
		/// Binds an Int64 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::Int64>& val, Direction dir = PD_IN);
		/// Binds an Int64 list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::Int64> >& val, Direction dir = PD_IN);
		/// Binds an Int64 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::Int64> >& val, Direction dir = PD_IN);
		/// Binds an Int64 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::Int64> >& val, Direction dir = PD_IN);
		/// Binds an Int64 list.

	virtual void bind(std::size_t pos, const Poco::UInt64& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an UInt64.

	virtual void bind(std::size_t pos, const std::vector<Poco::UInt64>& val, Direction dir = PD_IN);
		/// Binds an UInt64 vector.

	virtual void bind(std::size_t pos, const std::deque<Poco::UInt64>& val, Direction dir = PD_IN);
		/// Binds an UInt64 deque.

	virtual void bind(std::size_t pos, const std::list<Poco::UInt64>& val, Direction dir = PD_IN);
		/// Binds an UInt64 list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Poco::UInt64> >& val, Direction dir = PD_IN);
		/// Binds an UInt64 vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Poco::UInt64> >& val, Direction dir = PD_IN);
		/// Binds an UInt64 deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Poco::UInt64> >& val, Direction dir = PD_IN);
		/// Binds an UInt64 list.

#ifndef POCO_LONG_IS_64_BIT
	virtual void bind(std::size_t pos, const long& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a long.

	virtual void bind(std::size_t pos, const unsigned long& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds an unsiged long.

	virtual void bind(std::size_t pos, const std::vector<long>& val, Direction dir = PD_IN);
		/// Binds a long vector.

	virtual void bind(std::size_t pos, const std::deque<long>& val, Direction dir = PD_IN);
		/// Binds a long deque.

	virtual void bind(std::size_t pos, const std::list<long>& val, Direction dir = PD_IN);
		/// Binds a long list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<long> >& val, Direction dir = PD_IN);
		/// Binds a long vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<long> >& val, Direction dir = PD_IN);
		/// Binds a long deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<long> >& val, Direction dir = PD_IN);
		/// Binds a long list.
#endif

	virtual void bind(std::size_t pos, const bool& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a boolean.

	virtual void bind(std::size_t pos, const std::vector<bool>& val, Direction dir = PD_IN);
		/// Binds a boolean vector.

	virtual void bind(std::size_t pos, const std::deque<bool>& val, Direction dir = PD_IN);
		/// Binds a boolean deque.

	virtual void bind(std::size_t pos, const std::list<bool>& val, Direction dir = PD_IN);
		/// Binds a boolean list.


	virtual void bind(std::size_t pos, const std::vector<Nullable<bool> >& val, Direction dir = PD_IN);
		/// Binds a boolean vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<bool> >& val, Direction dir = PD_IN);
		/// Binds a boolean deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<bool> >& val, Direction dir = PD_IN);
		/// Binds a boolean list.

	virtual void bind(std::size_t pos, const float& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a float.

	virtual void bind(std::size_t pos, const std::vector<float>& val, Direction dir = PD_IN);
		/// Binds a float vector.

	virtual void bind(std::size_t pos, const std::deque<float>& val, Direction dir = PD_IN);
		/// Binds a float deque.

	virtual void bind(std::size_t pos, const std::list<float>& val, Direction dir = PD_IN);
		/// Binds a float list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<float> >& val, Direction dir = PD_IN);
		/// Binds a float vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<float> >& val, Direction dir = PD_IN);
		/// Binds a float deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<float> >& val, Direction dir = PD_IN);
		/// Binds a float list.

	virtual void bind(std::size_t pos, const double& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a double.

	virtual void bind(std::size_t pos, const std::vector<double>& val, Direction dir = PD_IN);
		/// Binds a double vector.

	virtual void bind(std::size_t pos, const std::deque<double>& val, Direction dir = PD_IN);
		/// Binds a double deque.

	virtual void bind(std::size_t pos, const std::list<double>& val, Direction dir = PD_IN);
		/// Binds a double list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<double> >& val, Direction dir = PD_IN);
		/// Binds a double vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<double> >& val, Direction dir = PD_IN);
		/// Binds a double deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<double> >& val, Direction dir = PD_IN);
		/// Binds a double list.

	virtual void bind(std::size_t pos, const char& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a single character.

	virtual void bind(std::size_t pos, const std::vector<char>& val, Direction dir = PD_IN);
		/// Binds a character vector.

	virtual void bind(std::size_t pos, const std::deque<char>& val, Direction dir = PD_IN);
		/// Binds a character deque.

	virtual void bind(std::size_t pos, const std::list<char>& val, Direction dir = PD_IN);
		/// Binds a character list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<char> >& val, Direction dir = PD_IN);
		/// Binds a character vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<char> >& val, Direction dir = PD_IN);
		/// Binds a character deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<char> >& val, Direction dir = PD_IN);
		/// Binds a character list.

	virtual void bind(std::size_t pos, const char* const& pVal, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a const char ptr.

	virtual void bind(std::size_t pos, const std::string& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a string.

	virtual void bind(std::size_t pos, const std::vector<std::string>& val, Direction dir = PD_IN);
		/// Binds a string vector.

	virtual void bind(std::size_t pos, const std::deque<std::string>& val, Direction dir = PD_IN);
		/// Binds a string deque.

	virtual void bind(std::size_t pos, const std::list<std::string>& val, Direction dir = PD_IN);
		/// Binds a string list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<std::string> >& val, Direction dir = PD_IN);
		/// Binds a string vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<std::string> >& val, Direction dir = PD_IN);
		/// Binds a string deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<std::string> >& val, Direction dir = PD_IN);
		/// Binds a string list.

	virtual void bind(std::size_t pos, const UTF16String& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb());
		/// Binds a UTF-16 Unicode string.

	virtual void bind(std::size_t pos, const std::vector<UTF16String>& val, Direction dir = PD_IN);
		/// Binds a UTF-16 Unicode string vector.

	virtual void bind(std::size_t pos, const std::deque<UTF16String>& val, Direction dir = PD_IN);
		/// Binds a UTF-16 Unicode string deque.

	virtual void bind(std::size_t pos, const std::list<UTF16String>& val, Direction dir = PD_IN);
		/// Binds a UTF-16 Unicode string list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<UTF16String> >& val, Direction dir = PD_IN);
		/// Binds a UTF-16 Unicode string vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<UTF16String> >& val, Direction dir = PD_IN);
		/// Binds a UTF-16 Unicode string deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<UTF16String> >& val, Direction dir = PD_IN);
		/// Binds a UTF-16 Unicode string list.

	virtual void bind(std::size_t pos, const BLOB& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a BLOB.

	virtual void bind(std::size_t pos, const CLOB& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a CLOB.

	virtual void bind(std::size_t pos, const std::vector<BLOB>& val, Direction dir = PD_IN);
		/// Binds a BLOB vector.

	virtual void bind(std::size_t pos, const std::deque<BLOB>& val, Direction dir = PD_IN);
		/// Binds a BLOB deque.

	virtual void bind(std::size_t pos, const std::list<BLOB>& val, Direction dir = PD_IN);
		/// Binds a BLOB list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<BLOB> >& val, Direction dir = PD_IN);
		/// Binds a BLOB vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<BLOB> >& val, Direction dir = PD_IN);
		/// Binds a BLOB deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<BLOB> >& val, Direction dir = PD_IN);
		/// Binds a BLOB list.

	virtual void bind(std::size_t pos, const std::vector<CLOB>& val, Direction dir = PD_IN);
		/// Binds a CLOB vector.

	virtual void bind(std::size_t pos, const std::deque<CLOB>& val, Direction dir = PD_IN);
		/// Binds a CLOB deque.

	virtual void bind(std::size_t pos, const std::list<CLOB>& val, Direction dir = PD_IN);
		/// Binds a CLOB list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<CLOB> >& val, Direction dir = PD_IN);
		/// Binds a CLOB vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<CLOB> >& val, Direction dir = PD_IN);
		/// Binds a CLOB deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<CLOB> >& val, Direction dir = PD_IN);
		/// Binds a CLOB list.

	virtual void bind(std::size_t pos, const DateTime& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a DateTime.

	virtual void bind(std::size_t pos, const std::vector<DateTime>& val, Direction dir = PD_IN);
		/// Binds a DateTime vector.

	virtual void bind(std::size_t pos, const std::deque<DateTime>& val, Direction dir = PD_IN);
		/// Binds a DateTime deque.

	virtual void bind(std::size_t pos, const std::list<DateTime>& val, Direction dir = PD_IN);
		/// Binds a DateTime list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<DateTime> >& val, Direction dir = PD_IN);
		/// Binds a DateTime vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<DateTime> >& val, Direction dir = PD_IN);
		/// Binds a DateTime deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<DateTime> >& val, Direction dir = PD_IN);
		/// Binds a DateTime list.

	virtual void bind(std::size_t pos, const Date& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a Date.

	virtual void bind(std::size_t pos, const std::vector<Date>& val, Direction dir = PD_IN);
		/// Binds a Date vector.

	virtual void bind(std::size_t pos, const std::deque<Date>& val, Direction dir = PD_IN);
		/// Binds a Date deque.

	virtual void bind(std::size_t pos, const std::list<Date>& val, Direction dir = PD_IN);
		/// Binds a Date list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Date> >& val, Direction dir = PD_IN);
		/// Binds a Date vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Date> >& val, Direction dir = PD_IN);
		/// Binds a Date deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Date> >& val, Direction dir = PD_IN);
		/// Binds a Date list.

	virtual void bind(std::size_t pos, const Time& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb()) = 0;
		/// Binds a Time.

	virtual void bind(std::size_t pos, const std::vector<Time>& val, Direction dir = PD_IN);
		/// Binds a Time vector.

	virtual void bind(std::size_t pos, const std::deque<Time>& val, Direction dir = PD_IN);
		/// Binds a Time deque.

	virtual void bind(std::size_t pos, const std::list<Time>& val, Direction dir = PD_IN);
		/// Binds a Time list.

	virtual void bind(std::size_t pos, const std::vector<Nullable<Time> >& val, Direction dir = PD_IN);
		/// Binds a Time vector.

	virtual void bind(std::size_t pos, const std::deque<Nullable<Time> >& val, Direction dir = PD_IN);
		/// Binds a Time deque.

	virtual void bind(std::size_t pos, const std::list<Nullable<Time> >& val, Direction dir = PD_IN);
		/// Binds a Time list.

	virtual void bind(std::size_t pos, const NullData& val, Direction dir = PD_IN, const std::type_info& bindType = typeid(void)) = 0;
		/// Binds a null.

	virtual void bind(std::size_t pos, const std::vector<NullData>& val, Direction dir = PD_IN, const std::type_info& bindElemType = typeid(void));
		/// Binds a null vector.

	virtual void bind(std::size_t pos, const std::deque<NullData>& val, Direction dir = PD_IN, const std::type_info& bindElemType = typeid(void));
		/// Binds a null deque.

	virtual void bind(std::size_t pos, const std::list<NullData>& val, Direction dir = PD_IN, const std::type_info& bindElemType = typeid(void));
		/// Binds a null list.

	void bind(std::size_t pos, const Any& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb());
		/// Binds an Any.
	
	void bind(std::size_t pos, const Poco::Dynamic::Var& val, Direction dir = PD_IN, const WhenNullCb& nullCb = WhenNullCb());
	/// Binds a Var.

	virtual void reset();
		/// Resets a binder. No-op by default. Implement for binders that cache data.

	static bool isOutBound(Direction dir);
		/// Returns true if direction is out bound;

	static bool isInBound(Direction dir);
		/// Returns true if direction is in bound;

	static bool isInOutBound(Direction dir);
	/// Returns true if direction is in and out bound;
};


//
// inlines
//
inline void AbstractBinder::reset()
{
	//no-op
}


inline bool AbstractBinder::isOutBound(Direction dir)
{
	return PD_OUT == dir || PD_IN_OUT == dir;
}

inline bool AbstractBinder::isInOutBound(Direction dir)
{
	return PD_IN_OUT == dir;
}

inline bool AbstractBinder::isInBound(Direction dir)
{
	return PD_IN == dir || PD_IN_OUT == dir;
}


} } // namespace Poco::SQL


#endif // Data_AbstractBinder_INCLUDED
