//
// ElementRef.h
//
// Library: XSD/Types
// Package: XSDElements
// Module:  ElementRef
//
// Definition of the ElementRef class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_ElementRef_INCLUDED
#define XSDTypes_ElementRef_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/Element.h"
#include "Poco/XSD/Types/QName.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API ElementRef: public Element
	/// An ElementRef always references another root-level element.
	/// References are only allowed to have min/maxOccurs, id and annotation,
	/// i.e. the members they inherit from OrderContent, only delegate
	/// the interface definitions from Element to the referenced object.
	/// Note that only const methods are delegated. It is not possible to change
	/// the referenced element via the ElementRef. All set methods are ignored.
{
public:
	ElementRef();
		/// Creates the ElementRef.

	ElementRef(const QName& ref);
		/// Creates the ElementRef. The references element needs not to exist yet.

	ElementRef(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc, const QName& ref);

	~ElementRef();
		/// Destroys the ElementRef.

	void fixup();
		/// Checks if the referenced element exists and set the element
		/// as member.

	const std::string& nameSpace() const;

	bool getAbstract() const;

	void setAbstract(bool abstr);

	void setBlockAll(bool block);

	bool getBlockRestriction() const;

	void setBlockRestriction(bool block);

	bool getBlockExtension() const;

	void setBlockExtension(bool block);

	bool getBlockSubstitution() const;

	void setBlockSubstitution(bool block);

	const std::string& getDefault() const;
		/// The default value of the element. Empty if no one exists.

	void setDefault(const std::string& value);

	bool hasDefault() const;

	void setFinalAll(bool fin);

	bool getFinalRestriction() const;

	void setFinalRestriction(bool fin);

	bool getFinalExtension() const;

	void setFinalExtension(bool fin);

	const std::string& getFixed() const;
		/// The fixed value of the element. Empty if no one exists.

	void setFixed(const std::string& value);

	bool hasFixed() const;

	bool getQualified() const;
	/// Returns if the element is in qualified form or not

	void setQualified(bool qual);

	const std::string& name() const;

	void setName(const std::string& name);

	bool getNillable() const;

	void setNillable(bool nillable);

	const QName& getSubstitutionGroup() const;

	void setSubstitutionGroup(const QName& ref);

	bool hasSubstitutionGroup() const;

	const Type& type() const;
	
	void setType(const Type& aType);

	void accept(Visitor& v) const;

private:
	QName _ref;
	const Element* _pElement;
		/// The referenced element.
};


//
// inlines
//
inline bool ElementRef::getAbstract() const
{
	return _pElement->getAbstract();
}


inline void ElementRef::setAbstract(bool)
{
}


inline void ElementRef::setBlockAll(bool)
{
}


inline bool ElementRef::getBlockRestriction() const
{
	return _pElement->getBlockRestriction();
}


inline void ElementRef::setBlockRestriction(bool)
{
}


inline bool ElementRef::getBlockExtension() const
{
	return _pElement->getBlockExtension();
}


inline void ElementRef::setBlockExtension(bool)
{
}


inline bool ElementRef::getBlockSubstitution() const
{
	return _pElement->getBlockSubstitution();
}


inline void ElementRef::setBlockSubstitution(bool)
{
}


inline const std::string& ElementRef::getDefault() const
{
	return _pElement->getDefault();
}


inline void ElementRef::setDefault(const std::string&)
{
}


inline bool ElementRef::hasDefault() const
{
	return _pElement->hasDefault();
}


inline void ElementRef::setFinalAll(bool)
{
}


inline bool ElementRef::getFinalRestriction() const
{
	return _pElement->getFinalRestriction();
}


inline void ElementRef::setFinalRestriction(bool)
{
}


inline bool ElementRef::getFinalExtension() const
{
	return _pElement->getFinalExtension();
}


inline void ElementRef::setFinalExtension(bool)
{
}


inline const std::string& ElementRef::getFixed() const
{
	return _pElement->getFixed();
}


inline void ElementRef::setFixed(const std::string&)
{
}


inline bool ElementRef::hasFixed() const
{
	return _pElement->hasFixed();
}


inline bool ElementRef::getQualified() const
{
	return _pElement->getQualified();
}


inline void ElementRef::setQualified(bool)
{
}


inline const std::string& ElementRef::name() const
{
	return _pElement->name();
}


inline void ElementRef::setName(const std::string&)
{
}


inline bool ElementRef::getNillable() const
{
	return _pElement->getNillable();
}


inline void ElementRef::setNillable(bool)
{
}


inline const QName& ElementRef::getSubstitutionGroup() const
{
	return _pElement->getSubstitutionGroup();
}


inline void ElementRef::setSubstitutionGroup(const QName&)
{
}


inline bool ElementRef::hasSubstitutionGroup() const
{
	return _pElement->hasSubstitutionGroup();
}


inline const Type& ElementRef::type() const
{
	return _pElement->type();
}


inline void ElementRef::setType(const Type&)
{
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_ElementRef_INCLUDED
