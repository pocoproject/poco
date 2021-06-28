//
// Types.h
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecordCompiler_Types_INCLUDED
#define ActiveRecordCompiler_Types_INCLUDED


#include "Poco/Types.h"
#include <vector>
#include <map>


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


struct Property
{
	static const char CARD_ZERO_OR_ONE  = '?';
	static const char CARD_ONE          = '1';
	static const char CARD_ZERO_OR_MORE = '*';
	static const char CARD_ONE_OR_MORE  = '+';

	std::string name;
	std::string column;
	std::string type;
	std::string referencedClass;
	std::string foreignKey;
	char cardinality = CARD_ONE;
	bool nullable = false;
};


struct Class
{
	std::string name;
	std::string nameSpace;
	std::string table;
	std::string key;
	bool autoIncrementID = false;
	std::vector<Property> properties;
	std::vector<std::string> references;
};


using ClassMap = std::map<std::string, Class>;


} } } // namespace Poco::ActiveRecord::Compiler


#endif // ActiveRecordCompiler_Types_INCLUDED
