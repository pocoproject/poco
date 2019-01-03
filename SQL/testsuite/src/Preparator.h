//
// Preparator.h
//
// Definition of the Preparator class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_Test_Preparator_INCLUDED
#define SQL_Test_Preparator_INCLUDED


#include "Poco/SQL/AbstractPreparator.h"
#include "Poco/Any.h"


namespace Poco {
namespace SQL {
namespace Test {


class Preparator: public Poco::SQL::AbstractPreparator
	/// A no-op implementation of AbstractPreparator for testing.
{
public:
	Preparator();
		/// Creates the Preparator.

	~Preparator();
		/// Destroys the Preparator.

	void prepare(std::size_t pos, const Poco::Int8&);
		/// Prepares an Int8.

	void prepare(std::size_t pos, const Poco::UInt8&);
		/// Prepares an UInt8.

	void prepare(std::size_t pos, const Poco::Int16&);
		/// Prepares an Int16.

	void prepare(std::size_t pos, const Poco::UInt16&);
		/// Prepares an UInt16.

	void prepare(std::size_t pos, const Poco::Int32&);
		/// Prepares an Int32.

	void prepare(std::size_t pos, const Poco::UInt32&);
		/// Prepares an UInt32.

	void prepare(std::size_t pos, const Poco::Int64&);
		/// Prepares an Int64.

	void prepare(std::size_t pos, const Poco::UInt64&);
		/// Prepares an UInt64.

#ifndef POCO_LONG_IS_64_BIT
	void prepare(std::size_t pos, const long&);
		/// Prepares a long.

	void prepare(std::size_t pos, const unsigned long&);
		/// Prepares an unsigned long.
#endif

	void prepare(std::size_t pos, const bool&);
		/// Prepares a boolean.

	void prepare(std::size_t pos, const float&);
		/// Prepares a float.

	void prepare(std::size_t pos, const double&);
		/// Prepares a double.

	void prepare(std::size_t pos, const char&);
		/// Prepares a single character.

	void prepare(std::size_t pos, const std::string&);
		/// Prepares a string.

	void prepare(std::size_t pos, const Poco::UTF16String&);
		/// Prepares a UTF16String.

	void prepare(std::size_t pos, const Poco::SQL::BLOB&);
		/// Prepares a BLOB.

	void prepare(std::size_t pos, const Poco::SQL::CLOB&);
		/// Prepares a CLOB.

	void prepare(std::size_t pos, const Poco::SQL::Date&);
		/// Prepares a Date.

	void prepare(std::size_t pos, const Poco::SQL::Time&);
		/// Prepares a Time.

	void prepare(std::size_t pos, const Poco::DateTime&);
		/// Prepares a DateTime.

	void prepare(std::size_t pos, const Poco::Any&);
		/// Prepares an Any.

	void prepare(std::size_t pos, const Poco::Dynamic::Var&);
		/// Prepares a Var.
};


} } } // namespace Poco::SQL::Test


#endif // Data_Test_Preparator_INCLUDED
