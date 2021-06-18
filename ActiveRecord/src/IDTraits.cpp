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


const std::string IDTraits<std::string>::INVALID_ID;
const Poco::UUID IDTraits<Poco::UUID>::INVALID_ID;


} } // namespace Poco::ActiveRecord
