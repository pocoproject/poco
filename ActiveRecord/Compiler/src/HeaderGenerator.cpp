//
// HeaderGenerator.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "HeaderGenerator.h"
#include "Poco/Exception.h"


using namespace std::string_literals;


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


HeaderGenerator::HeaderGenerator(const std::string& source, std::ostream& stream, const Class& clazz, const ClassMap& classes):
	CodeGenerator(source, stream),
	_class(clazz),
	_classes(classes)
{
}


void HeaderGenerator::generate() const
{
	writeHeaderComment(_class.name + ".h");
	std::string guard = includeGuard(_class.nameSpace, _class.name);
	stream()
		<< "#ifndef " << guard << "\n"
		<< "#define " << guard << "\n"
		<< "\n\n";
	stream() << "#include \"Poco/ActiveRecord/ActiveRecord.h\"\n";
	for (const auto& ref: _class.references)
	{
		writeInclude(_class.nameSpace, ref);
	}
	stream() << "\n\n";
	writeBeginNameSpace(_class.nameSpace);
	stream() << "\n\n";
	writeClass();
	writeEndNameSpace(_class.nameSpace);
	stream() << "\n\n";
	writeTypeHandler();
	stream()
		<< "\n\n"
		<< "#endif // " << guard << "\n";
}


std::string HeaderGenerator::includeGuard(const std::string& nameSpace, const std::string& name) const
{
	std::string guard;
	auto ns = splitNameSpace(nameSpace);
	for (const auto& s: ns)
	{
		guard += s;
		guard += '_';
	}
	guard += name;
	guard += "_INCLUDED";
	return guard;
}


void HeaderGenerator::writeClass() const
{
	stream() << "class " << _class.name << ": public ";
	if (_class.key.empty())
		stream() << "Poco::ActiveRecord::KeylessActiveRecord";
	else
		stream() << "Poco::ActiveRecord::ActiveRecord<" << keyType(_class) << ">";
	stream() << "\n{\npublic:\n";
	stream() << "\tusing Ptr = Poco::AutoPtr<" << _class.name << ">;\n\n";
	if (!_class.key.empty())
	{
		stream() << "\texplicit " << _class.name << "(ID id);\n";
	}
	stream()
		<< "\t" << _class.name << "() = default;\n"
		<< "\t" << _class.name << "(const " << _class.name << "& other);\n"
		<< "\t~" << _class.name << "() = default;\n"
		<< "\n";

	writeSimpleAccessors();
	writeReferencingAccessors();

	if (!_class.key.empty())
	{
		stream() << "\tstatic Ptr find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id);\n\n";
	}

	stream()
		<< "\tvoid insert();\n"
		<< "\tvoid update();\n"
		<< "\tvoid remove();\n"
		<< "\n"
		<< "\tstatic const std::vector<std::string>& columns();\n"
		<< "\tstatic const std::string& table();\n";

	stream() << "\nprivate:\n";
	writeVariables();
	stream() << "\n\tfriend class Poco::Data::TypeHandler<" << _class.name << ">;\n";
	stream() << "};\n";
	stream() << "\n\n";
	writeInlineAccessorImpls();
	writeInlineReferencingAccessorImpls();
}


void HeaderGenerator::writeTypeHandler() const
{
	stream()
		<< "namespace Poco {\n"
		<< "namespace Data {\n"
		<< "\n\n"
		<< "template <>\n"
		<< "class TypeHandler<" << fullClassName(_class) << ">\n"
		<< "{\n"
		<< "public:\n"
		<< "\tstatic std::size_t size()\n"
		<< "\t{\n"
		<< "\t\treturn " << _class.properties.size() - (_class.key.empty() ? 0 : 1) << ";\n"
		<< "\t}\n"
		<< "\n"
		<< "\tstatic void bind(std::size_t pos, const " << fullClassName(_class) << "& ar, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)\n"
		<< "\t{\n";

	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			stream() << "\t\tTypeHandler<" << propertyType(p) << ">::bind(pos++, ar._" << p.name << ", pBinder, dir);\n";
		}
	}

	stream()
		<< "}\n"
		<< "\n"
		<< "\tstatic void extract(std::size_t pos, " << fullClassName(_class) << "& ar, const " << fullClassName(_class) << "& deflt, AbstractExtractor::Ptr pExtr)\n"
		<< "\t{\n";

	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			stream() << "\t\tTypeHandler<" << propertyType(p) << ">::extract(pos++, ar._" << p.name << ", deflt._" << p.name << ", pExtr);\n";
		}
	}

	stream()
		<< "}\n"
		<< "\n"
		<< "\tstatic void prepare(std::size_t pos, const " << fullClassName(_class) << "& ar, AbstractPreparator::Ptr pPrep)\n"
		<< "\t{\n";

	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			stream() << "\t\tTypeHandler<" << propertyType(p) << ">::prepare(pos++, ar._" << p.name << ", pPrep);\n";
		}
	}

	stream()
		<< "\t}\n"
		<< "};\n"
		<< "\n\n"
		<< "} } // namespace Poco::Data\n";
}


void HeaderGenerator::writeSimpleAccessors() const
{
	for (const auto& p: _class.properties)
	{
		if (p.referencedClass.empty() && p.name != _class.key)
		{
			writeGetter(p);
			writeSetter(p);
			stream() << "\n";
		}
	}
}


void HeaderGenerator::writeReferencingAccessors() const
{
	for (const auto& p: _class.properties)
	{
		if (!p.referencedClass.empty() && (p.cardinality == Property::CARD_ZERO_OR_ONE || p.cardinality == Property::CARD_ONE))
		{
			writeReferenceGetter(p);
			writeReferenceSetter(p);
			stream() << "\n";
		}
	}
}


void HeaderGenerator::writeVariables() const
{
	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			stream() << "\t" << propertyType(p) << " _" << p.name;
			if (isSimpleType(p.type) && !p.nullable)
			{
				if (p.referencedClass.empty())
				{
					if (p.type == "bool")
						stream() << " = false";
					else
						stream() << " = 0";
				}
				else
				{
					const Class& refClass = referencedClass(p);
					stream() << " = " << refClass.name << "::INVALID_ID";
				}
			}
			stream() << ";\n";
		}
	}
}


void HeaderGenerator::writeGetter(const Property& property) const
{
	stream() << "\t" << paramType(property) << " " << property.name << "() const;\n";

}


void HeaderGenerator::writeSetter(const Property& property) const
{
	stream() << "\t" << _class.name << "& " << property.name << "(" << paramType(property) << " value);\n";
}


void HeaderGenerator::writeReferenceGetter(const Property& property) const
{
	const Class& refClass = referencedClass(property);
	stream()
		<< "\t" << refClass.name << "::Ptr " << property.name << "() const;\n"
		<< "\t" << paramType(property) << " " << property.name << "ID() const;\n";

}


void HeaderGenerator::writeReferenceSetter(const Property& property) const
{
	const Class& refClass = referencedClass(property);
	stream()
		<< "\t" << _class.name << "& " << property.name << "(" << refClass.name << "::Ptr pObject);\n"
		<< "\t" << _class.name << "& " << property.name << "ID(" << paramType(property) << " id);\n";
}


void HeaderGenerator::writeInlineAccessorImpls() const
{
	for (const auto& p: _class.properties)
	{
		if (p.referencedClass.empty() && p.name != _class.key)
		{
			writeInlineGetterImpl(p);
			stream() << "\n\n";
			writeInlineSetterImpl(p);
			stream() << "\n\n";
		}
	}
}


void HeaderGenerator::writeInlineReferencingAccessorImpls() const
{
	for (const auto& p: _class.properties)
	{
		if (!p.referencedClass.empty() && (p.cardinality == Property::CARD_ZERO_OR_ONE || p.cardinality == Property::CARD_ONE))
		{
			writeInlineReferencingGetterImpl(p);
			stream() << "\n\n";
			writeInlineReferencingSetterImpl(p);
			stream() << "\n\n";
		}
	}
}


void HeaderGenerator::writeInlineGetterImpl(const Property& property) const
{
	stream()
		<< "inline " << paramType(property) << " " << _class.name << "::" << property.name << "() const\n"
		<< "{\n"
		<< "\treturn _" << property.name << ";\n"
		<< "}\n";
}


void HeaderGenerator::writeInlineSetterImpl(const Property& property) const
{
	stream()
		<< "inline " << _class.name << "& " << _class.name << "::" << property.name << "(" << paramType(property) << " value)\n"
		<< "{\n"
		<< "\t_" << property.name << " = value;\n"
		<< "\treturn *this;\n"
		<< "}\n";
}


void HeaderGenerator::writeInlineReferencingGetterImpl(const Property& property) const
{
	stream()
		<< "inline " << paramType(property) << " " << _class.name << "::" << property.name << "ID() const\n"
		<< "{\n"
		<< "\treturn _" << property.name << ";\n"
		<< "}\n";
}


void HeaderGenerator::writeInlineReferencingSetterImpl(const Property& property) const
{
	stream()
		<< "inline " << _class.name << "& " << _class.name << "::" << property.name << "ID(" << paramType(property) << " value)\n"
		<< "{\n"
		<< "\t_" << property.name << " = value;\n"
		<< "\treturn *this;\n"
		<< "}\n";
}


const Class& HeaderGenerator::referencedClass(const Property& property) const
{
	poco_assert (!property.referencedClass.empty());

	auto it = _classes.find(property.referencedClass);
	if (it != _classes.end())
		return it->second;
	else
		throw Poco::NotFoundException("referenced class"s, property.referencedClass);
}


} } } // namespace Poco::ActiveRecord::Compiler
