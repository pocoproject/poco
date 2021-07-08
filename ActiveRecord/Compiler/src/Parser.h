//
// Parser.h
//
// Copyright (c) 2020, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ActiveRecordCompiler_Parser_INCLUDED
#define ActiveRecordCompiler_Parser_INCLUDED


#include "Types.h"
#include "Poco/SAX/DefaultHandler.h"
#include <istream>


namespace Poco {
namespace ActiveRecord {
namespace Compiler {


class Parser: protected Poco::XML::DefaultHandler
	/// A parser for the XML ORM (project/class/property) class specification file.
{
public:
	Parser();
		/// Creates the Parser.

	ClassMap parse(const std::string& systemId, std::istream& stream);
		/// Parses the XML file.

protected:
	// ContentHandler
	void setDocumentLocator(const Poco::XML::Locator* pLocator);
	void startElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes);
	void endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname);
	void handleProject(const Poco::XML::Attributes& attributes);
	void handleClass(const Poco::XML::Attributes& attributes);
	void handleProperty(const Poco::XML::Attributes& attributes);
	std::string where() const;
	std::string parseType(const std::string& type) const;
	char parseCardinality(const std::string& cardinality) const;
	bool parseBool(const std::string& name, const std::string& value, bool deflt = false) const;
	std::string convertCamelCase(const std::string& name);
	std::string toDatabaseName(const std::string& name);

private:
	const Poco::XML::Locator* _pLocator = nullptr;
	bool _convertCamelCase = false;
	std::string _nameSpace;
	Class _class;
	ClassMap _classes;
	std::vector<std::string> _elemStack;
};


} } } // namespace Poco::ActiveRecord::Compiler


#endif // ActiveRecordCompiler_Parser_INCLUDED
