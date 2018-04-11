//
// AbstractContainerNode.cpp
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DOM/AbstractContainerNode.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/Attr.h"
#include "Poco/DOM/DOMException.h"
#include "Poco/DOM/ElementsByTagNameList.h"
#include "Poco/NumberParser.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/RefPtr.h"


namespace Poco {
namespace XML {


const XMLString AbstractContainerNode::WILDCARD(toXMLString("*"));


AbstractContainerNode::AbstractContainerNode(Document* pOwnerDocument):
	AbstractNode(pOwnerDocument)
{
}


AbstractContainerNode::~AbstractContainerNode()
{
	AbstractNode::Ptr pChild = _pFirstChild.cast<AbstractNode>();
	while(pChild && pChild->_pNext)
	{
		AbstractNode::Ptr pDelNode(std::move(pChild->_pNext));
		pDelNode->_pParent = 0;
		pChild = std::move(pDelNode);
	}
}


Node::Ptr AbstractContainerNode::firstChild() const
{
	return _pFirstChild;
}


Node::Ptr AbstractContainerNode::lastChild() const
{
	AbstractNode::Ptr pChild = _pFirstChild;
	if (pChild)
	{
		while (pChild->_pNext) pChild = pChild->_pNext;
		return pChild;
	}
	return 0;
}


Node::Ptr AbstractContainerNode::insertBefore(Node::Ptr newChild, Node::Ptr refChild)
{
	poco_assert (newChild);

	AbstractNode* pNewChild = newChild.cast<AbstractNode>();
	AbstractNode* pRefChild = refChild.cast<AbstractNode>();

	if (pNewChild->_pOwner != _pOwner && pNewChild->_pOwner != this)
		throw DOMException(DOMException::WRONG_DOCUMENT_ERR);
	if (refChild && pRefChild->_pParent != this)
		throw DOMException(DOMException::NOT_FOUND_ERR);
	if (newChild == refChild)
		return newChild;
	if (this == newChild)
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);

	AbstractNode::Ptr pFirst;
	AbstractNode::Ptr pLast;
	if (newChild->nodeType() == Node::DOCUMENT_FRAGMENT_NODE)
	{
		AbstractContainerNode::Ptr pFrag = newChild.cast<AbstractContainerNode>();
		pFirst = pFrag->_pFirstChild;
		pLast  = pFirst;
		if (pFirst)
		{
			while (pLast->_pNext)
			{
				pLast->_pParent = Poco::RefPtr<AbstractContainerNode>(this, true);
				pLast = pLast->_pNext;
			}
			pLast->_pParent = Poco::RefPtr<AbstractContainerNode>(this, true);
		}
		pFrag->_pFirstChild = 0;
	}
	else
	{
		AbstractContainerNode::Ptr pParent = pNewChild->_pParent;
		if (pParent) pParent->removeChild(newChild);
		pFirst.assign(pNewChild, true);
		pLast  = pFirst;
		pFirst->_pParent = Poco::RefPtr<AbstractContainerNode>(this, true);
	}

	if (_pFirstChild && pFirst)
	{
		AbstractNode::Ptr pCur = _pFirstChild;
		if (pCur.cast<Node>() == refChild)
		{
			pLast->_pNext = _pFirstChild;
			_pFirstChild  = pFirst;
		}
		else
		{
			while(pCur && pCur->_pNext.cast<Node>() != refChild) pCur = pCur->_pNext;
			if(pCur)
			{
				pLast->_pNext = pCur->_pNext;
				pCur->_pNext = pFirst;
			}
			else throw DOMException(DOMException::NOT_FOUND_ERR);
		}
	}
	else _pFirstChild = pFirst;

	if (events())
	{
		while (pFirst && pFirst != pLast->_pNext)
		{
			pFirst->dispatchNodeInserted();
			pFirst->dispatchNodeInsertedIntoDocument();
			pFirst = pFirst->_pNext;
		}
		dispatchSubtreeModified();
	}
	return newChild;
}


Node::Ptr AbstractContainerNode::replaceChild(Node::Ptr newChild, Node::Ptr oldChild)
{
	poco_assert (newChild);
	poco_assert (oldChild);

	AbstractNode* pNewChild = newChild.cast<AbstractNode>();
	AbstractNode* pOldChild = oldChild.cast<AbstractNode>();

	if (pNewChild->_pOwner != _pOwner && pNewChild->_pOwner != this)
		throw DOMException(DOMException::WRONG_DOCUMENT_ERR);
	if (pOldChild->_pParent != this)
		throw DOMException(DOMException::NOT_FOUND_ERR);
	if (newChild == oldChild)
		return newChild;
	if (this == newChild)
		throw DOMException(DOMException::HIERARCHY_REQUEST_ERR);

	bool doEvents = events();
	if (newChild->nodeType() == Node::DOCUMENT_FRAGMENT_NODE)
	{
		insertBefore(newChild, oldChild);
		removeChild(oldChild);
	}
	else
	{
		if (pNewChild->_pParent) pNewChild->_pParent->removeChild(newChild);

		if (pOldChild == _pFirstChild)
		{
			if (doEvents)
			{
				_pFirstChild->dispatchNodeRemoved();
				_pFirstChild->dispatchNodeRemovedFromDocument();
			}
			pNewChild->_pNext   = pOldChild->_pNext;
			pNewChild->_pParent = Ptr(this, true);
			_pFirstChild->_pNext   = 0;
			_pFirstChild->_pParent = 0;
			_pFirstChild.assign(pNewChild, true);
			if (doEvents)
			{
				pNewChild->dispatchNodeInserted();
				pNewChild->dispatchNodeInsertedIntoDocument();
			}
		}
		else
		{
			AbstractNode* pCur = _pFirstChild;
			while (pCur && pCur->_pNext != pOldChild) pCur = pCur->_pNext;
			if (pCur)
			{
				poco_assert_dbg (pCur->_pNext == pOldChild);

				if (doEvents)
				{
					pOldChild->dispatchNodeRemoved();
					pOldChild->dispatchNodeRemovedFromDocument();
				}
				pNewChild->_pNext   = pOldChild->_pNext;
				pNewChild->_pParent = Ptr(this, true);
				pOldChild->_pNext   = 0;
				pOldChild->_pParent = 0;
				pCur->_pNext.assign(pNewChild, true);
				if (doEvents)
				{
					pNewChild->dispatchNodeInserted();
					pNewChild->dispatchNodeInsertedIntoDocument();
				}
			}
			else throw DOMException(DOMException::NOT_FOUND_ERR);
		}
	}
	if (doEvents) dispatchSubtreeModified();
	return oldChild;
}


Node::Ptr AbstractContainerNode::removeChild(Node::Ptr oldChild)
{
	poco_check_ptr (oldChild);

	AbstractNode* pOldChild = oldChild.cast<AbstractNode>();

	bool doEvents = events();
	if (oldChild == _pFirstChild.cast<Node>())
	{
		if (doEvents)
		{
			pOldChild->dispatchNodeRemoved();
			pOldChild->dispatchNodeRemovedFromDocument();
		}
		_pFirstChild = _pFirstChild->_pNext;
		pOldChild->_pNext.reset();
		pOldChild->_pParent = 0;
	}
	else
	{
		AbstractNode::Ptr pCur = _pFirstChild;
		while (pCur && pCur->_pNext != oldChild.cast<AbstractNode>()) pCur = pCur->_pNext;
		if (pCur)
		{
			if (doEvents)
			{
				pOldChild->dispatchNodeRemoved();
				pOldChild->dispatchNodeRemovedFromDocument();
			}
			pCur->_pNext = pCur->_pNext->_pNext;
			pOldChild->_pNext.reset();
			pOldChild->_pParent = 0;
		}
		else throw DOMException(DOMException::NOT_FOUND_ERR);
	}
	if (doEvents) dispatchSubtreeModified();
	return oldChild;
}


Node::Ptr AbstractContainerNode::appendChild(Node::Ptr newChild)
{
	return insertBefore(newChild, 0);
}


void AbstractContainerNode::dispatchNodeRemovedFromDocument()
{
	AbstractNode::dispatchNodeRemovedFromDocument();
	Node::Ptr pChild = firstChild();
	while (pChild)
	{
		pChild.cast<AbstractNode>()->dispatchNodeRemovedFromDocument();
		pChild = pChild->nextSibling();
	}
}


void AbstractContainerNode::dispatchNodeInsertedIntoDocument()
{
	AbstractNode::dispatchNodeInsertedIntoDocument();
	Node::Ptr pChild = firstChild();
	while (pChild)
	{
		pChild.cast<AbstractNode>()->dispatchNodeInsertedIntoDocument();
		pChild = pChild->nextSibling();
	}
}


bool AbstractContainerNode::hasChildNodes() const
{
	return !_pFirstChild.isNull();
}


bool AbstractContainerNode::hasAttributes() const
{
	return false;
}


Node::Ptr AbstractContainerNode::getNodeByPath(const XMLString& path) const
{
	XMLString::const_iterator it = path.begin();
	if (it != path.end() && *it == '/')
	{
		++it;
		if (it != path.end() && *it == '/')
		{
			++it;
			XMLString name;
			while (it != path.end() && *it != '/' && *it != '@' && *it != '[') name += *it++;
			if (it != path.end() && *it == '/') ++it;
			if (name.empty()) name = WILDCARD;
			RefPtr<ElementsByTagNameList> pList = new ElementsByTagNameList(RefPtr<AbstractContainerNode>(const_cast<AbstractContainerNode*>(this), true), name);
			unsigned long length = pList->length();
			for (unsigned long i = 0; i < length; i++)
			{
				XMLString::const_iterator beg = it;
				const Node::Ptr pNode = findNode(beg, path.end(), pList->item(i), 0);
				if (pNode) return pNode;
			}
			return 0;
		}
	}
	return findNode(it, path.end(), RefPtr<Node>(const_cast<AbstractContainerNode*>(this), true), 0);
}


Node::Ptr AbstractContainerNode::getNodeByPathNS(const XMLString& path, const NSMap& nsMap) const
{
	XMLString::const_iterator it = path.begin();
	if (it != path.end() && *it == '/')
	{
		++it;
		if (it != path.end() && *it == '/')
		{
			++it;
			XMLString name;
			while (it != path.end() && *it != '/' && *it != '@' && *it != '[') name += *it++;
			if (it != path.end() && *it == '/') ++it;
			XMLString namespaceURI;
			XMLString localName;
			bool nameOK = true;
			if (name.empty())
			{
				namespaceURI = WILDCARD;
				localName = WILDCARD;
			}
			else
			{
				nameOK = nsMap.processName(name, namespaceURI, localName, false);
			}
			if (nameOK)
			{
				RefPtr<ElementsByTagNameListNS> pList =
					new ElementsByTagNameListNS(RefPtr<AbstractContainerNode>(const_cast<AbstractContainerNode*>(this), true),
						namespaceURI, localName);
				unsigned long length = pList->length();
				for (unsigned long i = 0; i < length; i++)
				{
					XMLString::const_iterator beg = it;
					Node::Ptr pNode = findNode(beg, path.end(), pList->item(i), &nsMap);
					if (pNode) return pNode;
				}
			}
			return 0;
		}
	}
	return findNode(it, path.end(), RefPtr<Node>(const_cast<AbstractContainerNode*>(this), true), &nsMap);
}


Node::Ptr AbstractContainerNode::findNode(XMLString::const_iterator& it, const XMLString::const_iterator& end, const Node::Ptr pNode, const NSMap* pNSMap)
{
	if (pNode && it != end)
	{
		if (*it == '[')
		{
			++it;
			if (it != end && *it == '@')
			{
				++it;
				XMLString attr;
				while (it != end && *it != ']' && *it != '=') attr += *it++;
				if (it != end && *it == '=')
				{
					++it;
					XMLString value;
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
					return findNode(it, end, findElement(attr, value, pNode, pNSMap), pNSMap);
				}
				else
				{
					if (it != end) ++it;
					return findAttribute(attr, pNode, pNSMap);
				}
			}
			else
			{
				XMLString index;
				while (it != end && *it != ']') index += *it++;
				if (it != end) ++it;
#ifdef XML_UNICODE_WCHAR_T
				std::string idx;
				Poco::UnicodeConverter::convert(index, idx);
				int i = Poco::NumberParser::parse(idx);	
#else
				int i = Poco::NumberParser::parse(index);
#endif
				return findNode(it, end, findElement(i, pNode, pNSMap), pNSMap);
			}
		}
		else
		{
			while (it != end && *it == '/') ++it;
			XMLString key;
			while (it != end && *it != '/' && *it != '[') key += *it++;

			XMLString::const_iterator itStart(it);
			Node::Ptr pFound = 0;
			Node::Ptr pElem = findElement(key, pNode->firstChild(), pNSMap);
			while (!pFound && pElem)
			{
				pFound = findNode(it, end, pElem, pNSMap);
				if (!pFound) pElem = findElement(key, pElem->nextSibling(), pNSMap);
				it = itStart;
			}
			return pFound;
		}
	}
	else return pNode;
}


Node::Ptr AbstractContainerNode::findElement(const XMLString& name, Node::Ptr pNode, const NSMap* pNSMap)
{
	while (pNode)
	{
		if (pNode->nodeType() == Node::ELEMENT_NODE && namesAreEqual(pNode, name, pNSMap))
			return pNode;
		pNode = pNode->nextSibling();
	}
	return 0;
}


Node::Ptr AbstractContainerNode::findElement(int index, Node::Ptr pNode, const NSMap* pNSMap)
{
	Node::Ptr pRefNode = pNode;
	if (index > 0)
	{
		pNode = pNode->nextSibling();
		while (pNode)
		{
			if (namesAreEqual(pNode, pRefNode, pNSMap))
			{
				if (--index == 0) break;
			}
			pNode = pNode->nextSibling();
		}
	}
	return pNode;
}


Node::Ptr AbstractContainerNode::findElement(const XMLString& attr, const XMLString& value, Node::Ptr pNode, const NSMap* pNSMap)
{
	Node::Ptr pRefNode = pNode;
	Element::Ptr pElem = pNode.cast<Element>();
	if (!(pElem && pElem->hasAttributeValue(attr, value, pNSMap)))
	{
		pNode = pNode->nextSibling();
		while (pNode)
		{
			if (namesAreEqual(pNode, pRefNode, pNSMap))
			{
				pElem = pNode.cast<Element>();
				if (pElem && pElem->hasAttributeValue(attr, value, pNSMap)) break;
			}
			pNode = pNode->nextSibling();
		}
	}
	return pNode;
}


const Attr::Ptr AbstractContainerNode::findAttribute(const XMLString& name, Node::Ptr pNode, const NSMap* pNSMap)
{
	Attr::Ptr pResult;
	Element* pElem = pNode.cast<Element>();
	if (pElem)
	{
		if (pNSMap)
		{
			XMLString namespaceURI;
			XMLString localName;
			if (pNSMap->processName(name, namespaceURI, localName, true))
			{
				pResult = pElem->getAttributeNodeNS(namespaceURI, localName);
			}
		}
		else
		{
			pResult = pElem->getAttributeNode(name);
		}
	}
	return pResult;
}


bool AbstractContainerNode::hasAttributeValue(const XMLString& name, const XMLString& value, const NSMap* pNSMap) const
{
	const Attr::Ptr pAttr = findAttribute(name, RefPtr<AbstractContainerNode>(const_cast<AbstractContainerNode*>(this), true), pNSMap);
	return pAttr && pAttr->getValue() == value;
}


bool AbstractContainerNode::namesAreEqual(const Node::Ptr pNode1, Node::Ptr pNode2, const NSMap* pNSMap)
{
	if (pNSMap)
	{
		return pNode1->localName() == pNode2->localName() && pNode1->namespaceURI() == pNode2->namespaceURI();
	}
	else
	{
		return pNode1->nodeName() == pNode2->nodeName();
	}
}


bool AbstractContainerNode::namesAreEqual(const Node::Ptr pNode, const XMLString& name, const NSMap* pNSMap)
{
	if (pNSMap)
	{
		XMLString namespaceURI;
		XMLString localName;
		if (name == WILDCARD)
		{
			return true;
		}
		else if (pNSMap->processName(name, namespaceURI, localName, false))
		{
			return (pNode->namespaceURI() == namespaceURI || namespaceURI == WILDCARD) && (pNode->localName() == localName || localName == WILDCARD);
		}
		else return false;
	}
	else
	{
		return pNode->nodeName() == name || name == WILDCARD;
	}
}


} } // namespace Poco::XML
