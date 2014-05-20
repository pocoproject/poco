//
// UTFString.h
//
// $Id: //poco/1.4/Foundation/include/Poco/UTFString.h#2 $
//
// Library: Foundation
// Package: Text
// Module:  UTFString
//
// Definitions of strings for UTF encodings.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_UTFString_INCLUDED
#define Foundation_UTFString_INCLUDED


#include "Poco/Foundation.h"
#include <string>


namespace Poco {


// UTF string types

//#if defined(POCO_ENABLE_CPP11) //TODO
	//	typedef char16_t       UTF16Char;
	//	typedef std::u16string UTF16String;
	//	typedef char32_t       UTF32Char;
	//	typedef std::u32string UTF32String;
//#else
	#ifdef POCO_NO_WSTRING
		typedef Poco::UInt16              UTF16Char;
		typedef std::basic_string<UInt16> UTF16String;
		typedef UInt32                    UTF32Char;
		typedef std::basic_string<UInt32> UTF32String;
	#else // POCO_NO_WSTRING
		#if defined(POCO_OS_FAMILY_WINDOWS)
			typedef wchar_t                   UTF16Char;
			typedef std::wstring              UTF16String;
			typedef UInt32                    UTF32Char;
			typedef std::basic_string<UInt32> UTF32String;
		#elif defined(__SIZEOF_WCHAR_T__) //gcc
			#if (__SIZEOF_WCHAR_T__ == 2)
				typedef wchar_t                   UTF16Char;
				typedef std::wstring              UTF16String;
				typedef UInt32                    UTF32Char;
				typedef std::basic_string<UInt32> UTF32String;
			#elif (__SIZEOF_WCHAR_T__ == 4)
				typedef Poco::UInt16              UTF16Char;
				typedef std::basic_string<UInt16> UTF16String;
				typedef wchar_t                   UTF32Char;
				typedef std::wstring              UTF32String;
			#endif
		#else // default to 32-bit wchar_t
			typedef Poco::UInt16              UTF16Char;
			typedef std::basic_string<UInt16> UTF16String;
			typedef wchar_t                   UTF32Char;
			typedef std::wstring              UTF32String;
		#endif //POCO_OS_FAMILY_WINDOWS
	#endif //POCO_NO_WSTRING
//#endif // POCO_ENABLE_CPP11


} // namespace Poco


#endif // Foundation_UTFString_INCLUDED
