//
// NamespaceManager.cpp
//
// Library: XSD/Types
// Package: XSDTypes
// Module:  NamespaceManager
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/XSD/Types/NamespaceManager.h"


namespace Poco::XSD::Types {


NamespaceManager& NamespaceManager::instance()
{
	static Poco::SingletonHolder<NamespaceManager> instance;
	return *instance.get();
}


NamespaceManager::NamespaceMap::const_iterator NamespaceManager::set(const std::string& ns)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	if (NamespaceMap::const_iterator cIt = _namespaceToId.find(ns); cIt != _namespaceToId.end())
	{
		return cIt;
	}

	int val = _maxId++;

	_idToNamespace.try_emplace(val, ns);
	return _namespaceToId.try_emplace(ns, val).first;
}


NamespaceManager::NamespaceManager() = default;


NamespaceManager::~NamespaceManager() = default;


} // namespace Poco::XSD::Types
