//
// ElementsByTagNameList.cpp
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


#include "Poco/DOM/ElementsByTagNameList.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/Document.h"
#include <climits>


namespace Poco {
namespace XML {


ElementsByTagNameList::ElementsByTagNameList(Node::Ptr pParent, const XMLString& name):
	_pParent(std::move(pParent)),
	_name(name),
	_count(0)
{
	poco_check_ptr (_pParent);
}


ElementsByTagNameList::~ElementsByTagNameList()
{
}


Node::Ptr ElementsByTagNameList::item(unsigned long index) const
{
	_count = 0;
	return find(_pParent, index);
}


unsigned long ElementsByTagNameList::length() const
{
	_count = 0;
	find(_pParent, ULONG_MAX);
	return _count;
}


namespace
{
	static const XMLString asterisk = toXMLString("*");
}


Node::Ptr ElementsByTagNameList::find(Node::Ptr pParent, unsigned long index) const
{
	if (!pParent) return 0;

	// preorder search
	Node::Ptr pCur = pParent->firstChild();
	while (pCur)
	{
		if (pCur->nodeType() == Node::ELEMENT_NODE && (_name == asterisk || pCur->nodeName() == _name))
		{
			if (_count == index) return pCur;
			_count++;
		}
		Node::Ptr pNode = find(pCur, index);
		if (pNode) return pNode;
		pCur = pCur->nextSibling();
	}
	return pCur;
}


ElementsByTagNameListNS::ElementsByTagNameListNS(Node::Ptr pParent, const XMLString& namespaceURI, const XMLString& localName):
	_pParent(std::move(pParent)),
	_localName(localName),
	_namespaceURI(namespaceURI),
	_count(0)
{
	poco_check_ptr (_pParent);
}



ElementsByTagNameListNS::~ElementsByTagNameListNS()
{
}


Node::Ptr ElementsByTagNameListNS::item(unsigned long index) const
{
	_count = 0;
	return find(_pParent, index);
}


unsigned long ElementsByTagNameListNS::length() const
{
	_count = 0;
	find(_pParent, ULONG_MAX);
	return _count;
}


Node::Ptr ElementsByTagNameListNS::find(Node::Ptr pParent, unsigned long index) const
{
	if (!pParent) return 0;

	// preorder search
	Node::Ptr pCur = pParent->firstChild();
	while (pCur)
	{
		if (pCur->nodeType() == Node::ELEMENT_NODE && (_localName == asterisk || pCur->localName() == _localName) && (_namespaceURI == asterisk || pCur->namespaceURI() == _namespaceURI))
		{
			if (_count == index) return pCur;
			_count++;
		}
		Node::Ptr pNode = find(pCur, index);
		if (pNode) return pNode;
		pCur = pCur->nextSibling();
	}
	return pCur;
}


} } // namespace Poco::XML
