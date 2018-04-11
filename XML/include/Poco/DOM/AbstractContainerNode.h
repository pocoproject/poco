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


class Element;


class XML_API AbstractContainerNode: public AbstractNode
	/// AbstractContainerNode is an implementation of Node
	/// that stores and manages child nodes.
	///
	/// Child nodes are organized in a single linked list.
{
public:
	typedef Poco::RefPtr<AbstractContainerNode> Ptr;
	typedef Poco::WeakRefPtr<AbstractContainerNode> WeakPtr;

	// Node
	Node::Ptr firstChild() const;
	Node::Ptr lastChild() const;
	Node::Ptr insertBefore(Node::Ptr newChild, Node::Ptr refChild);
	Node::Ptr replaceChild(Node::Ptr newChild, Node::Ptr oldChild);
	Node::Ptr removeChild(Node::Ptr oldChild);
	Node::Ptr appendChild(Node::Ptr newChild);
	bool hasChildNodes() const;
	bool hasAttributes() const;
	Node::Ptr getNodeByPath(const XMLString& path) const;
	Node::Ptr getNodeByPathNS(const XMLString& path, const Node::NSMap& nsMap) const;

protected:
	AbstractContainerNode(Document* pOwnerDocument);
	~AbstractContainerNode();

	void dispatchNodeRemovedFromDocument();
	void dispatchNodeInsertedIntoDocument();

	static Node::Ptr findNode(XMLString::const_iterator& it, const XMLString::const_iterator& end, Node::Ptr pNode, const Node::NSMap* pNSMap);
	static Node::Ptr findElement(const XMLString& name, Node::Ptr pNode, const Node::NSMap* pNSMap);
	static Node::Ptr findElement(int index, Node::Ptr pNode, const Node::NSMap* pNSMap);
	static Node::Ptr findElement(const XMLString& attr, const XMLString& value, Node::Ptr pNode, const Node::NSMap* pNSMap);
	static const RefPtr<Attr> findAttribute(const XMLString& name, Node::Ptr pNode, const Node::NSMap* pNSMap);
	bool hasAttributeValue(const XMLString& name, const XMLString& value, const Node::NSMap* pNSMap) const;
	static bool namesAreEqual(const Node::Ptr pNode1, Node::Ptr pNode2, const Node::NSMap* pNSMap);
	static bool namesAreEqual(const Node::Ptr pNode, const XMLString& name, const Node::NSMap* pNSMap);

	static const XMLString WILDCARD;

private:
	AbstractNode::Ptr _pFirstChild;

	friend class AbstractNode;
	friend class NodeAppender;
};


} } // namespace Poco::XML


#endif // DOM_AbstractContainerNode_INCLUDED
