//
// ImplGenerator.cpp
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ImplGenerator.h"
#include "Poco/Exception.h"


using namespace std::string_literals;


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


ImplGenerator::ImplGenerator(const std::string& source, std::ostream& stream, const Class& clazz, const ClassMap& classes):
	CodeGenerator(source, stream),
	_class(clazz),
	_classes(classes)
{
}


void ImplGenerator::generate() const
{
	writeHeaderComment(_class.name + ".cpp");
	writeInclude(_class.nameSpace, _class.name);

	if (!_class.key.empty())
	{
		if (keyType(_class) == "Poco::UUID")
		{
			stream() << "#include \"Poco/UUIDGenerator.h\"\n";
		}
	}

	stream() << "\n\n";
	stream() << "using namespace std::string_literals;\n";
	stream() << "using namespace Poco::Data::Keywords;\n";
	stream() << "\n\n";
	writeBeginNameSpace(_class.nameSpace);
	stream() << "\n\n";
	writeClassMembers();
	writeEndNameSpace(_class.nameSpace);
}


void ImplGenerator::writeClassMembers() const
{
	if (!_class.key.empty())
	{
		stream()
			<< _class.name << "::" << _class.name << "(ID id):\n"
			<< "\tPoco::ActiveRecord::ActiveRecord<" << keyType(_class) << ">(id)\n"
			<< "{\n"
			<< "}\n"
			<< "\n\n";
	}
	writeCopyConstructor();
	stream() << "\n\n";
	writeReferencingAccessorsImpl();
	if (!_class.key.empty())
	{
		writeFind();
		stream() << "\n\n";
	}
	writeInsert();
	stream() << "\n\n";
	writeUpdate();
	stream() << "\n\n";
	writeRemove();
	stream() << "\n\n";
	writeColumns();
	stream() << "\n\n";
	writeTable();
	stream() << "\n\n";
}


void ImplGenerator::writeCopyConstructor() const
{
	stream() << _class.name << "::" << _class.name << "(const " << _class.name << "& other):\n";
	bool needComma = false;
	if (!_class.key.empty())
	{
		stream() << "\tPoco::ActiveRecord::ActiveRecord<" << keyType(_class) << ">(other)";
		needComma = true;
	}

	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			if (needComma) stream() << ",\n";
			stream() << "\t_" << p.name << "(other._" << p.name << ")";
			needComma = true;
		}
	}

	stream()
		<< "\n"
		<< "{\n"
		<< "}\n";
}


void ImplGenerator::writeReferencingAccessorsImpl() const
{
	for (const auto& p: _class.properties)
	{
		if (!p.referencedClass.empty() && (p.cardinality == Property::CARD_ZERO_OR_ONE || p.cardinality == Property::CARD_ONE))
		{
			writeReferencingGetterImpl(p);
			stream() << "\n\n";
			writeReferencingSetterImpl(p);
			stream() << "\n\n";
		}
	}
}


void ImplGenerator::writeReferencingGetterImpl(const Property& property) const
{
	const Class& refClass = referencedClass(property);
	stream()
		<< refClass.name << "::Ptr " << _class.name << "::" << property.name << "() const\n"
		<< "{\n";

	if (property.nullable)
	{
		stream()
			<< "\tif (!_" << property.name << ".isNull())\n"
			<< "\t\treturn " << refClass.name << "::find(context(), _" << property.name << ".value());\n"
			<< "\telse\n"
			<< "\t\treturn nullptr;\n";
	}
	else
	{
		stream() << "\treturn " << refClass.name << "::find(context(), _" << property.name << ");\n";
	}
	stream() << "}\n";
}


void ImplGenerator::writeReferencingSetterImpl(const Property& property) const
{
	const Class& refClass = referencedClass(property);
	stream()
		<< _class.name << "& " << _class.name << "::" << property.name << "(" << refClass.name << "::Ptr pObject)\n"
		<< "{\n"
		<< "\tif (pObject)\n"
		<< "\t\t_" << property.name << " = pObject->id();\n"
		<< "\telse\n"
		<< "\t\t_" << property.name << " = " << refClass.name << "::INVALID_ID;\n"
		<< "\treturn *this;\n"
		<< "}\n";
}


void ImplGenerator::writeFind() const
{
	stream()
		<< _class.name << "::Ptr " << _class.name << "::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)\n"
		<< "{\n"
		<< "\tPoco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());\n"
		<< "\t" << _class.name << "::Ptr pObject(new " << _class.name << ");\n"
		<< "\n"
		<< "\tpContext->session()\n"
		<< "\t\t<< \"SELECT "
		<< keyProperty(_class).column;

	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			stream() << ", " << p.column;
		}
	}

	stream()
		<< "\"\n"
		<< "\t\t<< \"  FROM " << _class.table << "\"\n"
		<< "\t\t<< \"  WHERE " << keyProperty(_class).column << " = \" << pSPP->next(),\n"
		<< "\t\tinto(pObject->mutableID()),\n"
		<< "\t\tinto(*pObject),\n"
		<< "\t\tbind(id),\n"
		<< "\t\tnow;\n"
		<< "\n"
		<< "\treturn withContext(pObject, pContext);\n"
		<< "}\n";
}


void ImplGenerator::writeInsert() const
{
	stream()
		<< "void " << _class.name << "::insert()\n"
		<< "{\n"
		<< "\tPoco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());\n"
		<< "\n";

	if (!_class.key.empty())
	{
		if (keyType(_class) == "Poco::UUID")
		{
			stream()
				<< "\tif (id().isNull())\n"
				<< "\t{\n"
				<< "\t\tmutableID() = Poco::UUIDGenerator().createRandom();\n"
				<< "\t}\n"
				<< "\n";
		}
	}

	stream()
		<< "\tcontext()->session()\n"
		<< "\t\t<< \"INSERT INTO " << _class.table << " (";

	bool needComma = false;
	if (!_class.key.empty())
	{
		stream() << keyProperty(_class).column;
		needComma = true;
	}

	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			if (needComma) stream() << ", ";
			stream() << p.column;
			needComma = true;
		}
	}

	stream()
		<< ")\"\n"
		<< "\t\t<< \"  VALUES (";

	needComma = false;
	if (!_class.key.empty())
	{
		if (_class.autoIncrementID)
			stream() << "NULL";
		else
			stream() << "\" << pSPP->next() << \"";
		needComma = true;
	}

	for (const auto& p: _class.properties)
	{
		if (p.name != _class.key)
		{
			if (needComma) stream() << ", ";
			stream() << "\" << pSPP->next() << \"";
			needComma = true;
		}
	}

	stream() << ")\",\n";

	if (!_class.key.empty() && !_class.autoIncrementID)
	{
		stream() << "\t\tbind(id()),\n";
	}
	stream()
		<< "\t\tuse(*this),\n"
		<< "\t\tnow;\n";

	if (_class.autoIncrementID)
	{
		stream() << "\tupdateID(context()->session());\n";
	}

	stream() << "}\n";
}


void ImplGenerator::writeUpdate() const
{
	stream()
		<< "void " << _class.name << "::update()\n"
		<< "{\n";

	if (_class.key.empty())
	{
		stream() << "\tthrow Poco::NotImplementedException(\"update not implemented for keyless class\", \"" << _class.name << "\");\n";
	}
	else
	{
		stream()
			<< "\tPoco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());\n"
			<< "\n"
			<< "\tcontext()->session()\n"
			<< "\t\t<< \"UPDATE " << _class.table << "\"\n"
			<< "\t\t<< \"  SET ";

		bool needComma = false;
		for (const auto& p: _class.properties)
		{
			if (p.name != _class.key)
			{
				if (needComma) stream() << " << \", ";
				stream() << p.column << " = \" << pSPP->next()";
				needComma = true;
			}
		}

		stream()
			<< "\n"
			<< "\t\t<< \"  WHERE " << keyProperty(_class).column << " = \" << pSPP->next(),\n"
			<< "\t\tuse(*this),\n"
			<< "\t\tbind(id()),\n"
			<< "\t\tnow;\n";
	}
	stream() << "}\n";
}


void ImplGenerator::writeRemove() const
{
	stream()
		<< "void " << _class.name << "::remove()\n"
		<< "{\n"
		<< "\tPoco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());\n"
		<< "\n"
		<< "\tcontext()->session()\n"
		<< "\t\t<< \"DELETE FROM " << _class.table << "\"\n"
		<< "\t\t<< \"  WHERE ";

	if (_class.key.empty())
	{
		bool needAnd = false;
		for (const auto& p: _class.properties)
		{
			if (needAnd) stream() << " << \" AND ";
			stream() << p.column << " = \" << pSPP->next()";
			needAnd = true;
		}
		stream()
			<< ",\n"
			<< "\t\tuse(*this),\n"
			<< "\t\tnow;\n";
	}
	else
	{
		stream()
			<< keyProperty(_class).column << " = \" << pSPP->next(),\n"
			<< "\t\tbind(id()),\n"
			<< "\t\tnow;\n";
	}
	stream() << "}\n";
}


void ImplGenerator::writeColumns() const
{
	stream()
		<< "const std::vector<std::string>& " << _class.name << "::columns()\n"
		<< "{\n"
		<< "\tstatic const std::vector<std::string> cols =\n"
		<< "\t{\n";

	for (const auto& p: _class.properties)
	{
		stream() << "\t\t\"" << p.column << "\"s,\n";
	}

	stream()
		<< "\t};\n"
		<< "\n"
		<< "\treturn cols;\n"
		<< "}\n";
}


void ImplGenerator::writeTable() const
{
	stream()
		<< "const std::string& " << _class.name << "::table()\n"
		<< "{\n"
		<< "\tstatic const std::string t = \"" << _class.table << "\";\n"
		<< "\treturn t;\n"
		<< "}\n";
}


const Property& ImplGenerator::keyProperty(const Class& clazz) const
{
	for (const auto& p: clazz.properties)
	{
		if (p.name == clazz.key)
			return p;
	}
	throw Poco::NotFoundException("key property"s, clazz.key);
}


const Class& ImplGenerator::referencedClass(const Property& property) const
{
	poco_assert (!property.referencedClass.empty());

	auto it = _classes.find(property.referencedClass);
	if (it != _classes.end())
		return it->second;
	else
		throw Poco::NotFoundException("referenced class"s, property.referencedClass);
}


} } } // namespace Poco::ActiveRecord::Compiler
