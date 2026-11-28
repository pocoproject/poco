//
// VarTypeIds.h
//
// Library: Data
// Package: DataCore
// Module:  VarTypeIds
//
// Definition of the value types of the Data library's Var holders,
// which Var identifies by a type id.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Data_VarTypeIds_INCLUDED
#define Data_VarTypeIds_INCLUDED


#include "Poco/Data/Data.h"
#include "Poco/ValueTypeId.h"


namespace Poco::Data {


class Date;
class Time;

template <typename T>
class LOB;


namespace Impl {


using VarValueTypes = TypeList<Date, Time, LOB<unsigned char>, LOB<char>>;
	/// The value types of the Var holders of the Data library, which Var
	/// identifies by a type id (see Poco::Impl::DATA_TYPE_ID_BASE).


static_assert(Poco::Impl::DATA_TYPE_ID_BASE + VarValueTypes::length <= Poco::Impl::RESERVED_TYPE_ID_BASE,
	"the Var holder types of the Data library must fit into its type id range");


} // namespace Impl


} // namespace Poco::Data


#endif // Data_VarTypeIds_INCLUDED
