//
// XMLConfiguration.cpp
//
// Library: Util
// Package: Configuration
// Module:  XMLConfiguration
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/XMLConfiguration.h"


#ifndef POCO_UTIL_NO_XMLCONFIGURATION


#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/Text.h"
#include "Poco/XML/XMLWriter.h"
#include "Poco/Exception.h"
#include "Poco/NumberParser.h"
#include "Poco/NumberFormatter.h"
#include <set>


namespace Poco {
namespace Util {


XMLConfiguration::XMLConfiguration():
	_delim('.')
{
	loadEmpty("config");
}


XMLConfiguration::XMLConfiguration(char delim):
	_delim(delim)
{
	loadEmpty("config");
}


XMLConfiguration::XMLConfiguration(Poco::XML::InputSource::Ptr pInputSource):
	_delim('.')
{
	load(pInputSource);
}


XMLConfiguration::XMLConfiguration(Poco::XML::InputSource::Ptr pInputSource, char delim):
	_delim(delim)
{
	load(pInputSource);
}


XMLConfiguration::XMLConfiguration(std::istream& istr):
	_delim('.')
{
	load(istr);
}


XMLConfiguration::XMLConfiguration(std::istream& istr, char delim):
	_delim(delim)
{
	load(istr);
}


XMLConfiguration::XMLConfiguration(const std::string& path):
	_delim('.')
{
	load(path);
}


XMLConfiguration::XMLConfiguration(const std::string& path, char delim):
	_delim(delim)
{
	load(path);
}


XMLConfiguration::XMLConfiguration(const Poco::XML::Document::Ptr pDocument):
	_delim('.')
{
	load(pDocument);
}


XMLConfiguration::XMLConfiguration(const Poco::XML::Document::Ptr pDocument, char delim):
	_delim(delim)
{
	load(pDocument);
}


XMLConfiguration::XMLConfiguration(const Poco::XML::Node::Ptr pNode):
	_delim('.')
{
	load(pNode);
}


XMLConfiguration::XMLConfiguration(const Poco::XML::Node::Ptr pNode, char delim):
	_delim(delim)
{
	load(pNode);
}


XMLConfiguration::~XMLConfiguration()
{
}


void XMLConfiguration::load(Poco::XML::InputSource::Ptr pInputSource, unsigned long namePoolSize)
{
	poco_check_ptr (pInputSource);

	Poco::XML::DOMParser parser(namePoolSize);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
	parser.setFeature(Poco::XML::DOMParser::FEATURE_FILTER_WHITESPACE, true);
	Poco::RefPtr<Poco::XML::Document> pDoc = parser.parse(pInputSource);
	load(pDoc);
}


void XMLConfiguration::load(Poco::XML::InputSource::Ptr pInputSource)
{
	load(pInputSource, POCO_XML_NAMEPOOL_DEFAULT_SIZE);
}


void XMLConfiguration::load(std::istream& istr)
{
	Poco::XML::InputSource::Ptr src = new Poco::XML::InputSource(istr);
	load(src);
}


void XMLConfiguration::load(const std::string& path)
{
	Poco::XML::InputSource::Ptr src = new Poco::XML::InputSource(path);
	load(src);
}


void XMLConfiguration::load(const Poco::XML::Document::Ptr pDocument)
{
	poco_check_ptr (pDocument);

	_pDocument = pDocument;
	_pRoot     = pDocument->documentElement();
}


void XMLConfiguration::load(const Poco::XML::Node::Ptr pNode)
{
	poco_check_ptr (pNode);

	if (pNode->nodeType() == Poco::XML::Node::DOCUMENT_NODE)
	{
		load(pNode);
	}
	else
	{
		_pDocument = pNode->ownerDocument();
		_pRoot     = pNode;
	}
}


void XMLConfiguration::loadEmpty(const std::string& rootElementName)
{
	_pDocument = new Poco::XML::Document;
	_pRoot     = _pDocument->createElement(rootElementName);
	_pDocument->appendChild(_pRoot);
}


void XMLConfiguration::save(const std::string& path) const
{
	Poco::XML::DOMWriter writer;
	writer.setNewLine("\n");
	writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT);
	writer.writeNode(path, _pDocument);
}


void XMLConfiguration::save(std::ostream& ostr) const
{
	Poco::XML::DOMWriter writer;
	writer.setNewLine("\n");
	writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT);
	writer.writeNode(ostr, _pDocument);
}


void XMLConfiguration::save(Poco::XML::DOMWriter& writer, const std::string& path) const
{
	writer.writeNode(path, _pDocument);
}


void XMLConfiguration::save(Poco::XML::DOMWriter& writer, std::ostream& ostr) const
{
	writer.writeNode(ostr, _pDocument);
}


bool XMLConfiguration::getRaw(const std::string& key, std::string& value) const
{
	const Poco::XML::Node::Ptr pNode = findNode(key);
	if (pNode)
	{
		value = pNode->innerText();
		return true;
	}
	else return false;
}


void XMLConfiguration::setRaw(const std::string& key, const std::string& value)
{
	std::string::const_iterator it = key.begin();
	Poco::XML::Node::Ptr pNode = findNode(it, key.end(), _pRoot, true);
	if (pNode)
	{
		unsigned short nodeType = pNode->nodeType();
		if (Poco::XML::Node::ATTRIBUTE_NODE == nodeType)
		{
			pNode->setNodeValue(value);
		}
		else if (Poco::XML::Node::ELEMENT_NODE == nodeType)
		{
			Poco::XML::Node::Ptr pChildNode = pNode->firstChild();
			if (pChildNode)
			{
				if (Poco::XML::Node::TEXT_NODE == pChildNode->nodeType())
				{
					pChildNode->setNodeValue(value);
				}
			}
			else
			{
				Poco::AutoPtr<Poco::XML::Node> pText = _pDocument->createTextNode(value);
				pNode->appendChild(pText);
			}
		}
	}
	else throw NotFoundException("Node not found in XMLConfiguration", key);
}


void XMLConfiguration::enumerate(const std::string& key, Keys& range) const
{
	using Poco::NumberFormatter;

	std::multiset<std::string> keySet;
	Poco::XML::Node::Ptr pNode = findNode(key);
	if (pNode)
	{
		Poco::XML::Node::Ptr pChild = pNode->firstChild();
		while (pChild)
		{
			if (pChild->nodeType() == Poco::XML::Node::ELEMENT_NODE)
			{
				const std::string& nodeName = pChild->nodeName();
				int n = (int) keySet.count(nodeName);
				if (n)
					range.push_back(nodeName + "[" + NumberFormatter::format(n) + "]");
				else
					range.push_back(nodeName);
				keySet.insert(nodeName);
			}
			pChild = pChild->nextSibling();
		}
	}
}


void XMLConfiguration::removeRaw(const std::string& key)
{
	Poco::XML::Node::Ptr pNode = findNode(key);

	if (pNode)
	{
		if (pNode->nodeType() == Poco::XML::Node::ELEMENT_NODE)
		{
			Poco::XML::Node::Ptr pParent = pNode->parentNode();
			if (pParent)
			{
				pParent->removeChild(pNode);
			}
		}
		else if (pNode->nodeType() == Poco::XML::Node::ATTRIBUTE_NODE)
		{
			Poco::XML::Attr::Ptr pAttr = pNode.cast<Poco::XML::Attr>();
			Poco::XML::Element::Ptr pOwner = pAttr->ownerElement();
			if (pOwner)
			{
				pOwner->removeAttributeNode(pAttr);
			}
		}
	}
}


const Poco::XML::Node::Ptr XMLConfiguration::findNode(const std::string& key) const
{
	std::string::const_iterator it = key.begin();
	Poco::XML::Node::Ptr pRoot = _pRoot;
	return findNode(it, key.end(), pRoot);
}


Poco::XML::Node::Ptr XMLConfiguration::findNode(const std::string& key)
{
	std::string::const_iterator it = key.begin();
	Poco::XML::Node::Ptr pRoot = _pRoot;
	return findNode(it, key.end(), pRoot);
}


Poco::XML::Node::Ptr XMLConfiguration::findNode(std::string::const_iterator& it, const std::string::const_iterator& end, Poco::XML::Node::Ptr pNode, bool create) const
{
	if (pNode && it != end)
	{
		if (*it == '[')
		{
			++it;
			if (it != end && *it == '@')
			{
				++it;
				std::string attr;
				while (it != end && *it != ']' && *it != '=') attr += *it++;
				if (it != end && *it == '=')
				{
					++it;
					std::string value;
					if (it != end && *it == '\'')
					{
						++it;
						while (it != end && *it != '\'') value += *it++;
						if (it != end) ++it;
					}
					else
					{
						while (it != end && *it != ']') value += *it++;
					}
					if (it != end) ++it;
					return findNode(it, end, findElement(attr, value, pNode), create);
				}
				else
				{
					if (it != end) ++it;
					return findAttribute(attr, pNode, create);
				}
			}
			else
			{
				std::string index;
				while (it != end && *it != ']') index += *it++;
				if (it != end) ++it;
				return findNode(it, end, findElement(Poco::NumberParser::parse(index), pNode, create), create);
			}
		}
		else
		{
			while (it != end && *it == _delim) ++it;
			std::string key;
			while (it != end && *it != _delim && *it != '[') key += *it++;
			return findNode(it, end, findElement(key, pNode, create), create);
		}
	}
	else return pNode;
}


Poco::XML::Node::Ptr XMLConfiguration::findElement(const std::string& name, Poco::XML::Node::Ptr pNode, bool create)
{
	Poco::XML::Node::Ptr pChild = pNode->firstChild();
	while (pChild)
	{
		if (pChild->nodeType() == Poco::XML::Node::ELEMENT_NODE && pChild->nodeName() == name)
			return pChild;
		pChild = pChild->nextSibling();
	}
	if (create)
	{
		Poco::AutoPtr<Poco::XML::Element> pElem = pNode->ownerDocument()->createElement(name);
		pNode->appendChild(pElem);
		return pElem;
	}
	else return 0;
}


Poco::XML::Node::Ptr XMLConfiguration::findElement(int index, Poco::XML::Node::Ptr pNode, bool create)
{
	Poco::XML::Node::Ptr pRefNode = pNode;
	if (index > 0)
	{
		pNode = pNode->nextSibling();
		while (pNode)
		{
			if (pNode->nodeName() == pRefNode->nodeName())
			{
				if (--index == 0) break;
			}
			pNode = pNode->nextSibling();
		}
	}
	if (!pNode && create)
	{
		if (index == 1)
		{
			Poco::AutoPtr<Poco::XML::Element> pElem = pRefNode->ownerDocument()->createElement(pRefNode->nodeName());
			pRefNode->parentNode()->appendChild(pElem);
			return pElem;
		}
		else throw Poco::InvalidArgumentException("Element index out of range.");
	}
	return pNode;
}


Poco::XML::Node::Ptr XMLConfiguration::findElement(const std::string& attr, const std::string& value, Poco::XML::Node::Ptr pNode)
{
	Poco::XML::Node::Ptr pRefNode = pNode;
	Poco::XML::Element::Ptr pElem = pNode.cast<Poco::XML::Element>();
	if (!(pElem && pElem->getAttribute(attr) == value))
	{
		pNode = pNode->nextSibling();
		while (pNode)
		{
			if (pNode->nodeName() == pRefNode->nodeName())
			{
				pElem = pNode.cast<Poco::XML::Element>();
				if (pElem && pElem->getAttribute(attr) == value) break;
			}
			pNode = pNode->nextSibling();
		}
	}
	return pNode;
}


Poco::XML::Node::Ptr XMLConfiguration::findAttribute(const std::string& name, Poco::XML::Node::Ptr pNode, bool create)
{
	Poco::XML::Node::Ptr pResult(0);
	Poco::XML::Element::Ptr pElem = pNode.cast<Poco::XML::Element>();
	if (pElem)
	{
		pResult = pElem->getAttributeNode(name);
		if (!pResult && create)
		{
			Poco::AutoPtr<Poco::XML::Attr> pAttr = pNode->ownerDocument()->createAttribute(name);
			pElem->setAttributeNode(pAttr);
			return pAttr;
		}
	}
	return pResult;
}


} } // namespace Poco::Util

#endif // POCO_UTIL_NO_XMLCONFIGURATION
