//
// AbstractNode.h
//
// Library: XML
// Package: DOM
// Module:  DOM
//
// Definition of the AbstractNode class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DOM_AbstractNode_INCLUDED
#define DOM_AbstractNode_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/MutationEvent.h"
#include "Poco/DOM/EventListener.h"


namespace Poco {
namespace XML {


class AbstractContainerNode;
class Attr;
class EventDispatcher;


class XML_API AbstractNode: public Node
	/// AbstractNode provides a basic implementation
	/// of the Node interface for all types of nodes
	/// that do not contain other nodes.
{
public:
	typedef Poco::AutoPtr<AbstractNode> Ptr;
 
	// Node
	const XMLString& nodeName() const;
	const XMLString& getNodeValue() const;
	void setNodeValue(const XMLString& value);
	Node::Ptr parentNode() const;
	Poco::AutoPtr<NodeList> childNodes() const;
	Node::Ptr firstChild() const;
	Node::Ptr lastChild() const;
	Node::Ptr previousSibling() const;
	Node::Ptr nextSibling() const;
	AutoPtr<NamedNodeMap> attributes() const;
	AutoPtr<Document> ownerDocument() const;
	Node::Ptr insertBefore(Node::Ptr newChild, Node::Ptr refChild);
	Node::Ptr replaceChild(Node::Ptr newChild, Node::Ptr oldChild);
	Node::Ptr removeChild(Node::Ptr oldChild);
	Node::Ptr appendChild(Node::Ptr newChild);
	bool hasChildNodes() const;
	Node::Ptr cloneNode(bool deep) const;
	void normalize();
	bool isSupported(const XMLString& feature, const XMLString& version) const;
	const XMLString& namespaceURI() const;
	XMLString prefix() const;
	const XMLString& localName() const;
	bool hasAttributes() const;

	// EventTarget
	void addEventListener(const XMLString& type, EventListener::Ptr listener, bool useCapture);
	void removeEventListener(const XMLString& type, EventListener::Ptr listener, bool useCapture);
	bool dispatchEvent(Event::Ptr evt);

	// Extensions
	XMLString innerText() const;
	Node::Ptr getNodeByPath(const XMLString& path) const;
	Node::Ptr getNodeByPathNS(const XMLString& path, const NSMap& nsMap) const;

protected:
	AbstractNode(AutoPtr<Document> pOwnerDocument);
	~AbstractNode();

	virtual Node::Ptr copyNode(bool deep, AutoPtr<Document> pOwnerDocument) const = 0;

	virtual bool events() const;
	virtual bool eventsSuspended() const;
	void captureEvent(Event::Ptr evt);
	void bubbleEvent(Event::Ptr evt);
	void dispatchSubtreeModified();
	void dispatchNodeInserted();
	void dispatchNodeRemoved();
	virtual void dispatchNodeRemovedFromDocument();
	virtual void dispatchNodeInsertedIntoDocument();
	void dispatchAttrModified(AutoPtr<Attr> pAttr, MutationEvent::AttrChangeType changeType, const XMLString& prevValue, const XMLString& newValue);
	void dispatchCharacterDataModified(const XMLString& prevValue, const XMLString& newValue);
	void setOwnerDocument(AutoPtr<Document> pOwnerDocument);

	static const XMLString EMPTY_STRING;

private:
	AbstractNode();

	AutoPtr<AbstractContainerNode> _pParent;
	AbstractNode::Ptr              _pNext;
	AutoPtr<Document>              _pOwner;
	AutoPtr<EventDispatcher>       _pEventDispatcher;

	static const XMLString NODE_NAME;

	friend class AbstractContainerNode;
	friend class Document;
	friend class DocumentFragment;
	friend class Element;
	friend class Attr;
	friend class CharacterData;
	friend class DOMBuilder;
	friend class NodeAppender;
};


} } // namespace Poco::XML


#endif // DOM_AbstractNode_INCLUDED
