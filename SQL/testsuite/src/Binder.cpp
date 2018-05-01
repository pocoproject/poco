//
// Binder.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Binder.h"
#include "Poco/SQL/LOB.h"
#include "Poco/Exception.h"


namespace Poco {
namespace SQL {
namespace Test {


Binder::Binder()
{
}


Binder::~Binder()
{
}


void Binder::bind(std::size_t pos, const Poco::Int8 &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::UInt8 &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::Int16 &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::UInt16 &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::Int32 &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::UInt32 &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::Int64 &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::UInt64 &val, Direction dir, const WhenNullCb& nullCb)
{
}


#ifndef POCO_LONG_IS_64_BIT
void Binder::bind(std::size_t pos, const long& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const unsigned long& val, Direction dir, const WhenNullCb& nullCb)
{
}
#endif


void Binder::bind(std::size_t pos, const bool &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const float &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const double &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const char &val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const char* const &pVal, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const std::string& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Poco::UTF16String& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const BLOB& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const CLOB& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Date& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const Time& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const DateTime& val, Direction dir, const WhenNullCb& nullCb)
{
}


void Binder::bind(std::size_t pos, const NullData& val, Direction dir, const std::type_info& bindType)
{
}


void Binder::reset()
{
}


} } } // namespace Poco::SQL::Test
