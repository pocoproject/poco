//
// QName.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  QName
//
// Definition of the QName class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef XSDTypes_QName_INCLUDED
#define XSDTypes_QName_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/XSD/Types/NamespaceManager.h"


namespace Poco::XSD::Types {


class XSDTypes_API QName
	/// A Qualified Name.
{
public:
	static const QName INVALID;

	QName();
	 /// Creates an invalid QName

	QName(const std::string& name, const std::string& ns);
		/// Creates the QName.

	QName(const QName& qname);

	QName& operator = (const QName& other);

	[[nodiscard]] bool operator == (const QName& other) const;

	[[nodiscard]] bool operator != (const QName& other) const;

	~QName();
		/// Destroys the QName.

	[[nodiscard]] const std::string& name() const;

	[[nodiscard]] const std::string& getNamespace() const;

	[[nodiscard]] NamespaceManager::NamespaceId getNamespaceId() const;

	[[nodiscard]] const NamespaceManager::NamespaceMap::const_iterator& getIterator() const;

private:
	std::string _name;
		/// The name of the QName

	NamespaceManager::NamespaceMap::const_iterator _it;
		/// The iterator to the namespace entry
};


//
// inlines
//
inline bool QName::operator == (const QName& other) const
{
	return other.name() == name() && other.getIterator() == getIterator();
}


inline bool QName::operator != (const QName& other) const
{
	return !this->operator ==(other);
}


inline const std::string& QName::name() const
{
	return _name;
}


inline const std::string& QName::getNamespace() const
{
	return _it->first;
}


inline NamespaceManager::NamespaceId QName::getNamespaceId() const
{
	return _it->second;
}


inline const NamespaceManager::NamespaceMap::const_iterator& QName::getIterator() const
{
	return _it;
}


} // namespace Poco::XSD::Types


#endif // XSDTypes_QName_INCLUDED
