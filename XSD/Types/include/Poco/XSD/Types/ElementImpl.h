//
// ElementImpl.h
//
// Library: XSD/Types
// Package: XSDElements
// Module:  ElementImpl
//
// Definition of the ElementImpl class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_ElementImpl_INCLUDED
#define XSDTypes_ElementImpl_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AbstractElementImpl.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API ElementImpl: public AbstractElementImpl
	/// ElementImpl handles the Element definition case with a unnamed complexType that is defined internally in the Element
{
public:
	ElementImpl();
		/// Creates the ElementImpl.

	ElementImpl(
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
		const QName& substitutionGroup = QName::INVALID);
		/// Creates an initialized ElementImpl

	~ElementImpl();
		/// Destroys the ElementImpl.

	void fixup();
		/// ElementImpl is fine, just fwd to the internal _pType.

	const Type& type() const;
	
	void setType(AutoPtr<Type> pType);
		/// Sets the type internally

	void accept(Visitor& v) const;

private:
	const Type* _pType;
};


//
// inlines
//
inline const Type& ElementImpl::type() const
{
	poco_check_ptr (_pType);
	return *_pType;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_ElementImpl_INCLUDED
