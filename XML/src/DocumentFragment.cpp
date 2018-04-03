//
// DocumentFragment.cpp
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


#include "Poco/DOM/DocumentFragment.h"
#include "Poco/DOM/Document.h"


namespace Poco {
namespace XML {


const XMLString DocumentFragment::NODE_NAME = toXMLString("#document-fragment");


DocumentFragment::DocumentFragment(Document::Ptr pOwnerDocument):
	AbstractContainerNode(pOwnerDocument)
{
}


DocumentFragment::~DocumentFragment()
{
}


const XMLString& DocumentFragment::nodeName() const
{
	return NODE_NAME;
}


unsigned short DocumentFragment::nodeType() const
{
	return Node::DOCUMENT_FRAGMENT_NODE;
}


Node::Ptr DocumentFragment::copyNode(bool deep, Document::Ptr pOwnerDocument) const
{
	DocumentFragment::Ptr pClone = new DocumentFragment(pOwnerDocument);
	if (deep)
	{
		Node::Ptr pCur = firstChild();
		while (pCur)
		{
			pClone->appendChild(pCur.unsafeCast<AbstractNode>()->copyNode(deep, pOwnerDocument));
			pCur = pCur->nextSibling();
		}
	}
	return pClone;
}


} } // namespace Poco::XML
