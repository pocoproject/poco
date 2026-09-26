//
// AbstractElementImpl.h
//
// Library: XSD/Types
// Package: XSDElements
// Module:  AbstractElementImpl
//
// Definition of the AbstractElementImpl class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_AbstractElementImpl_INCLUDED
#define XSDTypes_AbstractElementImpl_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/QName.h"
#include "Poco/XSD/Types/Element.h"


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API AbstractElementImpl: public Element
	/// AbstractElementImpl handles the Element definition case with a named complexType.
{
public:
	AbstractElementImpl();
		/// Creates the AbstractElementImpl.

	AbstractElementImpl(
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
		/// Creates an initialized AbstractElementImpl

	~AbstractElementImpl();
		/// Destroys the AbstractElementImpl.

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

	void setFixed(const std::string& value);

	bool hasFixed() const;

	bool getQualified() const;

	void setQualified(bool qual);

	const std::string& name() const;

	void setName(const std::string& name);

	bool getNillable() const;

	void setNillable(bool nillable);

	const QName& getSubstitutionGroup() const;

	void setSubstitutionGroup(const QName& ref);

	bool hasSubstitutionGroup() const;
	
	const std::string& nameSpace() const;

private:
	bool _abstract;
	bool _blockRestriction;
	bool _blockExtension;
	bool _blockSubstitution;
	std::string _default;
	bool _finalRestriction;
	bool _finalExtension;
	std::string _fixed;
	bool _qualified;
	std::string _name;
	std::string _nameSpace;
	bool _nillable;
	QName _substitutionGroup;
		/// A substitutiongroup can replace an element with its own definition
		/// (e.g a complex type contains element X, Y substitutes X, which changes the complex Type to contain Y)
};


//
// inlines
//
inline bool AbstractElementImpl::getAbstract() const
{
	return _abstract;
}


inline void AbstractElementImpl::setAbstract(bool abstr)
{
	_abstract = abstr;
}


inline void AbstractElementImpl::setBlockAll(bool block)
{
	_blockRestriction = _blockExtension = _blockSubstitution = block;
}


inline bool AbstractElementImpl::getBlockRestriction() const
{
	return _blockRestriction;
}


inline void AbstractElementImpl::setBlockRestriction(bool block)
{
	_blockRestriction = block;
}


inline bool AbstractElementImpl::getBlockExtension() const
{
	return _blockExtension;
}


inline void AbstractElementImpl::setBlockExtension(bool block)
{
	_blockExtension = block;
}


inline bool AbstractElementImpl::getBlockSubstitution() const
{
	return _blockSubstitution;
}


inline void AbstractElementImpl::setBlockSubstitution(bool block)
{
	_blockSubstitution = block;
}


inline const std::string& AbstractElementImpl::getDefault() const
{
	return _default;
}


inline void AbstractElementImpl::setDefault(const std::string& str)
{
	_default = str;
}


inline bool AbstractElementImpl::hasDefault() const
{
	return !_default.empty();
}


inline void AbstractElementImpl::setFinalAll(bool fin)
{
	_finalRestriction = _finalExtension = fin;
}


inline bool AbstractElementImpl::getFinalRestriction() const
{
	return _finalRestriction;
}


inline void AbstractElementImpl::setFinalRestriction(bool block)
{
	_finalRestriction = block;
}


inline bool AbstractElementImpl::getFinalExtension() const
{
	return _finalExtension;
}


inline void AbstractElementImpl::setFinalExtension(bool block)
{
	_finalExtension = block;
}


inline const std::string& AbstractElementImpl::getFixed() const
{
	return _fixed;
}


inline void AbstractElementImpl::setFixed(const std::string& value)
{
	_fixed = value;
}


inline bool AbstractElementImpl::hasFixed() const
{
	return !_fixed.empty();
}


inline bool AbstractElementImpl::getQualified() const
{
	return _qualified;
}


inline void AbstractElementImpl::setQualified(bool qual)
{
	_qualified = qual;
}


inline const std::string& AbstractElementImpl::name() const
{
	return _name;
}


inline void AbstractElementImpl::setName(const std::string& name)
{
	_name = name;
}


inline bool AbstractElementImpl::getNillable() const
{
	return _nillable;
}


inline void AbstractElementImpl::setNillable(bool nillable)
{
	_nillable = nillable;
}


inline const QName& AbstractElementImpl::getSubstitutionGroup() const
{
	return _substitutionGroup;
}


inline void AbstractElementImpl::setSubstitutionGroup(const QName& ref)
{
	_substitutionGroup = ref;
}


inline bool AbstractElementImpl::hasSubstitutionGroup() const
{
	return _substitutionGroup != QName::INVALID;
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_AbstractElementImpl_INCLUDED
