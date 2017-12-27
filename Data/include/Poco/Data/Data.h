//
// Data.h
//
// Forward for the Data class header.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_Data_INCLUDED
#define Data_Data_INCLUDED

#include "Poco/SQL/SQL.h"

#define Data_API Poco_SQL_API

namespace Poco {
namespace SQL {}
	namespace Data = SQL;
} // namespace Poco

#endif // Data_Data_INCLUDED
