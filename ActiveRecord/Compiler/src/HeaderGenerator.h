//
// HeaderGenerator.h
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecordCompiler_HeaderGenerator_INCLUDED
#define ActiveRecordCompiler_HeaderGenerator_INCLUDED


#include "CodeGenerator.h"
#include "Types.h"


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


class HeaderGenerator: public CodeGenerator
{
public:
	HeaderGenerator(const std::string& source, std::ostream& stream, const Class& clazz, const ClassMap& classes);

	void generate() const;
	void writeClass() const;
	void writeTypeHandler() const;
	void writeSimpleAccessors() const;
	void writeReferencingAccessors() const;
	void writeVariables() const;
	void writeGetter(const Property& property) const;
	void writeSetter(const Property& property) const;
	void writeReferenceGetter(const Property& property) const;
	void writeReferenceSetter(const Property& property) const;
	void writeInlineAccessorImpls() const;
	void writeInlineReferencingAccessorImpls() const;
	void writeInlineGetterImpl(const Property& property) const;
	void writeInlineSetterImpl(const Property& property) const;
	void writeInlineReferencingGetterImpl(const Property& property) const;
	void writeInlineReferencingSetterImpl(const Property& property) const;
	const Class& referencedClass(const Property& property) const;

protected:
	std::string includeGuard(const std::string& nameSpace, const std::string& name) const;

private:
	Class _class;
	const ClassMap& _classes;
};


} } } // namespace Poco::ActiveRecord::Compiler


#endif // ActiveRecordCompiler_HeaderGenerator_INCLUDED
