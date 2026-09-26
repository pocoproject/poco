//
// TypesManager.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  TypesManager
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/AttributeTypeRef.h"
#include "Poco/XSD/Types/AttributeRef.h"
#include "Poco/XSD/Types/AttributeGroup.h"
#include "Poco/XSD/Types/List.h"


namespace Poco {
namespace XSD {
namespace Types {


const std::string TypesManager::XSD_NAMESPACE("http://www.w3.org/2001/XMLSchema");
const std::string TypesManager::XSD_NAMESPACE1998("http://www.w3.org/XML/1998/namespace");
const std::string TypesManager::XSD_TYPE_ANYTYPE("anyType");
const std::string TypesManager::XSD_TYPE_ANYSIMPLETYPE("anySimpleType");
const std::string TypesManager::XSD_TYPE_STRING("string");
const std::string TypesManager::XSD_TYPE_BOOLEAN("boolean");
const std::string TypesManager::XSD_TYPE_DECIMAL("decimal");
const std::string TypesManager::XSD_TYPE_FLOAT("float");
const std::string TypesManager::XSD_TYPE_DOUBLE("double");
const std::string TypesManager::XSD_TYPE_DURATION("duration");
const std::string TypesManager::XSD_TYPE_DATETIME("dateTime");
const std::string TypesManager::XSD_TYPE_TIME("time");
const std::string TypesManager::XSD_TYPE_DATE("date");
const std::string TypesManager::XSD_TYPE_GYEARMONTH("gYearMonth");
const std::string TypesManager::XSD_TYPE_GYEAR("gYear");
const std::string TypesManager::XSD_TYPE_GMONTHDAY("gMonthDay");
const std::string TypesManager::XSD_TYPE_GDAY("gDay");
const std::string TypesManager::XSD_TYPE_GMONTH("gMonth");
const std::string TypesManager::XSD_TYPE_HEX_BINARY("hexBinary");
const std::string TypesManager::XSD_TYPE_BASE64BINARY("base64Binary");
const std::string TypesManager::XSD_TYPE_ANYURI("anyURI");
const std::string TypesManager::XSD_TYPE_QNAME("QName");
const std::string TypesManager::XSD_TYPE_NOTATION("NOTATION");
const std::string TypesManager::XSD_TYPE_NORMALIZEDSTRING("normalizedString");
const std::string TypesManager::XSD_TYPE_INTEGER("integer");
const std::string TypesManager::XSD_TYPE_TOKEN("token");
const std::string TypesManager::XSD_TYPE_NONPOSITIVEINTEGER("nonPositiveInteger");
const std::string TypesManager::XSD_TYPE_LONG("long");
const std::string TypesManager::XSD_TYPE_NONNEGATIVEINTEGER("nonNegativeInteger");
const std::string TypesManager::XSD_TYPE_LANGUAGE("language");
const std::string TypesManager::XSD_TYPE_NAME("Name");
const std::string TypesManager::XSD_TYPE_NMTOKEN("NMTOKEN");
const std::string TypesManager::XSD_TYPE_NEGATIVEINTEGER("negativeInteger");
const std::string TypesManager::XSD_TYPE_INT("int");
const std::string TypesManager::XSD_TYPE_UNSIGNEDLONG("unsignedLong");
const std::string TypesManager::XSD_TYPE_POSITIVEINTEGER("positiveInteger");
const std::string TypesManager::XSD_TYPE_NCNAME("NCName");
const std::string TypesManager::XSD_TYPE_NMTOKENS("NMTOKENS");
const std::string TypesManager::XSD_TYPE_SHORT("short");
const std::string TypesManager::XSD_TYPE_UNSIGNEDINT("unsignedInt");
const std::string TypesManager::XSD_TYPE_ID("ID");
const std::string TypesManager::XSD_TYPE_IDREF("IDREF");
const std::string TypesManager::XSD_TYPE_ENTITY("ENTITY");
const std::string TypesManager::XSD_TYPE_BYTE("byte");
const std::string TypesManager::XSD_TYPE_UNSIGNEDSHORT("unsignedShort");
const std::string TypesManager::XSD_TYPE_IDREFS("IDREFS");
const std::string TypesManager::XSD_TYPE_ENTITIES("ENTITIES");
const std::string TypesManager::XSD_TYPE_UNSIGNEDBYTE("unsignedByte");


TypesManager& TypesManager::instance()
{
	static Poco::SingletonHolder < TypesManager > instance;
	return *instance.get();
}


void TypesManager::fixupSchemas()
{
	for (Schemas::iterator it = _schemas.begin(); it != _schemas.end(); ++it)
	{
		it->second->fixup();
	}
}


bool TypesManager::hasSchema(const std::string& ns) const
{
	Poco::Mutex::ScopedLock lock(_mutex);
	return _schemas.find(ns) != _schemas.end();
}


bool TypesManager::hasDefinitions(const std::string& ns) const
{
	Poco::Mutex::ScopedLock lock(_mutex);
	return _definitions.find(ns) != _definitions.end();
}


bool TypesManager::eraseSchema(const std::string& ns)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	return (_schemas.erase(ns) > 0);
}


void TypesManager::addSchema(Schema::Ptr pSchema, const Poco::URI& schemaLocation)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	if (pSchema->targetNamespace() == XSD_NAMESPACE || pSchema->targetNamespace() == XSD_NAMESPACE1998)
		return;

	setSchemaInternal(pSchema, schemaLocation);
}


void TypesManager::addDefinitions(Definitions::Ptr pDefinitions)
{
	Poco::Mutex::ScopedLock lock(_mutex);

	_definitions[pDefinitions->targetNamespace()] = pDefinitions;
}


void TypesManager::setSchemaInternal(Schema::Ptr pSchema, const Poco::URI& schemaLocation)
{
	_schemaLocations[schemaLocation.toString()] = pSchema;
	std::pair<Schemas::iterator, bool> res = _schemas.insert(make_pair(pSchema->targetNamespace(), pSchema));
	Schema& theSchema = *(res.first->second);
	if (!res.second)
	{
		if (conflicts(theSchema, *pSchema))
			throw SchemaException("A different schema for that targetNamespace exists: " + pSchema->targetNamespace());

		// copy all types+elements+... from the the new schema
		Schema::Types::const_iterator it = pSchema->types().begin();
		Schema::Types::const_iterator itEnd = pSchema->types().end();
		for (; it != itEnd; ++it)
		{
			if (!theSchema.getType(it->second->name()))
				theSchema.addType(it->second);
		}

		Schema::Elements::const_iterator itE = pSchema->elements().begin();
		Schema::Elements::const_iterator itEEnd = pSchema->elements().end();
		for (; itE != itEEnd; ++itE)
		{
			if (!theSchema.getElement(itE->second->name()))
				theSchema.addElement(itE->second);
		}

		Schema::Attributes::const_iterator itA = pSchema->attributes().begin();
		Schema::Attributes::const_iterator itAEnd = pSchema->attributes().end();
		for (; itA != itAEnd; ++itA)
		{
			if (!theSchema.getAttribute(itA->second->name()))
				theSchema.addAttribute(itA->second);
		}

		Schema::AttributeGroups::const_iterator itAG = pSchema->attributeGroups().begin();
		Schema::AttributeGroups::const_iterator itAGEnd = pSchema->attributeGroups().end();
		for (; itAG != itAGEnd; ++itAG)
		{
			if (!theSchema.getAttributeGroup(itAG->second->name()))
				theSchema.addAttributeGroup(itAG->second);
		}

		Schema::Groups::const_iterator itG = pSchema->groups().begin();
		Schema::Groups::const_iterator itGEnd = pSchema->groups().end();
		for (; itG != itGEnd; ++itG)
		{
			if (!theSchema.getGroup(itG->second->name()))
				theSchema.addGroup(itG->second);
		}

		Schema::Notations::const_iterator itN = pSchema->notations().begin();
		Schema::Notations::const_iterator itNEnd = pSchema->notations().end();
		for (; itN != itNEnd; ++itN)
		{
			if (!theSchema.getNotation(itN->second->name()))
				theSchema.addNotation(itN->second);
		}
	}
}


Schema& TypesManager::getSchema(const std::string& ns)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::iterator it = _schemas.find(ns);
	if (it != _schemas.end())
		return *(it->second);
	else
		throw Poco::NotFoundException("schema", ns);
}


Schema::Ptr TypesManager::findSchema(const std::string& ns)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::iterator it = _schemas.find(ns);
	if (it != _schemas.end())
		return it->second;
	else
		return Schema::Ptr();
}


Schema::Ptr TypesManager::findSchema(const Poco::URI& schemaLocation)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::iterator it = _schemaLocations.find(schemaLocation.toString());
	if (it != _schemaLocations.end())
		return it->second;
	else
		return Schema::Ptr();
}


Definitions::Ptr TypesManager::findDefinitions(const std::string& ns)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	Definitionss::iterator it = _definitions.find(ns);
	if (it != _definitions.end())
		return it->second;
	else
		return Definitions::Ptr();
}


Definitions& TypesManager::getDefinitions(const std::string& ns)
{
	Poco::Mutex::ScopedLock lock(_mutex);
	Definitionss::iterator it = _definitions.find(ns);
	if (it != _definitions.end())
		return *(it->second);
	else
		throw Poco::NotFoundException("definitions", ns);
}


const Type* TypesManager::getType(const QName& ref) const
{
	const Type* pResult = 0;
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::const_iterator it = _schemas.find(ref.getNamespace());
	if (it != _schemas.end())
		pResult = it->second->getType(ref.name());

	return pResult;
}


const Element* TypesManager::getElement(const QName& ref) const
{
	const Element* pResult = 0;
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::const_iterator it = _schemas.find(ref.getNamespace());
	if (it != _schemas.end())
		pResult = it->second->getElement(ref.name());

	return pResult;
}


const AbstractAttribute* TypesManager::getAttribute(const QName& ref) const
{
	const AbstractAttribute* pResult = 0;
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::const_iterator it = _schemas.find(ref.getNamespace());
	if (it != _schemas.end())
		pResult = it->second->getAttribute(ref.name());

	return pResult;
}


const AbstractAttributeGroup* TypesManager::getAttributeGroup(const QName& ref) const
{
	const AbstractAttributeGroup* pResult = 0;
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::const_iterator it = _schemas.find(ref.getNamespace());
	if (it != _schemas.end())
		pResult = it->second->getAttributeGroup(ref.name());

	return pResult;
}


const Group* TypesManager::getGroup(const QName& ref) const
{
	const Group* pResult = 0;
	Poco::Mutex::ScopedLock lock(_mutex);
	Schemas::const_iterator it = _schemas.find(ref.getNamespace());
	if (it != _schemas.end())
		pResult = it->second->getGroup(ref.name());

	return pResult;
}


TypesManager::TypesManager()
{
	{
		// load the default XSD schema
		Schema::Ptr pSchema = new Schema(XSD_NAMESPACE, false, false, false, false, false, false, false, false, false, false);
		pSchema->addType(new SimpleType(XSD_TYPE_ANYTYPE, XSD_TYPE_ANYTYPE, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_ANYSIMPLETYPE, XSD_TYPE_ANYSIMPLETYPE, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_STRING, XSD_TYPE_STRING, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_BOOLEAN, XSD_TYPE_BOOLEAN, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_DECIMAL, XSD_TYPE_DECIMAL, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_FLOAT, XSD_TYPE_FLOAT, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_DOUBLE, XSD_TYPE_DOUBLE, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_DURATION, XSD_TYPE_DURATION, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_DATETIME, XSD_TYPE_DATETIME, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_TIME, XSD_TYPE_TIME, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_DATE, XSD_TYPE_DATE, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_GYEARMONTH, XSD_TYPE_GYEARMONTH, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_GYEAR, XSD_TYPE_GYEAR, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_GMONTHDAY, XSD_TYPE_GMONTHDAY, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_GDAY, XSD_TYPE_GDAY, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_GMONTH, XSD_TYPE_GMONTH, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_HEX_BINARY, XSD_TYPE_HEX_BINARY, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_BASE64BINARY, XSD_TYPE_BASE64BINARY, false, false, false));
		SimpleType::Ptr pAnyUri = new SimpleType(XSD_TYPE_ANYURI, XSD_TYPE_ANYURI, false, false, false);
		pSchema->addType(pAnyUri);
		pSchema->addType(new SimpleType(XSD_TYPE_QNAME, XSD_TYPE_QNAME, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_NOTATION, XSD_TYPE_NOTATION, false, false, false));
		
		// all other types extend via restriction from the primitive types
		// simplify and ignore that where possible
		pSchema->addType(new SimpleType(XSD_TYPE_NORMALIZEDSTRING, XSD_TYPE_NORMALIZEDSTRING, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_INTEGER, XSD_TYPE_INTEGER, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_TOKEN, XSD_TYPE_TOKEN, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_NONPOSITIVEINTEGER, XSD_TYPE_NONPOSITIVEINTEGER, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_LONG, XSD_TYPE_LONG, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_NONNEGATIVEINTEGER, XSD_TYPE_NONNEGATIVEINTEGER, false, false, false));
		SimpleType::Ptr pLang = new SimpleType(XSD_TYPE_LANGUAGE, XSD_TYPE_LANGUAGE, false, false, false);
		pSchema->addType(pLang);
		pSchema->addType(new SimpleType(XSD_TYPE_NAME, XSD_TYPE_NAME, false, false, false));
		SimpleType::Ptr pNM = new SimpleType(XSD_TYPE_NMTOKEN, XSD_TYPE_NMTOKEN, false, false, false);
		pSchema->addType(pNM);
		pSchema->addType(new SimpleType(XSD_TYPE_NEGATIVEINTEGER, XSD_TYPE_NEGATIVEINTEGER, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_INT, XSD_TYPE_INT, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_UNSIGNEDLONG, XSD_TYPE_UNSIGNEDLONG, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_POSITIVEINTEGER, XSD_TYPE_POSITIVEINTEGER, false, false, false));
		SimpleType::Ptr pNCName = new SimpleType(XSD_TYPE_NCNAME, XSD_TYPE_NCNAME, false, false, false);
		pSchema->addType(pNCName);
		pSchema->addType(new SimpleType(XSD_TYPE_SHORT, XSD_TYPE_SHORT, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_UNSIGNEDINT, XSD_TYPE_UNSIGNEDINT, false, false, false));
		SimpleType::Ptr pID = new SimpleType(XSD_TYPE_ID, XSD_TYPE_ID, false, false, false);
		pSchema->addType(pID);
		SimpleType::Ptr pIDREF = new SimpleType(XSD_TYPE_IDREF, XSD_TYPE_IDREF, false, false, false);
		pSchema->addType(pIDREF);
		SimpleType::Ptr pENT = new SimpleType(XSD_TYPE_ENTITY, XSD_TYPE_ENTITY, false, false, false);
		pSchema->addType(pENT);
		pSchema->addType(new SimpleType(XSD_TYPE_BYTE, XSD_TYPE_BYTE, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_UNSIGNEDSHORT, XSD_TYPE_UNSIGNEDSHORT, false, false, false));
		pSchema->addType(new SimpleType(XSD_TYPE_UNSIGNEDBYTE, XSD_TYPE_UNSIGNEDBYTE, false, false, false));

		// list types: here we have to model inheritance
		SimpleType::Ptr pNMs = new SimpleType(XSD_TYPE_NMTOKENS, XSD_TYPE_NMTOKENS, false, false, false);
		List::Ptr pListNMs = new List("");
		pListNMs->setType(pNM);
		pNMs->setContent(pListNMs);
		pSchema->addType(pNMs);

		SimpleType::Ptr pIDREFS = new SimpleType(XSD_TYPE_IDREFS, XSD_TYPE_IDREFS, false, false, false);
		List::Ptr pListIDREFS = new List("");
		pListIDREFS->setType(pIDREF);
		pIDREFS->setContent(pListIDREFS);
		pSchema->addType(pIDREFS);
		SimpleType::Ptr pEnts = new SimpleType(XSD_TYPE_ENTITIES, XSD_TYPE_ENTITIES, false, false, false);
		List::Ptr pListENTS = new List("");
		pListENTS->setType(pENT);
		pEnts->setContent(pListENTS);
		pSchema->addType(pEnts);
		AttributeTypeRef::Ptr pLangRef = new AttributeTypeRef("", "lang", XSD_NAMESPACE1998, QName(XSD_TYPE_LANGUAGE, XSD_NAMESPACE), pLang, "", "", false);
		pSchema->addAttribute(pLangRef);
		AttributeTypeRef::Ptr pSpaceRef = new AttributeTypeRef("", "space", XSD_NAMESPACE1998, QName(XSD_TYPE_NCNAME, XSD_NAMESPACE), pNCName, "", "", false);
		pSchema->addAttribute(pSpaceRef);
		AttributeTypeRef::Ptr pBaseRef = new AttributeTypeRef("", "base", XSD_NAMESPACE1998, QName(XSD_TYPE_ANYURI, XSD_NAMESPACE), pAnyUri, "", "", false);
		pSchema->addAttribute(pBaseRef);
		AttributeTypeRef::Ptr pIDRef = new AttributeTypeRef("", "id", XSD_NAMESPACE1998, QName(XSD_TYPE_ID, XSD_NAMESPACE), pID, "", "", false);
		pSchema->addAttribute(pIDRef);

		AttributeGroup::Ptr pAttr = new AttributeGroup("", "specialAttrs");
		pAttr->add(new AttributeRef("", QName("lang", XSD_NAMESPACE), pLangRef));
		pAttr->add(new AttributeRef("", QName("space", XSD_NAMESPACE), pSpaceRef));
		pAttr->add(new AttributeRef("", QName("base", XSD_NAMESPACE), pBaseRef));
		pAttr->add(new AttributeRef("", QName("id", XSD_NAMESPACE), pIDRef));
		pSchema->addAttributeGroup(pAttr);

		setSchemaInternal(pSchema, Poco::URI("http://www.w3.org/2001/xml.xsd"));
	}

	{
		Schema::Ptr pSchema98 = new Schema(XSD_NAMESPACE1998, false, false, false, false, false, false, false, false, false, false);
		pSchema98->addType(new SimpleType(XSD_TYPE_ANYTYPE, XSD_TYPE_ANYTYPE, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_ANYSIMPLETYPE, XSD_TYPE_ANYSIMPLETYPE, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_STRING, XSD_TYPE_STRING, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_BOOLEAN, XSD_TYPE_BOOLEAN, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_DECIMAL, XSD_TYPE_DECIMAL, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_FLOAT, XSD_TYPE_FLOAT, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_DOUBLE, XSD_TYPE_DOUBLE, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_DURATION, XSD_TYPE_DURATION, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_DATETIME, XSD_TYPE_DATETIME, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_TIME, XSD_TYPE_TIME, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_DATE, XSD_TYPE_DATE, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_GYEARMONTH, XSD_TYPE_GYEARMONTH, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_GYEAR, XSD_TYPE_GYEAR, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_GMONTHDAY, XSD_TYPE_GMONTHDAY, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_GDAY, XSD_TYPE_GDAY, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_GMONTH, XSD_TYPE_GMONTH, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_HEX_BINARY, XSD_TYPE_HEX_BINARY, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_BASE64BINARY, XSD_TYPE_BASE64BINARY, false, false, false));
		SimpleType::Ptr pAnyUri = new SimpleType(XSD_TYPE_ANYURI, XSD_TYPE_ANYURI, false, false, false);
		pSchema98->addType(pAnyUri);
		pSchema98->addType(new SimpleType(XSD_TYPE_QNAME, XSD_TYPE_QNAME, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_NOTATION, XSD_TYPE_NOTATION, false, false, false));
		
		// all other types extend via restriction from the primitive types
		// simplify and ignore that where possible
		pSchema98->addType(new SimpleType(XSD_TYPE_NORMALIZEDSTRING, XSD_TYPE_NORMALIZEDSTRING, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_INTEGER, XSD_TYPE_INTEGER, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_TOKEN, XSD_TYPE_TOKEN, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_NONPOSITIVEINTEGER, XSD_TYPE_NONPOSITIVEINTEGER, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_LONG, XSD_TYPE_LONG, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_NONNEGATIVEINTEGER, XSD_TYPE_NONNEGATIVEINTEGER, false, false, false));
		SimpleType::Ptr pLang = new SimpleType(XSD_TYPE_LANGUAGE, XSD_TYPE_LANGUAGE, false, false, false);
		pSchema98->addType(pLang);
		pSchema98->addType(new SimpleType(XSD_TYPE_NAME, XSD_TYPE_NAME, false, false, false));
		SimpleType::Ptr pNM = new SimpleType(XSD_TYPE_NMTOKEN, XSD_TYPE_NMTOKEN, false, false, false);
		pSchema98->addType(pNM);
		pSchema98->addType(new SimpleType(XSD_TYPE_NEGATIVEINTEGER, XSD_TYPE_NEGATIVEINTEGER, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_INT, XSD_TYPE_INT, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_UNSIGNEDLONG, XSD_TYPE_UNSIGNEDLONG, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_POSITIVEINTEGER, XSD_TYPE_POSITIVEINTEGER, false, false, false));
		SimpleType::Ptr pNCName = new SimpleType(XSD_TYPE_NCNAME, XSD_TYPE_NCNAME, false, false, false);
		pSchema98->addType(pNCName);
		pSchema98->addType(new SimpleType(XSD_TYPE_SHORT, XSD_TYPE_SHORT, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_UNSIGNEDINT, XSD_TYPE_UNSIGNEDINT, false, false, false));
		SimpleType::Ptr pID = new SimpleType(XSD_TYPE_ID, XSD_TYPE_ID, false, false, false);
		pSchema98->addType(pID);
		SimpleType::Ptr pIDREF = new SimpleType(XSD_TYPE_IDREF, XSD_TYPE_IDREF, false, false, false);
		pSchema98->addType(pIDREF);
		SimpleType::Ptr pENT = new SimpleType(XSD_TYPE_ENTITY, XSD_TYPE_ENTITY, false, false, false);
		pSchema98->addType(pENT);
		pSchema98->addType(new SimpleType(XSD_TYPE_BYTE, XSD_TYPE_BYTE, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_UNSIGNEDSHORT, XSD_TYPE_UNSIGNEDSHORT, false, false, false));
		pSchema98->addType(new SimpleType(XSD_TYPE_UNSIGNEDBYTE, XSD_TYPE_UNSIGNEDBYTE, false, false, false));

		// list types: here we have to model inheritance
		SimpleType::Ptr pNMs = new SimpleType(XSD_TYPE_NMTOKENS, XSD_TYPE_NMTOKENS, false, false, false);
		List::Ptr pListNMs = new List("");
		pListNMs->setType(pNM);
		pNMs->setContent(pListNMs);
		pSchema98->addType(pNMs);

		SimpleType::Ptr pIDREFS = new SimpleType(XSD_TYPE_IDREFS, XSD_TYPE_IDREFS, false, false, false);
		List::Ptr pListIDREFS = new List("");
		pListIDREFS->setType(pIDREF);
		pIDREFS->setContent(pListIDREFS);
		pSchema98->addType(pIDREFS);
		SimpleType::Ptr pEnts = new SimpleType(XSD_TYPE_ENTITIES, XSD_TYPE_ENTITIES, false, false, false);
		List::Ptr pListENTS = new List("");
		pListENTS->setType(pENT);
		pEnts->setContent(pListENTS);
		pSchema98->addType(pEnts);

		AttributeTypeRef::Ptr pLangRef = new AttributeTypeRef("", "lang", XSD_NAMESPACE1998, QName(XSD_TYPE_LANGUAGE, XSD_NAMESPACE1998), pLang, "", "", false);
		pSchema98->addAttribute(pLangRef);
		AttributeTypeRef::Ptr pSpaceRef = new AttributeTypeRef("", "space", XSD_NAMESPACE1998, QName(XSD_TYPE_NCNAME, XSD_NAMESPACE1998), pNCName, "", "", false);
		pSchema98->addAttribute(pSpaceRef);
		AttributeTypeRef::Ptr pBaseRef = new AttributeTypeRef("", "base", XSD_NAMESPACE1998, QName(XSD_TYPE_ANYURI, XSD_NAMESPACE1998), pAnyUri, "", "", false);
		pSchema98->addAttribute(pBaseRef);
		AttributeTypeRef::Ptr pIDRef = new AttributeTypeRef("", "id", XSD_NAMESPACE1998, QName(XSD_TYPE_ID, XSD_NAMESPACE1998), pID, "", "", false);
		pSchema98->addAttribute(pIDRef);

		AttributeGroup::Ptr pAttr = new AttributeGroup("", "specialAttrs");
		pAttr->add(new AttributeRef("", QName("lang", XSD_NAMESPACE1998), pLangRef));
		pAttr->add(new AttributeRef("", QName("space", XSD_NAMESPACE1998), pSpaceRef));
		pAttr->add(new AttributeRef("", QName("base", XSD_NAMESPACE1998), pBaseRef));
		pAttr->add(new AttributeRef("", QName("id", XSD_NAMESPACE1998), pIDRef));
		pSchema98->addAttributeGroup(pAttr);

		setSchemaInternal(pSchema98, Poco::URI("http://www.w3.org/XML/1998/xml.xsd"));
	}
}


TypesManager::~TypesManager()
{
}


} } } // namespace Poco::XSD::Types
