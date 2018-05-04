//
// AutoPtr.h
//
// Library: Foundation
// Package: Core
// Module:  AutoPtr
//
// Definition of the AutoPtr template class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_AutoPtr_INCLUDED
#define Foundation_AutoPtr_INCLUDED


#include "Poco/RefPtr.h"


namespace Poco {


template<class T>
using AutoPtr = RefPtr<T>;


} // namespace Poco


#endif // Foundation_AutoPtr_INCLUDED
