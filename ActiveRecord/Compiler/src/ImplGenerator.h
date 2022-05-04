//
// ImplGenerator.h
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecordCompiler_ImplGenerator_INCLUDED
#define ActiveRecordCompiler_ImplGenerator_INCLUDED


#include "CodeGenerator.h"
#include "Types.h"


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


class ImplGenerator: public CodeGenerator
{
public:
	ImplGenerator(const std::string& source, std::ostream& stream, const Class& clazz, const ClassMap& classes);

	void generate() const;
	void writeClassMembers() const;
	void writeCopyConstructor() const;
	void writeReferencingAccessorsImpl() const;
	void writeReferencingGetterImpl(const Property& property) const;
	void writeReferencingSetterImpl(const Property& property) const;
	void writeFind() const;
	void writeInsert() const;
	void writeUpdate() const;
	void writeRemove() const;
	void writeColumns() const;
	void writeTable() const;
	const Class& referencedClass(const Property& property) const;

protected:
	const Property& keyProperty(const Class& clazz) const;
	std::string includeGuard(const std::string& nameSpace, const std::string& name) const;

private:
	Class _class;
	const ClassMap& _classes;
};


} } } // namespace Poco::ActiveRecord::Compiler


#endif // ActiveRecordCompiler_ImplGenerator_INCLUDED
