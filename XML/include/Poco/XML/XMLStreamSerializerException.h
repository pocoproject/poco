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

#ifndef POCO_XML_XMLSTREAMSERIALIZEREXCEPTION_H_
#define POCO_XML_XMLSTREAMSERIALIZEREXCEPTION_H_

#include <Poco/DOM/DOMException.h>

namespace Poco
{
namespace XML
{
class XMLStreamSerializer;

struct XML_API XMLStreamSerializerException:
	public Poco::XML::XMLException
{
	virtual ~XMLStreamSerializerException() throw ();

	XMLStreamSerializerException(const std::string& name, const std::string& description);
	XMLStreamSerializerException(const XMLStreamSerializer&, const std::string& description);

	const char* name() const throw ();
	const std::string& description() const;
	virtual const char* what() const throw ();

private:
	void init();

	std::string _name;
	std::string _description;
	std::string _what;
};

}
/* namespace XML */
} /* namespace Poco */
#endif /* POCO_XML_XMLSTREAMPARSEREXCEPTION_H_ */
