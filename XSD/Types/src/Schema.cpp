//
// Schema.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Schema
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/Schema.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/Visitor.h"


namespace Poco {
namespace XSD {
namespace Types {


Schema::Schema(const std::string& targetNS,
		bool qualifiedAttributeForm,
		bool qualifiedElementForm,
		bool blockExtension,
		bool blockRestriction,
		bool blockSubstitution,
		bool finalExtension,
		bool finalRestriction,
		bool finalList,
		bool finalUnion,
		bool needsFixup):
	_targetNamespace(targetNS),
	_declaredTypes(),
	_exportedElements(),
	_exportedAttributes(),
	_exportedAttributeGroups(),
	_exportedGroups(),
	_qualifiedAttributeForm(qualifiedAttributeForm),
	_qualifiedElementForm(qualifiedElementForm),
	_blockExtension(blockExtension),
	_blockRestriction(blockRestriction),
	_blockSubstitution(blockSubstitution),
	_finalExtension(finalExtension),
	_finalRestriction(finalRestriction),
	_finalList(finalList),
	_finalUnion(finalUnion),
	_fixedUp(!needsFixup)
{
}


Schema::~Schema()
{
}


void Schema::addType(TypePtr pType)
{
	if (!pType)
		throw NullTypeException("Cannot add a null type to schema");

	const std::string& name = pType->name();

	if (name.empty())
		throw InvalidTypeException("Name is empty");
	pType->setSchema(this);
	std::pair<Types::iterator, bool> res = _declaredTypes.insert(std::make_pair(name, pType));
	if (!res.second)
		throw TypeAlreadyDefinedException(name);
}


const Type* Schema::getType(const std::string& name) const
{
	Types::const_iterator it = _declaredTypes.find(name);
	if (it == _declaredTypes.end())
		return 0;
	return it->second.get();
}


void Schema::addElement(ElementPtr pElement)
{
	if (!pElement)
		throw NullElementException("Cannot add a null element to schema");

	const std::string& name = pElement->name();

	if (name.empty())
		throw InvalidElementException("Name is empty");

	std::pair<Elements::iterator, bool> res = _exportedElements.insert(std::make_pair(name, pElement));
	if (!res.second)
		throw ElementAlreadyDefinedException(name);
}


const Element* Schema::getElement(const std::string& name) const
{
	Elements::const_iterator it = _exportedElements.find(name);
	if (it == _exportedElements.end())
		return 0;
	return it->second.get();
}


void Schema::addAttribute(AbstractAttribute::Ptr pAttr)
{
	if (!pAttr)
		throw NullTypeException("Cannot add a null attribute to schema");

	const std::string& name = pAttr->name();

	if (name.empty())
		throw InvalidTypeException("Attribute Name is empty");

	std::pair<Attributes::iterator, bool> res = _exportedAttributes.insert(std::make_pair(name, pAttr));
	if (!res.second)
		throw TypeAlreadyDefinedException(name);
}


const AbstractAttribute* Schema::getAttribute(const std::string& name) const
{
	Attributes::const_iterator it = _exportedAttributes.find(name);
	if (it == _exportedAttributes.end())
		return 0;
	return it->second.get();
}


void Schema::addAttributeGroup(AbstractAttributeGroup::Ptr pAttr)
{
	if (!pAttr)
		throw NullTypeException("Cannot add a null attribute group to schema");

	const std::string& name = pAttr->name();

	if (name.empty())
		throw InvalidTypeException("Attribute Group Name is empty");

	std::pair<AttributeGroups::iterator, bool> res = _exportedAttributeGroups.insert(std::make_pair(name, pAttr));
	if (!res.second)
		throw TypeAlreadyDefinedException(name);
}


const AbstractAttributeGroup* Schema::getAttributeGroup(const std::string& name) const
{
	AttributeGroups::const_iterator it = _exportedAttributeGroups.find(name);
	if (it == _exportedAttributeGroups.end())
		return 0;
	return it->second.get();
}


void Schema::addGroup(Group::Ptr pAttr)
{
	if (!pAttr)
		throw NullTypeException("Cannot add a null group to schema");

	const std::string& name = pAttr->name();

	if (name.empty())
		throw InvalidTypeException("Group Name is empty");

	std::pair<Groups::iterator, bool> res = _exportedGroups.insert(std::make_pair(name, pAttr));
	if (!res.second)
		throw TypeAlreadyDefinedException(name);
}


const Group* Schema::getGroup(const std::string& name) const
{
	Groups::const_iterator it = _exportedGroups.find(name);
	if (it == _exportedGroups.end())
		return 0;
	return it->second.get();
}


void Schema::addNotation(Notation::Ptr ptr)
{
	if (!ptr)
		throw NullTypeException("Cannot add a null Notation to schema");

	const std::string& name = ptr->name();

	if (name.empty())
		throw InvalidTypeException("Notation Name is empty");

	std::pair<Notations::iterator, bool> res = _notations.insert(std::make_pair(name, ptr));
	if (!res.second)
		throw TypeAlreadyDefinedException(name);
}


const Notation* Schema::getNotation(const std::string& name) const
{
	Notations::const_iterator it = _notations.find(name);
	if (it == _notations.end())
		return 0;
	return it->second.get();
}


void Schema::addImportedSchema(Ptr pSchema)
{
	_importedSchemas.push_back(pSchema);
}


void Schema::includeSchema(Ptr pSchema)
{
	_importedSchemas.insert(_importedSchemas.end(), pSchema->_importedSchemas.begin(), pSchema->_importedSchemas.end());
	_declaredTypes.insert(pSchema->_declaredTypes.begin(), pSchema->_declaredTypes.end());
	_exportedElements.insert(pSchema->_exportedElements.begin(), pSchema->_exportedElements.end());
	_exportedAttributes.insert(pSchema->_exportedAttributes.begin(), pSchema->_exportedAttributes.end());
	_exportedAttributeGroups.insert(pSchema->_exportedAttributeGroups.begin(), pSchema->_exportedAttributeGroups.end());
	_exportedGroups.insert(pSchema->_exportedGroups.begin(), pSchema->_exportedGroups.end());
	_notations.insert(pSchema->_notations.begin(), pSchema->_notations.end());
}


void Schema::accept(Visitor& v) const
{
	v.visit(*this);
}


void Schema::fixup()
{
	if (_fixedUp) return;
	_fixedUp = true;
	
	for (Schemas::iterator it = _importedSchemas.begin(); it != _importedSchemas.end(); ++it)
	{
		if (!(*it)->_fixedUp) (*it)->fixup();
	}

	for (Types::iterator it = _declaredTypes.begin(); it != _declaredTypes.end(); ++it)
	{
		it->second->fixup();
	}

	for (Elements::iterator it = _exportedElements.begin(); it != _exportedElements.end(); ++it)
	{
		it->second->fixup();
	}

	for (Attributes::iterator it = _exportedAttributes.begin(); it != _exportedAttributes.end(); ++it)
	{
		it->second->fixup();
	}

	for (AttributeGroups::iterator it = _exportedAttributeGroups.begin(); it != _exportedAttributeGroups.end(); ++it)
	{
		it->second->fixup();
	}

	for (Groups::iterator it = _exportedGroups.begin(); it != _exportedGroups.end(); ++it)
	{
		it->second->fixup();
	}
}


bool conflicts(const Schema& s1, const Schema& s2)
{
	if (s1.targetNamespace() != s2.targetNamespace())
		return false;

	// schema is equal: check props
	return (s1.blockExtension() ^ s2.blockExtension() ||
		s1.blockRestriction() ^ s2.blockRestriction() ||
		s1.blockSubstitution() ^ s2.blockSubstitution() ||
		s1.finalExtension() ^ s2.finalExtension() ||
		s1.finalList() ^ s2.finalList() ||
		s1.finalRestriction() ^ s2.finalRestriction() ||
		s1.finalUnion() ^ s2.finalUnion());
}


} } } // namespace Poco::XSD::Types
