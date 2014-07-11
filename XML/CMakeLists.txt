set(LIBNAME "PocoXML")

set(SRCS
    src/AbstractContainerNode.cpp
    src/AbstractNode.cpp
    src/Attr.cpp
    src/Attributes.cpp
    src/AttributesImpl.cpp
    src/AttrMap.cpp
    src/CDATASection.cpp
    src/CharacterData.cpp
    src/ChildNodesList.cpp
    src/Comment.cpp
    src/ContentHandler.cpp
    src/DeclHandler.cpp
    src/DefaultHandler.cpp
    src/Document.cpp
    src/DocumentEvent.cpp
    src/DocumentFragment.cpp
    src/DocumentType.cpp
    src/DOMBuilder.cpp
    src/DOMException.cpp
    src/DOMImplementation.cpp
    src/DOMObject.cpp
    src/DOMParser.cpp
    src/DOMSerializer.cpp
    src/DOMWriter.cpp
    src/DTDHandler.cpp
    src/DTDMap.cpp
    src/Element.cpp
    src/ElementsByTagNameList.cpp
    src/Entity.cpp
    src/EntityReference.cpp
    src/EntityResolver.cpp
    src/EntityResolverImpl.cpp
    src/ErrorHandler.cpp
    src/Event.cpp
    src/EventDispatcher.cpp
    src/EventException.cpp
    src/EventListener.cpp
    src/EventTarget.cpp
    src/InputSource.cpp
    src/LexicalHandler.cpp
    src/Locator.cpp
    src/LocatorImpl.cpp
    src/MutationEvent.cpp
    src/Name.cpp
    src/NamedNodeMap.cpp
    src/NamePool.cpp
    src/NamespaceStrategy.cpp
    src/NamespaceSupport.cpp
    src/NodeAppender.cpp
    src/Node.cpp
    src/NodeFilter.cpp
    src/NodeIterator.cpp
    src/NodeList.cpp
    src/Notation.cpp
    src/ParserEngine.cpp
    src/ProcessingInstruction.cpp
    src/SAXException.cpp
    src/SAXParser.cpp
    src/Text.cpp
    src/TreeWalker.cpp
    src/WhitespaceFilter.cpp
    src/XMLException.cpp
    src/XMLFilter.cpp
    src/XMLFilterImpl.cpp
    src/XMLReader.cpp
    src/XMLString.cpp
    src/XMLWriter.cpp
)

# If POCO_UNBUNDLED is enabled we try to find the required packages
# The configuration will fail if the packages are not found
if (POCO_UNBUNDLED)
    find_package(EXPAT REQUIRED)
    set(SYSLIBS ${SYSLIBS} ${EXPAT_LIBRARIES})
    include_directories(${EXPAT_INCLUDE_DIRS})
else()
    list(APPEND SRCS
    src/xmlparse.cpp
    src/xmlrole.c
    src/xmltok.c
    src/xmltok_impl.c
    src/xmltok_ns.c
    )
endif (POCO_UNBUNDLED)



add_definitions(-D_USRDLL -DXML_NS -DXML_DTD -DHAVE_EXPAT_CONFIG_H -DXML_EXPORTS)

if (NOT POCO_STATIC)
  add_definitions(-DXML_EXPORTS)
endif (NOT POCO_STATIC)

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME}
	PROPERTIES
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} ${SYSLIBS} PocoFoundation)

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )

install(
    TARGETS ${LIBNAME}
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    RUNTIME DESTINATION bin
    )

if (ENABLE_TESTS)
  add_subdirectory(samples)
  add_subdirectory(testsuite)
endif ()

