//
// Preparator.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Preparator.h"
#include "Poco/Data/LOB.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Data {
namespace Test {


Preparator::Preparator()
{
}


Preparator::~Preparator()
{
}


void Preparator::prepare(std::size_t pos, const Poco::Int8& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::UInt8& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Int16& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::UInt16& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Int32& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::UInt32& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Int64& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::UInt64& val)
{
}


#ifndef POCO_INT64_IS_LONG
void Preparator::prepare(std::size_t pos, const long& val)
{
}


void Preparator::prepare(std::size_t pos, const unsigned long& val)
{
}
#endif


void Preparator::prepare(std::size_t pos, const bool& val)
{
}


void Preparator::prepare(std::size_t pos, const float& val)
{
}


void Preparator::prepare(std::size_t pos, const double& val)
{
}


void Preparator::prepare(std::size_t pos, const char& val)
{
}


void Preparator::prepare(std::size_t pos, const std::string& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::UTF16String& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Data::BLOB& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Data::CLOB& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Data::Date& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Data::Time& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::DateTime& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::UUID& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Any& val)
{
}


void Preparator::prepare(std::size_t pos, const Poco::Dynamic::Var& val)
{
}


} } } // namespace Poco::Data::Test
