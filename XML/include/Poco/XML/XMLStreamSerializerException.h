//
// XMLStreamSerializerException.h
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  XMLStreamSerializerException
//
// Definition of the XMLStreamSerializerException class.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2013-2014 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file


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
	XMLStreamSerializerException(const std::string& name, const std::string& description);
	XMLStreamSerializerException(const XMLStreamSerializer&, const std::string& description);
	virtual ~XMLStreamSerializerException() throw ();

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
