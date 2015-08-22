///
/// \package metamodel
/// \file XMLStreamException.h
///
/// \author Marian Krivos <marian.krivos@rsys.sk>
/// \date Aug 21, 2015 - 6:52:24 PM
/// \brief definicia typu
///
/// (C) Copyright 2015 R-SYS,s.r.o
/// All rights reserved.
///

#ifndef POCO_XML_XMLSTREAMPARSEREXCEPTION_H_
#define POCO_XML_XMLSTREAMPARSEREXCEPTION_H_

#include <Poco/DOM/DOMException.h>

namespace Poco
{
namespace XML
{
class XMLStreamParser;

class XML_API XMLStreamParserException :
public Poco::XML::XMLException
{
public:
	XMLStreamParserException(const std::string& name, Poco::UInt64 line, Poco::UInt64 column, const std::string& description);

	XMLStreamParserException(const XMLStreamParser&, const std::string& description);

	virtual ~XMLStreamParserException() throw ();

	const char* name() const throw();
	Poco::UInt64 line() const;
	Poco::UInt64 column() const;
	const std::string& description() const;
	virtual const char* what() const throw ();

private:
	void init();

	std::string _name;
	Poco::UInt64 _line;
	Poco::UInt64 _column;
	std::string _description;
	std::string _what;
};

}
/* namespace XML */
} /* namespace Poco */
#endif /* POCO_XML_XMLSTREAMPARSEREXCEPTION_H_ */
