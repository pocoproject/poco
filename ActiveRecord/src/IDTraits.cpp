//
// IDTraits.cpp
//
// Library: ActiveRecord
// Package: ActiveRecord
// Module:  IDTraits
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/ActiveRecord/IDTraits.h"


namespace Poco {
namespace ActiveRecord {


const Poco::UInt64 IDTraits<Poco::UInt64>::INVALID_ID = std::numeric_limits<Poco::UInt64>::max();
const Poco::Int64 IDTraits<Poco::Int64>::INVALID_ID = std::numeric_limits<Poco::Int64>::max();
const Poco::UInt32 IDTraits<Poco::UInt32>::INVALID_ID = std::numeric_limits<Poco::UInt32>::max();
const Poco::Int32 IDTraits<Poco::Int32>::INVALID_ID = std::numeric_limits<Poco::Int32>::max();
const Poco::UInt16 IDTraits<Poco::UInt16>::INVALID_ID = std::numeric_limits<Poco::UInt16>::max();
const Poco::Int16 IDTraits<Poco::Int16>::INVALID_ID = std::numeric_limits<Poco::Int16>::max();
const std::string IDTraits<std::string>::INVALID_ID;
const Poco::UUID IDTraits<Poco::UUID>::INVALID_ID;


} } // namespace Poco::ActiveRecord
