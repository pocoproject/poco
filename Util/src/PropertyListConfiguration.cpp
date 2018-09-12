//
// PropertyListConfiguration.cpp
//
//
// Library: Util
// Package: Configuration
// Module:  PropertyListConfiguration
//
// Copyright (c) 2017-2018, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/PropertyListConfiguration.h"


#ifndef POCO_UTIL_NO_XMLCONFIGURATION

#include "Poco/DOM/DOMImplementation.h"
#include "Poco/DOM/DocumentType.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Text.h"
#include "Poco/SAX/XMLReader.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/NumberFormatter.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/Base64Encoder.h"
#include "Poco/Base64Decoder.h"
#include <fstream>
#include <sstream>

namespace Poco {
namespace Util {


PropertyListConfiguration::PropertyListConfiguration()
{
	Poco::AutoPtr<Poco::XML::DocumentType> doctype = Poco::XML::DOMImplementation::instance().createDocumentType("plist", "-//Apple//DTD PLIST 1.0//EN", "http://www.apple.com/DTDs/PropertyList-1.0.dtd");
	_pDocument = new Poco::XML::Document(doctype);
	_pRoot = _pDocument->createElement("plist");
	_pRoot->setAttribute("version", "1.0");
	_pDocument->appendChild(_pRoot);
	Poco::AutoPtr<Poco::XML::Element> pElem = _pDocument->createElement("dict");
	_pRoot->appendChild(pElem);
}

PropertyListConfiguration::PropertyListConfiguration(const std::string& path)
{
	load(path);
}


PropertyListConfiguration::PropertyListConfiguration(std::istream& istr)
{
	load(istr);
}

PropertyListConfiguration::~PropertyListConfiguration()
{
}


void PropertyListConfiguration::load(const std::string& path)
{
	Poco::XML::InputSource src(path);

	Poco::XML::DOMParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
	parser.setFeature(Poco::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);

	_pDocument = parser.parse(&src);
	_pRoot = Poco::XML::AutoPtr<Poco::XML::Element>(_pDocument->documentElement(), true);
}

void PropertyListConfiguration::load(std::istream& istr)
{
	Poco::XML::InputSource src(istr);

	Poco::XML::DOMParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
	parser.setFeature(Poco::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);

	_pDocument = parser.parse(&src);
	_pRoot = Poco::XML::AutoPtr<Poco::XML::Element>(_pDocument->documentElement(), true);

}

void PropertyListConfiguration::save(const std::string& path) const
{
	std::ofstream out(path);
	save(out);
}

void PropertyListConfiguration::save(std::ostream& ostr) const
{
	Poco::XML::DOMWriter writer;
	writer.setNewLine("\n");
	writer.setOptions(Poco::XML::XMLWriter::WRITE_XML_DECLARATION | Poco::XML::XMLWriter::PRETTY_PRINT);
	writer.writeNode(ostr, _pDocument);
}

void PropertyListConfiguration::setInt(const std::string& key, int value)
{
	// set this, so it fires event
	AbstractConfiguration::setInt(key, value);

	// find the node we just inserted
	Poco::XML::Node* thekey = findNode(key, true);
	Poco::XML::Node* thevalue = thekey->nextSibling();

	// change the value to an integer element
	Poco::AutoPtr<Poco::XML::Node> pElem = _pDocument->createElement("integer");
	thevalue->parentNode()->replaceChild(pElem, thevalue);
	thevalue = pElem;
	Poco::AutoPtr<Poco::XML::Node> pText = _pDocument->createTextNode(Poco::NumberFormatter::format(value));
	thevalue->appendChild(pText);
}

void PropertyListConfiguration::setDouble(const std::string& key, double value)
{
	// set this, so it fires event
	AbstractConfiguration::setDouble(key, value);

	// find the node we just inserted
	Poco::XML::Node* thekey = findNode(key, true);
	Poco::XML::Node* thevalue = thekey->nextSibling();

	// change the value to an integer element
	Poco::AutoPtr<Poco::XML::Node> pElem = _pDocument->createElement("real");
	thevalue->parentNode()->replaceChild(pElem, thevalue);
	thevalue = pElem;
	Poco::AutoPtr<Poco::XML::Node> pText = _pDocument->createTextNode(Poco::NumberFormatter::format(value));
	thevalue->appendChild(pText);
}

void PropertyListConfiguration::setBool(const std::string& key, bool value)
{
	// set this, so it fires event
	AbstractConfiguration::setBool(key, value);

	// find the node we just inserted
	Poco::XML::Node* thekey = findNode(key, true);
	Poco::XML::Node* thevalue = thekey->nextSibling();

	// change the value to a boolean element
	Poco::AutoPtr<Poco::XML::Node> pElem = _pDocument->createElement(value ? "true" : "false");
	thevalue->parentNode()->replaceChild(pElem, thevalue);
	thevalue = pElem;
}

void PropertyListConfiguration::setData(const std::string& key, std::istream &istr)
{
	Poco::XML::Node* thekey = findNode(key, true);
	Poco::XML::Node* thevalue = thekey->nextSibling();

	Poco::AutoPtr<Poco::XML::Node> pElem = _pDocument->createElement("data");
	thevalue->parentNode()->replaceChild(pElem, thevalue);
	thevalue = pElem;

	std::ostringstream ostr;
	Poco::Base64Encoder encoder(ostr);
	encoder << istr.rdbuf();
	encoder.close();
	Poco::AutoPtr<Poco::XML::Node> pText = _pDocument->createTextNode(ostr.str());
	thevalue->appendChild(pText);
}

bool PropertyListConfiguration::getData(const std::string& key, std::ostream &ostr)
{
	const Poco::XML::Node* thekey = findNode(key);

	if (thekey)
	{
		Poco::XML::Node* thevalue = thekey->nextSibling();
		if (thevalue->firstChild())
		{
			std::stringstream strs(thevalue->firstChild()->nodeValue());
			Poco::Base64Decoder decoder(strs);
			ostr << decoder.rdbuf();
		}
		else
			return false;
		return true;
	}

	return false;
}

bool PropertyListConfiguration::getRaw(const std::string& key, std::string& value) const
{
	const Poco::XML::Node* thekey = findNode(key);

	if (thekey)
	{
		Poco::XML::Node* thevalue = thekey->nextSibling();
		if (thevalue->firstChild())
			value = thevalue->firstChild()->nodeValue();
		else
			value = thevalue->nodeName();	// the case for true/false
		return true;
	}

	return false;
}


void PropertyListConfiguration::setRaw(const std::string& key, const std::string& value)
{
	Poco::XML::Node* thekey = findNode(key, true);
	Poco::XML::Node* thevalue = thekey->nextSibling();

	Poco::AutoPtr<Poco::XML::Node> pElem = _pDocument->createElement("string");
	thevalue->parentNode()->replaceChild(pElem, thevalue);
	thevalue = pElem;
	Poco::AutoPtr<Poco::XML::Node> pText = _pDocument->createTextNode(value);
	thevalue->appendChild(pText);
}

void PropertyListConfiguration::enumerate(const std::string& key, Keys& range) const
{
	const Poco::XML::Node* thekey = _pRoot->firstChild();

	if (key.length() != 0)
	{
		thekey = findNode(key);
		if (!thekey)
			return;

		thekey = thekey->nextSibling();
	}

	if (thekey && thekey->nodeName() == "dict")
	{
		Poco::XML::Node* pChild = thekey->firstChild();
		while (pChild != NULL)
		{
			if (pChild->nodeName() == "key")
			{
				range.push_back(pChild->firstChild()->nodeValue());
			}
			pChild = pChild->nextSibling();
		}
	}
}

void PropertyListConfiguration::removeRaw(const std::string& key)
{
	Poco::XML::Node* thekey = findNode(key);

	if (thekey)
	{
		Poco::XML::Node* thevalue = thekey->nextSibling();
		thekey->parentNode()->removeChild(thekey);
		thevalue->parentNode()->removeChild(thevalue);
	}
}

Poco::XML::Node* PropertyListConfiguration::findNode(const std::string& key, bool create)
{
	Poco::XML::Node* dict = const_cast<Poco::XML::Node*>(_pRoot->firstChild());
	return findNode(key, dict, create);
}

const Poco::XML::Node* PropertyListConfiguration::findNode(const std::string& key, bool create) const
{
	Poco::XML::Node* dict = const_cast<Poco::XML::Node*>(_pRoot->firstChild());
	return findNode(key, dict, create);
}

Poco::XML::Node* PropertyListConfiguration::findNode(const std::string& key, Poco::XML::Node* dict, bool create)
{
	if (!dict) return NULL;
	if (dict->nodeName() != "dict") return NULL;

	std::string firstkey = key;
	std::string::size_type dot = key.find('.');
	if (dot != std::string::npos)
	{
		firstkey = key.substr(0, dot);
	}

	Poco::XML::Node* pNode = dict->firstChild();
	while (pNode != NULL)
	{
		if (pNode->nodeName() == "key" && pNode->firstChild()->nodeValue() == firstkey)
		{
			if (dot != std::string::npos)	// keep searching
				return findNode(key.substr(dot +1), pNode->nextSibling(), create);
			else
				return pNode;
		}

		pNode = pNode->nextSibling();
	}

	if (create)
	{
		Poco::AutoPtr<Poco::XML::Element> pElem, pNew;
		Poco::AutoPtr<Poco::XML::Node> pText;

		pNew = pElem = dict->ownerDocument()->createElement("key");
		dict->appendChild(pElem);
		pText = dict->ownerDocument()->createTextNode(firstkey);
		pElem->appendChild(pText);

		if (dot != std::string::npos)
		{
			pElem = dict->ownerDocument()->createElement("dict");
			dict->appendChild(pElem);

			return findNode(key.substr(dot + 1), pElem, true);
		}
		else
		{
			pElem = dict->ownerDocument()->createElement("string");
			dict->appendChild(pElem);
			pText = dict->ownerDocument()->createTextNode("");
			pElem->appendChild(pText);

			return pNew;
		}
	}

	return NULL;
}

} } // namespace Poco::Util

#endif // POCO_UTIL_NO_XMLCONFIGURATION
