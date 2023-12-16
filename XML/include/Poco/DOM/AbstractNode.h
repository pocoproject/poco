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
#include "Poco/XML/XMLString.h"


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
	// Node
	const XMLString& nodeName() const override;
	const XMLString& getNodeValue() const override;
	void setNodeValue(const XMLString& value) override;
	Node* parentNode() const override;
	NodeList* childNodes() const override;
	Node* firstChild() const override;
	Node* lastChild() const override;
	Node* previousSibling() const override;
	Node* nextSibling() const override;
	NamedNodeMap* attributes() const override;
	Document* ownerDocument() const override;
	Node* insertBefore(Node* newChild, Node* refChild) override;
	Node* insertAfterNP(Node* newChild, Node* refChild) override;
	Node* replaceChild(Node* newChild, Node* oldChild) override;
	Node* removeChild(Node* oldChild) override;
	Node* appendChild(Node* newChild) override;
	bool hasChildNodes() const override;
	Node* cloneNode(bool deep) const override;
	void normalize() override;
	bool isSupported(const XMLString& feature, const XMLString& version) const override;
	const XMLString& namespaceURI() const override;
	XMLString prefix() const override;
	const XMLString& localName() const override;
	bool hasAttributes() const override;

	// EventTarget
	void addEventListener(const XMLString& type, EventListener* listener, bool useCapture) override;
	void removeEventListener(const XMLString& type, EventListener* listener, bool useCapture) override;
	bool dispatchEvent(Event* evt) override;

	// Extensions
	XMLString innerText() const override;
	Node* getNodeByPath(const XMLString& path) const override;
	Node* getNodeByPathNS(const XMLString& path, const NSMap& nsMap) const override;

	void autoRelease() override;

protected:
	AbstractNode(Document* pOwnerDocument);
	AbstractNode(Document* pOwnerDocument, const AbstractNode& node);
	~AbstractNode() override;

	virtual Node* copyNode(bool deep, Document* pOwnerDocument) const = 0;

	virtual bool events() const;
	virtual bool eventsSuspended() const;
	void captureEvent(Event* evt);
	void bubbleEvent(Event* evt);
	void dispatchSubtreeModified();
	void dispatchNodeInserted();
	void dispatchNodeRemoved();
	virtual void dispatchNodeRemovedFromDocument();
	virtual void dispatchNodeInsertedIntoDocument();
	void dispatchAttrModified(Attr* pAttr, MutationEvent::AttrChangeType changeType, const XMLString& prevValue, const XMLString& newValue);
	void dispatchCharacterDataModified(const XMLString& prevValue, const XMLString& newValue);
	void setOwnerDocument(Document* pOwnerDocument);

	static const XMLString EMPTY_STRING;

private:
	AbstractNode();

	AbstractContainerNode* _pParent;
	AbstractNode*          _pNext;
	Document*              _pOwner;
	EventDispatcher*       _pEventDispatcher;

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
