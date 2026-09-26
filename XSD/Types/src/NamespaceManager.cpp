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
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "Poco/XSD/Types/NamespaceManager.h"


namespace Poco {
namespace XSD {
namespace Types {


NamespaceManager& NamespaceManager::instance()
{
	static Poco::SingletonHolder<NamespaceManager> instance;
	return *instance.get();
}


NamespaceManager::NamespaceMap::const_iterator NamespaceManager::set(const std::string& ns)
{
	Poco::FastMutex::ScopedLock lock(_mutex);

	NamespaceMap::const_iterator cIt = _namespaceToId.find(ns);
	if (cIt != _namespaceToId.end())
	{
		return cIt;
	}

	int val = _maxId++;

	_idToNamespace.insert(make_pair(val, ns));
	cIt = _namespaceToId.insert(make_pair(ns, val)).first;
	return cIt;
}


NamespaceManager::NamespaceManager(): _maxId(0)
{
}


NamespaceManager::~NamespaceManager()
{
}


} } } // namespace Poco::XSD::Types
