//
// OrderedSet.h
//
// Library: Foundation
// Package: Core
// Module:  OrderedSet
//
// Definition of the OrderedSet class template.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_OrderedSet_INCLUDED
#define Foundation_OrderedSet_INCLUDED


#include "Poco/Foundation.h"


#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include "Poco/ordered_hash.h"
#include "Poco/ordered_set.h"


namespace Poco {


template<class Key,
		class Hash = std::hash<Key>,
		class KeyEqual = std::equal_to<Key>,
		class Allocator = std::allocator<Key>,
		class ValueTypeContainer = std::deque<Key, Allocator>>
using OrderedSet = tsl::ordered_set<Key, Hash, KeyEqual, Allocator, ValueTypeContainer>;
	/// For documentation, see https://tessil.github.io/ordered-map/

} // namespace Poco


#endif // Foundation_OrderedSet_INCLUDED
