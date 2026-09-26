//
// NamespaceManager.h
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  NamespaceManager
//
// Definition of the NamespaceManager class.
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef XSDTypes_NamespaceManager_INCLUDED
#define XSDTypes_NamespaceManager_INCLUDED


#include "Poco/XSD/Types/XSDTypes.h"
#include "Poco/Mutex.h"
#include "Poco/SingletonHolder.h"
#include <map>


namespace Poco {
namespace XSD {
namespace Types {


class XSDTypes_API NamespaceManager
	/// A NamespaceManager stores all known namespaces
	/// and assigns an id to each namespace. Is a singleton.
{
public:
	using NamespaceId = Poco::UInt32;
	using IdMap = std::map<NamespaceId, std::string>;
	using NamespaceMap = std::map<std::string, NamespaceId>;

	static NamespaceManager& instance();
		/// Returns the singleton.

	bool hasNamespace(const std::string& ns) const;

	bool hasNamespace(NamespaceManager::NamespaceId id) const;

	NamespaceManager::NamespaceMap::const_iterator get(const std::string& ns) const;

	NamespaceManager::NamespaceId getId(const std::string& ns) const;

	const std::string& getNamespace(NamespaceManager::NamespaceId id) const;

	NamespaceManager::NamespaceMap::const_iterator set(const std::string& ns);
		/// Inserts the namespace into the manager and returns the assigned id.
		/// If the namespace already exists, the insert is ignored and the id is returned.

	NamespaceManager::NamespaceMap::const_iterator end() const;
		/// Returns the end iterator.

private:
	NamespaceManager();
		/// Creates the NamespaceManager.

	NamespaceManager(const NamespaceManager&);

	NamespaceManager& operator= (const NamespaceManager&);

	~NamespaceManager();
		/// Destroys the NamespaceManager.

private:
	IdMap                 _idToNamespace;
	NamespaceMap          _namespaceToId;
	int                   _maxId;
	Poco::FastMutex       _mutex;

	friend class Poco::SingletonHolder<NamespaceManager>;
};


//
// inlines
//
inline bool NamespaceManager::hasNamespace(const std::string& ns) const
{
	return _namespaceToId.find(ns) != _namespaceToId.end();
}


inline bool NamespaceManager::hasNamespace(NamespaceManager::NamespaceId id) const
{
	return _idToNamespace.find(id) != _idToNamespace.end();
}


inline NamespaceManager::NamespaceMap::const_iterator NamespaceManager::get(const std::string& ns) const
{
	return _namespaceToId.find(ns);
}


inline NamespaceManager::NamespaceId NamespaceManager::getId(const std::string& ns) const
{
	poco_assert_dbg (hasNamespace(ns));
	return _namespaceToId.find(ns)->second;
}


inline const std::string& NamespaceManager::getNamespace(NamespaceManager::NamespaceId id) const
{
	poco_assert_dbg (hasNamespace(id));
	return _idToNamespace.find(id)->second;
}


inline NamespaceManager::NamespaceMap::const_iterator NamespaceManager::end() const
{
	return _namespaceToId.end();
}


} } } // namespace Poco::XSD::Types


#endif // XSDTypes_NamespaceManager_INCLUDED
