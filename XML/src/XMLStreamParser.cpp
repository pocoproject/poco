//
// XMLStreamParser.cpp
//
// $Id$
//
// Library: XML
// Package: XML
// Module:  XMLStreamParser
//
// Definition of the XMLStreamParser class.
//
// Copyright (c) 2004-2015, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
// copyright : Copyright (c) 2009-2013 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "Poco/XML/XMLStreamParser.h"
#include <new>     // std::bad_alloc
#include <cassert>
#include <cstring> // std::strchr
#include <istream>
#include <ostream>
#include <sstream>

using namespace std;

namespace Poco
{
namespace XML
{

struct stream_exception_controller
{
	~stream_exception_controller()
	{
		istream::iostate s = is_.rdstate();
		s &= ~istream::failbit;

		// If our error state (sans failbit) intersects with the
		// exception state then that means we have an active
		// exception and changing error/exception state will
		// cause another to be thrown.
		//
		if (!(old_state_ & s))
		{
			// Clear failbit if it was caused by eof.
			//
			if (is_.fail() && is_.eof())
				is_.clear(s);

			is_.exceptions(old_state_);
		}
	}

	stream_exception_controller(istream& is)
		: is_(is), old_state_(is_.exceptions())
	{
		is_.exceptions(old_state_ & ~istream::failbit);
	}

private:
	stream_exception_controller(const stream_exception_controller&);

	stream_exception_controller&
	operator=(const stream_exception_controller&);

private:
	istream& is_;
	istream::iostate old_state_;
};


static const char* parser_event_str[] =
{
	"start element",
	"end element",
	"start attribute",
	"end attribute",
	"characters",
	"start namespace declaration",
	"end namespace declaration",
	"end of file"
};


ostream& operator<<(ostream& os, XMLStreamParser::EventType e)
{
	return os << parser_event_str[e];
}


XMLStreamParser::XMLStreamParser(std::istream& is, const std::string& iname, FeatureType f)
	: size_(0), iname_(iname), feature_(f)
{
	data_.is = &is;
	init();
}


XMLStreamParser::XMLStreamParser(const void* data, std::size_t size, const std::string& iname, FeatureType f)
	: size_(size), iname_(iname), feature_(f)
{
	assert(data != 0 && size != 0);

	data_.buf = data;
	init();
}


XMLStreamParser::~XMLStreamParser()
{
	if (p_ != 0)
		XML_ParserFree(p_);
}


void XMLStreamParser::init()
{
	depth_ = 0;
	state_ = state_next;
	event_ = Eof;
	queue_ = Eof;

	pqname_ = &qname_;
	pvalue_ = &value_;

	line_ = 0;
	column_ = 0;

	attr_i_ = 0;
	start_ns_i_ = 0;
	end_ns_i_ = 0;

	if ((feature_ & RECEIVE_ATTRIBUTE_MAP) != 0 && (feature_ & RECEIVE_ATTRIBUTES_EVENT) != 0)
		feature_ &= ~RECEIVE_ATTRIBUTE_MAP;

	// Allocate the XMLStreamParser. Make sure nothing else can throw after
	// this call since otherwise we will leak it.
	//
	p_ = XML_ParserCreateNS(0, XML_Char(' '));

	if (p_ == 0)
		throw bad_alloc();

	// Get prefixes in addition to namespaces and local names.
	//
	XML_SetReturnNSTriplet(p_, true);

	// Set handlers.
	//
	XML_SetUserData(p_, this);

	if ((feature_ & RECEIVE_ELEMENTS) != 0)
	{
		XML_SetStartElementHandler(p_, &start_element_);
		XML_SetEndElementHandler(p_, &end_element_);
	}

	if ((feature_ & RECEIVE_CHARACTERS) != 0)
		XML_SetCharacterDataHandler(p_, &characters_);

	if ((feature_ & RECEIVE_NAMESPACE_DECLS) != 0)
		XML_SetNamespaceDeclHandler(p_, &start_namespace_decl_, &end_namespace_decl_);
}


void XMLStreamParser::handle_error()
{
	XML_Error e(XML_GetErrorCode(p_));

	if (e == XML_ERROR_ABORTED)
	{
		// For now we only abort the XMLStreamParser in the characters_() and
		// start_element_() handlers.
		//
		switch (content())
		{
		case Content::Empty:
			throw XMLStreamParserException(*this, "characters in empty content");
		case Content::Simple:
			throw XMLStreamParserException(*this, "element in simple content");
		case Content::Complex:
			throw XMLStreamParserException(*this, "characters in complex content");
		default:
			assert(false);
		}
	}
	else
		throw XMLStreamParserException(iname_, XML_GetCurrentLineNumber(p_), XML_GetCurrentColumnNumber(p_), XML_ErrorString(e));
}


XMLStreamParser::EventType XMLStreamParser::next()
{
	if (state_ == state_next)
		return next_(false);
	else
	{
		// If we previously peeked at start/end_element, then adjust
		// state accordingly.
		//
		switch (event_)
		{
		case EndElement:
		{
			if (!element_state_.empty() && element_state_.back().depth == depth_)
				popElement();

			depth_--;
			break;
		}
		case StartElement:
		{
			depth_++;
			break;
		}
		default:
			break;
		}

		state_ = state_next;
		return event_;
	}
}


const string& XMLStreamParser::attribute(const QName& qn) const
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
			return i->second.value;
		}
	}

	throw XMLStreamParserException(*this, "attribute '" + qn.string() + "' expected");
}


string XMLStreamParser::attribute(const QName& qn, const string& dv) const
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
			return i->second.value;
		}
	}

	return dv;
}


bool XMLStreamParser::attributePresent(const QName& qn) const
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
			return true;
		}
	}

	return false;
}


void XMLStreamParser::nextExpect(EventType e)
{
	if (next() != e)
		throw XMLStreamParserException(*this, string(parser_event_str[e]) + " expected");
}


void XMLStreamParser::nextExpect(EventType e, const string& ns, const string& n)
{
	if (next() != e || namespace_() != ns || name() != n)
		throw XMLStreamParserException(*this, string(parser_event_str[e]) + " '" + QName(ns, n).string() + "' expected");
}


string XMLStreamParser::element()
{
	content(Content::Simple);
	string r;

	// The content of the element can be empty in which case there
	// will be no characters event.
	//
	EventType e(next());
	if (e == Characters)
	{
		r.swap(value());
		e = next();
	}

	// We cannot really get anything other than end_element since
	// the simple content validation won't allow it.
	//
	assert(e == EndElement);

	return r;
}


string XMLStreamParser::element(const QName& qn, const string& dv)
{
	if (peek() == StartElement && getQName() == qn)
	{
		next();
		return element();
	}

	return dv;
}


const XMLStreamParser::ElementEntry* XMLStreamParser::get_element_() const
{
	// The start_element_() Expat handler may have already provisioned
	// an entry in the element stack. In this case, we need to get the
	// one before it, if any.
	//
	const ElementEntry* r(0);
	ElementState::size_type n(element_state_.size() - 1);

	if (element_state_[n].depth == depth_)
		r = &element_state_[n];
	else if (n != 0 && element_state_[n].depth > depth_)
	{
		n--;
		if (element_state_[n].depth == depth_)
			r = &element_state_[n];
	}

	return r;
}


void XMLStreamParser::popElement()
{
	// Make sure there are no unhandled attributes left.
	//
	const ElementEntry& e(element_state_.back());
	if (e.attr_unhandled_ != 0)
	{
		// Find the first unhandled attribute and report it.
		//
		for (AttributeMapType::const_iterator i(e.attr_map_.begin()); i != e.attr_map_.end(); ++i)
		{
			if (!i->second.handled)
				throw XMLStreamParserException(*this, "unexpected attribute '" + i->first.string() + "'");
		}
		assert(false);
	}

	element_state_.pop_back();
}


XMLStreamParser::EventType XMLStreamParser::next_(bool peek)
{
	EventType e(next_body());

	// Content-specific processing. Note that we handle characters in the
	// characters_() Expat handler for two reasons. Firstly, it is faster
	// to ignore the whitespaces at the source. Secondly, this allows us
	// to distinguish between element and attribute characters. We can
	// move this processing to the handler because the characters event
	// is never queued.
	//
	switch (e)
	{
	case EndElement:
	{
		// If this is a peek, then avoid popping the stack just yet.
		// This way, the attribute map will still be valid until we
		// call next().
		//
		if (!peek)
		{
			if (!element_state_.empty() && element_state_.back().depth == depth_)
				popElement();

			depth_--;
		}
		break;
	}
	case StartElement:
	{
		if (const ElementEntry* e = getElement())
		{
			switch (e->content)
			{
			case Content::Empty:
				throw XMLStreamParserException(*this, "element in empty content");
			case Content::Simple:
				throw XMLStreamParserException(*this, "element in simple content");
			default:
				break;
			}
		}

		// If this is a peek, then delay adjusting the depth.
		//
		if (!peek)
			depth_++;

		break;
	}
	default:
		break;
	}

	return e;
}


XMLStreamParser::EventType XMLStreamParser::next_body()
{
	// See if we have any start namespace declarations we need to return.
	//
	if (start_ns_i_ < start_ns_.size())
	{
		// Based on the previous event determine what's the next one must be.
		//
		switch (event_)
		{
		case StartNamespaceDecl:
		{
			if (++start_ns_i_ == start_ns_.size())
			{
				start_ns_i_ = 0;
				start_ns_.clear();
				pqname_ = &qname_;
				break; // No more declarations.
			}
			// Fall through.
		}
		case StartElement:
		{
			event_ = StartNamespaceDecl;
			pqname_ = &start_ns_[start_ns_i_];
			return event_;
		}
		default:
		{
			assert(false);
			return event_ = Eof;
		}
		}
	}

	// See if we have any attributes we need to return as events.
	//
	if (attr_i_ < attr_.size())
	{
		// Based on the previous event determine what's the next one must be.
		//
		switch (event_)
		{
		case StartAttribute:
		{
			event_ = Characters;
			pvalue_ = &attr_[attr_i_].value;
			return event_;
		}
		case Characters:
		{
			event_ = EndAttribute; // Name is already set.
			return event_;
		}
		case EndAttribute:
		{
			if (++attr_i_ == attr_.size())
			{
				attr_i_ = 0;
				attr_.clear();
				pqname_ = &qname_;
				pvalue_ = &value_;
				break; // No more attributes.
			}
			// Fall through.
		}
		case StartElement:
		case StartNamespaceDecl:
		{
			event_ = StartAttribute;
			pqname_ = &attr_[attr_i_].qname;
			return event_;
		}
		default:
		{
			assert(false);
			return event_ = Eof;
		}
		}
	}

	// See if we have any end namespace declarations we need to return.
	//
	if (end_ns_i_ < end_ns_.size())
	{
		// Based on the previous event determine what's the next one must be.
		//
		switch (event_)
		{
		case EndNamespaceDecl:
		{
			if (++end_ns_i_ == end_ns_.size())
			{
				end_ns_i_ = 0;
				end_ns_.clear();
				pqname_ = &qname_;
				break; // No more declarations.
			}
			// Fall through.
		}
			// The end namespace declaration comes before the end element
			// which means it can follow pretty much any other event.
			//
		default:
		{
			event_ = EndNamespaceDecl;
			pqname_ = &end_ns_[end_ns_i_];
			return event_;
		}
		}
	}

	// Check the queue.
	//
	if (queue_ != Eof)
	{
		event_ = queue_;
		queue_ = Eof;

		line_ = XML_GetCurrentLineNumber(p_);
		column_ = XML_GetCurrentColumnNumber(p_);

		return event_;
	}

	// Reset the character accumulation flag.
	//
	accumulate_ = false;

	XML_ParsingStatus ps;
	XML_GetParsingStatus(p_, &ps);

	switch (ps.parsing)
	{
	case XML_INITIALIZED:
	{
		// As if we finished the previous chunk.
		break;
	}
	case XML_PARSING:
	{
		assert(false);
		return event_ = Eof;
	}
	case XML_FINISHED:
	{
		return event_ = Eof;
	}
	case XML_SUSPENDED:
	{
		switch (XML_ResumeParser(p_))
		{
		case XML_STATUS_SUSPENDED:
		{
			// If the XMLStreamParser is again in the suspended state, then
			// that means we have the next event.
			//
			return event_;
		}
		case XML_STATUS_OK:
		{
			// Otherwise, we need to get and parse the next chunk of data
			// unless this was the last chunk, in which case this is eof.
			//
			if (ps.finalBuffer)
				return event_ = Eof;

			break;
		}
		case XML_STATUS_ERROR:
			handle_error();
		}
		break;
	}
	}

	// Get and parse the next chunk of data until we get the next event
	// or reach eof.
	//
	if (!accumulate_)
		event_ = Eof;

	XML_Status s;
	do
	{
		if (size_ != 0)
		{
			s = XML_Parse(p_, static_cast<const char*>(data_.buf), static_cast<int>(size_), true);

			if (s == XML_STATUS_ERROR)
				handle_error();

			break;
		}
		else
		{
			const size_t cap(4096);

			char* b(static_cast<char*>(XML_GetBuffer(p_, cap)));
			if (b == 0)
				throw bad_alloc();

			// Temporarily unset the exception failbit. Also clear the fail bit
			// when we reset the old state if it was caused by eof.
			//
			istream& is(*data_.is);
			{
				stream_exception_controller sec(is);
				is.read(b, static_cast<streamsize>(cap));
			}

			// If the caller hasn't configured the stream to use exceptions,
			// then use the parsing exception to report an error.
			//
			if (is.bad() || (is.fail() && !is.eof()))
				throw XMLStreamParserException(*this, "io failure");

			bool eof(is.eof());

			s = XML_ParseBuffer(p_, static_cast<int>(is.gcount()), eof);

			if (s == XML_STATUS_ERROR)
				handle_error();

			if (eof)
				break;
		}
	} while (s != XML_STATUS_SUSPENDED);

	return event_;
}


static void splitName(const XML_Char* s, QName& qn)
{
	string& ns(qn.namespace_());
	string& name(qn.name());
	string& prefix(qn.prefix());

	const char* p(strchr(s, ' '));

	if (p == 0)
	{
		ns.clear();
		name = s;
		prefix.clear();
	}
	else
	{
		ns.assign(s, 0, p - s);

		s = p + 1;
		p = strchr(s, ' ');

		if (p == 0)
		{
			name = s;
			prefix.clear();
		}
		else
		{
			name.assign(s, 0, p - s);
			prefix = p + 1;
		}
	}
}


void XMLCALL XMLStreamParser::start_element_(void* v, const XML_Char* name, const XML_Char** atts)
{
	XMLStreamParser& p(*static_cast<XMLStreamParser*>(v));

	XML_ParsingStatus ps;
	XML_GetParsingStatus(p.p_, &ps);

	// Expat has a (mis)-feature of a possibily calling handlers even
	// after the non-resumable XML_StopParser call.
	//
	if (ps.parsing == XML_FINISHED)
		return;

	// Cannot be a followup event.
	//
	assert(ps.parsing == XML_PARSING);

	// When accumulating characters in simple content, we expect to
	// see more characters or end element. Seeing start element is
	// possible but means violation of the content model.
	//
	if (p.accumulate_)
	{
		// It would have been easier to throw the exception directly,
		// however, the Expat code is most likely not exception safe.
		//
		p.line_ = XML_GetCurrentLineNumber(p.p_);
		p.column_ = XML_GetCurrentColumnNumber(p.p_);
		XML_StopParser(p.p_, false);
		return;
	}

	p.event_ = StartElement;
	splitName(name, p.qname_);

	p.line_ = XML_GetCurrentLineNumber(p.p_);
	p.column_ = XML_GetCurrentColumnNumber(p.p_);

	// Handle attributes.
	//
	if (*atts != 0)
	{
		bool am((p.feature_ & RECEIVE_ATTRIBUTE_MAP) != 0);
		bool ae((p.feature_ & RECEIVE_ATTRIBUTES_EVENT) != 0);

		// Provision an entry for this element.
		//
		ElementEntry* pe(0);
		if (am)
		{
			p.element_state_.push_back(ElementEntry(p.depth_ + 1));
			pe = &p.element_state_.back();
		}

		if (am || ae)
		{
			for (; *atts != 0; atts += 2)
			{
				if (am)
				{
					QName qn;
					splitName(*atts, qn);
					AttributeMapType::value_type v(qn, AttributeValueType());
					v.second.value = *(atts + 1);
					v.second.handled = false;
					pe->attr_map_.insert(v);
				}
				else
				{
					p.attr_.push_back(attribute_type());
					splitName(*atts, p.attr_.back().qname);
					p.attr_.back().value = *(atts + 1);
				}
			}

			if (am)
				pe->attr_unhandled_ = pe->attr_map_.size();
		}
	}

	XML_StopParser(p.p_, true);
}


void XMLCALL XMLStreamParser::end_element_(void* v, const XML_Char* name)
{
	XMLStreamParser& p(*static_cast<XMLStreamParser*>(v));

	XML_ParsingStatus ps;
	XML_GetParsingStatus(p.p_, &ps);

	// Expat has a (mis)-feature of a possibily calling handlers even
	// after the non-resumable XML_StopParser call.
	//
	if (ps.parsing == XML_FINISHED)
		return;

	// This can be a followup event for empty elements (<foo/>). In this
	// case the element name is already set.
	//
	if (ps.parsing != XML_PARSING)
		p.queue_ = EndElement;
	else
	{
		splitName(name, p.qname_);

		// If we are accumulating characters, then queue this event.
		//
		if (p.accumulate_)
			p.queue_ = EndElement;
		else
		{
			p.event_ = EndElement;

			p.line_ = XML_GetCurrentLineNumber(p.p_);
			p.column_ = XML_GetCurrentColumnNumber(p.p_);
		}

		XML_StopParser(p.p_, true);
	}
}


void XMLCALL XMLStreamParser::characters_(void* v, const XML_Char* s, int n)
{
	XMLStreamParser& p(*static_cast<XMLStreamParser*>(v));

	XML_ParsingStatus ps;
	XML_GetParsingStatus(p.p_, &ps);

	// Expat has a (mis)-feature of a possibily calling handlers even
	// after the non-resumable XML_StopParser call.
	//
	if (ps.parsing == XML_FINISHED)
		return;

	Content cont(p.content());

	// If this is empty or complex content, see if these are whitespaces.
	//
	switch (cont)
	{
	case Content::Empty:
	case Content::Complex:
	{
		for (int i(0); i != n; ++i)
		{
			char c(s[i]);
			if (c == 0x20 || c == 0x0A || c == 0x0D || c == 0x09)
				continue;

			// It would have been easier to throw the exception directly,
			// however, the Expat code is most likely not exception safe.
			//
			p.line_ = XML_GetCurrentLineNumber(p.p_);
			p.column_ = XML_GetCurrentColumnNumber(p.p_);
			XML_StopParser(p.p_, false);
			break;
		}
		return;
	}
	default:
		break;
	}

	// Append the characters if we are accumulating. This can also be a
	// followup event for another character event. In this case also
	// append the data.
	//
	if (p.accumulate_ || ps.parsing != XML_PARSING)
	{
		assert(p.event_ == Characters);
		p.value_.append(s, n);
	}
	else
	{
		p.event_ = Characters;
		p.value_.assign(s, n);

		p.line_ = XML_GetCurrentLineNumber(p.p_);
		p.column_ = XML_GetCurrentColumnNumber(p.p_);

		// In simple content we need to accumulate all the characters
		// into a single event. To do this we will let the XMLStreamParser run
		// until we reach the end of the element.
		//
		if (cont == Content::Simple)
			p.accumulate_ = true;
		else
			XML_StopParser(p.p_, true);
	}
}


void XMLCALL XMLStreamParser::start_namespace_decl_(void* v, const XML_Char* prefix, const XML_Char* ns)
{
	XMLStreamParser& p(*static_cast<XMLStreamParser*>(v));

	XML_ParsingStatus ps;
	XML_GetParsingStatus(p.p_, &ps);

	// Expat has a (mis)-feature of a possibily calling handlers even
	// after the non-resumable XML_StopParser call.
	//
	if (ps.parsing == XML_FINISHED)
		return;

	p.start_ns_.push_back(QName());
	p.start_ns_.back().prefix() = (prefix != 0 ? prefix : "");
	p.start_ns_.back().namespace_() = (ns != 0 ? ns : "");
}


void XMLCALL XMLStreamParser::end_namespace_decl_(void* v, const XML_Char* prefix)
{
	XMLStreamParser& p(*static_cast<XMLStreamParser*>(v));

	XML_ParsingStatus ps;
	XML_GetParsingStatus(p.p_, &ps);

	// Expat has a (mis)-feature of a possibily calling handlers even
	// after the non-resumable XML_StopParser call.
	//
	if (ps.parsing == XML_FINISHED)
		return;

	p.end_ns_.push_back(QName());
	p.end_ns_.back().prefix() = (prefix != 0 ? prefix : "");
}


}
}
