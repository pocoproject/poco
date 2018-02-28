//
// SQL.h
//
// Library: Data
// Package: DataCore
// Module:  Constants
//
// Constant definitions for the Poco Data library.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef SQL_Constants_INCLUDED
#define SQL_Constants_INCLUDED


#undef max
#include <limits>
#include <cstddef>


namespace Poco {
namespace SQL {


static const std::size_t POCO_DATA_INVALID_ROW = std::numeric_limits<std::size_t>::max();


} } // namespace Poco::SQL


#endif // Data_Constants_INCLUDED
