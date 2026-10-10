//
// Type.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  Type
//
// Definition of the Type class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_Type_INCLUDED
#define XSDTypes_Type_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/AnnotatedObject.h"
#include "Poco/XSD/Types/OrderIterator.h"
#include <vector>


namespace Poco::XSD::Types {


class Schema;


class XSDTypes_API Type: public AnnotatedObject
	/// Super class for all Types.
{
public:
	using Ptr = Poco::AutoPtr<Type>;

	static const std::vector<const Type*> NOPARENTS;

	Type();
		/// Creates the Type.

	Type(const std::string& id, const std::string& name);
		/// Creates the Type.

	~Type() override;
		/// Destroys the Type.

	void setName(const std::string& name);
		/// Sets the name

	[[nodiscard]] const std::string& name() const;
		/// Returns the name, can be empty

	virtual void fixup() = 0;
		/// Resolves type references to a parent class

	[[nodiscard]] virtual const std::vector<const Type*>& parents() const = 0;
		/// Returns the parent classes 

	[[nodiscard]] OrderIterator iterator() const;
		/// Creates an iterator for the type taking care of inheritance information

	void setSchema(const Schema* pSchema);
		/// Sets the schema

	[[nodiscard]] const Schema* getSchema() const;
		/// Gets the schema. Note that internal inline types have no schema associated!

protected:
	void iteratorRec(std::vector<OrderIterator>& seq) const;
		/// Creates itertaors recursively

	virtual void createIterator(std::vector<OrderIterator>& seq) const = 0;
		/// Appends/replace an iterator to/in the sequence

private:
	std::string _name;
	const Schema*  _pSchema = nullptr;
};


//
// inlines
//
inline void Type::setName(const std::string& name)
{
	_name = name;
}


inline const std::string& Type::name() const
{
	return _name;
}


inline void Type::setSchema(const Schema* pSchema)
{
	_pSchema = pSchema;
}


inline const Schema* Type::getSchema() const
{
	return _pSchema;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_Type_INCLUDED
