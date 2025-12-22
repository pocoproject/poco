//
// UTFString.h
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
#include "Poco/Types.h"
#include <string>


namespace Poco {


struct UTF16CharTraits
{
	using u16streampos = std::fpos<std::mbstate_t>;
	using char_type = UInt16;
	using int_type = int;
	using off_type = std::streamoff;
	using pos_type = u16streampos;
	using state_type = std::mbstate_t;

	static void assign(char_type& c1, const char_type& c2)
	{
		c1 = c2;
	}

	static bool eq(char_type c1, char_type c2)
	{
		return c1 == c2;
	}

	static bool lt(char_type c1, char_type c2)
	{
		return c1 < c2;
	}

	static int compare(const char_type* s1, const char_type* s2, std::size_t n)
	{
		for (; n; --n, ++s1, ++s2)
		{
			if (lt(*s1, *s2))
				return -1;
			if (lt(*s2, *s1))
				return 1;
		}
		return 0;
	}

	static std::size_t length(const char_type* s)
	{
		std::size_t len = 0;
		for (; !eq(*s, char_type(0)); ++s)
			++len;
		return len;
	}

	static const char_type* find(const char_type* s, std::size_t n, const char_type& a)
	{
		for (; n; --n)
		{
			if (eq(*s, a))
				return s;
			++s;
		}
		return nullptr;
	}

	static char_type* move(char_type* s1, const char_type* s2, std::size_t n)
	{
		char_type* r = s1;
		if (s1 < s2)
		{
			for (; n; --n, ++s1, ++s2)
				assign(*s1, *s2);
		}
		else if (s2 < s1)
		{
			s1 += n;
			s2 += n;
			for (; n; --n)
				assign(*--s1, *--s2);
		}
		return r;
	}

	static char_type* copy(char_type* s1, const char_type* s2, std::size_t n)
	{
		poco_assert(s2 < s1 || s2 >= s1 + n);
		char_type* r = s1;
		for (; n; --n, ++s1, ++s2)
			assign(*s1, *s2);
		return r;
	}

	static char_type* assign(char_type* s, std::size_t n, char_type a)
	{
		char_type* r = s;
		for (; n; --n, ++s)
			assign(*s, a);
		return r;
	}

	static int_type  not_eof(int_type c)
	{
		return eq_int_type(c, eof()) ? ~eof() : c;
	}

	static char_type to_char_type(int_type c)
	{
		return char_type(c);
	}

	static int_type to_int_type(char_type c)
	{
		return int_type(c);
	}

	static bool eq_int_type(int_type c1, int_type c2)
	{
		return c1 == c2;
	}

	static int_type eof()
	{
		return int_type(0xDFFF);
	}
};


struct UTF32CharTraits
{
	using u32streampos = std::fpos<std::mbstate_t>;
	using char_type = UInt32;
	using int_type = int;
	using off_type = std::streamoff;
	using pos_type = u32streampos;
	using state_type = std::mbstate_t;

	static void assign(char_type& c1, const char_type& c2)
	{
		c1 = c2;
	}

	static bool eq(char_type c1, char_type c2)
	{
		return c1 == c2;
	}

	static bool lt(char_type c1, char_type c2)
	{
		return c1 < c2;
	}

	static int compare(const char_type* s1, const char_type* s2, std::size_t n)
	{
		for (; n; --n, ++s1, ++s2)
		{
			if (lt(*s1, *s2))
				return -1;
			if (lt(*s2, *s1))
				return 1;
		}
		return 0;
	}

	static std::size_t length(const char_type* s)
	{
		std::size_t len = 0;
		for (; !eq(*s, char_type(0)); ++s)
			++len;
		return len;
	}

	static const char_type* find(const char_type* s, std::size_t n, const char_type& a)
	{
		for (; n; --n)
		{
			if (eq(*s, a))
				return s;
			++s;
		}
		return nullptr;
	}

	static char_type* move(char_type* s1, const char_type* s2, std::size_t n)
	{
		char_type* r = s1;
		if (s1 < s2)
		{
			for (; n; --n, ++s1, ++s2)
				assign(*s1, *s2);
		}
		else if (s2 < s1)
		{
			s1 += n;
			s2 += n;
			for (; n; --n)
				assign(*--s1, *--s2);
		}
		return r;
	}

	static char_type* copy(char_type* s1, const char_type* s2, std::size_t n)
	{
		poco_assert(s2 < s1 || s2 >= s1 + n);
		char_type* r = s1;
		for (; n; --n, ++s1, ++s2)
			assign(*s1, *s2);
		return r;
	}

	static char_type* assign(char_type* s, std::size_t n, char_type a)
	{
		char_type* r = s;
		for (; n; --n, ++s)
			assign(*s, a);
		return r;
	}

	static int_type  not_eof(int_type c)
	{
		return eq_int_type(c, eof()) ? ~eof() : c;
	}

	static char_type to_char_type(int_type c)
	{
		return char_type(c);
	}

	static int_type to_int_type(char_type c)
	{
		return int_type(c);
	}

	static bool eq_int_type(int_type c1, int_type c2)
	{
		return c1 == c2;
	}

	static int_type eof()
	{
		return int_type(0xDFFF);
	}
};


	#ifdef POCO_NO_WSTRING
		using UTF16Char = Poco::UInt16;
		using UTF16String = std::basic_string<UTF16Char, UTF16CharTraits>;
		using UTF32Char = UInt32;
		using UTF32String = std::basic_string<UTF32Char, UTF32CharTraits>;
		#define POCO_USE_STRING16
		#define POCO_USE_STRING32
	#else // POCO_NO_WSTRING
		#if defined(POCO_OS_FAMILY_WINDOWS)
			using UTF16Char = wchar_t;
			using UTF16String = std::wstring;
			using UTF32Char = UInt32;
			using UTF32String = std::basic_string<UTF32Char, UTF32CharTraits>;
		#elif defined(__SIZEOF_WCHAR_T__) //gcc
			#if (__SIZEOF_WCHAR_T__ == 2)
				using UTF16Char = wchar_t;
				using UTF16String = std::wstring;
				using UTF32Char = UInt32;
				using UTF32String = std::basic_string<UTF32Char, UTF32CharTraits>;
				#define POCO_USE_STRING32
			#elif (__SIZEOF_WCHAR_T__ == 4)
				using UTF16Char = Poco::UInt16;
				using UTF16String = std::basic_string<UTF16Char, UTF16CharTraits>;
				using UTF32Char = wchar_t;
				using UTF32String = std::wstring;
				#define POCO_USE_STRING16
			#endif
		#else // default to 32-bit wchar_t
			using UTF16Char = Poco::UInt16;
			using UTF16String = std::basic_string<UTF16Char, UTF16CharTraits>;
			using UTF32Char = wchar_t;
			using UTF32String = std::wstring;
			#define POCO_USE_STRING16
		#endif //POCO_OS_FAMILY_WINDOWS
	#endif //POCO_NO_WSTRING


} // namespace Poco


#if defined(POCO_USE_STRING16)
extern template class Foundation_API std::basic_string<Poco::UTF16Char, Poco::UTF16CharTraits>;
#endif

#if defined(POCO_USE_STRING32)
extern template class Foundation_API std::basic_string<Poco::UTF32Char, Poco::UTF32CharTraits>;
#endif


#endif // Foundation_UTFString_INCLUDED
