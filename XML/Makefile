#
# Makefile
#
# Makefile for Poco XML
#

include $(POCO_BASE)/build/rules/global

COMMONFLAGS += -DXML_NS -DXML_DTD -DXML_GE -DHAVE_EXPAT_CONFIG_H -DEXPAT_POCO

objects = AbstractContainerNode AbstractNode Attr AttrMap Attributes \
	AttributesImpl CDATASection CharacterData ChildNodesList Comment \
	ContentHandler DOMBuilder DOMException DOMImplementation DOMObject \
	DOMParser DOMSerializer DOMWriter DTDHandler DTDMap DeclHandler \
	DefaultHandler Document DocumentEvent DocumentFragment DocumentType \
	Element ElementsByTagNameList Entity EntityReference EntityResolver \
	EntityResolverImpl ErrorHandler Event EventDispatcher EventException \
	EventListener EventTarget InputSource LexicalHandler Locator LocatorImpl \
	MutationEvent Name NamePool NamedNodeMap NamespaceStrategy \
	NamespaceSupport NodeAppender Node NodeFilter NodeIterator NodeList Notation \
	ParserEngine ProcessingInstruction QName SAXException SAXParser Text \
	TreeWalker WhitespaceFilter XMLException XMLFilter XMLFilterImpl XMLReader \
	XMLString XMLWriter XMLStreamParser XMLStreamParserException ValueTraits

expat_objects = xmlparse xmlrole xmltok

ifdef POCO_UNBUNDLED
	SYSLIBS += -lexpat
else
	objects += $(expat_objects)
endif

target         = PocoXML
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
