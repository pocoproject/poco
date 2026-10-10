//
// Element.h
//
// Library: XSD/Types
// Package: XSDElements
// Module:  Element
//
// Definition of the Element class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Element_INCLUDED
#define XSDTypes_Element_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/OrderContent.h"


namespace Poco::XSD::Types {


class QName;
class Type;
class OrderIterator;


class XSDTypes_API Element: public OrderContent
	/// This class represents an element definition in an XML Schema.
{
public:
	using Ptr = Poco::AutoPtr<Element>;

	Element();
		/// Creates the Element.

	Element(const std::string& id, Poco::UInt32 minOccurs, Poco::UInt32 maxOccurs);
		/// Creates the Element.

	~Element() override;
		/// Destroys the Element.

	[[nodiscard]] OrderIterator iterator() const override;
		/// Returns an iterator for iterating over the element's content.

	void fixup() override = 0;
		/// Fixes the element definition. Should be called once
		/// for each element after the whole XSD file was parsed.

	[[nodiscard]] virtual bool getAbstract() const = 0;

	virtual void setAbstract(bool abstr) = 0;

	virtual void setBlockAll(bool block) = 0;

	[[nodiscard]] virtual bool getBlockRestriction() const = 0;

	virtual void setBlockRestriction(bool block) = 0;

	[[nodiscard]] virtual bool getBlockExtension() const = 0;

	virtual void setBlockExtension(bool block) = 0;

	[[nodiscard]] virtual bool getBlockSubstitution() const = 0;

	virtual void setBlockSubstitution(bool block) = 0;

	[[nodiscard]] virtual const std::string& getDefault() const = 0;
		/// Returns the default value of the element. Empty if no one exists.

	virtual void setDefault(const std::string& value) = 0;
		/// Sets the default value of the element.

	[[nodiscard]] virtual bool hasDefault() const = 0;

	virtual void setFinalAll(bool fin) = 0;

	[[nodiscard]] virtual bool getFinalRestriction() const = 0;

	virtual void setFinalRestriction(bool fin) = 0;

	[[nodiscard]] virtual bool getFinalExtension() const = 0;

	virtual void setFinalExtension(bool fin) = 0;

	[[nodiscard]] virtual const std::string& getFixed() const = 0;
		/// The fixed value of the element. Empty if no one exists.

	virtual void setFixed(const std::string& value) = 0;

	[[nodiscard]] virtual bool hasFixed() const = 0;

	[[nodiscard]] virtual bool getQualified() const = 0;
		/// Returns true if the element is in qualified form or not

	virtual void setQualified(bool qual) = 0;

	[[nodiscard]] virtual const std::string& name() const = 0;

	virtual void setName(const std::string& name) = 0;

	[[nodiscard]] virtual bool getNillable() const = 0;

	virtual void setNillable(bool nillable) = 0;

	[[nodiscard]] virtual const QName& getSubstitutionGroup() const = 0;

	virtual void setSubstitutionGroup(const QName& ref) = 0;

	[[nodiscard]] virtual bool hasSubstitutionGroup() const = 0;

	[[nodiscard]] virtual const Type& type() const = 0;
	
	[[nodiscard]] virtual const std::string& nameSpace() const = 0;
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_Element_INCLUDED
