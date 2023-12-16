//
// AbstractContainerNode.h
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Definition of the AbstractContainerNode class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DOM_AbstractContainerNode_INCLUDED
#define DOM_AbstractContainerNode_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/DOM/AbstractNode.h"


namespace Poco {
namespace XML {


class XML_API AbstractContainerNode: public AbstractNode
	/// AbstractContainerNode is an implementation of Node
	/// that stores and manages child nodes.
	///
	/// Child nodes are organized in a single linked list.
{
public:
	// Node
	Node* firstChild() const override;
	Node* lastChild() const override;
	Node* insertBefore(Node* newChild, Node* refChild) override;
	Node* insertAfterNP(Node* newChild, Node* refChild) override;
	Node* replaceChild(Node* newChild, Node* oldChild) override;
	Node* removeChild(Node* oldChild) override;
	Node* appendChild(Node* newChild) override;
	bool hasChildNodes() const override;
	bool hasAttributes() const override;
	Node* getNodeByPath(const XMLString& path) const override;
	Node* getNodeByPathNS(const XMLString& path, const NSMap& nsMap) const override;

protected:
	AbstractContainerNode(Document* pOwnerDocument);
	AbstractContainerNode(Document* pOwnerDocument, const AbstractContainerNode& node);
	~AbstractContainerNode() override;

	void dispatchNodeRemovedFromDocument() override;
	void dispatchNodeInsertedIntoDocument() override;

	static const Node* findNode(XMLString::const_iterator& it, const XMLString::const_iterator& end, const Node* pNode, const NSMap* pNSMap, bool& indexBound);
	static const Node* findElement(const XMLString& name, const Node* pNode, const NSMap* pNSMap);
	static const Node* findElement(int index, const Node* pNode, const NSMap* pNSMap);
	static const Node* findElement(const XMLString& attr, const XMLString& value, const Node* pNode, const NSMap* pNSMap);
	static const Attr* findAttribute(const XMLString& name, const Node* pNode, const NSMap* pNSMap);
	bool hasAttributeValue(const XMLString& name, const XMLString& value, const NSMap* pNSMap) const;
	static bool namesAreEqual(const Node* pNode1, const Node* pNode2, const NSMap* pNSMap);
	static bool namesAreEqual(const Node* pNode, const XMLString& name, const NSMap* pNSMap);

	static const XMLString WILDCARD;

private:
	AbstractNode* _pFirstChild;

	friend class AbstractNode;
	friend class NodeAppender;
};


} } // namespace Poco::XML


#endif // DOM_AbstractContainerNode_INCLUDED
