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

	const char* name() const throw()
	{
		return name_.c_str();
	}

	Poco::UInt64 line() const
	{
		return line_;
	}

	Poco::UInt64 column() const
	{
		return column_;
	}

	const std::string& description() const
	{
		return description_;
	}

	virtual const char* what() const throw ();

private:
	void init();

	std::string name_;
	Poco::UInt64 line_;
	Poco::UInt64 column_;
	std::string description_;
	std::string what_;
};

}
/* namespace XML */
} /* namespace Poco */
#endif /* POCO_XML_XMLSTREAMPARSEREXCEPTION_H_ */
