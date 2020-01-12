//
// OrderedMap.h
//
// Library: Foundation
// Package: Core
// Module:  OrderedMap
//
// Definition of the OrderedMap class template.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_OrderedMap_INCLUDED
#define Foundation_OrderedMap_INCLUDED


#include "Poco/Foundation.h"


#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include "Poco/ordered_hash.h"
#include "Poco/ordered_map.h"


namespace Poco {


template<class Key,
		class T,
		class Hash = std::hash<Key>,
		class KeyEqual = std::equal_to<Key>,
		class Allocator = std::allocator<std::pair<Key, T>>,
		class ValueTypeContainer = std::deque<std::pair<Key, T>, Allocator>>
using OrderedMap = tsl::ordered_map<Key, T, Hash, KeyEqual, Allocator, ValueTypeContainer>;
	/// For documentation, see https://tessil.github.io/ordered-map/


} // namespace Poco


#endif // Foundation_OrderedMap_INCLUDED
