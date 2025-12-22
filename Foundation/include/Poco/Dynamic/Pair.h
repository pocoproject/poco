//
// Pair.h
//
// Library: Foundation
// Package: Dynamic
// Module:  Pair
//
// Definition of the Pair class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Pair_INCLUDED
#define Foundation_Pair_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Dynamic/VarHolder.h"
#include <utility>


namespace Poco {
namespace Dynamic {


template <typename K>
class Pair
	/// Pair allows to define a pair of values.
{
public:
	using Data = typename std::pair<K, Var>;

	Pair(): _data()
		/// Creates an empty Pair
	{
	}

	Pair(const Pair& other): _data(other._data)
		/// Creates the Pair from another pair.
	{
	}

	Pair(const Data& val): _data(val)
		/// Creates the Pair from the given value.
	{
	}

	template <typename T>
	Pair(const std::pair<K, T>& val): _data(std::make_pair(val.first, val.second))
		/// Creates Pair form standard pair.
	{
	}

	template <typename T>
	Pair(const K& first, const T& second): _data(std::make_pair(first, second))
		/// Creates pair from two values.
	{
	}

	virtual ~Pair() = default;
		/// Destroys the Pair.

	Pair& swap(Pair& other) noexcept
		/// Swaps the content of the two Pairs.
	{
		std::swap(_data, other._data);
		return *this;
	}

	Pair& operator = (const Pair& other)
		/// Copy constructs Pair from another pair.
	{
		Pair(other).swap(*this);
		return *this;
	}

	inline const K& first() const
		/// Returns the first member of the pair.
	{
		return _data.first;
	}

	inline const Var& second() const
		/// Returns the second member of the pair.
	{
		return _data.second;
	}

	std::string toString()
	{
		std::string str;
		Var(*this).template convert<std::string>(str);
		return str;
	}

private:
	Data _data;
};


template <>
class VarHolderImpl<Pair<std::string>>: public VarHolder
{
public:
	VarHolderImpl(const Pair<std::string>& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	const std::type_info& type() const override
	{
		return typeid(Pair<std::string>);
	}

	void convert(Int8&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int8");
	}

	void convert(Int16&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int16");
	}

	void convert(Int32&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int32");
	}

	void convert(Int64&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int64");
	}

	void convert(UInt8&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt8");
	}

	void convert(UInt16&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt16");
	}

	void convert(UInt32&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt32");
	}

	void convert(UInt64&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt64");
	}

	void convert(bool&) const override
	{
		throw BadCastException("Cannot cast Pair type to bool");
	}

	void convert(float&) const override
	{
		throw BadCastException("Cannot cast Pair type to float");
	}

	void convert(double&) const override
	{
		throw BadCastException("Cannot cast Pair type to double");
	}

	void convert(char&) const override
	{
		throw BadCastException("Cannot cast Pair type to char");
	}

	void convert(std::string& val) const override
	{
		// Serialize in JSON format: equals an object
		// JSON format definition: { string ':' value } string:value pair n-times, sep. by ','
		val.append("{ ");
		const Var key(_val.first());
		Impl::appendJSONKey(val, key);
		val.append(": ");
		Impl::appendJSONValue(val, _val.second());
		val.append(" }");
	}

	void convert(Poco::DateTime&) const override
	{
		throw BadCastException("Pair -> Poco::DateTime");
	}

	void convert(Poco::LocalDateTime&) const override
	{
		throw BadCastException("Pair -> Poco::LocalDateTime");
	}

	void convert(Poco::Timestamp&) const override
	{
		throw BadCastException("Pair -> Poco::Timestamp");
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Pair<std::string>& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override
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

private:
	Pair<std::string> _val;
};


template <>
class VarHolderImpl<Pair<int>>: public VarHolder
{
public:
	VarHolderImpl(const Pair<int>& val): _val(val)
	{
	}

	~VarHolderImpl() override = default;

	const std::type_info& type() const override
	{
		return typeid(Pair<int>);
	}

	void convert(Int8&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int8");
	}

	void convert(Int16&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int16");
	}

	void convert(Int32&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int32");
	}

	void convert(Int64&) const override
	{
		throw BadCastException("Cannot cast Pair type to Int64");
	}

	void convert(UInt8&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt8");
	}

	void convert(UInt16&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt16");
	}

	void convert(UInt32&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt32");
	}

	void convert(UInt64&) const override
	{
		throw BadCastException("Cannot cast Pair type to UInt64");
	}

	void convert(bool&) const override
	{
		throw BadCastException("Cannot cast Pair type to bool");
	}

	void convert(float&) const override
	{
		throw BadCastException("Cannot cast Pair type to float");
	}

	void convert(double&) const override
	{
		throw BadCastException("Cannot cast Pair type to double");
	}

	void convert(char&) const override
	{
		throw BadCastException("Cannot cast Pair type to char");
	}

	void convert(std::string& val) const override
	{
		// Serialize in JSON format: equals an object
		// JSON format definition: { string ':' value } string:value pair n-times, sep. by ','
		val.append("{ ");
		const Var key(_val.first());
		Impl::appendJSONKey(val, key);
		val.append(": ");
		Impl::appendJSONValue(val, _val.second());
		val.append(" }");
	}

	void convert(Poco::DateTime&) const override
	{
		throw BadCastException("Pair -> Poco::DateTime");
	}

	void convert(Poco::LocalDateTime&) const override
	{
		throw BadCastException("Pair -> Poco::LocalDateTime");
	}

	void convert(Poco::Timestamp&) const override
	{
		throw BadCastException("Pair -> Poco::Timestamp");
	}

	VarHolder* clone(Placeholder<VarHolder>* pVarHolder = nullptr) const override
	{
		return cloneHolder(pVarHolder, _val);
	}

	const Pair<int>& value() const
	{
		return _val;
	}

	bool isArray() const override
	{
		return false;
	}

	bool isStruct() const override {
		return false;
	}

	bool isInteger() const override
	{
		return false;
	}

	bool isSigned() const override {
		return false;
	}

	bool isNumeric() const override
	{
		return false;
	}

	bool isString() const override {
		return false;
	}

private:
	Pair<int> _val;
};


} // namespace Dynamic


} // namespace Poco


#endif // Foundation_Pair_INCLUDED
