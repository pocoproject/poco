//
// ValueTypeId.h
//
// Library: Foundation
// Package: Core
// Module:  ValueTypeId
//
// Definition of the type ids by which the holders of Poco::Any and
// Poco::Dynamic::Var identify their value types.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ValueTypeId_INCLUDED
#define Foundation_ValueTypeId_INCLUDED


#include "Poco/Foundation.h"
#define POCO_TYPELIST_INTERNAL
#include "Poco/TypeList.h"
#undef POCO_TYPELIST_INTERNAL


namespace Poco::Impl {


inline constexpr unsigned char TYPE_ID_OTHER = 0;
	/// The type id of a value whose type the holders do not identify
	/// by a number (see Placeholder::holderTypeId()).


inline constexpr unsigned char FOUNDATION_TYPE_ID_BASE = 0;
	/// Foundation's holders use the type ids 1 to 127.


inline constexpr unsigned char JSON_TYPE_ID_BASE = 127;
	/// The Var holders of the JSON library use the type ids 128 to 159.


inline constexpr unsigned char DATA_TYPE_ID_BASE = 159;
	/// The Var holders of the Data library use the type ids 160 to 191.


inline constexpr unsigned char RESERVED_TYPE_ID_BASE = 191;
	/// The type ids 192 to 255 are reserved.


template <typename V, typename List, unsigned char BaseV>
inline constexpr unsigned char TYPE_ID_IN = (TypeIndex<V, List>::value < 0) ?
	TYPE_ID_OTHER : static_cast<unsigned char>(BaseV + 1 + TypeIndex<V, List>::value);
	/// The type id of the value type V in a family of holders whose value
	/// types are List and whose type ids follow BaseV, or TYPE_ID_OTHER
	/// if List does not contain V.


} // namespace Poco::Impl


#endif // Foundation_ValueTypeId_INCLUDED
