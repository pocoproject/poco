//
// Types.cppm
//
// C++ module file
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_XSD_TYPES
#include "Poco/XSD/Types/AbstractAttributeGroup.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/AbstractElementImpl.h"
#include "Poco/XSD/Types/AbstractGroup.h"
#include "Poco/XSD/Types/AbstractList.h"
#include "Poco/XSD/Types/All.h"
#include "Poco/XSD/Types/AllIterator.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/AnnotationContent.h"
#include "Poco/XSD/Types/Annotation.h"
#include "Poco/XSD/Types/AnyAttribute.h"
#include "Poco/XSD/Types/Any.h"
#include "Poco/XSD/Types/AnyIterator.h"
#include "Poco/XSD/Types/AppInfo.h"
#include "Poco/XSD/Types/AttributeContent.h"
#include "Poco/XSD/Types/AttributeGroup.h"
#include "Poco/XSD/Types/AttributeGroupRef.h"
#include "Poco/XSD/Types/Attribute.h"
#include "Poco/XSD/Types/AttributeHolder.h"
#include "Poco/XSD/Types/AttributeRef.h"
#include "Poco/XSD/Types/AttributeTypeRef.h"
#include "Poco/XSD/Types/Binding.h"
#include "Poco/XSD/Types/BindingProperties.h"
#include "Poco/XSD/Types/Choice.h"
#include "Poco/XSD/Types/ChoiceIterator.h"
#include "Poco/XSD/Types/CollectionContent.h"
#include "Poco/XSD/Types/ComplexType.h"
#include "Poco/XSD/Types/Content.h"
#include "Poco/XSD/Types/Definitions.h"
#include "Poco/XSD/Types/Documentation.h"
#include "Poco/XSD/Types/Element.h"
#include "Poco/XSD/Types/ElementImpl.h"
#include "Poco/XSD/Types/ElementIterator.h"
#include "Poco/XSD/Types/ElementRef.h"
#include "Poco/XSD/Types/ElementTypeRef.h"
#include "Poco/XSD/Types/Group.h"
#include "Poco/XSD/Types/GroupRef.h"
#include "Poco/XSD/Types/InheritanceInfo.h"
#include "Poco/XSD/Types/List.h"
#include "Poco/XSD/Types/ListTypeRef.h"
#include "Poco/XSD/Types/Message.h"
#include "Poco/XSD/Types/NamespaceManager.h"
#include "Poco/XSD/Types/Notation.h"
#include "Poco/XSD/Types/Operation.h"
#include "Poco/XSD/Types/OrderContent.h"
#include "Poco/XSD/Types/Order.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include "Poco/XSD/Types/OrderIteratorImpl.h"
#include "Poco/XSD/Types/PortType.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/Schema.h"
#include "Poco/XSD/Types/Sequence.h"
#include "Poco/XSD/Types/SequenceIterator.h"
#include "Poco/XSD/Types/Service.h"
#include "Poco/XSD/Types/SimpleRestriction.h"
#include "Poco/XSD/Types/SimpleRestrictionInlineType.h"
#include "Poco/XSD/Types/SimpleType.h"
#include "Poco/XSD/Types/SimpleTypeInheritance.h"
#include "Poco/XSD/Types/Type.h"
#include "Poco/XSD/Types/TypesManager.h"
#include "Poco/XSD/Types/Union.h"
#include "Poco/XSD/Types/Visitable.h"
#include "Poco/XSD/Types/Visitor.h"
#include "Poco/XSD/Types/XSDException.h"
#include "Poco/XSD/Types/XSDTypes.h"
#endif

export module Poco:XSD.Types;

export namespace Poco::XSD::Types {
	#ifdef ENABLE_XSD_TYPES
	using Poco::XSD::Types::AbstractAttribute;
	using Poco::XSD::Types::AbstractAttributeGroup;
	using Poco::XSD::Types::AbstractElementImpl;
	using Poco::XSD::Types::AbstractGroup;
	using Poco::XSD::Types::AbstractList;
	using Poco::XSD::Types::All;
	using Poco::XSD::Types::AllIterator;
	using Poco::XSD::Types::AnnotatedObject;
	using Poco::XSD::Types::Annotation;
	using Poco::XSD::Types::AnnotationContent;
	using Poco::XSD::Types::Any;
	using Poco::XSD::Types::AnyAttribute;
	using Poco::XSD::Types::AnyIterator;
	using Poco::XSD::Types::AppInfo;
	using Poco::XSD::Types::Attribute;
	using Poco::XSD::Types::AttributeContent;
	using Poco::XSD::Types::AttributeGroup;
	using Poco::XSD::Types::AttributeGroupRef;
	using Poco::XSD::Types::AttributeHolder;
	using Poco::XSD::Types::AttributeRef;
	using Poco::XSD::Types::AttributeTypeRef;
	using Poco::XSD::Types::Binding;
	using Poco::XSD::Types::BindingProperties;
	using Poco::XSD::Types::Choice;
	using Poco::XSD::Types::ChoiceIterator;
	using Poco::XSD::Types::CollectionContent;
	using Poco::XSD::Types::ComplexType;
	using Poco::XSD::Types::Content;
	using Poco::XSD::Types::Definitions;
	using Poco::XSD::Types::Documentation;
	using Poco::XSD::Types::Element;
	using Poco::XSD::Types::ElementAlreadyDefinedException;
	using Poco::XSD::Types::ElementException;
	using Poco::XSD::Types::ElementImpl;
	using Poco::XSD::Types::ElementIterator;
	using Poco::XSD::Types::ElementRef;
	using Poco::XSD::Types::ElementTypeRef;
	using Poco::XSD::Types::Group;
	using Poco::XSD::Types::GroupRef;
	using Poco::XSD::Types::IllegalOrderException;
	using Poco::XSD::Types::InheritanceInfo;
	using Poco::XSD::Types::InvalidElementException;
	using Poco::XSD::Types::InvalidTypeException;
	using Poco::XSD::Types::List;
	using Poco::XSD::Types::ListTypeRef;
	using Poco::XSD::Types::Message;
	using Poco::XSD::Types::NamespaceManager;
	using Poco::XSD::Types::Notation;
	using Poco::XSD::Types::NullElementException;
	using Poco::XSD::Types::NullTypeException;
	using Poco::XSD::Types::Operation;
	using Poco::XSD::Types::Order;
	using Poco::XSD::Types::OrderContent;
	using Poco::XSD::Types::OrderIterator;
	using Poco::XSD::Types::OrderIteratorImpl;
	using Poco::XSD::Types::PortType;
	using Poco::XSD::Types::QName;
	using Poco::XSD::Types::Schema;
	using Poco::XSD::Types::SchemaException;
	using Poco::XSD::Types::Sequence;
	using Poco::XSD::Types::SequenceIterator;
	using Poco::XSD::Types::Service;
	using Poco::XSD::Types::SimpleRestriction;
	using Poco::XSD::Types::SimpleRestrictionInlineType;
	using Poco::XSD::Types::SimpleType;
	using Poco::XSD::Types::SimpleTypeInheritance;
	using Poco::XSD::Types::Type;
	using Poco::XSD::Types::TypeAlreadyDefinedException;
	using Poco::XSD::Types::TypeException;
	using Poco::XSD::Types::TypesManager;
	using Poco::XSD::Types::Union;
	using Poco::XSD::Types::Visitable;
	using Poco::XSD::Types::Visitor;
	using Poco::XSD::Types::XSDException;

	using Poco::XSD::Types::conflicts;
	#endif
}
