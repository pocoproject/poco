//
// HashMap.h
//
// Library: Foundation
// Package: Hashing
// Module:  HashMap
//
// Definition of the HashMap class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_HashMap_INCLUDED
#define Foundation_HashMap_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/LinearHashTable.h"
#include "Poco/Exception.h"
#include <utility>


namespace Poco {


template <class Key, class Value>
struct HashMapEntry
	/// This class template is used internally by HashMap.
{
	Key   first;
	Value second;

	HashMapEntry():
		first(),
		second()
	{
	}

	HashMapEntry(const Key& key):
		first(key),
		second()
	{
	}

	HashMapEntry(const Key& key, const Value& value):
		first(key),
		second(value)
	{
	}

	bool operator == (const HashMapEntry& entry) const
	{
		return first == entry.first;
	}

	bool operator != (const HashMapEntry& entry) const
	{
		return first != entry.first;
	}
};


template <class HME, class KeyHashFunc>
struct HashMapEntryHash
	/// This class template is used internally by HashMap.
{
	std::size_t operator () (const HME& entry) const
	{
		return _func(entry.first);
	}

private:
	KeyHashFunc _func;
};


template <class Key, class Mapped, class HashFunc = Hash<Key>>
class HashMap
	/// This class implements a map using a LinearHashTable.
	///
	/// A HashMap can be used just like a std::map.
{
public:
	using KeyType = Key;
	using MappedType = Mapped;
	using Reference = Mapped &;
	using ConstReference = const Mapped &;
	using Pointer = Mapped *;
	using ConstPointer = const Mapped *;

	using ValueType = HashMapEntry<Key, Mapped>;
	using PairType = std::pair<KeyType, MappedType>;

	using HashType = HashMapEntryHash<ValueType, HashFunc>;
	using HashTable = LinearHashTable<ValueType, HashType>;

	using Iterator = typename HashTable::Iterator;
	using ConstIterator = typename HashTable::ConstIterator;

	HashMap() = default;

	HashMap(std::size_t initialReserve):
		_table(initialReserve)
		/// Creates the HashMap with room for initialReserve entries.
	{
	}

	HashMap& operator = (const HashMap& map)
		/// Assigns another HashMap.
	{
		HashMap tmp(map);
		swap(tmp);
		return *this;
	}

	void swap(HashMap& map) noexcept
		/// Swaps the HashMap with another one.
	{
		_table.swap(map._table);
	}

	ConstIterator begin() const
	{
		return _table.begin();
	}

	ConstIterator end() const
	{
		return _table.end();
	}

	Iterator begin()
	{
		return _table.begin();
	}

	Iterator end()
	{
		return _table.end();
	}

	ConstIterator find(const KeyType& key) const
	{
		ValueType value(key);
		return _table.find(value);
	}

	Iterator find(const KeyType& key)
	{
		ValueType value(key);
		return _table.find(value);
	}

	std::size_t count(const KeyType& key) const
	{
		ValueType value(key);
		return _table.find(value) != _table.end() ? 1 : 0;
	}

	std::pair<Iterator, bool> insert(const PairType& pair)
	{
		ValueType value(pair.first, pair.second);
		return _table.insert(value);
	}

	std::pair<Iterator, bool> insert(const ValueType& value)
	{
		return _table.insert(value);
	}

	void erase(Iterator it)
	{
		_table.erase(it);
	}

	void erase(const KeyType& key)
	{
		Iterator it = find(key);
		_table.erase(it);
	}

	void clear()
	{
		_table.clear();
	}

	std::size_t size() const
	{
		return _table.size();
	}

	bool empty() const
	{
		return _table.empty();
	}

	ConstReference operator [] (const KeyType& key) const
	{
		ConstIterator it = _table.find(key);
		if (it != _table.end())
			return it->second;
		else
			throw NotFoundException();
	}

	Reference operator [] (const KeyType& key)
	{
		ValueType value(key);
		std::pair<Iterator, bool> res = _table.insert(value);
		return res.first->second;
	}

private:
	HashTable _table;
};


} // namespace Poco


#endif // Foundation_HashMap_INCLUDED
