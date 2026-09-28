//
// XSDContentHandler.cpp
//
// Library: XSD/Parser
// Package: XSDParser
// Module:  XSDContentHandler
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Parser/XSDContentHandler.h"
#include "Poco/XSD/Parser/Constants.h"
#include "Utility.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/ElementImpl.h"
#include "Poco/XSD/Types/ElementTypeRef.h"
#include "Poco/XSD/Types/ElementRef.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/Documentation.h"
#include "Poco/XSD/Types/AppInfo.h"
#include "Poco/XSD/Types/All.h"
#include "Poco/XSD/Types/Any.h"
#include "Poco/XSD/Types/Choice.h"
#include "Poco/XSD/Types/Sequence.h"
#include "Poco/XSD/Types/Attribute.h"
#include "Poco/XSD/Types/AttributeRef.h"
#include "Poco/XSD/Types/AttributeGroup.h"
#include "Poco/XSD/Types/AttributeTypeRef.h"
#include "Poco/XSD/Types/AnyAttribute.h"
#include "Poco/XSD/Types/Group.h"
#include "Poco/XSD/Types/GroupRef.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/SimpleRestriction.h"
#include "Poco/XSD/Types/SimpleRestrictionInlineType.h"
#include "Poco/XSD/Types/ComplexType.h"
#include "Poco/XSD/Types/Message.h"
#include "Poco/XSD/Types/PortType.h"
#include "Poco/XSD/Types/Operation.h"
#include "Poco/XSD/Types/Binding.h"
#include "Poco/XSD/Types/Service.h"
#include "Poco/SAX/Locator.h"
#include "Poco/SAX/SAXParser.h"
#include "Poco/SAX/XMLReader.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/SharedPtr.h"
#include "Poco/Path.h"
#include "Poco/StringTokenizer.h"
#include "Poco/Format.h"
#include "Poco/NumberFormatter.h"


using namespace Poco::XSD::Types;
using namespace Poco::XML;


namespace Poco::XSD::Parser {


XSDContentHandler::XSDContentHandler(const Poco::URI& schemaLocation, const SchemaNSToLocationMap& schemaMap):
	_schemaLocation(schemaLocation),
	_states(),
	_schemaMap(schemaMap)
{
	_states.push_back(StateMachine::ST_INUNINITIALIZED);
	_ndc.push("Schema: " + schemaLocation.toString());
}


XSDContentHandler::~XSDContentHandler()
{
	clear();
}


void XSDContentHandler::setDocumentLocator(const Locator* loc)
{
	_pLocator = loc;
}


void XSDContentHandler::startDocument()
{
	poco_assert_dbg (_states.back() == StateMachine::ST_INUNINITIALIZED);
	// the document state can not be executed
	_states.push_back(StateMachine::ST_INDOCUMENT);
}


void XSDContentHandler::endDocument()
{
	_states.pop_back();
	poco_assert_dbg (_states.back() == StateMachine::ST_INUNINITIALIZED);

	// endDocument() is called only for a document that parsed without error, so a document
	// that fails, even after its last end tag, adds nothing to the registered schemas
	TypesManager& tm = TypesManager::instance();
	for (const auto& pSchema: _registeredSchemas)
	{
		tm.completeSchema(pSchema);
	}
}


void XSDContentHandler::startElement(const std::string& uri, const std::string& localName, const std::string& qname, const Attributes& attrList)
{
	_ndc.push(localName + " " + attrList.getValue("name"));

	_namespaces.pushContext();

	_characters.clear();
	// handlemetaany state sep.
	StateMachine::State lastState = _states.back();
	bool ok = stateMachine().stateInfo(lastState).isValidSuccessor(uri, localName);
	if (ok)
	{
		StateMachine::State newState = stateMachine().state(uri, localName, lastState);

		if (_states.back() == StateMachine::ST_INMETAANY)
			newState = StateMachine::ST_INMETAANY;
		if (newState == StateMachine::ST_INUNINITIALIZED)
		{
			if (stateMachine().stateInfo(lastState).containsAny())
				newState = StateMachine::ST_INMETAANY;
			else
				throw XSDException("Illegal Element: " + qname, location());
		}

		_states.push_back(newState);
		CompactAttributes compAttr;
		convertAttributes(attrList, compAttr);
		const StateMachine::StateInfo& info = stateMachine().stateInfo(newState);
		(this->*info._start)(uri, localName, qname, compAttr);
	}
	else
	{
		throw XSDException("Illegal Element: " + qname, location());
	}
}


void XSDContentHandler::endElement(const std::string& uri, const std::string& localName, const std::string& qname)
{
	const StateMachine::StateInfo& info = stateMachine().stateInfo(_states.back());
	(this->*info._end)(uri, localName, qname);
	_states.pop_back();
	_characters.clear();

	_ndc.pop();

	_namespaces.popContext();
}


void XSDContentHandler::characters(const XMLChar* ch, int start, int length)
{
	_characters.append(ch+start, length);
}


void XSDContentHandler::ignorableWhitespace(const XMLChar* ch, int start, int length)
{
	_characters.append(ch+start, length);
}


void XSDContentHandler::processingInstruction(const std::string& target, const std::string& data)
{
}


void XSDContentHandler::startPrefixMapping(const std::string& prefix, const std::string& uri)
{
	_namespaces.declarePrefix(prefix, uri);
}


void XSDContentHandler::endPrefixMapping(const std::string& prefix)
{
	_namespaces.undeclarePrefix(prefix);
}


void XSDContentHandler::skippedEntity(const std::string& name)
{
}


void XSDContentHandler::stateAllStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//	<all
	//    id = ID
	//    maxOccurs = 1 : 1
	//    minOccurs = (0 | 1) : 1
	//    {any attributes with non-schema namespace . . .}>
	//    Content: (annotation?, element*)
	//  </all>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	Poco::UInt32 maxOccurs = Utility::getMaxOccurs(itEnd, attrList.find(Constants::XSD_MAXOCCURS));
	poco_assert (maxOccurs == 1);
	Poco::UInt32 minOccurs = Utility::getMinOccurs(itEnd, attrList.find(Constants::XSD_MINOCCURS));
	All::Ptr ptr = new All(id, minOccurs);
	_objects.push(ptr);
	_all.push(ptr);
	_orders.push(ptr);
	_orderContents.push(ptr);
}


void XSDContentHandler::stateAnnotationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<annotation
	//  id = ID
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (appinfo | documentation)*
	//</annotation>

	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	Annotation::Ptr pAnn = new Annotation(id);
	_annot.push(pAnn);
}


void XSDContentHandler::stateAnyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<any
	//  id = ID
	//  maxOccurs = (nonNegativeInteger | unbounded)  : 1
	//  minOccurs = nonNegativeInteger : 1
	//  namespace = ((##any | ##other) | List of (anyURI | (##targetNamespace | ##local)) )  : ##any
	//  processContents = (lax | skip | strict) : strict
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</any>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	Poco::UInt32 maxOccurs = Utility::getMaxOccurs(itEnd, attrList.find(Constants::XSD_MAXOCCURS));
	Poco::UInt32 minOccurs = Utility::getMinOccurs(itEnd, attrList.find(Constants::XSD_MINOCCURS));
	const std::string& ns = Utility::getString(itEnd, attrList.find(Constants::XSD_NAMESPACE), Constants::XSD_DOUBLEHASH_ANY);
	Types::Any::ProcessStyle ps = Utility::getAnyProcessStyle(itEnd, attrList.find(Constants::XSD_PROCESSCONTENTS));
	Types::Any::Ptr ptr = new Types::Any(id, minOccurs, maxOccurs, ns, ps);
	_any.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateMetaAnyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	// truly anything!
	// we could serialize to string, but then, we would loose namespace info
	// the correct way is to build up a domtree
	// ignore for the moment!
}


void XSDContentHandler::stateAnyAttributeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<anyAttribute
	//  id = ID
	//  namespace = ((##any | ##other) | List of (anyURI | (##targetNamespace | ##local)) )  : ##any
	//  processContents = (lax | skip | strict) : strict
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</anyAttribute>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	const std::string& ns = Utility::getString(itEnd, attrList.find(Constants::XSD_NAMESPACE), Constants::XSD_DOUBLEHASH_ANY);
	AnyAttribute::ProcessStyle ps = Utility::getAnyAttrProcessStyle(itEnd, attrList.find(Constants::XSD_PROCESSCONTENTS));
	AnyAttribute::Ptr pAny(new AnyAttribute(id, ns, ps));
	_attributes.push(pAny);
	_objects.push(pAny);
}


void XSDContentHandler::stateAppInfoStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<appinfo
	//  source = anyURI
	//  {any attributes with non-schema namespace . . .}>
	//  Content: ({any})*
	//</appinfo>

	auto itEnd = attrList.end();
	const std::string& src = Utility::getString(itEnd, attrList.find(Constants::XSD_SOURCE), Constants::XSD_EMPTY_STRING);
	AnnotationContent::Ptr ptr = new AppInfo(src);
	_annotContent.push(ptr);
}


void XSDContentHandler::stateAttributeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<attribute
	//  default = string
	//  fixed = string
	//  form = (qualified | unqualified)
	//  id = ID
	//  name = NCName
	//  ref = QName
	//  type = QName
	//  use = (optional | prohibited | required) : optional
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, simpleType?)
	//</attribute>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	// do we have a ref?
	auto itRef = attrList.find(Constants::XSD_REF);
	auto itTypeRef = attrList.find(Constants::XSD_TYPE);
	AttributeContent::Ptr pAttr;
	bool inlineAttrWritten = false;
	if (itRef != itEnd)
	{
		/// a ref cannot have a type
		poco_assert_dbg(itTypeRef == itEnd);
		pAttr = new AttributeRef(id, createQName(itRef->second));
	}
	else
	{
		const std::string& fixedValue = Utility::getString(itEnd, attrList.find(Constants::XSD_FIXED), Constants::XSD_EMPTY_STRING);
		const std::string& defaultValue = Utility::getString(itEnd, attrList.find(Constants::XSD_DEFAULT), Constants::XSD_EMPTY_STRING);
		if (!fixedValue.empty() && !defaultValue.empty())
			throw SchemaException("fixed and default attribute illegal");
		bool qualified = Utility::getQualified(itEnd, attrList.find(Constants::XSD_QUALIFIED), _qualifiedAttributeForm);
		AbstractAttribute::Usage use = Utility::getAttrUsage(itEnd, attrList.find(Constants::XSD_USE), AbstractAttribute::USE_OPTIONAL);
		if (itTypeRef != itEnd)
		{
			// we use an existing type
			pAttr = new AttributeTypeRef(id, name, _pSchema->targetNamespace(), createQName(itTypeRef->second), fixedValue, defaultValue, qualified, use);
		}
		else
		{
			Attribute::Ptr ptr(new Attribute(id, name, _pSchema->targetNamespace(), fixedValue, defaultValue, qualified, use));
			inlineAttrWritten = true;
			_attributesInline.push(ptr);
			pAttr = ptr;
		}
	}
	if (!inlineAttrWritten)
		_attributesInline.push(Attribute::Ptr());
	_attributes.push(pAttr);
	_objects.push(pAttr);
}


void XSDContentHandler::stateAttributeGroupStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<attributeGroup
	//  id = ID
	//  name = NCName
	//  ref = QName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, ((attribute | attributeGroup)*, anyAttribute?))
	//</attributeGroup>

	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	// do we have a ref?
	auto itRef = attrList.find(Constants::XSD_REF);
	AbstractAttributeGroup::Ptr pGroup;
	if (itRef != itEnd)
	{
		pGroup = new AttributeGroupRef(id, createQName(itRef->second));
	}
	else
	{
		const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
		pGroup = new AttributeGroup(id, name);
	}

	_abstractAttributeGroups.push(pGroup);
	_objects.push(pGroup);
}


void XSDContentHandler::stateChoiceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<choice
	//  id = ID
	//  maxOccurs = (nonNegativeInteger | unbounded)  : 1
	//  minOccurs = nonNegativeInteger : 1
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (element | group | choice | sequence | any)*)
	//</choice>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	Poco::UInt32 maxOccurs = Utility::getMaxOccurs(itEnd, attrList.find(Constants::XSD_MAXOCCURS));
	Poco::UInt32 minOccurs = Utility::getMinOccurs(itEnd, attrList.find(Constants::XSD_MINOCCURS));
	Choice::Ptr ptr (new Choice(id, minOccurs, maxOccurs));
	_orders.push(ptr);
	_orderContents.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateComplexContentStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<complexContent
	//  id = ID
	//  mixed = boolean
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (restriction | extension))
	//</complexContent>
	//ignore id and mixed!!
	InheritanceInfo::Ptr ptr = new InheritanceInfo();
	ptr->setSimpleContent(false);
	_complexInheritance.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateComplexTypeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<complexType
	//  abstract = boolean : false
	//  block = (#all | List of (extension | restriction))
	//  final = (#all | List of (extension | restriction))
	//  id = ID
	//  mixed = boolean : false
	//  name = NCName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (simpleContent | complexContent | ((group | all | choice | sequence)?, ((attribute | attributeGroup)*, anyAttribute?))))
	//</complexType>
	if (getParentState() == StateMachine::ST_INELEMENT && _elementsImpl.top() == nullptr)
		throw Types::SchemaException("An element with a type or ref attribute must not declare an inline type");

	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	bool mixed = Utility::getBool(itEnd, attrList.find(Constants::XSD_MIXED), false);
	bool isAbstract = Utility::getBool(itEnd, attrList.find(Constants::XSD_ABSTRACT), false);
	std::string name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	if (name.empty())
	{
		if (!_elements.empty())
		{
			std::string topElemName = _elements.top()->name();
			const auto [itName, inserted] = _generatedTypeNames.try_emplace(topElemName, 0);
			if (inserted)
				name = "#" + topElemName;
			else
				name = "#" + topElemName + "_" + Poco::NumberFormatter::format(++itName->second);
		}
	}
	bool blockRestriction = _blockRestriction;
	bool blockExtension = _blockExtension;
	bool blockSubstitution = _blockSubstitution;
	Utility::getBlock(itEnd, attrList.find(Constants::XSD_BLOCK), blockRestriction, blockExtension, blockSubstitution);
	bool finalExtension = _finalExtension;
	bool finalRestriction = _finalRestriction;
	Utility::getFinal(itEnd, attrList.find(Constants::XSD_FINAL), finalRestriction, finalExtension);
	ComplexType::Ptr pComplexType(new ComplexType(id, name, isAbstract, blockExtension, blockRestriction, finalExtension, finalRestriction, mixed));
	_complexTypes.push(pComplexType);
	_objects.push(pComplexType);
}


void XSDContentHandler::stateDocumentationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<documentation
	//  source = anyURI
	//  xml:lang = language
	//  {any attributes with non-schema namespace . . .}>
	//  Content: ({any})*
	//</documentation>

	auto itEnd = attrList.end();
	const std::string& src = Utility::getString(itEnd, attrList.find(Constants::XSD_SOURCE), Constants::XSD_EMPTY_STRING);
	const std::string& lang = Utility::getString(itEnd, attrList.find(Constants::XSD_LANG), Constants::XSD_EMPTY_STRING);
	AnnotationContent::Ptr ptr = new Documentation(src, lang);
	_annotContent.push(ptr);
}


void XSDContentHandler::stateElementStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<element
	//  abstract = boolean : false
	//  block = (#all | List of (extension | restriction | substitution))
	//  default = string
	//  final = (#all | List of (extension | restriction))
	//  fixed = string
	//  form = (qualified | unqualified)
	//  id = ID
	//  maxOccurs = (nonNegativeInteger | unbounded)  : 1
	//  minOccurs = nonNegativeInteger : 1
	//  name = NCName
	//  nillable = boolean : false
	//  ref = QName
	//  substitutionGroup = QName
	//  type = QName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, ((simpleType | complexType)?, (unique | key | keyref)*))
	//</element>
	auto it = attrList.find(Constants::XSD_REF);
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	Poco::UInt32 maxOccurs = Utility::getMaxOccurs(itEnd, attrList.find(Constants::XSD_MAXOCCURS));
	Poco::UInt32 minOccurs = Utility::getMinOccurs(itEnd, attrList.find(Constants::XSD_MINOCCURS));
	Element* pElem = nullptr;
	bool pushedImpl = false;
	if (it != itEnd)
	{
		if (getParentState() == StateMachine::ST_INSCHEMA)
			throw Types::SchemaException("A top-level element declaration must not have a ref attribute");
		// a reference
		pElem = new ElementRef(id, minOccurs, maxOccurs, createQName(it->second));
	}
	else
	{
		// a normal element definition
		bool isAbstract = Utility::getBool(itEnd, attrList.find(Constants::XSD_ABSTRACT), false);
		bool blockRestriction = _blockRestriction;
		bool blockExtension = _blockExtension;
		bool blockSubstitution = _blockSubstitution;
		Utility::getBlock(itEnd, attrList.find(Constants::XSD_BLOCK), blockRestriction, blockExtension, blockSubstitution);
		const std::string& defaultValue = Utility::getString(itEnd, attrList.find(Constants::XSD_DEFAULT), Constants::XSD_EMPTY_STRING);
		bool finalExtension = _finalExtension;
		bool finalRestriction = _finalRestriction;
		Utility::getFinal(itEnd, attrList.find(Constants::XSD_FINAL), finalRestriction, finalExtension);
		const std::string& fixedValue = Utility::getString(itEnd, attrList.find(Constants::XSD_FIXED), Constants::XSD_EMPTY_STRING);
		bool qualifiedForm = Utility::getBool(itEnd, attrList.find(Constants::XSD_FORM), _qualifiedElementForm);
		const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
		bool nillable = Utility::getBool(itEnd, attrList.find(Constants::XSD_NILLABLE), false);

		it = attrList.find(Constants::XSD_SUBSTITUTIONGROUP);
		QName qName;
		if (it != itEnd)
			qName = createQName(it->second);

		// check for a type define
		it = attrList.find(Constants::XSD_TYPE);
		if (it != itEnd)
		{
			QName typeRef = createQName(it->second);
			pElem = new ElementTypeRef(id,
				minOccurs,
				maxOccurs,
				isAbstract,
				blockRestriction,
				blockExtension,
				blockSubstitution,
				defaultValue,
				finalRestriction,
				finalExtension,
				fixedValue,
				qualifiedForm,
				name,
				_pSchema->targetNamespace(),
				nillable,
				qName, typeRef);
		}
		else
		{
			auto* pElemImpl =
				new ElementImpl(id,
				minOccurs,
				maxOccurs,
				isAbstract,
				blockRestriction,
				blockExtension,
				blockSubstitution,
				defaultValue,
				finalRestriction,
				finalExtension,
				fixedValue,
				qualifiedForm,
				name,
				_pSchema->targetNamespace(),
				nillable,
				qName);
			pElem = pElemImpl;
			_elementsImpl.push(pElemImpl);
			pushedImpl = true;
		}
	}
	if (!pushedImpl)
		_elementsImpl.push(nullptr); // for a simpler elementEnd impl
	_elements.push(pElem);
	OrderContent::Ptr ptr = pElem;
	_orderContents.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateSimpleExtensionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//parent is simpleContent
	//<extension
	//  base = QName
	//  id = ID
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, ((attribute | attributeGroup)*, anyAttribute?))
	//</extension>
	auto itEnd = attrList.end();
	const std::string& base = Utility::getString(itEnd, attrList.find(Constants::XSD_BASE));
	poco_assert_dbg (_complexInheritance.top()->getSimpleContent());
	_complexInheritance.top()->setRestriction(false);
	_complexInheritance.top()->setType(createQName(base));
}


void XSDContentHandler::stateComplexExtensionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//parent is complexContent:
	//<extension
	//  base = QName
	//  id = ID
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, ((group | all | choice | sequence)?, ((attribute | attributeGroup)*, anyAttribute?)))
	//</extension>

	auto itEnd = attrList.end();
	const std::string& base = Utility::getString(itEnd, attrList.find(Constants::XSD_BASE));
	poco_assert_dbg (_complexInheritance.top()->getSimpleContent() == false);
	_complexInheritance.top()->setRestriction(false);
	_complexInheritance.top()->setType(createQName(base));
}


void XSDContentHandler::stateFieldStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<field
	//  id = ID
	//  xpath = a subset of XPath expression, see below
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</field>

	// not supported, but we have to add a dummy entry to _objects to catch possible content
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateGroupStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<group
	//  id = ID
	//  maxOccurs = (nonNegativeInteger | unbounded)  : 1
	//  minOccurs = nonNegativeInteger : 1
	//  name = NCName
	//  ref = QName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (all | choice | sequence)?)
	//</group>

	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	Poco::UInt32 maxOccurs = Utility::getMaxOccurs(itEnd, attrList.find(Constants::XSD_MAXOCCURS));
	Poco::UInt32 minOccurs = Utility::getMinOccurs(itEnd, attrList.find(Constants::XSD_MINOCCURS));
	auto itRef = attrList.find(Constants::XSD_REF);
	OrderContent::Ptr ptr;
	if (itRef != itEnd)
	{
		ptr = new GroupRef(id, createQName(itRef->second), minOccurs, maxOccurs);
		_group.push(Group::Ptr()); //push null element
	}
	else
	{
		const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
		Group::Ptr pGrp (new Group(id, name, minOccurs, maxOccurs));
		_group.push(pGrp);
		ptr = pGrp;
	}
	_orderContents.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateXSDImportStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<import
	//  id = ID
	//  namespace = anyURI
	//  schemaLocation = anyURI
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</import>

	auto itEnd = attrList.end();
	const std::string& ns = Utility::getString(itEnd, attrList.find(Constants::XSD_NAMESPACE), Constants::XSD_EMPTY_STRING);
	const std::string& loc = Utility::getString(itEnd, attrList.find(Constants::XSD_SCHEMALOCATION), Constants::XSD_EMPTY_STRING);
	Poco::URI url;
	bool imported = false;
	if (!loc.empty())
	{
		// a schemaLocation is a pair of url location
		Poco::StringTokenizer tok(loc, " ", StringTokenizer::TOK_IGNORE_EMPTY | StringTokenizer::TOK_TRIM);
		if (tok.count() == 1)
		{
			imported = true;
			url = tok[0];
			resolveSchemaLocation(url, Poco::URI(_schemaLocation));
			importXSD(url, _schemaMap);
		}
		else if (tok.count() > 1)
		{
			poco_assert (tok.count() % 2 == 0);
			for (std::size_t i = 0; i < tok.count(); i+=2)
			{
				imported = true;
				url = tok[i+1];
				resolveSchemaLocation(url, Poco::URI(_schemaLocation));
				importXSD(url, _schemaMap);
			}
		}
	}

	if (!imported)
	{
		// no namespace and no schemaLocation is allowed!
		if (!ns.empty())
		{
			if (const auto it = _schemaMap.find(ns); it != _schemaMap.end())
			{
				url = it->second;
				resolveSchemaLocation(url, Poco::URI(_schemaLocation));
				importXSD(url, _schemaMap);
			}
		}
	}

	// catch content annotation, add dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateIncludeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<include
	//  id = ID
	//  schemaLocation = anyURI
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</include>

	auto itEnd = attrList.end();
	const std::string& loc = Utility::getString(itEnd, attrList.find(Constants::XSD_SCHEMALOCATION));
	// only one single xsd entry! no uri_location pair at includes!
	Poco::URI url(loc);
	resolveSchemaLocation(url, Poco::URI(_schemaLocation));
	Schema::Ptr pSchema = loadDocument(url, _schemaMap);
	_pSchema->includeSchema(pSchema);

	// catch content annotation, add dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateKeyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<key
	//  id = ID
	//  name = NCName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (selector, field+))
	//</key>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateKeyrefStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<keyref
	//  id = ID
	//  name = NCName
	//  refer = QName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (selector, field+))
	//</keyref>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateListStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<list
	//  id = ID
	//  itemType = QName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, simpleType?)
	//</list>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	auto it = attrList.find(Constants::XSD_ITEMTYPE);

	SimpleTypeInheritance::Ptr ptr;
	ListTypeRef::Ptr pRef;
	List::Ptr pL;
	if (it != itEnd)
	{
		pRef = new ListTypeRef(id, createQName(it->second));
		ptr = pRef;
	}
	else
	{
		pL = new List(id);
		ptr = pL;
	}
	poco_assert_dbg (!pRef.isNull() || !pL.isNull());
	poco_assert_dbg (ptr);
	_listRef.push(pRef);
	_list.push(pL);
	_simpleInheritance.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateNotationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<notation
	//  id = ID
	//  name = NCName
	//  public = token
	//  system = anyURI
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</notation>

	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME));
	const std::string& pub = Utility::getString(itEnd, attrList.find(Constants::XSD_PUBLIC), Constants::XSD_EMPTY_STRING);
	const std::string& sys = Utility::getString(itEnd, attrList.find(Constants::XSD_SYSTEM), Constants::XSD_EMPTY_STRING);
	Notation::Ptr ptr(new Notation(id, name, pub, sys));
	_notation.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateRedefineStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<redefine
	//  id = ID
	//  schemaLocation = anyURI
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation | (simpleType | complexType | group | attributeGroup))*
	//</redefine>
	throw NotImplementedException("redefine not supported");
}


void XSDContentHandler::stateComplexRestrictionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//parent is complexContent:
	//<restriction
	//  base = QName
	//  id = ID
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, ((group | all | choice | sequence)?, ((attribute | attributeGroup)*, anyAttribute?)))
	//</restriction>
	auto itEnd = attrList.end();
	const std::string& base = Utility::getString(itEnd, attrList.find(Constants::XSD_BASE));
	poco_assert_dbg (_complexInheritance.top()->getSimpleContent() == false);
	_complexInheritance.top()->setRestriction(true);
	_complexInheritance.top()->setType(createQName(base));
}


void XSDContentHandler::stateSimpleContentRestrictionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	// parent is simpleContent
	//<restriction
	//  base = QName
	//  id = ID
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (simpleType?, (minExclusive | minInclusive | maxExclusive | maxInclusive | totalDigits | fractionDigits | length | minLength | maxLength | enumeration | whiteSpace | pattern)*)?, ((attribute | attributeGroup)*, anyAttribute?))
	//</restriction>
	auto itEnd = attrList.end();
	const std::string& base = Utility::getString(itEnd, attrList.find(Constants::XSD_BASE));
	poco_assert_dbg (_complexInheritance.top()->getSimpleContent());
	_complexInheritance.top()->setRestriction(true);
	_complexInheritance.top()->setType(createQName(base));
}



void XSDContentHandler::stateSimpleTypeRestrictionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	// parent is SimpleType
	//<restriction
	//  base = QName
	//  id = ID
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (simpleType?, (minExclusive | minInclusive | maxExclusive | maxInclusive | totalDigits | fractionDigits | length | minLength | maxLength | enumeration | whiteSpace | pattern)*))
	//</restriction>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	const std::string& base = Utility::getString(itEnd, attrList.find(Constants::XSD_BASE), Constants::XSD_EMPTY_STRING);
	if (base.empty())
	{
		SimpleTypeInheritance::Ptr ptr (new SimpleRestrictionInlineType(id));
		_simpleInheritance.push(ptr);
		_objects.push(ptr);
	}
	else
	{
		SimpleTypeInheritance::Ptr ptr (new SimpleRestriction(id, createQName(base)));
		_simpleInheritance.push(ptr);
		_objects.push(ptr);
	}
}


void XSDContentHandler::stateSchemaStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<schema
	//  attributeFormDefault = (qualified | unqualified) : unqualified
	//  blockDefault = (#all | List of (extension | restriction | substitution))  : ''
	//  elementFormDefault = (qualified | unqualified) : unqualified
	//  finalDefault = (#all | List of (extension | restriction | list | union))  : ''
	//  id = ID
	//  targetNamespace = anyURI
	//  version = token
	//  xml:lang = language
	//  {any attributes with non-schema namespace . . .}>
	//  Content: ((include | import | redefine | annotation)*, (((simpleType | complexType | group | attributeGroup) | element | attribute | notation), annotation*)*)
	//</schema>

	auto itEnd = attrList.end();
	_qualifiedAttributeForm = Utility::getQualified(itEnd, attrList.find(Constants::XSD_ATTRIBUTEFORMDEFAULT), false);
	Utility::getBlock(itEnd, attrList.find(Constants::XSD_BLOCKDEFAULT), _blockRestriction, _blockExtension, _blockSubstitution);
	_qualifiedElementForm = Utility::getQualified(itEnd, attrList.find(Constants::XSD_ELEMENTFORMDEFAULT), false);
	Utility::getFinalDefault(itEnd, attrList.find(Constants::XSD_FINALDEFAULT), _finalRestriction, _finalExtension, _finalList, _finalUnion);

	auto it = attrList.find(Constants::XSD_TARGETNAMESPACE);
	std::string tns;
	if (it != itEnd)
	{
		tns = it->second;
	}
	_pSchema = new Schema(tns,
					_qualifiedAttributeForm,
					_qualifiedElementForm,
					_blockExtension,
					_blockRestriction,
					_blockSubstitution,
					_finalExtension,
					_finalRestriction,
					_finalList,
					_finalUnion);

	_objects.push(_pSchema);

	TypesManager& tm = TypesManager::instance();
	if ((_pSchema->targetNamespace() != TypesManager::XSD_NAMESPACE) && (_pSchema->targetNamespace() != TypesManager::XSD_NAMESPACE1998))
	{
		tm.addSchema(_pSchema, _schemaLocation); // fails if schema exists and conflicts with the old one
		_registeredSchemas.push_back(_pSchema);
	}
}


void XSDContentHandler::stateSelectorStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<selector
	//  id = ID
	//  xpath = a subset of XPath expression, see below
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</selector>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateSequenceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<sequence
	//  id = ID
	//  maxOccurs = (nonNegativeInteger | unbounded)  : 1
	//  minOccurs = nonNegativeInteger : 1
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (element | group | choice | sequence | any)*)
	//</sequence>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	Poco::UInt32 maxOccurs = Utility::getMaxOccurs(itEnd, attrList.find(Constants::XSD_MAXOCCURS));
	Poco::UInt32 minOccurs = Utility::getMinOccurs(itEnd, attrList.find(Constants::XSD_MINOCCURS));
	Order::Ptr ptr (new Sequence(id, minOccurs, maxOccurs));
	_orders.push(ptr);
	_orderContents.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateSimpleContentStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<simpleContent
	//  id = ID
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (restriction | extension))
	//</simpleContent>

	InheritanceInfo::Ptr ptr = new InheritanceInfo();
	ptr->setSimpleContent(true);
	_complexInheritance.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateSimpleTypeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<simpleType
	//  final = (#all | List of (list | union | restriction))
	//  id = ID
	//  name = NCName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (restriction | list | union))
	//</simpleType>
	const StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INELEMENT && _elementsImpl.top() == nullptr)
		throw Types::SchemaException("An element with a type or ref attribute must not declare an inline type");
	if (parent == StateMachine::ST_INATTRIBUTE && _attributesInline.top() == nullptr)
		throw Types::SchemaException("An attribute with a type or ref attribute must not declare an inline simpleType");
	if (parent == StateMachine::ST_INSIMPLETYPERESTRICTION)
	{
		const AutoPtr<SimpleRestrictionInlineType> pRestriction = _simpleInheritance.top().cast<SimpleRestrictionInlineType>();
		if (pRestriction == nullptr)
			throw Types::SchemaException("A restriction with a base attribute must not declare an inline simpleType");
		if (pRestriction->hasInlineType())
			throw Types::SchemaException("A restriction must not declare more than one inline simpleType");
	}

	bool finalRestriction = false;
	bool finalList = false;
	bool finalUnion = false;
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	SimpleType::Ptr ptr(new SimpleType(id, name, finalRestriction, finalList, finalUnion));
	_simpleTypes.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateUnionStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<union
	//  id = ID
	//  memberTypes = List of QName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, simpleType*)
	//</union>
	auto itEnd = attrList.end();
	const std::string& id = Utility::getString(itEnd, attrList.find(Constants::XSD_ID), Constants::XSD_EMPTY_STRING);
	const std::string& memberTypes = Utility::getString(itEnd, attrList.find(Constants::XSD_MEMBERTYPES), Constants::XSD_EMPTY_STRING);
	StringTokenizer tok(memberTypes, " ", StringTokenizer::TOK_IGNORE_EMPTY|StringTokenizer::TOK_TRIM);
	std::vector<QName> qn;
	for (const auto& memberType: tok)
	{
		qn.push_back(createQName(memberType));
	}

	Union::Ptr ptr(new Union(id, qn));
	_union.push(ptr);
	_simpleInheritance.push(ptr);
	_objects.push(ptr);
}


void XSDContentHandler::stateUniqueStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<unique
	//  id = ID
	//  name = NCName
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?, (selector, field+))
	//</unique>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateEnumerationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<enumeration
	//  id = ID
	//  value = anySimpleType
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</enumeration>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateFractionDigitsStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<fractionDigits
	//  fixed = boolean : false
	//  id = ID
	//  value = nonNegativeInteger
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</fractionDigits>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateLengthStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<length
	//  fixed = boolean : false
	//  id = ID
	//  value = nonNegativeInteger
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</length>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateMaxExclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<maxExclusive
	//  fixed = boolean : false
	//  id = ID
	//  value = anySimpleType
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</maxExclusive>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateMaxInclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<maxInclusive
	//  fixed = boolean : false
	//  id = ID
	//  value = anySimpleType
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</maxInclusive>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateMaxLengthStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<maxLength
	//  fixed = boolean : false
	//  id = ID
	//  value = nonNegativeInteger
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</maxLength>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateMinExclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<minExclusive
	//  fixed = boolean : false
	//  id = ID
	//  value = anySimpleType
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</minExclusive>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateMinInclusiveStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<minInclusive
	//  fixed = boolean : false
	//  id = ID
	//  value = anySimpleType
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</minInclusive>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateMinLengthStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<minLength
	//  fixed = boolean : false
	//  id = ID
	//  value = nonNegativeInteger
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</minLength>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::statePatternStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<pattern
	//  id = ID
	//  value = string
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</pattern>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateTotalDigitsStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<totalDigits
	//  fixed = boolean : false
	//  id = ID
	//  value = positiveInteger
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</totalDigits>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateUninitializedStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	throw XSDException("Illegal startElement method call on uninitialized XSDContentHandler");
}


void XSDContentHandler::stateWhiteSpaceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<whiteSpace
	//  fixed = boolean : false
	//  id = ID
	//  value = (collapse | preserve | replace)
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</whiteSpace>

	// not supported: catch content by adding dummy entry
	_objects.push(new SimpleType());
}


void XSDContentHandler::stateAllEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert_dbg (!_all.empty());

	// pushed: _all, _objects, _orders, _orderContents
	All::Ptr pAll = _all.top();
	_all.pop();
	_objects.pop();
	_orders.pop();
	_orderContents.pop();

	// valid parents: complexType, complexExtension, group, complexRestriction
	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INCOMPLEXEXTENSION || parent == StateMachine::ST_INCOMPLEXRESTRICTION)
	{
		poco_assert_dbg (!_complexInheritance.empty());
		poco_assert_dbg (!_complexTypes.empty());
		_complexTypes.top()->setContent(pAll);
	}
	else if (parent == StateMachine::ST_INCOMPLEXTYPE)
	{
		poco_assert_dbg (!_complexTypes.empty());
		_complexTypes.top()->setContent(pAll);
	}
	else if (parent == StateMachine::ST_INGROUP)
	{
		poco_assert_dbg (!_group.empty());
		_group.top()->setChild(pAll);
	}
	else
		throw Poco::IllegalStateException("in stateAllEnd");
}



void XSDContentHandler::stateAnnotationEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert_dbg (!_annot.empty());
	poco_assert_dbg (!_objects.empty());
	_objects.top()->addAnnotation(*_annot.top());
	_annot.pop();
}


void XSDContentHandler::stateAnyEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// pushed: _any ,_objects
	Types::Any::Ptr ptr = _any.top();
	_any.pop();
	_objects.pop();

	// valid parents: choice, sequence
	poco_assert_dbg (getParentState() == StateMachine::ST_INCHOICE || getParentState() ==  StateMachine::ST_INSEQUENCE);

	_orders.top()->add(ptr);
}


void XSDContentHandler::stateAnyAttributeEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// pushed: _attributes, _objects
	AttributeContent::Ptr ptr = _attributes.top();
	_attributes.pop();
	_objects.pop();

	// valid parents: attributeGroup, complexType, simpleExtension, complexExtension, simpleContentRestriction, complexRestriction
	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INATTRIBUTEGROUP)
	{
		_abstractAttributeGroups.top()->add(ptr);
	}
	else if (parent == StateMachine::ST_INCOMPLEXTYPE || parent == StateMachine::ST_INSIMPLEEXTENSION
		|| parent == StateMachine::ST_INCOMPLEXEXTENSION || parent == StateMachine::ST_INSIMPLECONTENTRESTRICTION
		|| parent == StateMachine::ST_INCOMPLEXRESTRICTION)
	{
		_complexTypes.top()->addAttribute(ptr);
	}
	else
	{
		// we do not support restriction
		throw Poco::IllegalStateException("Unsupported anyAttribute in restriction");
	}
}


void XSDContentHandler::stateAppInfoEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert_dbg (!_annotContent.empty());
	poco_assert_dbg (!_annot.empty());
	/// stored in _annotContent
	// parent stored in _annot
	AnnotationContent::Ptr ptr = _annotContent.top();
	ptr->setData(_characters);
	_annot.top()->annotationContent().push_back(ptr);
	_annotContent.pop();
}


void XSDContentHandler::stateAttributeEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: schema attributeGroup complexType simpleContentExtension complexContentExtension complexContentRestriction simpleContentRestriction

	// pushed: _attributes, _objects, _attributesInline
	AttributeContent::Ptr ptr = _attributes.top();
	_attributes.pop();
	_objects.pop();
	_attributesInline.pop();
	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INATTRIBUTEGROUP)
	{
		_abstractAttributeGroups.top()->add(ptr);
	}
	else if (parent == StateMachine::ST_INCOMPLEXTYPE || parent == StateMachine::ST_INSIMPLEEXTENSION
		|| parent == StateMachine::ST_INCOMPLEXEXTENSION || parent == StateMachine::ST_INSIMPLECONTENTRESTRICTION
		|| parent == StateMachine::ST_INCOMPLEXRESTRICTION)
	{
		_complexTypes.top()->addAttribute(ptr);
	}
	else if (parent == StateMachine::ST_INSCHEMA)
	{
		_pSchema->addAttribute(ptr.cast<AbstractAttribute>());
	}
	else
		throw Poco::IllegalStateException("Unsupported/illegal schema");
}


void XSDContentHandler::stateAttributeGroupEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: attributeGroup complexType simpleContentExtension complexContentExtension refine(NOTSUP)
	// complexContentRestriction simpleContentRestriction schema

	// pushed: _abstractAttributeGroups, _objects
	AbstractAttributeGroup::Ptr ptr = _abstractAttributeGroups.top();
	_abstractAttributeGroups.pop();
	_objects.pop();
	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INATTRIBUTEGROUP)
	{
		_abstractAttributeGroups.top()->add(ptr);
	}
	else if (parent == StateMachine::ST_INCOMPLEXTYPE || parent == StateMachine::ST_INSIMPLEEXTENSION
		|| parent == StateMachine::ST_INCOMPLEXEXTENSION || parent == StateMachine::ST_INSIMPLECONTENTRESTRICTION
		|| parent == StateMachine::ST_INCOMPLEXRESTRICTION)
	{
		_complexTypes.top()->addAttribute(ptr);
	}
	else if (parent == StateMachine::ST_INSCHEMA)
	{
		_pSchema->addAttributeGroup(ptr);
	}
	else
		throw Poco::IllegalStateException("Unsupported/illegal schema");
}


void XSDContentHandler::stateChoiceEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: choice complexType complexContentExtension group complexContentRestriction sequence

	// pushed: _orders _orderContents _objects
	Order::Ptr ptr = _orders.top();
	OrderContent::Ptr pContent = _orderContents.top();
	_orders.pop();
	_orderContents.pop();
	_objects.pop();
	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INCHOICE || parent == StateMachine::ST_INSEQUENCE)
	{
		_orders.top()->add(pContent);
	}
	else if (parent == StateMachine::ST_INGROUP)
	{
		poco_assert_dbg(!_group.top()->getChild());
		_group.top()->setChild(ptr);
	}
	else if (parent == StateMachine::ST_INCOMPLEXTYPE || parent == StateMachine::ST_INCOMPLEXEXTENSION
		|| parent == StateMachine::ST_INCOMPLEXRESTRICTION)
	{
		_complexTypes.top()->setContent(pContent);
	}
	else
		throw Poco::IllegalStateException("Unsupported/illegal schema");
}


void XSDContentHandler::stateComplexContentEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: complexType

	// pushed: _complexInheritance _objects

	poco_assert_dbg (!_complexInheritance.empty());
	InheritanceInfo::Ptr ptr = _complexInheritance.top();
	_complexInheritance.pop();
	_objects.pop();

	poco_assert_dbg (getParentState() == StateMachine::ST_INCOMPLEXTYPE);
	poco_assert_dbg (!_complexTypes.empty());
	_complexTypes.top()->setParent(ptr);
}


void XSDContentHandler::stateComplexTypeEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: element redefine(NOTSUP) schema

	// pushed: _complexTypes, _objects

	poco_assert_dbg (!_complexTypes.empty());
	ComplexType::Ptr ptr = _complexTypes.top();
	_complexTypes.pop();
	_objects.pop();

	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INELEMENT)
	{
		poco_assert_dbg (!_elementsImpl.empty());
		_elementsImpl.top()->setType(ptr);
		_pSchema->addType(ptr);
	}
	else if (parent == StateMachine::ST_INSCHEMA)
	{
		_pSchema->addType(ptr);
	}
	else throw Poco::IllegalStateException("Unsupported/illegal schema");
}


void XSDContentHandler::stateDocumentationEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// in case of a misplaced xsd:documentation, _annotContent and _annot will be empty.
	if (!_annotContent.empty() && !_annot.empty())
	{
		// stored in _annotContent
		// parent stored in _annot
		AnnotationContent::Ptr ptr = _annotContent.top();
		ptr->setData(_characters);
		_annot.top()->annotationContent().push_back(ptr);
		_annotContent.pop();
	}
}


void XSDContentHandler::stateElementEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: all choice schema sequence

	// pushed: _elementsImpl, _elements, _orderContents, _objects
	poco_assert_dbg (!_elementsImpl.empty());
	poco_assert_dbg (!_elements.empty());
	poco_assert_dbg (!_orderContents.empty());
	Element* pElem = _elements.top();
	OrderContent::Ptr pOrder = _orderContents.top();
	_orderContents.pop();
	_elements.pop();
	_elementsImpl.pop();

	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INALL || parent == StateMachine::ST_INCHOICE || parent == StateMachine::ST_INSEQUENCE)
	{
		poco_assert_dbg (!_orders.empty());
		_orders.top()->add(pOrder);
	}
	else if (parent == StateMachine::ST_INSCHEMA)
	{
		_pSchema->addElement(Element::Ptr(pElem, true));
	}
	else
		throw Poco::IllegalStateException("Illegal schema");

	// pop _objects at the end
	_objects.pop();
}


void XSDContentHandler::stateSimpleExtensionEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: simpleContent

	// pushed: [nothing]

	//nothing to do
}


void XSDContentHandler::stateComplexExtensionEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: complexContent

	// pushed: [nothing]

	//nothing to do
}


void XSDContentHandler::stateFieldEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported, but we had to add a dummy entry to _objects to catch possible content
	_objects.pop();
}


void XSDContentHandler::stateGroupEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: choice complexType complexContentExtension redefine (NOTSUP) complexContentRestriction schema sequence

	// pushed: _group (null for GroupRef), _orderContents, _objects
	poco_assert_dbg (!_group.empty());
	poco_assert_dbg (!_orderContents.empty());
	Group::Ptr ptr = _group.top();
	OrderContent::Ptr pContent = _orderContents.top();
	_group.pop();
	_orderContents.pop();
	_objects.pop();

	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INCHOICE || parent == StateMachine::ST_INSEQUENCE)
	{
		poco_assert_dbg (!_orders.empty());
		_orders.top()->add(pContent);
	}
	else if (parent == StateMachine::ST_INSCHEMA)
	{
		poco_check_ptr (ptr);
		_pSchema->addGroup(ptr);
	}
	else if (parent == StateMachine::ST_INCOMPLEXRESTRICTION || parent == StateMachine::ST_INCOMPLEXEXTENSION
		|| parent == StateMachine::ST_INCOMPLEXTYPE)
	{
		poco_assert_dbg (!_complexTypes.empty());
		_complexTypes.top()->setContent(pContent);
	}
	else
		throw Poco::IllegalStateException("Illegal schema");
}


void XSDContentHandler::stateXSDImportEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// pop dummy entry
	_objects.pop();
}


void XSDContentHandler::stateIncludeEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// pop dummy entry
	_objects.pop();
}


void XSDContentHandler::stateKeyEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateKeyrefEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateListEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: SimpleType

	// pushed: _listRef, _list _simpleInheritance _objects
	// only one of _listref,_list is not null
	poco_assert_dbg (!_listRef.empty());
	poco_assert_dbg (!_list.empty());
	poco_assert_dbg (!_simpleInheritance.empty());
	poco_assert_dbg (!_simpleTypes.empty());
	poco_assert_dbg (getParentState() == StateMachine::ST_INSIMPLETYPE);

	SimpleTypeInheritance::Ptr ptr = _simpleInheritance.top();
	_simpleInheritance.pop();
	_listRef.pop();
	_list.pop();
	_objects.pop();

	_simpleTypes.top()->setContent(ptr);
}


void XSDContentHandler::stateNotationEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: schema

	// pushed: _notation, _objects
	poco_assert_dbg (_notation.empty());
	poco_assert_dbg (getParentState() == StateMachine::ST_INSCHEMA);
	_pSchema->addNotation(_notation.top());
	_notation.pop();
	_objects.pop();
}


void XSDContentHandler::stateRedefineEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported state
	throw Poco::NotImplementedException();
}


void XSDContentHandler::stateSimpleTypeRestrictionEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: SimpleType

	// pushed: _simpleInheritance._objects
	poco_assert_dbg (getParentState() == StateMachine::ST_INSIMPLETYPE);
	poco_assert_dbg (!_simpleTypes.empty());
	poco_assert_dbg (!_simpleInheritance.empty());

	SimpleTypeInheritance::Ptr ptr = _simpleInheritance.top();
	_simpleInheritance.pop();
	_objects.pop();

	_simpleTypes.top()->setContent(ptr);
}


void XSDContentHandler::stateSimpleContentRestrictionEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: SimpleContent

	// pushed: [nothing]

	//nothing to do
}


void XSDContentHandler::stateComplexRestrictionEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: ComplexContent

	// pushed: [nothing]

	//nothing to do
}


void XSDContentHandler::stateSchemaEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	_objects.pop();
	poco_assert_dbg (_objects.empty());
	poco_assert_dbg (_any.empty());
	poco_assert_dbg (_elements.empty());
	poco_assert_dbg (_elementsImpl.empty());
	poco_assert_dbg (_annotContent.empty());
	poco_assert_dbg (_annot.empty());
	poco_assert_dbg (_all.empty());
	poco_assert_dbg (_attributes.empty());
	poco_assert_dbg (_orders.empty());
	poco_assert_dbg (_orderContents.empty());
	poco_assert_dbg (_complexTypes.empty());
	poco_assert_dbg (_complexInheritance.empty());
	poco_assert_dbg (_simpleInheritance.empty());
	poco_assert_dbg (_list.empty());
	poco_assert_dbg (_listRef.empty());
	poco_assert_dbg (_union.empty());
	poco_assert_dbg (_group.empty());
	poco_assert_dbg (_notation.empty());
	poco_assert_dbg (_simpleTypes.empty());
	poco_assert_dbg (_attributesInline.empty());
}


void XSDContentHandler::stateSelectorEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateSequenceEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: choice complexType complexContentExtension group complexContentRestriction sequence

	// pushed: _orders _orderContents _objects
	poco_assert_dbg (!_orders.empty());
	poco_assert_dbg (!_orderContents.empty());
	OrderContent::Ptr ptr = _orderContents.top();
	Order::Ptr pOrder = _orders.top();
	_orderContents.pop();
	_orders.pop();
	_objects.pop();

	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INCHOICE)
	{
		poco_assert_dbg (!_orders.empty());
		_orders.top()->add(ptr);
	}
	else if (parent == StateMachine::ST_INCOMPLEXTYPE || parent == StateMachine::ST_INCOMPLEXEXTENSION
		|| parent == StateMachine::ST_INCOMPLEXRESTRICTION)
	{
		poco_assert_dbg (!_complexTypes.empty());
		_complexTypes.top()->setContent(ptr);
	}
	else if (parent == StateMachine::ST_INGROUP)
	{
		poco_assert_dbg (!_group.empty());
		_group.top()->setChild(pOrder);
	}
	else if (parent == StateMachine::ST_INSEQUENCE)
	{
		poco_assert_dbg (!_orders.empty());
		_orders.top()->add(ptr);
	}
	else
		throw Poco::IllegalStateException("Illegal schema");
}


void XSDContentHandler::stateSimpleContentEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: complexType

	// pushed: _complexInheritance, _objects
	poco_assert_dbg (!_complexTypes.empty());
	poco_assert_dbg (!_complexInheritance.empty());
	poco_assert_dbg (getParentState() == StateMachine::ST_INCOMPLEXTYPE);

	_complexTypes.top()->setParent(_complexInheritance.top());
	_complexInheritance.pop();
	_objects.pop();
}


void XSDContentHandler::stateSimpleTypeEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// valid parents: union attribute element list redefine(NOTSUP) simpleContentRestriction simpleTypeRestriction schema

	// pushed: _simpleTypes, _objects
	poco_assert_dbg (!_simpleTypes.empty());
	SimpleType::Ptr ptr = _simpleTypes.top();
	_simpleTypes.pop();
	_objects.pop();

	StateMachine::State parent = getParentState();
	if (parent == StateMachine::ST_INUNION)
	{
		poco_assert_dbg (!_union.empty());
		_union.top()->inlineTypes().push_back(ptr);
	}
	else if (parent == StateMachine::ST_INATTRIBUTE)
	{
		poco_assert_dbg (!_attributesInline.empty());
		_attributesInline.top()->setType(ptr);
	}
	else if (parent == StateMachine::ST_INELEMENT)
	{
		poco_assert_dbg(!_elementsImpl.empty());
		_elementsImpl.top()->setType(ptr);
	}
	else if (parent == StateMachine::ST_INLIST)
	{
		poco_assert_dbg (!_list.empty());
		_list.top()->setType(ptr);
	}
	else if (parent == StateMachine::ST_INSIMPLECONTENTRESTRICTION)
	{
		poco_assert_dbg (!_complexInheritance.empty());
		_complexInheritance.top()->setRestrictionType(ptr);
	}
	else if (parent == StateMachine::ST_INSIMPLETYPERESTRICTION)
	{
		poco_assert_dbg (!_simpleInheritance.empty());
		_simpleInheritance.top().cast<SimpleRestrictionInlineType>()->setType(ptr);
	}
	else if (parent == StateMachine::ST_INSCHEMA)
	{
		_pSchema->addType(ptr);
	}
	else
		throw Poco::IllegalStateException("Illegal schema");
}


void XSDContentHandler::stateUnionEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	//valid parents: SimpleType
	poco_assert_dbg (getParentState() == StateMachine::ST_INSIMPLETYPE);
	poco_assert_dbg(!_union.empty());

	// union pushed: _union, _simpleInheritance, _objects
	Union::Ptr ptr = _union.top();
	_union.pop();
	_simpleInheritance.pop();
	_objects.pop();
	SimpleType::Ptr pSimple = _simpleTypes.top();
	pSimple->setContent(ptr);
}


void XSDContentHandler::stateUniqueEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateEnumerationEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateFractionDigitsEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateLengthEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateMaxExclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateMaxInclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateMaxLengthEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateMinExclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateMinInclusiveEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateMinLengthEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::statePatternEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateTotalDigitsEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateUninitializedEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	throw XSDException("Illegal endElement method call on uninitialized XSDContentHandler");
}


void XSDContentHandler::stateWhiteSpaceEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	// not supported: catched content by adding dummy entry
	_objects.pop();
}


void XSDContentHandler::stateMetaAnyEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert_dbg(_states.size() > 1);
	if (_states[_states.size()-2] != StateMachine::ST_INMETAANY)
	{
		if (!_annotContent.empty())
		{
			AnnotationContent::Ptr& ptr = _annotContent.top();
			ptr->setData(_characters);
		}
	}
}


void XSDContentHandler::stateDefinitionsStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	auto itEnd = attrList.end();
	const std::string& tns = Utility::getString(itEnd, attrList.find(Constants::XSD_TARGETNAMESPACE), Constants::XSD_EMPTY_STRING);
	if (tns.empty()) throw Types::SchemaException("No targetNamespace attribute in definitions element");
	_pDefinitions = new Types::Definitions(tns);
	Types::TypesManager::instance().addDefinitions(_pDefinitions);
	_registeredDefinitions.push_back(_pDefinitions);
}


void XSDContentHandler::stateDefinitionsEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	_pDefinitions = nullptr;
}


void XSDContentHandler::stateWSDLImportStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<import
	//  id = ID
	//  namespace = anyURI
	//  schemaLocation = anyURI
	//  {any attributes with non-schema namespace . . .}>
	//  Content: (annotation?)
	//</import>

	auto itEnd = attrList.end();
	const std::string& ns = Utility::getString(itEnd, attrList.find(Constants::WSDL_NAMESPACE), Constants::XSD_EMPTY_STRING);
	const std::string& loc = Utility::getString(itEnd, attrList.find(Constants::WSDL_LOCATION), Constants::XSD_EMPTY_STRING);
	Poco::URI url(loc);
	if (!loc.empty())
	{
		resolveSchemaLocation(url, Poco::URI(_schemaLocation));
		importWSDL(ns, url, _schemaMap);
	}
}


void XSDContentHandler::stateWSDLImportEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateTypesStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
}


void XSDContentHandler::stateTypesEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateMessageStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	if (name.empty()) throw Types::SchemaException("No name attribute in message element");
	_objects.push(new Types::Message(name));
}


void XSDContentHandler::stateMessageEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert (!_objects.empty());
	Types::Message::Ptr pMessage = _objects.top().cast<Types::Message>();
	poco_check_ptr(pMessage);
	_pDefinitions->addMessage(pMessage);
	_objects.pop();
}


void XSDContentHandler::statePartStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Message::Ptr pMessage = _objects.top().cast<Types::Message>();
	if (pMessage)
	{
		auto itEnd = attrList.end();
		const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
		const std::string& elemq = Utility::getString(itEnd, attrList.find(Constants::XSD_ELEMENT), Constants::XSD_EMPTY_STRING);
		const std::string& typeq = Utility::getString(itEnd, attrList.find(Constants::XSD_TYPE), Constants::XSD_EMPTY_STRING);
		if (!elemq.empty())
		{
			const XML::Name element = resolveName(elemq);
			pMessage->addElementPart(name, element);
		}
		else if (!typeq.empty())
		{
			const XML::Name type = resolveName(typeq);
			pMessage->addTypePart(name, type);
		}
		else throw Types::SchemaException("Message part has neither element nor type attribute", name);
	}
}


void XSDContentHandler::statePartEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::statePortTypeStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	if (name.empty()) throw Types::SchemaException("No name attribute in portType element");
	_objects.push(new Types::PortType(name));
}


void XSDContentHandler::statePortTypeEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert (!_objects.empty());
	Types::PortType::Ptr pPortType = _objects.top().cast<Types::PortType>();
	poco_check_ptr(pPortType);
	_pDefinitions->addPortType(pPortType);
	_objects.pop();
}


void XSDContentHandler::stateOperationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	if (name.empty()) throw Types::SchemaException("No name attribute in operation element");

	const std::string& parameterOrder = Utility::getString(itEnd, attrList.find(Constants::WSDL_PARAMETER_ORDER), Constants::XSD_EMPTY_STRING);

  	poco_assert (!_objects.empty());
	Types::PortType::Ptr pPortType = _objects.top().cast<Types::PortType>();
	Types::Binding::Ptr pBinding = _objects.top().cast<Types::Binding>();
	if (!pPortType)
	{
		poco_check_ptr (pBinding);
		pPortType = pBinding->getPortType();
	}
	Types::Operation::Ptr pOperation;
	if (pBinding)
	{
		pOperation = pPortType->findOperation(name);
		if (!pOperation) throw Types::SchemaException("Operation in binding references non-existing operation in PortType", name);
	}
	else
	{
		pOperation = new Types::Operation(name);
		pOperation->setParameterOrder(parameterOrder);
	}
	_objects.push(pOperation);
}


void XSDContentHandler::stateOperationEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);
	_objects.pop();

	poco_assert (!_objects.empty());
	Types::PortType::Ptr pPortType = _objects.top().cast<Types::PortType>();
	Types::Binding::Ptr pBinding = _objects.top().cast<Types::Binding>();
	if (!pPortType)
	{
		poco_check_ptr (pBinding);
		pPortType = pBinding->getPortType();
	}
	poco_check_ptr (pPortType);
	if (!pBinding)
	{
		pPortType->addOperation(pOperation);
	}
}


void XSDContentHandler::stateInputStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	const std::string& message = Utility::getString(itEnd, attrList.find(Constants::WSDL_MESSAGE), Constants::XSD_EMPTY_STRING);
	const std::string& wsaAction = Utility::getString(itEnd, attrList.find(Constants::WSA_ACTION), Constants::XSD_EMPTY_STRING);
	pOperation->setInputName(name);
	if (!message.empty())
	{
		const XML::Name element = resolveName(message);
		pOperation->setInputMessage(element);
	}
	pOperation->inputBindingProperties().set("wsa.action", wsaAction);
}


void XSDContentHandler::stateInputEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateOutputStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	const std::string& message = Utility::getString(itEnd, attrList.find(Constants::WSDL_MESSAGE), Constants::XSD_EMPTY_STRING);
	const std::string& wsaAction = Utility::getString(itEnd, attrList.find(Constants::WSA_ACTION), Constants::XSD_EMPTY_STRING);
	pOperation->setOutputName(name);
	if (!message.empty())
	{
		const XML::Name element = resolveName(message);
		pOperation->setOutputMessage(element);
	}
	pOperation->outputBindingProperties().set("wsa.action", wsaAction);
}


void XSDContentHandler::stateOutputEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateFaultStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	const std::string& message = Utility::getString(itEnd, attrList.find(Constants::WSDL_MESSAGE), Constants::XSD_EMPTY_STRING);
	const std::string& wsaAction = Utility::getString(itEnd, attrList.find(Constants::WSA_ACTION), Constants::XSD_EMPTY_STRING);
	pOperation->setFaultName(name);
	if (!message.empty())
	{
		const XML::Name element = resolveName(message);
		pOperation->setFaultMessage(element);
	}
	pOperation->outputBindingProperties().set("wsa.action", wsaAction);
}


void XSDContentHandler::stateFaultEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateBindingStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	if (name.empty()) throw Types::SchemaException("No name attribute in binding element");
	const std::string& type = Utility::getString(itEnd, attrList.find(Constants::XSD_TYPE), Constants::XSD_EMPTY_STRING);
	if (type.empty()) throw Types::SchemaException("No type attribute in binding element");
	Types::Binding::Ptr pBinding = new Types::Binding(name);

	const XML::Name portTypeName = resolveName(type);

	const Types::Definitions& defs = Types::TypesManager::instance().getDefinitions(portTypeName.namespaceURI());
	const Types::Definitions::PortTypes& portTypes = defs.portTypes();
	auto it = portTypes.find(portTypeName.localName());
	if (it == portTypes.end()) throw Poco::NotFoundException("Port type: " + portTypeName.localName() + " in namespace: " + portTypeName.namespaceURI() + " referenced by binding", name);

	pBinding->setPortType(it->second);

	_objects.push(pBinding);
	_pBinding = pBinding;
}


void XSDContentHandler::stateBindingEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert (!_objects.empty());
	Types::Binding::Ptr pBinding = _objects.top().cast<Types::Binding>();
	poco_check_ptr(pBinding);
	_pDefinitions->addBinding(pBinding);
	_objects.pop();
	_pBinding = nullptr;
}


void XSDContentHandler::stateSoapBindingStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_check_ptr (_pBinding);

	auto itEnd = attrList.end();
	std::string style = Utility::getString(itEnd, attrList.find(Constants::SOAP_STYLE), "document");
	std::string transport = Utility::getString(itEnd, attrList.find(Constants::SOAP_TRANSPORT), Constants::XSD_EMPTY_STRING);
	if (transport.empty()) throw Types::SchemaException("No transport attribute in SOAP binding element");

	_pBinding->bindingProperties().set("soap.style", style);
	_pBinding->bindingProperties().set("soap.transport", transport);
	_pBinding->bindingProperties().set("soap.version", uri);
}


void XSDContentHandler::stateSoapBindingEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateSoapOperationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& soapAction = Utility::getString(itEnd, attrList.find(Constants::SOAP_ACTION), Constants::XSD_EMPTY_STRING);
	const std::string& style = Utility::getString(itEnd, attrList.find(Constants::SOAP_STYLE), Constants::XSD_EMPTY_STRING);
	pOperation->bindingProperties().set("soap.soapAction", soapAction);
	pOperation->bindingProperties().set("soap.style", style);
}


void XSDContentHandler::stateSoapOperationEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateSoapHeaderStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& message = Utility::getString(itEnd, attrList.find(Constants::SOAP_MESSAGE), Constants::XSD_EMPTY_STRING);
	const XML::Name messageName = resolveName(message);
	const std::string& part = Utility::getString(itEnd, attrList.find(Constants::SOAP_PART), Constants::XSD_EMPTY_STRING);
	const std::string& use = Utility::getString(itEnd, attrList.find(Constants::SOAP_USE), Constants::XSD_EMPTY_STRING);
	const std::string& encodingStyle = Utility::getString(itEnd, attrList.find(Constants::SOAP_ENCODINGSTYLE), Constants::XSD_EMPTY_STRING);
	const std::string& nameSpace = Utility::getString(itEnd, attrList.find(Constants::SOAP_NAMESPACE), Constants::XSD_EMPTY_STRING);

	Types::BindingProperties& bindingProps = operationBindingProperties(*pOperation);

	int index = 0;
	while (bindingProps.has(Poco::format("soap.header[%d].message", index))) index++;

	bindingProps.set(Poco::format("soap.header[%d].message", index), message);
	bindingProps.set(Poco::format("soap.header[%d].message.localName", index), messageName.localName());
	bindingProps.set(Poco::format("soap.header[%d].message.namespaceURI", index), messageName.namespaceURI());
	bindingProps.set(Poco::format("soap.header[%d].part", index), part);
	bindingProps.set(Poco::format("soap.header[%d].use", index), use);
	bindingProps.set(Poco::format("soap.header[%d].encodingStyle", index), encodingStyle);
	bindingProps.set(Poco::format("soap.header[%d].namespace", index), nameSpace);
	bindingProps.set(Poco::format("soap.header[%d].soapVersion", index), uri);
}


void XSDContentHandler::stateSoapHeaderEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateSoapHeaderFaultStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& message = Utility::getString(itEnd, attrList.find(Constants::SOAP_MESSAGE), Constants::XSD_EMPTY_STRING);
	const XML::Name messageName = resolveName(message);
	const std::string& part = Utility::getString(itEnd, attrList.find(Constants::SOAP_PART), Constants::XSD_EMPTY_STRING);
	const std::string& use = Utility::getString(itEnd, attrList.find(Constants::SOAP_USE), Constants::XSD_EMPTY_STRING);
	const std::string& encodingStyle = Utility::getString(itEnd, attrList.find(Constants::SOAP_ENCODINGSTYLE), Constants::XSD_EMPTY_STRING);
	const std::string& nameSpace = Utility::getString(itEnd, attrList.find(Constants::SOAP_NAMESPACE), Constants::XSD_EMPTY_STRING);

	Types::BindingProperties& bindingProps = operationBindingProperties(*pOperation);

	int headerCount = 0;
	while (bindingProps.has(Poco::format("soap.header[%d].message", headerCount))) headerCount++;
	if (headerCount == 0) throw Types::SchemaException("soap:headerfault outside soap:header");
	const int index = headerCount - 1;

	bindingProps.set(Poco::format("soap.header[%d].headerfault.message", index), message);
	bindingProps.set(Poco::format("soap.header[%d].headerfault.message.localName", index), messageName.localName());
	bindingProps.set(Poco::format("soap.header[%d].headerfault.message.namespaceURI", index), messageName.namespaceURI());
	bindingProps.set(Poco::format("soap.header[%d].headerfault.part", index), part);
	bindingProps.set(Poco::format("soap.header[%d].headerfault.use", index), use);
	bindingProps.set(Poco::format("soap.header[%d].headerfault.encodingStyle", index), encodingStyle);
	bindingProps.set(Poco::format("soap.header[%d].headerfault.namespace", index), nameSpace);
	bindingProps.set(Poco::format("soap.header[%d].headerfault.soapVersion", index), uri);
}


void XSDContentHandler::stateSoapHeaderFaultEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateSoapBodyStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& parts = Utility::getString(itEnd, attrList.find(Constants::SOAP_PARTS), Constants::XSD_EMPTY_STRING);
	const std::string& use = Utility::getString(itEnd, attrList.find(Constants::SOAP_USE), Constants::XSD_EMPTY_STRING);
	const std::string& encodingStyle = Utility::getString(itEnd, attrList.find(Constants::SOAP_ENCODINGSTYLE), Constants::XSD_EMPTY_STRING);
	const std::string& nameSpace = Utility::getString(itEnd, attrList.find(Constants::SOAP_NAMESPACE), Constants::XSD_EMPTY_STRING);

	Types::BindingProperties& bindingProps = operationBindingProperties(*pOperation);
	bindingProps.set("soap.body.parts", parts);
	bindingProps.set("soap.body.use", use);
	bindingProps.set("soap.body.encodingStyle", encodingStyle);
	bindingProps.set("soap.body.namespace", nameSpace);
}


void XSDContentHandler::stateSoapBodyEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateSoapFaultStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	poco_assert (!_objects.empty());
	Types::Operation::Ptr pOperation = _objects.top().cast<Types::Operation>();
	poco_check_ptr(pOperation);

	auto itEnd = attrList.end();
	const std::string& parts = Utility::getString(itEnd, attrList.find(Constants::SOAP_PARTS), Constants::XSD_EMPTY_STRING);
	const std::string& use = Utility::getString(itEnd, attrList.find(Constants::SOAP_USE), Constants::XSD_EMPTY_STRING);
	const std::string& encodingStyle = Utility::getString(itEnd, attrList.find(Constants::SOAP_ENCODINGSTYLE), Constants::XSD_EMPTY_STRING);
	const std::string& nameSpace = Utility::getString(itEnd, attrList.find(Constants::SOAP_NAMESPACE), Constants::XSD_EMPTY_STRING);

	Types::BindingProperties& bindingProps = operationBindingProperties(*pOperation);
	bindingProps.set("soap.fault.parts", parts);
	bindingProps.set("soap.fault.use", use);
	bindingProps.set("soap.fault.encodingStyle", encodingStyle);
	bindingProps.set("soap.fault.namespace", nameSpace);
}


void XSDContentHandler::stateSoapFaultEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateServiceStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	if (name.empty()) throw Types::SchemaException("No name attribute in service element");
	Types::Service::Ptr pService = new Types::Service(name);
	_objects.push(pService);
}


void XSDContentHandler::stateServiceEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert (!_objects.empty());
	Types::Service::Ptr pService = _objects.top().cast<Types::Service>();
	poco_check_ptr(pService);
	_pDefinitions->addService(pService);
	_objects.pop();
}


void XSDContentHandler::statePortStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	auto itEnd = attrList.end();
	const std::string& name = Utility::getString(itEnd, attrList.find(Constants::XSD_NAME), Constants::XSD_EMPTY_STRING);
	if (name.empty()) throw Types::SchemaException("No name attribute in port element");
	const std::string& bindingq = Utility::getString(itEnd, attrList.find(Constants::WSDL_BINDING), Constants::XSD_EMPTY_STRING);
	if (bindingq.empty()) throw Types::SchemaException("No binding attribute in port element");
	const XML::Name binding = resolveName(bindingq);

	poco_assert (!_objects.empty());
	Types::Service::Ptr pService = _objects.top().cast<Types::Service>();
	poco_check_ptr (pService);

	pService->addPort(name, binding);
}


void XSDContentHandler::statePortEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateSoapAddressStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
}


void XSDContentHandler::stateSoapAddressEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
}


void XSDContentHandler::stateWSDLDocumentationStart(const std::string& uri, const std::string& localName, const std::string& qname, const CompactAttributes& attrList)
{
	//<documentation
	//  source = anyURI
	//  xml:lang = language
	//  {any attributes with non-schema namespace . . .}>
	//  Content: ({any})*
	//</documentation>

	Annotation::Ptr pAnn = new Annotation("");
	_annot.push(pAnn);

	auto itEnd = attrList.end();
	const std::string& src = Utility::getString(itEnd, attrList.find(Constants::XSD_SOURCE), Constants::XSD_EMPTY_STRING);
	const std::string& lang = Utility::getString(itEnd, attrList.find(Constants::XSD_LANG), Constants::XSD_EMPTY_STRING);
	AnnotationContent::Ptr ptr = new Documentation(src, lang);
	_annotContent.push(ptr);
}


void XSDContentHandler::stateWSDLDocumentationEnd(const std::string& uri, const std::string& localName, const std::string& qname)
{
	poco_assert_dbg (!_annotContent.empty());
	poco_assert_dbg (!_annot.empty());

	AnnotationContent::Ptr ptr = _annotContent.top();
	ptr->setData(_characters);
	_annot.top()->annotationContent().push_back(ptr);
	_annotContent.pop();

	poco_assert_dbg (!_annot.empty());
	if (!_objects.empty()) // in case we have documentation under the definitions element there'll be no object
	{
		_objects.top()->addAnnotation(*_annot.top());
	}
	_annot.pop();
}


std::string XSDContentHandler::location() const
{
	std::string loc;
	if (_pLocator)
	{
		loc += Poco::format("<%s>, line %d, col %d",
			_pLocator->getSystemId(),
			_pLocator->getLineNumber(),
			_pLocator->getColumnNumber());
	}
	else
	{
		loc = "Unknown location";
	}
	return loc;
}


void XSDContentHandler::convertAttributes(const XML::Attributes& attr, CompactAttributes& attrList)
{
	for (int i = 0; i < attr.getLength(); ++i)
	{
		attrList.try_emplace(attr.getLocalName(i), attr.getValue(i));
	}
}


QName XSDContentHandler::createQName(const std::string& str) const
{
	const XML::Name name = resolveName(str);
	return QName(name.localName(), name.namespaceURI());
}


XML::Name XSDContentHandler::resolveName(const std::string& qname) const
{
	std::string namespaceURI;
	std::string localName;
	// an undeclared prefix is not an error; it leaves the name without a namespace
	static_cast<void>(_namespaces.processName(qname, namespaceURI, localName, false));
	return XML::Name(qname, namespaceURI, localName);
}


Types::BindingProperties& XSDContentHandler::operationBindingProperties(Types::Operation& operation) const
{
	for (auto it = _states.rbegin(); it != _states.rend(); ++it)
	{
		switch (*it)
		{
		case StateMachine::ST_ININPUT:
			return operation.inputBindingProperties();
		case StateMachine::ST_INOUTPUT:
			return operation.outputBindingProperties();
		case StateMachine::ST_INFAULT:
			return operation.faultBindingProperties();
		default:
			break;
		}
	}
	throw Types::SchemaException("SOAP binding element outside wsdl:input, wsdl:output or wsdl:fault");
}


void XSDContentHandler::revokeRegistrations()
{
	Types::TypesManager& tm = Types::TypesManager::instance();
	for (const auto& pSchema: _registeredSchemas)
	{
		tm.removeSchema(pSchema);
	}
	for (const auto& pDefinitions: _registeredDefinitions)
	{
		tm.removeDefinitions(pDefinitions);
	}
}


void XSDContentHandler::parse()
{
	SharedPtr<std::istream> pIn = URIStreamOpener::defaultOpener().open(_schemaLocation);
	Poco::XML::InputSource in(*pIn);
	in.setSystemId(_schemaLocation.toString());
	Poco::XML::SAXParser parser;
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parser.setContentHandler(this);
	try
	{
		parser.parse(&in);
	}
	catch (...)
	{
		revokeRegistrations();
		throw;
	}
}


void XSDContentHandler::clear()
{
}


StateMachine& XSDContentHandler::stateMachine()
{
	static StateMachine machine;
	return machine;
}


void XSDContentHandler::resolveSchemaLocation(Poco::URI& schemaLocation, const Poco::URI& parentSchemaLocation)
{
	if (schemaLocation.isRelative())
	{
		Poco::Path p(schemaLocation.toString());
		if (p.isRelative())
		{
			if (parentSchemaLocation.empty())
			{
				p.makeAbsolute();
				schemaLocation = "file://" + p.toString(Poco::Path::PATH_UNIX);
			}
			else
			{
				if (parentSchemaLocation.isRelative())
				{
					Poco::Path pp(parentSchemaLocation.toString());
					pp.makeAbsolute();
					pp.resolve(p);
					schemaLocation = "file://" + pp.toString(Poco::Path::PATH_UNIX);
				}
				else
				{
					schemaLocation = parentSchemaLocation;
					schemaLocation.resolve(p.toString(Poco::Path::PATH_UNIX));
				}
			}
		}
	}
}


Poco::XSD::Types::Schema::Ptr XSDContentHandler::loadXSD(const Poco::URI& schemaLocation, const XSDContentHandler::SchemaNSToLocationMap& schemaMap)
{
	Poco::XSD::Types::Schema::Ptr pSchema = loadDocument(schemaLocation, schemaMap);
	// a document of the XML Schema namespace itself is not registered
	Poco::XSD::Types::Schema::Ptr pRegistered = Types::TypesManager::instance().findSchema(schemaLocation);
	return pRegistered != nullptr ? pRegistered : pSchema;
}


Poco::XSD::Types::Schema::Ptr XSDContentHandler::loadDocument(const Poco::URI& schemaLocation,
		const SchemaNSToLocationMap& schemaMap)
{
	Poco::XSD::Types::Schema::Ptr pSchema = Types::TypesManager::instance().findSchema(schemaLocation);
	if (pSchema == nullptr)
	{
		XSDContentHandler xsd(schemaLocation, schemaMap);
		xsd.parse();
		pSchema = xsd._pSchema;
	}
	return pSchema;
}


void XSDContentHandler::importXSD(const Poco::URI& schemaLocation, const XSDContentHandler::SchemaNSToLocationMap& schemaMap)
{
	Poco::XSD::Types::Schema::Ptr pSchema = loadXSD(schemaLocation, schemaMap);
	_pSchema->addImportedSchema(pSchema);
}


void XSDContentHandler::importWSDL(const std::string& targetNamespace, const Poco::URI& schemaLocation, const XSDContentHandler::SchemaNSToLocationMap& schemaMap)
{
	Poco::XSD::Types::Definitions::Ptr pDefinitions = Types::TypesManager::instance().findDefinitions(targetNamespace);
	if (!pDefinitions)
	{
		XSDContentHandler xsd(schemaLocation, schemaMap);
		xsd.parse();
	}
}


} // namespace Poco::XSD::Parser
