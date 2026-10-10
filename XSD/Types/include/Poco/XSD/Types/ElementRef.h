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


namespace Poco::XSD::Types {


class XSDTypes_API ElementRef: public Element
	/// An ElementRef always references another root-level element.
	/// References are only allowed to have min/maxOccurs, id and annotation,
	/// i.e. the members they inherit from OrderContent, only delegate
	/// the interface definitions from Element to the referenced object.
	/// Note that only const methods are delegated. It is not possible to change
	/// the referenced element via the ElementRef. All set methods are ignored.
{
public:
	explicit ElementRef(const QName& ref);
		/// Creates the ElementRef. The references element needs not to exist yet.

	ElementRef(const std::string& id, Poco::UInt32 minOcc, Poco::UInt32 maxOcc, const QName& ref);

	~ElementRef() override;
		/// Destroys the ElementRef.

	void fixup() override;
		/// Checks if the referenced element exists and set the element
		/// as member.

	[[nodiscard]] const std::string& nameSpace() const override;

	[[nodiscard]] bool getAbstract() const override;

	void setAbstract(bool abstr) override;

	void setBlockAll(bool block) override;

	[[nodiscard]] bool getBlockRestriction() const override;

	void setBlockRestriction(bool block) override;

	[[nodiscard]] bool getBlockExtension() const override;

	void setBlockExtension(bool block) override;

	[[nodiscard]] bool getBlockSubstitution() const override;

	void setBlockSubstitution(bool block) override;

	[[nodiscard]] const std::string& getDefault() const override;
		/// The default value of the element. Empty if no one exists.

	void setDefault(const std::string& value) override;

	[[nodiscard]] bool hasDefault() const override;

	void setFinalAll(bool fin) override;

	[[nodiscard]] bool getFinalRestriction() const override;

	void setFinalRestriction(bool fin) override;

	[[nodiscard]] bool getFinalExtension() const override;

	void setFinalExtension(bool fin) override;

	[[nodiscard]] const std::string& getFixed() const override;
		/// The fixed value of the element. Empty if no one exists.

	void setFixed(const std::string& value) override;

	[[nodiscard]] bool hasFixed() const override;

	[[nodiscard]] bool getQualified() const override;
	/// Returns if the element is in qualified form or not

	void setQualified(bool qual) override;

	[[nodiscard]] const std::string& name() const override;

	void setName(const std::string& name) override;

	[[nodiscard]] bool getNillable() const override;

	void setNillable(bool nillable) override;

	[[nodiscard]] const QName& getSubstitutionGroup() const override;

	void setSubstitutionGroup(const QName& ref) override;

	[[nodiscard]] bool hasSubstitutionGroup() const override;

	[[nodiscard]] const Type& type() const override;

	void accept(Visitor& v) const override;

private:
	QName _ref;
	const Element* _pElement = nullptr;
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


} // namespace Poco::XSD::Types


#endif // XSDTypes_ElementRef_INCLUDED
