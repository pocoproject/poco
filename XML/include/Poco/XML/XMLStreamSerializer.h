//
// XMLStreamSerializer.h
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  XMLStreamSerializer
//
// Definition of the XMLStreamSerializer class.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2013-2014 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file


#ifndef POCO_XML_XMLSERIALIZER_Included
#define POCO_XML_XMLSERIALIZER_Included


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


class XML_API XMLStreamSerializer
	/// see: http://www.codesynthesis.com/projects/libstudxml/doc/intro.xhtml
{
public:
	XMLStreamSerializer(std::ostream&, const std::string& output_name, unsigned short indentation = 2);
		/// Serialize to std::ostream. Output name is used in diagnostics to
		/// identify the document being serialized. The indentation argument
		/// specifies the number of indentation spaces that should be used for
		/// pretty-printing. If 0 is passed, no pretty-printing is performed.
		///
		/// If stream exceptions are enabled then std::ios_base::failure
		/// exception is used to report io errors (badbit and failbit).
		/// Otherwise, those are reported as the XMLStreamSerializerException exception.

	~XMLStreamSerializer();

	const std::string& outputName() const;
	void startElement(const QName& qname);
	void startElement(const std::string& name);
	void startElement(const std::string& ns, const std::string& name);

	void endElement();

	// "Checked" end element. That is, it checks that the element
    // you think you are ending matches the current one.
    //
    void    endElement (const QName& qname);
    void    endElement (const std::string& name);
    void    endElement (const std::string& ns, const std::string& name);

	// Helpers for serializing elements with simple content. The first two
	// functions assume that startElement() has already been called. The
	// other two serialize the complete element, from start to end.

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
    // "Checked" end attribute. That is, it checks that the attribute
    // you think you are ending matches the current one.
    //
    void    endAttribute (const QName& qname);

    void    endAttribute (const std::string& name);

    void    endAttribute (const std::string& ns, const std::string& name);

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

	void namespaceDecl(const std::string& ns, const std::string& prefix);
	/// Namespaces declaration. If prefix is empty, then the default
	/// namespace is declared. If both prefix and namespace are empty,
	/// then the default namespace declaration is cleared (xmlns="").
    /// This function should be called after start_element().

	void xmlDecl(const std::string& version = "1.0", const std::string& encoding = "UTF-8", const std::string& standalone = "");
	/// XML declaration. If encoding or standalone are not specified,
	/// then these attributes are omitted from the output.

    // DOCTYPE declaration. If encoding or standalone are not specified,
    // then these attributes are omitted from the output.
    //
    void  doctypeDecl (const std::string& root_element,
                  const std::string& public_id = "",
                  const std::string& system_id = "",
                  const std::string& internal_subset = "");

	// Utility functions.
	//

	bool lookupNamespacePrefix(const std::string& ns, std::string& prefix) const;
	/// Return true if there is a mapping. In this case, prefix contains
	/// the mapped prefix.

    // Return the current element, that is, the latest element for which
    // start_element() but not endElement() have been called.
    //
	QName currentElement () const;

    // Return the current attribute, that is, the latest attribute for
    // which start_attribute() but not endAttribute() have been called.
    //
	QName currentAttribute () const;

    // Suspend/resume indentation.
    //
    // Indentation can be suspended only inside an element and, unless
    // explicitly resumed, it will remain suspended until the end of
    // that element. You should only explicitly resume indentation at
    // the element nesting level of suspension. If indentation is already
    // suspended at an outer nesting level, then subsequent calls to
    // suspend/resume are ignored. The indentation_suspended() function
    // can be used to check if indentation is currently suspended. If it
    // is not, then this function returns 0. Otherwise, it returns the
    // level at which pretty-printing was suspended, with root element
    // being level 1.
    //
    void    suspendIndentation ();

    void    resumeIndentation ();

    std::size_t    indentationSuspended () const;

private:
	XMLStreamSerializer(const XMLStreamSerializer&);
	XMLStreamSerializer& operator=(const XMLStreamSerializer&);

	void handleError(genxStatus) const;

	std::ostream& _outputStream;
	std::ostream::iostate _lastStreamState;// Original exception state.
	const std::string _outputName;

	genxWriter _writer;
	genxSender _sender;
	std::size_t _depth;
};

XMLStreamSerializer& operator<< (XMLStreamSerializer&, void (*func) (XMLStreamSerializer&));
	/// Stream-like interface for serializer. If the passed argument
	/// is a function with the void f(serializer&) signature or is a
	/// function object with the void operator() (serializer&) const
	/// operator, then this function (object) is called with the passed
	/// serializer. Otherwise, the argument is passed to the serializer's
	// characters() function.


template <typename T>
XMLStreamSerializer& operator<< (XMLStreamSerializer&, const T& value);

inline void XMLStreamSerializer::startElement(const QName& qname)
{
	startElement(qname.namespace_(), qname.name());
}

inline void XMLStreamSerializer::startElement(const std::string& name)
{
	startElement(std::string(), name);
}


inline void XMLStreamSerializer::endElement (const QName& qname)
{
  endElement (qname.namespace_ (), qname.name ());
}


inline void XMLStreamSerializer::endElement (const std::string& name)
{
  endElement (std::string (), name);
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


inline void XMLStreamSerializer::endAttribute (const QName& qname)
{
  endAttribute (qname.namespace_ (), qname.name ());
}


inline void XMLStreamSerializer::endAttribute (const std::string& name)
{
  endAttribute (std::string (), name);
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


// operator<<
//

inline XMLStreamSerializer& operator<< (XMLStreamSerializer& s, void (*func) (XMLStreamSerializer&))
{
  func (s);
  return s;
}


namespace details
{
  // Detect whether T defines void operator(A) const.
  //
  template <typename T, typename A>
  struct is_functor
  {
    typedef char no[1];
    typedef char yes[2];

    template <typename X, X> struct check;

    template <typename>
    static no& test (...);

    template <typename X>
    static yes& test (check<void (X::*) (A) const, &X::operator ()>*);

    static const bool value = sizeof (test<T> (0)) == sizeof (yes);
  };

  template <typename T, bool = is_functor<T, XMLStreamSerializer&>::value>
  struct inserter;

  template <typename T>
  struct inserter<T, true>
  {
    static void insert (XMLStreamSerializer& s, const T& f) {f (s);}
  };

  template <typename T>
  struct inserter<T, false>
  {
    static void insert (XMLStreamSerializer& s, const T& v) {s.characters (v);}
  };
}


template <typename T>
inline XMLStreamSerializer& operator<< (XMLStreamSerializer& s, const T& value)
{
  details::inserter<T>::insert (s, value);
  return s;
}


}
}


#endif // XML_SERIALIZER
