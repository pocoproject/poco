//
// AttributeRef.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeRef
//
// Definition of the AttributeRef class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AttributeRef_INCLUDED
#define XSDTypes_AttributeRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco::XSD::Types {


class XSDTypes_API AttributeRef: public AbstractAttribute
	/// AttributeRef references another Attribute.
{
public:
	using Ptr = AutoPtr<AttributeRef>;

	AttributeRef(const std::string& id, const QName& ref);
		/// Creates the AttributeRef.

	~AttributeRef() override;
		/// Destroys the AttributeRef.

	// AbstractAttribute
	[[nodiscard]] const std::string& nameSpace() const override;
	[[nodiscard]] const std::string& defaultValue() const override;
	[[nodiscard]] const std::string& fixedValue() const override;
	[[nodiscard]] bool qualifiedForm() const override;
	[[nodiscard]] const SimpleType* type() const override;
	void fixup() override;
	[[nodiscard]] AbstractAttribute::Usage usage() const override;
	void accept(Visitor& v) const override;
	[[nodiscard]] const std::string& name() const override;

private:
	AttributeRef(const std::string& id, const QName& ref, const AbstractAttribute* pAttr);

private:
	QName                    _ref;
	const AbstractAttribute* _pAttr;

	friend class TypesManager;
};


} // namespace Poco::XSD::Types


#endif // XSDTypes_AttributeRef_INCLUDED
