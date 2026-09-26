//
// Schema.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Schema
//
// Definition of the Schema class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Schema_INCLUDED
#define XSDTypes_Schema_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/Element.h"
#include "Poco/XSD/Types/Notation.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/AbstractAttributeGroup.h"
#include "Poco/XSD/Types/Group.h"
#include <vector>
#include <map>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API Schema: public AnnotatedObject
	/// This class represents an entire XML Schema definition.
{
public:
	using Ptr = Poco::AutoPtr<Schema>;
	using TypePtr = Type::Ptr;
	using ElementPtr = Element::Ptr;
	using Types = std::map<std::string, TypePtr>;
	using Elements = std::map<std::string, ElementPtr>;
	using Attributes = std::map<std::string, AbstractAttribute::Ptr>;
	using AttributeGroups = std::map<std::string, AbstractAttributeGroup::Ptr>;
	using Groups = std::map<std::string, Group::Ptr>;
	using Notations = std::map<std::string, Notation::Ptr>;
	using Schemas = std::vector<Ptr>;

	Schema(const std::string& targetNS,
		bool qualifiedAttributeForm,
		bool qualifiedElementForm,
		bool blockExtension,
		bool blockRestriction,
		bool blockSubstitution,
		bool finalExtension,
		bool finalRestriction,
		bool finalList,
		bool finalUnion,
		bool needsFixup = true);
		/// Creates the Schema.

	~Schema();
		/// Destroys the Schema.

	const std::string& targetNamespace() const;
		/// The namespace of the Schema

	void addType(TypePtr pType);
		/// Adds a type to the Schema, will fail if the type already
		/// exists.
		/// Note that once a type was added to the Schema, it can not
		/// be removed!

	const Type* getType(const std::string& name) const;
		/// Returns a type for the given name, returns a null pointer if not found

	void addElement(ElementPtr pElem);
		/// Adds an Element to the Schema, will fail if the element already
		/// exists.
		/// Note that once an element was added to the Schema, it can not
		/// be removed!

	const Element* getElement(const std::string& name) const;
		/// Returns an element for the given name, returns a null pointer if not found

	void addAttribute(AbstractAttribute::Ptr pAttr);
		/// Adds an attribute to the Schema, will fail if the attribute already
		/// exists.
		/// Note that once an attribute was added to the Schema, it can not
		/// be removed!

	const AbstractAttribute* getAttribute(const std::string& name) const;
		/// Returns a attribute for the given name, returns a null pointer if not found

	void addAttributeGroup(AbstractAttributeGroup::Ptr pAttr);
		/// Adds an attributegroup to the Schema, will fail if the attributegroup already
		/// exists.
		/// Note that once an attributegroup was added to the Schema, it can not
		/// be removed!

	const AbstractAttributeGroup* getAttributeGroup(const std::string& name) const;
		/// Returns an attributegroup for the given name, returns a null pointer if not found

	void addGroup(Group::Ptr pAttr);
		/// Adds a group to the Schema, will fail if the group already
		/// exists.
		/// Note that once a group was added to the Schema, it can not
		/// be removed!

	const Group* getGroup(const std::string& name) const;
		/// Returns a group for the given name, returns a null pointer if not found

	void addNotation(Notation::Ptr ptr);
		/// Adds an Notation to the Schema, will fail if the Notation already
		/// exists.
		/// Note that once an Notation was added to the Schema, it can not
		/// be removed!

	const Notation* getNotation(const std::string& name) const;
		/// Returns a Notation for the given name, returns a null pointer if not found

	void addImportedSchema(Ptr pSchema);
		/// Adds an imported schema.

	void includeSchema(Ptr pSchema);
		/// Includes the given schema.

	const Schema::Types& types() const;
		/// Returns all types of the schema

	const Schema::Elements& elements() const;
		/// Returns all elements exported by the schema

	const Schema::Attributes& attributes() const;
		/// Returns all attributes exported by the schema

	const Schema::AttributeGroups& attributeGroups() const;
		/// Returns all attribute groups exported by the schema

	const Schema::Groups& groups() const;
		/// Returns all groups exported by the schema

	const Schema::Notations& notations() const;
		/// Returns all notations of the schema
		
	bool qualifiedAttributeForm() const;
		/// Are attributes qualified?

	bool qualifiedElementForm() const;
		/// Are elements qualified?

	bool blockExtension() const;
		/// Are extensions forbidden?

	bool blockRestriction() const;
		/// Are restrictions forbidden?

	bool blockSubstitution() const;
		/// Is substitution forbidden?

	bool finalExtension() const;
		/// Is the default for extension final?

	bool finalRestriction() const;
		/// Is the default for restriction final?

	bool finalList() const;
		/// Is the default for list final?

	bool finalUnion() const;
		/// Is the default for union final?

	void accept(Visitor& v) const;

	void fixup();

private:
	std::string    _targetNamespace;
	Types          _declaredTypes;
	Elements       _exportedElements;
	Attributes     _exportedAttributes;
	AttributeGroups _exportedAttributeGroups;
	Groups         _exportedGroups;
	Notations      _notations;
	bool           _qualifiedAttributeForm;
	bool           _qualifiedElementForm;
	bool           _blockExtension;
	bool           _blockRestriction;
	bool           _blockSubstitution;
	bool           _finalExtension;
	bool           _finalRestriction;
	bool           _finalList;
	bool           _finalUnion;
	Schemas        _importedSchemas;
	bool           _fixedUp;
};


//
// inlines
//
inline const std::string& Schema::targetNamespace() const
{
	return _targetNamespace;
}


inline bool Schema::qualifiedAttributeForm() const
{
	return _qualifiedAttributeForm;
}


inline bool Schema::qualifiedElementForm() const
{
	return _qualifiedElementForm;
}


inline bool Schema::blockExtension() const
{
	return _blockExtension;
}


inline bool Schema::blockRestriction() const
{
	return _blockRestriction;
}


inline bool Schema::blockSubstitution() const
{
	return _blockSubstitution;
}


inline bool Schema::finalExtension() const
{
	return _finalExtension;
}


inline bool Schema::finalRestriction() const
{
	return _finalRestriction;
}


inline bool Schema::finalList() const
{
	return _finalList;
}


inline bool Schema::finalUnion() const
{
	return _finalUnion;
}


inline const Schema::Types& Schema::types() const
{
	return _declaredTypes;
}


inline const Schema::Elements& Schema::elements() const
{
	return _exportedElements;
}


inline const Schema::Attributes& Schema::attributes() const
{
	return _exportedAttributes;
}


inline const Schema::AttributeGroups& Schema::attributeGroups() const
{
	return _exportedAttributeGroups;
}


inline const Schema::Groups& Schema::groups() const
{
	return _exportedGroups;
}


inline const Schema::Notations& Schema::notations() const
{
	return _notations;
}


bool XSDTypes_API conflicts(const Schema& s1, const Schema& s2);
	/// Compares two schemas. A schema conflicts with one another
	/// when they have the same namespace but different defaults


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Schema_INCLUDED
