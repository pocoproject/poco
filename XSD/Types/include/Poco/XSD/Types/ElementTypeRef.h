//
// ElementTypeRef.h
//
// Library: XSD/Types
// Package: XSDElements
// Module:  ElementTypeRef
//
// Definition of the ElementTypeRef class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_ElementTypeRef_INCLUDED
#define XSDTypes_ElementTypeRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractElementImpl.h"


namespace Poco::XSD::Types {


class XSDTypes_API ElementTypeRef: public AbstractElementImpl
	/// ElementTypeRef handles the Element definition case with a given external type.
{
public:
	ElementTypeRef();
		/// Creates the ElementTypeRef.

	ElementTypeRef(
		const std::string& id, 
		Poco::UInt32 minOcc, 
		Poco::UInt32 maxOcc,
		bool isAbstract,
		bool blockRestriction,
		bool blockExtension,
		bool blockSubstitution,
		const std::string& defaultValue,
		bool finalRestriction,
		bool finalExtension,
		const std::string& fixedValue,
		bool qualified,
		const std::string& name,
		const std::string& nameSpace,
		bool nillable,
		const QName& substitutionGroup,
		const QName& typeRef);
		/// Creates an initialized ElementTypeRef

	~ElementTypeRef() override;
		/// Destroys the ElementTypeRef.

	void fixup() override;

	[[nodiscard]] const Type& type() const override;

	void accept(Visitor& v) const override;

private:
	const Type* _pType = nullptr;
	QName _typeRef;
};


//
// inlines
//
inline const Type& ElementTypeRef::type() const
{
	poco_check_ptr (_pType);
	return *_pType;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_ElementTypeRef_INCLUDED
