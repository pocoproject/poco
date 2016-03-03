//
// Binder.h
//
// $Id: //poco/Main/Data/SQLite/include/Poco/Data/SQLite/Binder.h#4 $
//
// Library: SQLite
// Package: SQLite
// Module:  Binder
//
// Definition of the Binder class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_SQLite_Binder_INCLUDED
#define Data_SQLite_Binder_INCLUDED


#include "Poco/Data/SQLite/SQLite.h"
#include "Poco/Data/AbstractBinder.h"
#include "Poco/Data/LOB.h"
#include "Poco/Any.h"
#include "Poco/DynamicAny.h"
#include "sqlite3.h"


namespace Poco {
namespace Data {
namespace SQLite {


class SQLite_API Binder: public Poco::Data::AbstractBinder
	/// Binds placeholders in the sql query to the provided values. Performs data types mapping.
{
public:
	Binder(sqlite3_stmt* pStmt);
		/// Creates the Binder.

	~Binder();
		/// Destroys the Binder.

	void bind(std::size_t pos, const Poco::Int8 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int8.

	void bind(std::size_t pos, const Poco::UInt8 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt8.

	void bind(std::size_t pos, const Poco::Int16 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int16.

	void bind(std::size_t pos, const Poco::UInt16 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt16.

	void bind(std::size_t pos, const Poco::Int32 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int32.

	void bind(std::size_t pos, const Poco::UInt32 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt32.

	void bind(std::size_t pos, const Poco::Int64 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an Int64.

	void bind(std::size_t pos, const Poco::UInt64 &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an UInt64.

#ifndef POCO_LONG_IS_64_BIT
	void bind(std::size_t pos, const long &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a long

	void bind(std::size_t pos, const unsigned long &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds an unsigned long
#endif

	void bind(std::size_t pos, const bool &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a boolean.

	void bind(std::size_t pos, const float &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a float.

	void bind(std::size_t pos, const double &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a double.

	void bind(std::size_t pos, const char &val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a single character.

	void bind(std::size_t pos, const char* const &pVal, Direction dir, const WhenNullCb& nullCb);
		/// Binds a const char ptr.

	void bind(std::size_t pos, const std::string& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a string.

	void bind(std::size_t pos, const Poco::Data::BLOB& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a BLOB.

	void bind(std::size_t pos, const Poco::Data::CLOB& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a CLOB.

	void bind(std::size_t pos, const Date& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a Date.

	void bind(std::size_t pos, const Time& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a Time.

	void bind(std::size_t pos, const DateTime& val, Direction dir, const WhenNullCb& nullCb);
		/// Binds a DateTime.

	void bind(std::size_t pos, const NullData& val, Direction dir, const std::type_info& bindType);
		/// Binds a null.

private:
	void checkReturn(int rc);
		/// Checks the SQLite return code and throws an appropriate exception
		/// if error has occurred.

	template <typename T>
	void bindLOB(std::size_t pos, const Poco::Data::LOB<T>& val, Direction dir, const WhenNullCb& nullCb)
	{
		// convert a blob to a an unsigned char* array
		const T* pData = reinterpret_cast<const T*>(val.rawContent());
		int valSize = static_cast<int>(val.size());

		int rc = sqlite3_bind_blob(_pStmt, static_cast<int>(pos), pData, valSize, SQLITE_STATIC); // no deep copy, do not free memory
		checkReturn(rc);
	}

	sqlite3_stmt* _pStmt;
};


//
// inlines
//
inline void Binder::bind(std::size_t pos, const Poco::Int8 &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int32 tmp = val;
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt8 &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int32 tmp = val;
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const Poco::Int16 &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int32 tmp = val;
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt16 &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int32 tmp = val;
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt32 &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int32 tmp = static_cast<Poco::Int32>(val);
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const Poco::UInt64 &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int64 tmp = static_cast<Poco::Int64>(val);
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const bool &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int32 tmp = (val ? 1 : 0);
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const float &val, Direction dir, const WhenNullCb& nullCb)
{
	double tmp = val;
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const char &val, Direction dir, const WhenNullCb& nullCb)
{
	Poco::Int32 tmp = val;
	bind(pos, tmp, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const char* const &pVal, Direction dir, const WhenNullCb& nullCb)
{
	std::string val(pVal);
	bind(pos, val, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const Poco::Data::BLOB& val, Direction dir, const WhenNullCb& nullCb)
{
	bindLOB<Poco::Data::BLOB::ValueType>(pos, val, dir, nullCb);
}


inline void Binder::bind(std::size_t pos, const Poco::Data::CLOB& val, Direction dir, const WhenNullCb& nullCb)
{
	bindLOB<Poco::Data::CLOB::ValueType>(pos, val, dir, nullCb);
}


} } } // namespace Poco::Data::SQLite


#endif // Data_SQLite_Binder_INCLUDED
