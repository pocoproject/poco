//
// AttributeTypeRef.h
//
// Library: XSD/Types
// Package: XSDAttributes
// Module:  AttributeTypeRef
//
// Definition of the AttributeTypeRef class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_AttributeTypeRef_INCLUDED
#define XSDTypes_AttributeTypeRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractAttribute.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AttributeTypeRef: public AbstractAttribute
	/// A reference to an attribute type.
{
public:
	using Ptr = AutoPtr<AttributeTypeRef>;

	AttributeTypeRef(const std::string& id, 
		const std::string& name, 
		const std::string& nameSpace,
		const QName& typeRef, 
		const std::string& fixedValue, 
		const std::string& defaultValue, 
		bool qualifiedForm, 
		AbstractAttribute::Usage use = AbstractAttribute::USE_OPTIONAL);
		/// Creates the AttributeTypeRef.

	virtual ~AttributeTypeRef();
		/// Destroys the AttributeTypeRef.

	// AbstractAttribute
	const std::string& nameSpace() const;
	const std::string& defaultValue() const;
	const std::string& fixedValue() const;
	bool qualifiedForm() const;
	const SimpleType* type() const;
	void fixup();
	AbstractAttribute::Usage usage() const;
	void accept(Visitor& v) const;

private:
	AttributeTypeRef(const std::string& id, 
		const std::string& name, 
		const std::string& nameSpace,
		const QName& typeRef, 
		const SimpleType* pType,
		const std::string& fixedValue, 
		const std::string& defaultValue, 
		bool qualifiedForm, 
		AbstractAttribute::Usage use = AbstractAttribute::USE_OPTIONAL);

	std::string _nameSpace;
	QName _typeRef;
	std::string _fixedValue;
	std::string _defaultValue;
	bool _qualifiedForm;
	AbstractAttribute::Usage _use;
	const SimpleType* _pType;

	friend class TypesManager;
};


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_Attribute_INCLUDED
