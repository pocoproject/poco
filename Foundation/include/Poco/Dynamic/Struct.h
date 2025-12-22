//
// Struct.h
//
// Library: Foundation
// Package: Dynamic
// Module:  Struct
//
// Definition of the Struct class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Struct_INCLUDED
#define Foundation_Struct_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Dynamic/VarHolder.h"
#include "Poco/SharedPtr.h"
#include "Poco/OrderedMap.h"
#include "Poco/OrderedSet.h"
#include <map>
#include <set>


namespace Poco {
namespace Dynamic {


template <typename S, typename I = typename S::ConstIterator>
std::string structToString(const S& data, bool wrap = true)
	/// Utility function for converting DynamicStruct to std::string.
	/// Set wrap to false in order to prevent string values wrapping
	/// (useful to prevent JSON fragments from being treated as strings).
{
	std::string val;
	val.append("{ ");
	I it = data.begin();
	I itEnd = data.end();
	if (!data.empty())
	{
		const Var key(it->first);
		Impl::appendJSONKey(val, key);
		val.append(": ");
		Impl::appendJSONValue(val, it->second, wrap);
		++it;
	}
	for (; it != itEnd; ++it)
	{
		val.append(", ");
		const Var key(it->first);
		Impl::appendJSONKey(val, key);
		val.append(": ");
		Impl::appendJSONValue(val, it->second, wrap);
	}
	val.append(" }");
	return val;
}


template <typename K, typename M = std::map<K, Var>, typename S = std::set<K>>
class Struct
	/// Struct allows to define a named collection of Var objects.
{
public:
	using Data = M;
	using NameSet = S;
	using Iterator = typename Data::iterator;
	using ConstIterator = typename Data::const_iterator;
	using ValueType = typename Struct<K>::Data::value_type;
	using SizeType = typename Struct<K>::Data::size_type;
	using InsRetVal = typename std::pair<typename Struct<K, M, S>::Iterator, bool>;
	using Ptr = typename Poco::SharedPtr<Struct<K, M, S>>;

	Struct(): _data()
		/// Creates an empty Struct
	{
	}

	Struct(const Data& val): _data(val)
		/// Creates the Struct from the given value.
	{
	}

	template <typename T>
	Struct(const std::map<K, T>& val)
	{
		assignMap(val);
	}

	template <typename T>
	Struct(const OrderedMap<K, T>& val)
	{
		assignMap(val);
	}

	virtual ~Struct() = default;
		/// Destroys the Struct.

	inline Var& operator [] (const K& name)
		/// Returns the Var with the given name, creates an entry if not found.
	{
		return _data[name];
	}

	const Var& operator [] (const K& name) const
		/// Returns the Var with the given name, throws a
		/// NotFoundException if the data member is not found.
	{
		const auto it = find(name);
		if (it == end()) throw NotFoundException(name);
		return it->second;
	}

	inline bool contains(const K& name) const
		/// Returns true if the Struct contains a member with the given name
	{
		return find(name) != end();
	}

	inline Iterator find(const K& name)
		/// Returns an iterator, pointing to the <name,Var> pair containing
		/// the element, or it returns end() if the member was not found
	{
		return _data.find(name);
	}

	inline ConstIterator find(const K& name) const
		/// Returns a const iterator, pointing to the <name,Var> pair containing
		/// the element, or it returns end() if the member was not found
	{
		return _data.find(name);
	}

	inline Iterator end()
		/// Returns the end iterator for the Struct
	{
		return _data.end();
	}

	inline ConstIterator end() const
		/// Returns the end const iterator for the Struct
	{
		return _data.end();
	}

	inline Iterator begin()
		/// Returns the begin iterator for the Struct
	{
		return _data.begin();
	}

	inline ConstIterator begin() const
		/// Returns the begin const iterator for the Struct
	{
		return _data.begin();
	}

	template <typename T>
	inline InsRetVal insert(const K& key, const T& value)
		/// Inserts a <name, Var> pair into the Struct,
		/// returns a pair containing the iterator and a boolean which
		/// indicates success or not (is true, when insert succeeded, false,
		/// when already another element was present, in this case Iterator
		/// points to that other element)
	{
		// fix: SunPro C++ is silly ...
		ValueType valueType(key, value);
		return insert(valueType);
	}

	inline InsRetVal insert(const ValueType& aPair)
		/// Inserts a <name, Var> pair into the Struct,
		/// returns a pair containing the iterator and a boolean which
		/// indicates success or not (is true, when insert succeeded, false,
		/// when already another element was present, in this case Iterator
		/// points to that other element)
	{
		return _data.insert(aPair);
	}

	inline SizeType erase(const K& key)
		/// Erases the element if found, returns number of elements deleted
	{
		return _data.erase(key);
	}

	inline void erase(Iterator& it)
		/// Erases the element at the given position
	{
		_data.erase(it);
	}

	inline void clear()
		/// Remove all elements from the struct
	{
		_data.clear();
	}

	inline void swap(Struct& other) noexcept
		/// Swap content of Struct with another Struct
	{
		_data.swap(other._data);
	}

	inline bool empty() const
		/// Returns true if the Struct doesn't contain any members
	{
		return _data.empty();
	}

	SizeType size() const
		/// Returns the number of members the Struct contains
	{
		return _data.size();
	}

	inline NameSet members() const
		/// Returns a sorted collection containing all member names
	{
		NameSet keys;
		ConstIterator it = begin();
		ConstIterator itEnd = end();
		for (; it != itEnd; ++it) keys.insert(it->first);
		return keys;
	}

	inline Var getVar(const K& key) const
		/// Returns the var value of the element with the given name.
		/// Throws a NotFoundException if the key does not exist.
	{
		ConstIterator it = find(key);
		if(it == end())
		{
			throw NotFoundException("Key not found in Struct");
		}
		return it->second;
	}

	template<typename DefT = Var>
	inline Var getVar(const K& key, const DefT& defaultValue) const
		/// Returns the var value of the element with the given name.
		/// or defaultValue if none is found.
	{
		ConstIterator it = find(key);
		if(it == end())
		{
			return defaultValue;
		}
		return it->second;
	}

	std::string toString(bool wrap = true) const
		/// Returns the DynamicStruct as string.
		///
		/// To prevent unwanted string wrapping
		/// (eg. when a value is JSON string),
		/// `wrap` should be false. Note, however,
		/// that wrap argument is of a limited utility
		/// because it applies to the entire Struct,
		/// so it should not be relied on when mixed content
		/// (ie. plain string, which should be wrapped,
		/// and JSON-as-string entries, which shouldn't)
		/// is held.
	{
		return structToString<Data, ConstIterator>(_data, wrap);
	}

private:
	template <typename T>
	void assignMap(const T& map)
	{
		using MapConstIterator = typename T::const_iterator;

		MapConstIterator it = map.begin();
		MapConstIterator end = map.end();
		for (; it != end; ++it) _data.insert(ValueType(it->first, Var(it->second)));
	}

	Data _data;
};


#if defined(POCO_OS_FAMILY_WINDOWS)

extern template class Struct<std::string>;
extern template class Struct<int>;

extern template class Struct<std::string, Poco::OrderedMap<std::string, Var>, Poco::OrderedSet<std::string>>;
extern template class Struct<int, OrderedMap<int, Var>, OrderedSet<int>>;

#else

extern template class Foundation_API Struct<std::string>;
extern template class Foundation_API Struct<int>;

extern template class Foundation_API Struct<std::string, Poco::OrderedMap<std::string, Var>, Poco::OrderedSet<std::string>>;
extern template class Foundation_API Struct<int, OrderedMap<int, Var>, OrderedSet<int>>;

#endif


template <>
class VarHolderImpl<Struct<std::string, std::map<std::string, Var>, std::set<std::string>>>: public VarHolder
{
public:
	using KeyType = std::string;
	using MapType = std::map<KeyType, Var>;
	using SetType = std::set<KeyType>;
	using ValueType = Struct<KeyType, MapType, SetType>;

	VarHolderImpl(const ValueType& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	const std::type_info& type() const override
	{
		return typeid(ValueType);
	}

	void convert(Int8&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int8");
	}

	void convert(Int16&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int16");
	}

	void convert(Int32&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int32");
	}

	void convert(Int64&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int64");
	}

	void convert(UInt8&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt8");
	}

	void convert(UInt16&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt16");
	}

	void convert(UInt32&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt32");
	}

	void convert(UInt64&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt64");
	}

	void convert(bool&) const override
	{
		throw BadCastException("Cannot cast Struct type to bool");
	}

	void convert(float&) const override
	{
		throw BadCastException("Cannot cast Struct type to float");
	}

	void convert(double&) const override
	{
		throw BadCastException("Cannot cast Struct type to double");
	}

	void convert(char&) const override
	{
		throw BadCastException("Cannot cast Struct type to char");
	}

	void convert(std::string& val) const override
	{
		val = structToString(_val);
	}

	void convert(Poco::DateTime&) const override
	{
		throw BadCastException("Struct -> Poco::DateTime");
	}

	void convert(Poco::LocalDateTime&) const override
	{
		throw BadCastException("Struct -> Poco::LocalDateTime");
	}

	void convert(Poco::Timestamp&) const override
	{
		throw BadCastException("Struct -> Poco::Timestamp");
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const ValueType& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return true;
	}

	bool isOrdered() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	std::size_t size() const override
	{
		return _val.size();
	}

	Var& operator [] (const KeyType& name)
	{
		return _val[name];
	}

	const Var& operator [] (const KeyType& name) const
	{
		return _val[name];
	}

private:
	ValueType _val;
};


template <>
class VarHolderImpl<Struct<int, std::map<int, Var>, std::set<int>>> : public VarHolder
{
public:
	using KeyType = int;
	using MapType = std::map<KeyType, Var>;
	using SetType = std::set<KeyType>;
	using ValueType = Struct<KeyType, MapType, SetType>;

	VarHolderImpl(const ValueType& val) : _val(val)
	{
	}

	~VarHolderImpl() override = default;

	const std::type_info& type() const override
	{
		return typeid(ValueType);
	}

	void convert(Int8&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int8");
	}

	void convert(Int16&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int16");
	}

	void convert(Int32&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int32");
	}

	void convert(Int64&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int64");
	}

	void convert(UInt8&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt8");
	}

	void convert(UInt16&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt16");
	}

	void convert(UInt32&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt32");
	}

	void convert(UInt64&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt64");
	}

	void convert(bool&) const override
	{
		throw BadCastException("Cannot cast Struct type to bool");
	}

	void convert(float&) const override
	{
		throw BadCastException("Cannot cast Struct type to float");
	}

	void convert(double&) const override
	{
		throw BadCastException("Cannot cast Struct type to double");
	}

	void convert(char&) const override
	{
		throw BadCastException("Cannot cast Struct type to char");
	}

	void convert(std::string& val) const override
	{
		val = structToString(_val);
	}

	void convert(Poco::DateTime&) const override
	{
		throw BadCastException("Struct -> Poco::DateTime");
	}

	void convert(Poco::LocalDateTime&) const override
	{
		throw BadCastException("Struct -> Poco::LocalDateTime");
	}

	void convert(Poco::Timestamp&) const override
	{
		throw BadCastException("Struct -> Poco::Timestamp");
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const ValueType& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return true;
	}

	bool isOrdered() const override
	{
		return false;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	std::size_t size() const override
	{
		return _val.size();
	}

	Var& operator [] (const KeyType& name)
	{
		return _val[name];
	}

	const Var& operator [] (const KeyType& name) const
	{
		return _val[name];
	}

private:
	ValueType _val;
};


template <>
class VarHolderImpl<Struct<std::string, Poco::OrderedMap<std::string, Var>, Poco::OrderedSet<std::string>>> : public VarHolder
{
public:
	using KeyType = std::string;
	using MapType = Poco::OrderedMap<KeyType, Var>;
	using SetType = Poco::OrderedSet<KeyType>;
	using ValueType = Struct<KeyType, MapType, SetType>;

	VarHolderImpl(const ValueType& val) : _val(val)
	{
	}

	~VarHolderImpl() override = default;

	const std::type_info& type() const override
	{
		return typeid(ValueType);
	}

	void convert(Int8&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int8");
	}

	void convert(Int16&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int16");
	}

	void convert(Int32&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int32");
	}

	void convert(Int64&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int64");
	}

	void convert(UInt8&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt8");
	}

	void convert(UInt16&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt16");
	}

	void convert(UInt32&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt32");
	}

	void convert(UInt64&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt64");
	}

	void convert(bool&) const override
	{
		throw BadCastException("Cannot cast Struct type to bool");
	}

	void convert(float&) const override
	{
		throw BadCastException("Cannot cast Struct type to float");
	}

	void convert(double&) const override
	{
		throw BadCastException("Cannot cast Struct type to double");
	}

	void convert(char&) const override
	{
		throw BadCastException("Cannot cast Struct type to char");
	}

	void convert(std::string&val) const override
	{
		val = structToString(_val);
	}

	void convert(Poco::DateTime&) const override
	{
		throw BadCastException("Struct -> Poco::DateTime");
	}

	void convert(Poco::LocalDateTime&) const override
	{
		throw BadCastException("Struct -> Poco::LocalDateTime");
	}

	void convert(Poco::Timestamp&) const override
	{
		throw BadCastException("Struct -> Poco::Timestamp");
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const ValueType& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return true;
	}

	bool isOrdered() const override
	{
		return true;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	std::size_t size() const override
	{
		return _val.size();
	}

	Var& operator [] (const KeyType& name)
	{
		return _val[name];
	}

	const Var& operator [] (const KeyType& name) const
	{
		return _val[name];
	}

private:
	ValueType _val;
};


template <>
class VarHolderImpl<Struct<int, Poco::OrderedMap<int, Var>, Poco::OrderedSet<int>>> : public VarHolder
{
public:
	using KeyType = int;
	using MapType = Poco::OrderedMap<KeyType, Var>;
	using SetType = Poco::OrderedSet<KeyType>;
	using ValueType = Struct<KeyType, MapType, SetType>;

	VarHolderImpl(const ValueType& val) : _val(val)
	{
	}

	~VarHolderImpl() override = default;

	const std::type_info&type() const override
	{
		return typeid(ValueType);
	}

	void convert(Int8&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int8");
	}

	void convert(Int16&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int16");
	}

	void convert(Int32&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int32");
	}

	void convert(Int64&) const override
	{
		throw BadCastException("Cannot cast Struct type to Int64");
	}

	void convert(UInt8&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt8");
	}

	void convert(UInt16&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt16");
	}

	void convert(UInt32&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt32");
	}

	void convert(UInt64&) const override
	{
		throw BadCastException("Cannot cast Struct type to UInt64");
	}

	void convert(bool&) const override
	{
		throw BadCastException("Cannot cast Struct type to bool");
	}

	void convert(float&) const override
	{
		throw BadCastException("Cannot cast Struct type to float");
	}

	void convert(double&) const override
	{
		throw BadCastException("Cannot cast Struct type to double");
	}

	void convert(char&) const override
	{
		throw BadCastException("Cannot cast Struct type to char");
	}

	void convert(std::string& val) const override
	{
		val = structToString(_val);
	}

	void convert(Poco::DateTime&) const override
	{
		throw BadCastException("Struct -> Poco::DateTime");
	}

	void convert(Poco::LocalDateTime&) const override
	{
		throw BadCastException("Struct -> Poco::LocalDateTime");
	}

	void convert(Poco::Timestamp&) const override
	{
		throw BadCastException("Struct -> Poco::Timestamp");
	}

	VarHolder *clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const ValueType& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
	{
		return true;
	}

	bool isOrdered() const override
	{
		return true;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override
	{
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isString() const override
	{
		return false;
	}

	std::size_t size() const override
	{
		return _val.size();
	}

	Var& operator [] (const KeyType& name)
	{
		return _val[name];
	}

	const Var& operator [] (const KeyType& name) const
	{
		return _val[name];
	}

private:
	ValueType _val;
};


} // namespace Dynamic


using DynamicStruct = Dynamic::Struct<std::string>;
using OrderedDynamicStruct = Dynamic::Struct<std::string, Poco::OrderedMap<std::string, Dynamic::Var>, Poco::OrderedSet<std::string>>;


} // namespace Poco


#endif // Foundation_Struct_INCLUDED
