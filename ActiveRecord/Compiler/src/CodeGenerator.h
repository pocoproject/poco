//
// CodeGenerator.h
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecordCompiler_CodeGenerator_INCLUDED
#define ActiveRecordCompiler_CodeGenerator_INCLUDED


#include "Types.h"
#include <string>
#include <ostream>


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


class CodeGenerator
{
public:
	CodeGenerator(const std::string& source, std::ostream& stream);

	static std::vector<std::string> splitNameSpace(const std::string& nameSpace);

protected:
	const std::string& source() const;
	std::ostream& stream() const;
	void writeBeginNameSpace(const std::string& nameSpace) const;
	void writeEndNameSpace(const std::string& nameSpace) const;
	void writeHeaderComment(const std::string& fileName) const;
	void writeInclude(const std::string& nameSpace, const std::string& name) const;
	std::string propertyType(const Property& prop) const;
	std::string paramType(const Property& prop) const;
	std::string keyType(const Class& clazz) const;
	std::string fullClassName(const Class& clazz) const;
	static bool isSimpleType(const std::string& type);

private:
	std::string _source;
	std::ostream& _stream;
};


//
// inlines
//


inline std::ostream& CodeGenerator::stream() const
{
	return _stream;
}


inline const std::string& CodeGenerator::source() const
{
	return _source;
}


} } } // namespace Poco::ActiveRecord::Compiler


#endif // ActiveRecordCompiler_CodeGenerator_INCLUDED
