//
// ChildNodesList.cpp
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


#include "Poco/DOM/ChildNodesList.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/Document.h"


namespace Poco {
namespace XML {


ChildNodesList::ChildNodesList(const Node::Ptr pParent):
		_pParentNode(pParent)
{
	poco_check_ptr (pParent);
}


ChildNodesList::~ChildNodesList()
{
}


Node::Ptr ChildNodesList::item(unsigned long index) const
{
	unsigned long n = 0;
	Node::Ptr pCur = _pParentNode->firstChild();
	while (pCur && n++ < index)
	{
		pCur = pCur->nextSibling();
	}
	return pCur;
}


unsigned long ChildNodesList::length() const
{
	unsigned long n = 0;
	Node::Ptr pCur = _pParentNode->firstChild();
	while (pCur)
	{
		++n;
		pCur = pCur->nextSibling();
	}
	return n;
}


} } // namespace Poco::XML
