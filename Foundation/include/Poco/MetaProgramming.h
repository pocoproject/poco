//
// MetaProgramming.h
//
// Library: Foundation
// Package: Core
// Module:  MetaProgramming
//
// Common definitions useful for Meta Template Programming
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_MetaProgramming_INCLUDED
#define Foundation_MetaProgramming_INCLUDED


#include "Poco/Foundation.h"
#include <type_traits>


namespace Poco {


template <typename T>
struct IsReference
	/// Use this struct to determine if a template type is a reference.
{
	static constexpr int VALUE = std::is_reference_v<T> ? 1 : 0;
};


template <typename T>
struct IsConst
	/// Use this struct to determine if a template type is a const type.
{
	static constexpr int VALUE = std::is_const_v<std::remove_reference_t<T>> ? 1 : 0;
};


template <typename T>
struct TypeWrapper
	/// Use the type wrapper if you want to decouple constness and references from template types.
{
private:
	using BaseType = std::remove_cv_t<std::remove_reference_t<T>>;
public:
	using TYPE = BaseType;
	using CONSTTYPE = const BaseType;
	using REFTYPE = BaseType&;
	using CONSTREFTYPE = const BaseType&;
};


} // namespace Poco


#endif // Foundation_MetaProgramming_INCLUDED
