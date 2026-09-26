//
// XSDContentHandler.h
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  XSDContentHandler
//
// Definition of the XSDContentHandler class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDParser_XSDContentHandler_INCLUDED
#define XSDParser_XSDContentHandler_INCLUDED


#include "Poco/XSD/Parser/XSDParser.h"
#include "Poco/XSD/Parser/StateMachine.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/AttributeContent.h"
#include "Poco/XSD/Types/AttributeGroupRef.h"
#include "Poco/XSD/Types/AttributeGroup.h"
#include "Poco/XSD/Types/Attribute.h"
#include "Poco/XSD/Types/Order.h"
#include "Poco/XSD/Types/ComplexType.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/InheritanceInfo.h"
#include "Poco/XSD/Types/Group.h"
#include "Poco/XSD/Types/GroupRef.h"
#include "Poco/XSD/Types/List.h"
#include "Poco/XSD/Types/ListTypeRef.h"
#include "Poco/XSD/Types/Notation.h"
#include "Poco/XSD/Types/Union.h"
#include "Poco/XSD/Types/All.h"
#include "Poco/XSD/Types/Any.h"
#include "Poco/XSD/Types/Schema.h"
#include "Poco/XSD/Types/Definitions.h"
#include "Poco/SAX/ContentHandler.h"
#include "Poco/SAX/NamespaceSupport.h"
#include "Poco/SAX/Attributes.h"
#include "Poco/XML/Name.h"
#include "Poco/URI.h"
#include "Poco/NestedDiagnosticContext.h"
#include <vector>
#include <stack>
#include <map>


namespace Poco {
namespace XSD {
namespace Types {
	class Element;
	class ElementImpl;
	class Any;
	class Attribute;
	class Operation;
	class BindingProperties;
} } } 


namespace Poco {
namespace XSD {
namespace Parser {


class XSDParser_API XSDContentHandler: public XML::ContentHandler
	/// The ContentHandler for processing an XML Schema document.
	///
	/// The handler registers the schemas and WSDL definitions it parses in the TypesManager
	/// singleton, so, like the Types model, it must be used from one thread at a time.
	/// loadXSD() removes the registrations of a document that fails to parse; a handler given
	/// to a SAXParser directly leaves them in the TypesManager.
{
public:
	using SchemaNSToLocationMap = std::map<std::string, std::string>;

	XSDContentHandler(const Poco::URI& schemaLocation, const SchemaNSToLocationMap& schemaMap);
		/// Creates the XSDContentHandler. set fixupSchema to false if you include another schema from within another one

	virtual ~XSDContentHandler();
		/// Destroys the XSDContentHandler.

	static void resolveSchemaLocation(Poco::URI& schemaLocation, const Poco::URI& parentSchemaLocation);
		/// Resolves and normalizes a schemaLocation URI.

	static Poco::XSD::Types::Schema::Ptr loadXSD(const Poco::URI& schemaLocation, const XSDContentHandler::SchemaNSToLocationMap& schemaMap);
		/// Loads the XSD document at schemaLocation and returns the schema that the TypesManager holds
		/// for its target namespace, into which the declarations of all loaded documents of that
		/// namespace are merged. A document that was already loaded is not parsed again.

	void setDocumentLocator(const XML::Locator* loc);
	void startDocument();
	void endDocument();
	void startElement(const std::string& uri, const std::string& localName, const std::string& qname, const XML::Attributes& attrList);
	void endElement(const std::string& uri, const std::string& localName, const std::string& qname);
	void characters(const XML::XMLChar ch[], int start, int length);
	void ignorableWhitespace(const XML::XMLChar ch[], int start, int length);
	void processingInstruction(const std::string& target, const std::string& data);
	void startPrefixMapping(const std::string& prefix, const std::string& uri);
	void endPrefixMapping(const std::string& prefix);
	void skippedEntity(const std::string& name);

private:
	using StateList = std::vector<StateMachine::State>;

	// XSD
	void stateAllStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateAnnotationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateAnyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateAnyAttributeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateAppInfoStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateAttributeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateAttributeGroupStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateChoiceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateComplexContentStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateComplexTypeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateDocumentationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateElementStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSimpleExtensionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateComplexExtensionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateFieldStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateGroupStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateXSDImportStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateIncludeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateKeyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateKeyrefStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateListStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateNotationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateRedefineStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSimpleTypeRestrictionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateComplexRestrictionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSimpleContentRestrictionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSchemaStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSelectorStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSequenceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSimpleContentStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSimpleTypeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateUnionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateUniqueStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateEnumerationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateFractionDigitsStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateLengthStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMaxExclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMaxInclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMaxLengthStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMinExclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMinInclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMinLengthStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void statePatternStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateTotalDigitsStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateUninitializedStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateWhiteSpaceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMetaAnyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateAllEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateAnnotationEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateAnyEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateAnyAttributeEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateAppInfoEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateAttributeEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateAttributeGroupEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateChoiceEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateComplexContentEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateComplexTypeEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateDocumentationEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateElementEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSimpleExtensionEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateComplexExtensionEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateFieldEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateGroupEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateXSDImportEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateIncludeEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateKeyEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateKeyrefEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateListEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateNotationEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateRedefineEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSimpleTypeRestrictionEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSimpleContentRestrictionEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateComplexRestrictionEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSchemaEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSelectorEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSequenceEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSimpleContentEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSimpleTypeEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateUnionEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateUniqueEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateEnumerationEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateFractionDigitsEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateLengthEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMaxExclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMaxInclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMaxLengthEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMinExclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMinInclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMinLengthEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void statePatternEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateTotalDigitsEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateUninitializedEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateWhiteSpaceEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMetaAnyEnd(const std::string& uri, const std::string& localName, const std::string& qname);

	// WSDL
	void stateDefinitionsStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateDefinitionsEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateWSDLImportStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateWSDLImportEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateTypesStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateTypesEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateMessageStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateMessageEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void statePartStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void statePartEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void statePortTypeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void statePortTypeEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateOperationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateOperationEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateInputStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateInputEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateOutputStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateOutputEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateFaultStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateFaultEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateBindingStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateBindingEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSoapBindingStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSoapBindingEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSoapOperationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSoapOperationEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSoapHeaderStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSoapHeaderEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSoapHeaderFaultStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSoapHeaderFaultEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSoapBodyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSoapBodyEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSoapFaultStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSoapFaultEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateServiceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateServiceEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void statePortStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void statePortEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateSoapAddressStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateSoapAddressEnd(const std::string& uri, const std::string& localName, const std::string& qname);
	void stateWSDLDocumentationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList);
	void stateWSDLDocumentationEnd(const std::string& uri, const std::string& localName, const std::string& qname);

	std::string location() const;
		/// PrettyPrints the locator

	void convertAttributes(const XML::Attributes& attr, CompactAttributes& attrList);
		/// Adds the attributes from attr to attrList

	Poco::XSD::Types::QName createQName(const std::string& str) const;
		/// Creates a qualified name for a given string
		
	void importXSD(const Poco::URI& schemaLocation, const XSDContentHandler::SchemaNSToLocationMap& schemaMap);
	void importWSDL(const std::string& targetNamespace, const Poco::URI& schemaLocation, const XSDContentHandler::SchemaNSToLocationMap& schemaMap);

private:
	void clear();
		/// Releases all objects created during parsing

	static StateMachine& stateMachine();
		/// Returns the state machine definition. 

	StateMachine::State getParentState() const;
		/// Returns the state of the parent

	XML::Name resolveName(const std::string& qname) const;
		/// Returns the name for a qualified name from an attribute value, with its prefix
		/// resolved to a namespace URI.

	Types::BindingProperties& operationBindingProperties(Types::Operation& operation) const;
		/// Returns the binding properties of the wsdl:input, wsdl:output or wsdl:fault
		/// element that encloses the current element.

	void revokeRegistrations();
		/// Removes the schemas and definitions that this handler registered from the
		/// TypesManager. Called when the document fails to parse.

	void parse();
		/// Parses the document at the schema location given to the constructor with this handler.
		/// If parsing fails, removes the registrations this handler made in the TypesManager
		/// and rethrows the exception.

	static Poco::XSD::Types::Schema::Ptr loadDocument(const Poco::URI& schemaLocation,
		const SchemaNSToLocationMap& schemaMap);
		/// Returns the schema of the document at schemaLocation. A document that is not loaded yet
		/// is parsed, and its own schema is returned, because xs:include takes over the declarations
		/// of that document only, not those of other documents merged into the same registered schema
		/// (for example other documents without a target namespace); otherwise the schema the
		/// TypesManager holds for schemaLocation is returned.

private:
	Poco::URI _schemaLocation;
	const XML::Locator*   _pLocator;
	XML::NamespaceSupport _namespaces;

	// schema specific info
	bool _qualifiedAttributeForm;
	bool _qualifiedElementForm;
	bool _blockExtension;
	bool _blockRestriction;
	bool _blockSubstitution;
	bool _finalExtension;
	bool _finalRestriction;
	bool _finalList;
	bool _finalUnion;
	
	StateList _states;
		/// Contains all active states. Is not a stack because some childs need information about
		/// their parent states to decide if an attribute is valid or not.

	std::stack<Poco::XSD::Types::Element*> _elements;
		/// Stack containing elements. Note that all elements stored here are also in the _objects member!

	std::stack<Poco::XSD::Types::ElementImpl*> _elementsImpl;
		/// Stack containing elements which require the type to be set later. 
		/// Note that all elements stored here are also in the _elements member!

	std::stack<Poco::XSD::Types::AnnotatedObject::Ptr> _objects;
		/// Contains all objects created during parsing for which no stateEnd was encountered.

	std::stack<Poco::XSD::Types::AnnotationContent::Ptr> _annotContent;
		/// Contains all AnnotationContent objects created during parsing for which no stateEnd was encountered.

	std::stack<Poco::XSD::Types::Annotation::Ptr> _annot;
		/// Contains all Annotationobjects created during parsing for which no stateEnd was encountered.

	std::stack<Poco::XSD::Types::All::Ptr> _all;
	std::stack<Poco::XSD::Types::Any::Ptr> _any;
	std::stack<Poco::XSD::Types::AttributeContent::Ptr> _attributes;
	std::stack<Poco::XSD::Types::Attribute::Ptr> _attributesInline;
	std::stack<Poco::XSD::Types::AbstractAttributeGroup::Ptr> _abstractAttributeGroups;
	std::stack<Poco::XSD::Types::Order::Ptr> _orders;
	std::stack<Poco::XSD::Types::OrderContent::Ptr> _orderContents;
	std::stack<Poco::XSD::Types::ComplexType::Ptr> _complexTypes;
	std::stack<Poco::XSD::Types::InheritanceInfo::Ptr> _complexInheritance;
	std::stack<Poco::XSD::Types::SimpleTypeInheritance::Ptr> _simpleInheritance;
	std::stack<Poco::XSD::Types::List::Ptr> _list;
	std::stack<Poco::XSD::Types::ListTypeRef::Ptr> _listRef;
	std::stack<Poco::XSD::Types::Group::Ptr> _group;
	std::stack<Poco::XSD::Types::Union::Ptr> _union;
	std::stack<Poco::XSD::Types::Notation::Ptr> _notation;
	std::stack<Poco::XSD::Types::SimpleType::Ptr> _simpleTypes;

	std::string _characters;
	Poco::XSD::Types::Schema::Ptr _pSchema;
	Poco::XSD::Types::Definitions::Ptr _pDefinitions;
	Poco::XSD::Types::Binding::Ptr _pBinding;
	std::map<std::string, int> _generatedTypeNames;
	
	Poco::NestedDiagnosticContext _ndc;

	const SchemaNSToLocationMap& _schemaMap;

	std::vector<Poco::XSD::Types::Schema::Ptr> _registeredSchemas;
	std::vector<Poco::XSD::Types::Definitions::Ptr> _registeredDefinitions;
		/// The schemas and definitions this handler registered with the TypesManager.

	friend class StateMachine;
	friend struct StateMachine::StateInfo;
};


//
// inlines
//
inline StateMachine::State XSDContentHandler::getParentState() const
{
	StateList::size_type s = _states.size();
	poco_assert_dbg (s > 1);
	return _states[s-2];
}


} } } // namespace Poco::XSD::Parser


#endif // XSDParser_XSDContentHandler_INCLUDED
