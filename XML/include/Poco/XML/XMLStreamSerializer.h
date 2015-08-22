// file      : xml/XMLStreamSerializer -*- C++ -*-
// copyright : Copyright (c) 2013-2014 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef POCO_XML_XMLSERIALIZER
#define POCO_XML_XMLSERIALIZER

#include "QName.h"
#include "ValueTraits.h"
#include "genx.h"

#include <string>
#include <ostream>
#include <cstddef> // std::size_t

namespace Poco
{
namespace XML
{
class XMLStreamSerializer;

class XML_API XMLStreamSerializer
{
public:
	// Serialize to std::ostream. Output name is used in diagnostics to
	// identify the document being serialized. The indentation argument
	// specifies the number of indentation spaces that should be used for
	// pretty-printing. If 0 is passed, no pretty-printing is performed.
	//
	// If stream exceptions are enabled then std::ios_base::failure
	// exception is used to report io errors (badbit and failbit).
	// Otherwise, those are reported as the XMLStreamSerializerException exception.
	//
	XMLStreamSerializer(std::ostream&, const std::string& output_name, unsigned short indentation = 2);

	const std::string& outputName() const
	{
		return oname_;
	}

	~XMLStreamSerializer();

	void startElement(const QName& qname);

	void startElement(const std::string& name);

	void startElement(const std::string& ns, const std::string& name);

	void endElement();

	// Helpers for serializing elements with simple content. The first two
	// functions assume that startElement() has already been called. The
	// other two serialize the complete element, from start to end.
	//
	void element(const std::string& value);

	template<typename T>
	void element(const T& value);

	void element(const std::string& name, const std::string& value);

	template<typename T>
	void element(const std::string& name, const T& value);

	void element(const QName& qname, const std::string& value);

	template<typename T>
	void element(const QName& qname, const T& value);

	void element(const std::string& namespace_, const std::string& name, const std::string& value);

	template<typename T>
	void element(const std::string& namespace_, const std::string& name, const T& value);

	// Attributes.
	//
	void startAttribute(const QName& qname);

	void startAttribute(const std::string& name);

	void startAttribute(const std::string& ns, const std::string& name);

	void endAttribute();

	void attribute(const QName& qname, const std::string& value);

	template<typename T>
	void attribute(const QName& qname, const T& value);

	void attribute(const std::string& name, const std::string& value);

	template<typename T>
	void attribute(const std::string& name, const T& value);

	void attribute(const std::string& ns, const std::string& name, const std::string& value);

	template<typename T>
	void attribute(const std::string& ns, const std::string& name, const T& value);

	// Characters.
	//
	void characters(const std::string& value);

	template<typename T>
	void characters(const T& value);

	// Namespaces declaration. If prefix is empty, then the default
	// namespace is declared. If both prefix and namespace are empty,
	// then the default namespace declaration is cleared (xmlns="").
	//
	void namespaceDecl(const std::string& ns, const std::string& prefix);

	// XML declaration. If encoding or standalone are not specified,
	// then these attributes are omitted from the output.
	//
	void xmlDecl(const std::string& version = "1.0", const std::string& encoding = "UTF-8", const std::string& standalone = "");

	// Utility functions.
	//
	// Return true if there is a mapping. In this case, prefix contains
	// the mapped prefix.
	//
	bool lookupNamespacePrefix(const std::string& ns, std::string& prefix);

private:
	XMLStreamSerializer(const XMLStreamSerializer&);
	XMLStreamSerializer& operator=(const XMLStreamSerializer&);

	void handleError(genxStatus);

	std::ostream& os_;
	std::ostream::iostate os_state_;// Original exception state.
	const std::string oname_;

	genxWriter s_;
	genxSender sender_;
	std::size_t depth_;
};

inline void XMLStreamSerializer::startElement(const QName& qname)
{
	startElement(qname.namespace_(), qname.name());
}

inline void XMLStreamSerializer::startElement(const std::string& name)
{
	startElement(std::string(), name);
}

inline void XMLStreamSerializer::element(const std::string& v)
{
	if (!v.empty())
		characters(v);

	endElement();
}

template<typename T>
inline void XMLStreamSerializer::element(const T& v)
{
	element(ValueTraits < T > ::serialize(v, *this));
}

inline void XMLStreamSerializer::element(const std::string& n, const std::string& v)
{
	element(std::string(), n, v);
}

template<typename T>
inline void XMLStreamSerializer::element(const std::string& n, const T& v)
{
	element(n, ValueTraits < T > ::serialize(v, *this));
}

inline void XMLStreamSerializer::element(const QName& qn, const std::string& v)
{
	element(qn.namespace_(), qn.name(), v);
}

template<typename T>
inline void XMLStreamSerializer::element(const QName& qn, const T& v)
{
	element(qn, ValueTraits < T > ::serialize(v, *this));
}

template<typename T>
inline void XMLStreamSerializer::element(const std::string& ns, const std::string& n, const T& v)
{
	element(ns, n, ValueTraits < T > ::serialize(v, *this));
}

inline void XMLStreamSerializer::startAttribute(const QName& qname)
{
	startAttribute(qname.namespace_(), qname.name());
}

inline void XMLStreamSerializer::startAttribute(const std::string& name)
{
	startAttribute(std::string(), name);
}

inline void XMLStreamSerializer::attribute(const QName& qname, const std::string& value)
{
	attribute(qname.namespace_(), qname.name(), value);
}

template<typename T>
inline void XMLStreamSerializer::attribute(const QName& qname, const T& value)
{
	attribute(qname, ValueTraits < T > ::serialize(value, *this));
}

inline void XMLStreamSerializer::attribute(const std::string& name, const std::string& value)
{
	attribute(std::string(), name, value);
}

template<typename T>
inline void XMLStreamSerializer::attribute(const std::string& name, const T& value)
{
	attribute(name, ValueTraits < T > ::serialize(value, *this));
}

template<typename T>
inline void XMLStreamSerializer::attribute(const std::string& ns, const std::string& name, const T& value)
{
	attribute(ns, name, ValueTraits < T > ::serialize(value, *this));
}

template<typename T>
inline void XMLStreamSerializer::characters(const T& value)
{
	characters(ValueTraits < T > ::serialize(value, *this));
}

}
}

#endif // XML_SERIALIZER
