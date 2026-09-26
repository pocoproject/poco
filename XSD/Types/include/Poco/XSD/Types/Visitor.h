//
// Visitor.h
//
// Library: XSD/Types
// Package: Visitor
// Module:  Visitor
//
// Definition of the Visitor class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Visitor_INCLUDED
#define XSDTypes_Visitor_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"


namespace Poco {
namespace XSD {
namespace Types {


class All;
class Annotation;
class Any;
class AnyAttribute;
class AppInfo;
class Attribute;
class AttributeGroup;
class AttributeGroupRef;
class AttributeRef;
class AttributeTypeRef;
class Choice;
class ComplexType;
class Documentation;
class ElementImpl;
class ElementRef;
class ElementTypeRef;
class Group;
class GroupRef;
class InheritanceInfo;
class List;
class ListTypeRef;
class Notation;
class Schema;
class Sequence;
class SimpleType;
class SimpleRestriction;
class SimpleRestrictionInlineType;
class Union;
class Definitions;
class Message;
class Operation;
class PortType;
class Binding;
class Service;


class XSDTypes_API Visitor
	/// The Visitor interface.
{
public:
	Visitor();
		/// Creates the Visitor.

	virtual ~Visitor();
		/// Destroys the Visitor.

	virtual void visit(const Annotation& ann) = 0;
		/// Visits an object of type Annotation

	virtual void visit(const ElementImpl& ann) = 0;
		/// Visits an object of type ElementImpl

	virtual void visit(const ElementRef& ann) = 0;
		/// Visits an object of type ElementRef

	virtual void visit(const ElementTypeRef& ann) = 0;
		/// Visits an object of type ElementTypeRef

	virtual void visit(const Schema& ann) = 0;
		/// Visits an object of type Schema

	virtual void visit(const Sequence& ann) = 0;
		/// Visits an object of type Sequence

	virtual void visit(const Documentation& doc) = 0;
		/// Visits an object of type Documentation

	virtual void visit(const AppInfo& doc) = 0;
		/// Visits an object of type AppInfo

	virtual void visit(const SimpleType& val) = 0;
		/// Visits an object of type SimpleType

	virtual void visit(const ComplexType& val) = 0;
		/// Visits an object of type ComplexType

	virtual void visit(const Attribute& val) = 0;
		/// Visits an object of type Attribute

	virtual void visit(const AttributeRef& val) = 0;
		/// Visits an object of type AttributeRef

	virtual void visit(const AttributeTypeRef& val) = 0;
		/// Visits an object of type AttributeTypeRef

	virtual void visit(const AttributeGroup& val) = 0;
		/// Visits an object of type AttributeGroup

	virtual void visit(const AttributeGroupRef& val) = 0;
		/// Visits an object of type AttributeGroupRef

	virtual void visit(const Group& val) = 0;
		/// Visits an object of type Group

	virtual void visit(const GroupRef& val) = 0;
		/// Visits an object of type GroupRef

	virtual void visit(const All& val) = 0;
		/// Visits an object of type All

	virtual void visit(const Any& val) = 0;
		/// Visits an object of type Any

	virtual void visit(const AnyAttribute& val) = 0;
		/// Visits an object of type AnyAttribute

	virtual void visit(const Choice& val) = 0;
		/// Visits an object of type Choice

	virtual void visit(const Notation& val) = 0;
		/// Visits an object of type Notation

	virtual void visit(const Union& val) = 0;
		/// Visits an object of type Union

	virtual void visit(const InheritanceInfo& val) = 0;
		/// Visits an object of type InheritanceInfo

	virtual void visit(const List& val) = 0;
		/// Visits an object of type List

	virtual void visit(const ListTypeRef& val) = 0;
		/// Visits an object of type ListTypeRef

	virtual void visit(const SimpleRestriction& val) = 0;
		/// Visits an object of type SimpleRestriction

	virtual void visit(const SimpleRestrictionInlineType& val) = 0;
		/// Visits an object of type SimpleRestrictionInlineType

	virtual void visit(const Definitions& val) = 0;
		/// Visits an object of type Definitions

	virtual void visit(const Message& val) = 0;
		/// Visits an object of type Message

	virtual void visit(const Operation& val) = 0;
		/// Visits an object of type Operation

	virtual void visit(const PortType& val) = 0;
		/// Visits an object of type Operation

	virtual void visit(const Binding& val) = 0;
		/// Visits an object of type Binding.

	virtual void visit(const Service& val) = 0;
		/// Visits an object of type Operation
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Visitor_INCLUDED
