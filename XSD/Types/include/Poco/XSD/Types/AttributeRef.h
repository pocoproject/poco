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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_AttributeRef_INCLUDED
#define XSDTypes_AttributeRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AttributeRef: public AbstractAttribute
	/// AttributeRef references another Attribute.
{
public:
	using Ptr = AutoPtr<AttributeRef>;

	AttributeRef(const std::string& id, const QName& ref);
		/// Creates the AttributeRef.

	virtual ~AttributeRef();
		/// Destroys the AttributeRef.

	// AbstractAttribute
	const std::string& nameSpace() const;
	const std::string& defaultValue() const;
	const std::string& fixedValue() const;
	bool qualifiedForm() const;
	const SimpleType* type() const;
	void fixup();
	AbstractAttribute::Usage usage() const;
	void accept(Visitor& v) const;
	const std::string& name() const;

private:
	AttributeRef(const std::string& id, const QName& ref, const AbstractAttribute* pAttr);

private:
	QName                    _ref;
	const AbstractAttribute* _pAttr;

	friend class TypesManager;
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AttributeRef_INCLUDED
