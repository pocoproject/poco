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


#ifndef Data_Test_Preparator_INCLUDED
#define Data_Test_Preparator_INCLUDED


#include "Poco/Data/AbstractPreparator.h"
#include "Poco/Any.h"


namespace Poco {
namespace Data {
namespace Test {


class Preparator: public Poco::Data::AbstractPreparator
	/// A no-op implementation of AbstractPreparator for testing.
{
public:
	Preparator();
		/// Creates the Preparator.

	~Preparator();
		/// Destroys the Preparator.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Int8& val);
		/// Prepares an Int8.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::UInt8& val);
		/// Prepares an UInt8.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Int16& val);
		/// Prepares an Int16.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::UInt16& val);
		/// Prepares an UInt16.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Int32& val);
		/// Prepares an Int32.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::UInt32& val);
		/// Prepares an UInt32.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Int64& val);
		/// Prepares an Int64.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::UInt64& val);
		/// Prepares an UInt64.

#ifndef POCO_INT64_IS_LONG
	void prepare(std::size_t pos, [[maybe_unused]] const long& val);
		/// Prepares a long.

	void prepare(std::size_t pos, [[maybe_unused]] const unsigned long& val);
		/// Prepares an unsigned long.
#endif

	void prepare(std::size_t pos, [[maybe_unused]] const bool& val);
		/// Prepares a boolean.

	void prepare(std::size_t pos, [[maybe_unused]] const float& val);
		/// Prepares a float.

	void prepare(std::size_t pos, [[maybe_unused]] const double& val);
		/// Prepares a double.

	void prepare(std::size_t pos, [[maybe_unused]] const char& val);
		/// Prepares a single character.

	void prepare(std::size_t pos, [[maybe_unused]] const std::string& val);
		/// Prepares a string.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::UTF16String& val);
		/// Prepares a UTF16String.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Data::BLOB& val);
		/// Prepares a BLOB.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Data::CLOB& val);
		/// Prepares a CLOB.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Data::Date& val);
		/// Prepares a Date.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Data::Time& val);
		/// Prepares a Time.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::DateTime& val);
		/// Prepares a DateTime.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::UUID& val);
		/// Prepares a UUID.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Any& val);
		/// Prepares an Any.

	void prepare(std::size_t pos, [[maybe_unused]] const Poco::Dynamic::Var& val);
		/// Prepares a Var.
};


} } } // namespace Poco::Data::Test


#endif // Data_Test_Preparator_INCLUDED
