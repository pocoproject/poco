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
#include <algorithm>


namespace Poco::XSD::Types {


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


Schema::~Schema() = default;


void Schema::addType(TypePtr pType)
{
	if (!pType)
		throw NullTypeException("Cannot add a null type to schema");

	const std::string& name = pType->name();

	if (name.empty())
		throw InvalidTypeException("Name is empty");
	pType->setSchema(this);
	if (!_declaredTypes.try_emplace(name, pType).second)
		throw TypeAlreadyDefinedException(name);
}


const Type* Schema::getType(const std::string& name) const
{
	auto it = _declaredTypes.find(name);
	if (it == _declaredTypes.end())
		return nullptr;
	return it->second.get();
}


void Schema::addElement(ElementPtr pElement)
{
	if (!pElement)
		throw NullElementException("Cannot add a null element to schema");

	const std::string& name = pElement->name();

	if (name.empty())
		throw InvalidElementException("Name is empty");

	if (!_exportedElements.try_emplace(name, pElement).second)
		throw ElementAlreadyDefinedException(name);
}


const Element* Schema::getElement(const std::string& name) const
{
	auto it = _exportedElements.find(name);
	if (it == _exportedElements.end())
		return nullptr;
	return it->second.get();
}


void Schema::addAttribute(AbstractAttribute::Ptr pAttr)
{
	if (!pAttr)
		throw NullTypeException("Cannot add a null attribute to schema");

	const std::string& name = pAttr->name();

	if (name.empty())
		throw InvalidTypeException("Attribute Name is empty");

	if (!_exportedAttributes.try_emplace(name, pAttr).second)
		throw TypeAlreadyDefinedException(name);
}


const AbstractAttribute* Schema::getAttribute(const std::string& name) const
{
	auto it = _exportedAttributes.find(name);
	if (it == _exportedAttributes.end())
		return nullptr;
	return it->second.get();
}


void Schema::addAttributeGroup(AbstractAttributeGroup::Ptr pAttr)
{
	if (!pAttr)
		throw NullTypeException("Cannot add a null attribute group to schema");

	const std::string& name = pAttr->name();

	if (name.empty())
		throw InvalidTypeException("Attribute Group Name is empty");

	if (!_exportedAttributeGroups.try_emplace(name, pAttr).second)
		throw TypeAlreadyDefinedException(name);
}


const AbstractAttributeGroup* Schema::getAttributeGroup(const std::string& name) const
{
	auto it = _exportedAttributeGroups.find(name);
	if (it == _exportedAttributeGroups.end())
		return nullptr;
	return it->second.get();
}


void Schema::addGroup(Group::Ptr pAttr)
{
	if (!pAttr)
		throw NullTypeException("Cannot add a null group to schema");

	const std::string& name = pAttr->name();

	if (name.empty())
		throw InvalidTypeException("Group Name is empty");

	if (!_exportedGroups.try_emplace(name, pAttr).second)
		throw TypeAlreadyDefinedException(name);
}


const Group* Schema::getGroup(const std::string& name) const
{
	auto it = _exportedGroups.find(name);
	if (it == _exportedGroups.end())
		return nullptr;
	return it->second.get();
}


void Schema::addNotation(Notation::Ptr ptr)
{
	if (!ptr)
		throw NullTypeException("Cannot add a null Notation to schema");

	const std::string& name = ptr->name();

	if (name.empty())
		throw InvalidTypeException("Notation Name is empty");

	if (!_notations.try_emplace(name, ptr).second)
		throw TypeAlreadyDefinedException(name);
}


const Notation* Schema::getNotation(const std::string& name) const
{
	auto it = _notations.find(name);
	if (it == _notations.end())
		return nullptr;
	return it->second.get();
}


void Schema::addImportedSchema(Ptr pSchema)
{
	_importedSchemas.push_back(pSchema);
}


void Schema::includeSchema(Ptr pSchema)
{
	// A schema document included a second time comes from the location cache as the
	// registered schema, which may be this one.
	if (pSchema.get() == this)
		return;

	for (const auto& pImported: pSchema->_importedSchemas)
	{
		if (pImported.get() != this && std::find(_importedSchemas.begin(), _importedSchemas.end(), pImported) == _importedSchemas.end())
			_importedSchemas.push_back(pImported);
	}
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
	
	for (auto& pImported: _importedSchemas)
	{
		if (!pImported->_fixedUp) pImported->fixup();
	}

	for (auto& [name, pType]: _declaredTypes)
	{
		pType->fixup();
	}

	for (auto& [name, pElement]: _exportedElements)
	{
		pElement->fixup();
	}

	for (auto& [name, pAttr]: _exportedAttributes)
	{
		pAttr->fixup();
	}

	for (auto& [name, pAttrGroup]: _exportedAttributeGroups)
	{
		pAttrGroup->fixup();
	}

	for (auto& [name, pGroup]: _exportedGroups)
	{
		pGroup->fixup();
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


} // namespace Poco::XSD::Types
