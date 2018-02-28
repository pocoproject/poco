//
// Types.h
//
// Library: Foundation
// Package: Core
// Module:  Types
//
// Definitions of fixed-size integer types for various platforms
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_Types_INCLUDED
#define Foundation_Types_INCLUDED


#include "Poco/Foundation.h"
#include <cstdint>


#define POCO_HAVE_INT64 1


namespace Poco {


typedef std::int8_t   Int8;
typedef std::uint8_t  UInt8;
typedef std::int16_t  Int16;
typedef std::uint16_t UInt16;
typedef std::int32_t  Int32;
typedef std::uint32_t UInt32;
typedef std::int64_t  Int64;
typedef std::uint64_t UInt64;


#if defined(_MSC_VER)
	//
	// Windows/Visual C++
	//

	#if defined(_WIN64)
		#define POCO_PTR_IS_64_BIT 1
		typedef std::int64_t  IntPtr;
		typedef std::uint64_t UIntPtr;
	#else
		typedef std::int32_t  IntPtr;
		typedef std::uint32_t UIntPtr;
	#endif

#elif defined(__GNUC__) || defined(__clang__)
	//
	// Unix/GCC/Clang
	//

	#if defined(_WIN64)
		#define POCO_PTR_IS_64_BIT 1
		typedef std::int64_t  IntPtr;
		typedef std::uint64_t UIntPtr;
	#else
		#if defined(__LP64__)
			typedef std::int64_t  IntPtr;
			typedef std::uint64_t UIntPtr;
			#define POCO_PTR_IS_64_BIT 1
			#define POCO_LONG_IS_64_BIT 1
		#else
			typedef std::int32_t  IntPtr;
			typedef std::uint32_t UIntPtr;
		#endif
	#endif

#elif defined(__DECCXX)
	//
	// Compaq C++
	//

	typedef Int64 IntPtr;
	typedef UInt64 UIntPtr;
	#define POCO_PTR_IS_64_BIT 1
	#define POCO_LONG_IS_64_BIT 1

#elif defined(__HP_aCC)
	//
	// HP Ansi C++
	//

	#if defined(__LP64__)
		#define POCO_PTR_IS_64_BIT 1
		#define POCO_LONG_IS_64_BIT 1
		typedef Int64 IntPtr;
		typedef UInt64 UIntPtr;
	#else
		typedef Int32 IntPtr;
		typedef UInt32 UIntPtr;
	#endif

#elif defined(__SUNPRO_CC)
	//
	// SUN Forte C++
	//

	#if defined(__sparcv9)
		#define POCO_PTR_IS_64_BIT 1
		#define POCO_LONG_IS_64_BIT 1
		typedef Int64 IntPtr;
		typedef UInt64 UIntPtr;
	#else
		typedef Int32 IntPtr;
		typedef UInt32 UIntPtr;
	#endif

#elif defined(__IBMCPP__)
	//
	// IBM XL C++
	//

	#if defined(__64BIT__)
		#define POCO_PTR_IS_64_BIT 1
		#define POCO_LONG_IS_64_BIT 1
		typedef Int64 IntPtr;
		typedef UInt64 UIntPtr;
	#else
		typedef Int32 IntPtr;
		typedef UInt32 UIntPtr;
	#endif

#elif defined(__sgi)
	//
	// MIPSpro C++
	//

	#if _MIPS_SZLONG == 64
		#define POCO_PTR_IS_64_BIT 1
		#define POCO_LONG_IS_64_BIT 1
		typedef Int64 IntPtr;
		typedef UInt64 UIntPtr;
	#else
		typedef Int32 IntPtr;
		typedef UInt32 UIntPtr;
	#endif

#endif


#if defined(POCO_PTR_IS_64_BIT) && (POCO_PTR_IS_64_BIT == 1)
	#define POCO_64_BIT
#endif


} // namespace Poco


#endif // Foundation_Types_INCLUDED
