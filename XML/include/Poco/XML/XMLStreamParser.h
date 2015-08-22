// file      : XMLStreamParser.hxx
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef POCO_XML_PARSER_HXX
#define POCO_XML_PARSER_HXX

// We only support UTF-8 expat.
//
#ifdef XML_UNICODE
#  error UTF-16 expat (XML_UNICODE defined) is not supported
#endif

#include "Poco/XML/QName.h"
#include "Poco/XML/ValueTraits.h"
#include "Poco/XML/Content.h"
#include <Poco/XML/expat.h>

#include <map>
#include <vector>
#include <string>
#include <iosfwd>
#include <cstddef> // std::size_t
#include <cassert>


namespace Poco
{
namespace XML
{
class XML_API XMLStreamParser
{
public:
	typedef unsigned short FeatureType;

	// If both receive_attributes_event and receive_attributes_map are
	// specified, then receive_attributes_event is assumed.
	//
	static const FeatureType RECEIVE_ELEMENTS = 0x0001;
	static const FeatureType RECEIVE_CHARACTERS = 0x0002;
	static const FeatureType RECEIVE_ATTRIBUTE_MAP = 0x0004;
	static const FeatureType RECEIVE_ATTRIBUTES_EVENT = 0x0008;
	static const FeatureType RECEIVE_NAMESPACE_DECLS = 0x0010;

	static const FeatureType RECEIVE_DEFAULT = RECEIVE_ELEMENTS | RECEIVE_CHARACTERS | RECEIVE_ATTRIBUTE_MAP;

	// Parse std::istream. Input name is used in diagnostics to identify
	// the document being parsed.
	//
	// If stream exceptions are enabled then std::ios_base::failure
	// exception is used to report io errors (badbit and failbit).
	// Otherwise, those are reported as the parsing exception.
	//
	XMLStreamParser(std::istream&, const std::string& input_name, FeatureType = RECEIVE_DEFAULT);

	// Parse memory buffer that contains the whole document. Input name
	// is used in diagnostics to identify the document being parsed.
	//
	XMLStreamParser(const void* data, std::size_t size, const std::string& input_name, FeatureType = RECEIVE_DEFAULT);

	const std::string& input_name() const
	{
		return iname_;
	}

	~XMLStreamParser();

	/// Parsing events.
	enum EventType
	{
		// If adding new events, also update the stream insertion operator.
		//
		StartElement,
		EndElement,
		StartAttribute,
		EndAttribute,
		Characters,
		StartNamespaceDecl,
		EndNamespaceDecl,
		Eof
	};

	EventType next();

	// Get the next event and make sure that it's what's expected. If it
	// is not, then throw an appropriate parsing exception.
	//
	void nextExpect(EventType);

	void nextExpect(EventType, const std::string& name);

	void nextExpect(EventType, const QName& qname);

	void nextExpect(EventType, const std::string& ns, const std::string& name);

	EventType peek();

	// Return the even that was last returned by the call to next() or
	// peek().
	//
	EventType event()
	{
		return event_;
	}

	// Event data.
	//
	const QName& qname() const
	{
		return *pqname_;
	}

	const std::string& namespace_() const
	{
		return pqname_->namespace_();
	}

	const std::string& name() const
	{
		return pqname_->name();
	}

	const std::string& prefix() const
	{
		return pqname_->prefix();
	}

	std::string& value()
	{
		return *pvalue_;
	}

	const std::string& value() const
	{
		return *pvalue_;
	}

	template<typename T> T value() const;

	Poco::UInt64 line() const
	{
		return line_;
	}

	Poco::UInt64 column() const
	{
		return column_;
	}

	// Attribute map lookup. If attribute is not found, then the version
	// without the default value throws an appropriate parsing exception
	// while the version with the default value returns that value.
	//
	// Note also that there is no attribute(ns,name) version since it
	// would conflict with attribute(name,dv) (qualified attributes
	// are not very common).
	//
	// Attribute map is valid throughout at the "element level" until
	// end_element and not just during startElement. As a special case,
	// the map is still valid after peek() that returned end_element until
	// this end_element event is retrieved with next().
	//
	const std::string&
	attribute(const std::string& name) const;

	template<typename T>
	T attribute(const std::string& name) const;

	std::string attribute(const std::string& name, const std::string& default_value) const;

	template<typename T>
	T attribute(const std::string& name, const T& default_value) const;

	const std::string& attribute(const QName& qname) const;

	template<typename T>
	T attribute(const QName& qname) const;

	std::string attribute(const QName& qname, const std::string& default_value) const;

	template<typename T>
	T attribute(const QName& qname, const T& default_value) const;

	bool attributePresent(const std::string& name) const;

	bool attributePresent(const QName& qname) const;

	// Low-level attribute map access. Note that this API assumes
	// all attributes are handled.
	//
	struct AttributeValueType
	{
		std::string value;
		mutable bool handled;
	};

	typedef std::map<QName, AttributeValueType> AttributeMapType;

	const AttributeMapType& attributeMap() const;

	// Optional content processing.
	//

	// Note that you cannot get/set content while peeking.
	//
	void content(Content);

	Content content() const;

	// Versions that also set the content. Event type must be startElement.
	//
	void nextExpect(EventType, const std::string& name, Content);

	void nextExpect(EventType, const QName& qname, Content);

	void nextExpect(EventType, const std::string& ns, const std::string& name, Content);

	// Helpers for parsing elements with simple content. The first two
	// functions assume that startElement has already been parsed. The
	// rest parse the complete element, from start to end.
	//
	// Note also that as with attribute(), there is no (namespace,name)
	// overload since it would conflicts with (namespace,default_value).
	//
	std::string element();

	template<typename T>
	T element();

	std::string element(const std::string& name);

	std::string element(const QName& qname);

	template<typename T>
	T element(const std::string& name);

	template<typename T>
	T element(const QName& qname);

	std::string element(const std::string& name, const std::string& default_value);

	std::string element(const QName& qname, const std::string& default_value);

	template<typename T>
	T element(const std::string& name, const T& default_value);

	template<typename T>
	T element(const QName& qname, const T& default_value);

	// C++11 range-based for support. Generally, the iterator interface
	// doesn't make much sense for the XMLStreamParser so for now we have an
	// implementation that is just enough to the range-based for.
	//
	struct Iterator
	{
		typedef EventType value_type;

		Iterator(XMLStreamParser* p = 0, EventType e = Eof) :
		p_(p),
		e_(e)
		{
		}
		value_type operator*() const
		{
			return e_;
		}
		Iterator& operator++()
		{
			e_ = p_->next();
			return *this;
		}

		// Comparison only makes sense when comparing to end (eof).
		//
		bool operator==(Iterator y) const
		{
			return e_ == Eof && y.e_ == Eof;
		}
		bool operator!=(Iterator y) const
		{
			return !(*this == y);
		}

	private:
		XMLStreamParser* p_;
		EventType e_;
	};

	Iterator begin()
	{
		return Iterator(this, next());
	}
	Iterator end()
	{
		return Iterator(this, Eof);
	}

private:
	XMLStreamParser(const XMLStreamParser&);
	XMLStreamParser& operator=(const XMLStreamParser&);

	static void XMLCALL start_element_(void*, const XML_Char*, const XML_Char**);
	static void XMLCALL end_element_(void*, const XML_Char*);
	static void XMLCALL characters_(void*, const XML_Char*, int);
	static void XMLCALL start_namespace_decl_(void*, const XML_Char*, const XML_Char*);
	static void XMLCALL end_namespace_decl_(void*, const XML_Char*);

	void init();
	EventType next_(bool peek);
	EventType next_body();
	void handle_error();

	// If size_ is 0, then data is std::istream. Otherwise, it is a buffer.
	//
	union
	{
		std::istream* is;
		const void* buf;
	}data_;

	std::size_t size_;

	const std::string iname_;
	FeatureType feature_;

	XML_Parser p_;
	std::size_t depth_;
	bool accumulate_; // Whether we are accumulating character content.
	enum
	{
		state_next, state_peek
	}state_;
	EventType event_;
	EventType queue_;

	QName qname_;
	std::string value_;

	// These are used to avoid copying when we are handling attributes
	// and namespace decls.
	//
	const QName* pqname_;
	std::string* pvalue_;

	Poco::UInt64 line_;
	Poco::UInt64 column_;

	// Attributes as events.
	//
	struct attribute_type
	{
		QName qname;
		std::string value;
	};

	typedef std::vector<attribute_type> attributes;

	attributes attr_;
	attributes::size_type attr_i_; // Index of the current attribute.

	// Namespace declarations.
	//
	typedef std::vector<QName> namespace_decls;

	namespace_decls start_ns_;
	namespace_decls::size_type start_ns_i_;// Index of the current decl.

	namespace_decls end_ns_;
	namespace_decls::size_type end_ns_i_;// Index of the current decl.

	// Element state consisting of the content model and attribute map.
	//
	struct ElementEntry
	{
		ElementEntry(std::size_t d, Content c = Content::Mixed) :
		depth(d),
		content(c),
		attr_unhandled_(0)
		{
		}

		std::size_t depth;
		Content content;
		AttributeMapType attr_map_;
		mutable AttributeMapType::size_type attr_unhandled_;
	};

	typedef std::vector<ElementEntry> ElementState;
	std::vector<ElementEntry> element_state_;

	// Empty attribute map to return when an element has no attributes.
	//
	const AttributeMapType empty_attr_map_;

	// Return the element entry corresponding to the current depth, if
	// exists, and NULL otherwise.
	//
	const ElementEntry* getElement() const;

	const ElementEntry* get_element_() const;

	void pop_element();
};

XML_API std::ostream& operator<<(std::ostream&, XMLStreamParser::EventType);

inline XMLStreamParser::XMLStreamParser(std::istream& is, const std::string& iname, FeatureType f)
	: size_(0), iname_(iname), feature_(f)
{
	data_.is = &is;
	init();
}

inline XMLStreamParser::XMLStreamParser(const void* data, std::size_t size, const std::string& iname, FeatureType f)
	: size_(size), iname_(iname), feature_(f)
{
	assert(data != 0 && size != 0);

	data_.buf = data;
	init();
}

inline XMLStreamParser::EventType XMLStreamParser::peek()
{
	if (state_ == state_peek)
		return event_;
	else
	{
		EventType e(next_(true));
		state_ = state_peek; // Set it after the call to next_().
		return e;
	}
}

template<typename T>
inline T XMLStreamParser::value() const
{
	return ValueTraits < T > ::parse(value(), *this);
}

inline const std::string& XMLStreamParser::attribute(const std::string& n) const
{
	return attribute(QName(n));
}

template<typename T>
inline T XMLStreamParser::attribute(const std::string& n) const
{
	return attribute < T > (QName(n));
}

inline std::string XMLStreamParser::attribute(const std::string& n, const std::string& dv) const
{
	return attribute(QName(n), dv);
}

template<typename T>
inline T XMLStreamParser::attribute(const std::string& n, const T& dv) const
{
	return attribute < T > (QName(n), dv);
}

template<typename T>
inline T XMLStreamParser::attribute(const QName& qn) const
{
	return ValueTraits < T > ::parse(attribute(qn), *this);
}

inline bool XMLStreamParser::attributePresent(const std::string& n) const
{
	return attributePresent(QName(n));
}

inline const XMLStreamParser::AttributeMapType& XMLStreamParser::attributeMap() const
{
	if (const ElementEntry* e = getElement())
	{
		e->attr_unhandled_ = 0; // Assume all handled.
		return e->attr_map_;
	}

	return empty_attr_map_;
}

inline void XMLStreamParser::nextExpect(EventType e, const QName& qn)
{
	nextExpect(e, qn.namespace_(), qn.name());
}

inline void XMLStreamParser::nextExpect(EventType e, const std::string& n)
{
	nextExpect(e, std::string(), n);
}

inline void XMLStreamParser::nextExpect(EventType e, const QName& qn, Content c)
{
	nextExpect(e, qn);
	assert(e == StartElement);
	content(c);
}

inline void XMLStreamParser::nextExpect(EventType e, const std::string& n, Content c)
{
	nextExpect(e, std::string(), n);
	assert(e == StartElement);
	content(c);
}

inline void XMLStreamParser::nextExpect(EventType e, const std::string& ns, const std::string& n, Content c)
{
	nextExpect(e, ns, n);
	assert(e == StartElement);
	content(c);
}

template<typename T>
inline T XMLStreamParser::element()
{
	return ValueTraits < T > ::parse(element(), *this);
}

inline std::string XMLStreamParser::element(const std::string& n)
{
	nextExpect(StartElement, n);
	return element();
}

inline std::string XMLStreamParser::element(const QName& qn)
{
	nextExpect(StartElement, qn);
	return element();
}

template<typename T>
inline T XMLStreamParser::element(const std::string& n)
{
	return ValueTraits < T > ::parse(element(n), *this);
}

template<typename T>
inline T XMLStreamParser::element(const QName& qn)
{
	return ValueTraits < T > ::parse(element(qn), *this);
}

inline std::string XMLStreamParser::element(const std::string& n, const std::string& dv)
{
	return element(QName(n), dv);
}

template<typename T>
inline T XMLStreamParser::element(const std::string& n, const T& dv)
{
	return element < T > (QName(n), dv);
}

inline void XMLStreamParser::content(Content c)
{
	assert(state_ == state_next);

	if (!element_state_.empty() && element_state_.back().depth == depth_)
		element_state_.back().content = c;
	else
		element_state_.push_back(ElementEntry(depth_, c));
}

inline Content XMLStreamParser::content() const
{
	assert(state_ == state_next);

	return !element_state_.empty() && element_state_.back().depth == depth_ ? element_state_.back().content : Content(Content::Mixed);
}

inline const XMLStreamParser::ElementEntry* XMLStreamParser::getElement() const
{
	return element_state_.empty() ? 0 : get_element_();
}

template<typename T>
T XMLStreamParser::attribute(const QName& qn, const T& dv) const
{
	if (const ElementEntry* e = getElement())
	{
		AttributeMapType::const_iterator i(e->attr_map_.find(qn));

		if (i != e->attr_map_.end())
		{
			if (!i->second.handled)
			{
				i->second.handled = true;
				e->attr_unhandled_--;
			}
			return ValueTraits < T > ::parse(i->second.value, *this);
		}
	}

	return dv;
}

template<typename T>
T XMLStreamParser::element(const QName& qn, const T& dv)
{
	if (peek() == StartElement && qname() == qn)
	{
		next();
		return element<T>();
	}

	return dv;
}
}
}

#endif // CUTL_XML_PARSER_HXX
