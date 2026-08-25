//
// popcnt64_win32.c
//
// 32-bit MSVC has no __popcnt64 intrinsic, but the unmodified bundled
// sqlite-vec.c maps __builtin_popcountl to it under _MSC_VER, leaving an
// unresolved external on Win32. Provide the symbol out of line. Bit-parallel
// count: the POPCNT instruction cannot be assumed on 32-bit targets.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#if defined(_MSC_VER) && !defined(_M_X64)

unsigned __int64 __popcnt64(unsigned __int64 v)
{
	v = v - ((v >> 1) & 0x5555555555555555ull);
	v = (v & 0x3333333333333333ull) + ((v >> 2) & 0x3333333333333333ull);
	v = (v + (v >> 4)) & 0x0F0F0F0F0F0F0F0Full;
	return (v * 0x0101010101010101ull) >> 56;
}

#else

// ISO C requires a translation unit to contain at least one declaration
// (MSVC warns with C4206, clang/gcc with -Wempty-translation-unit). This
// branch is not reached by the gated CMake build; the typedef keeps the file
// well-formed if it is ever compiled outside that gate.
typedef int poco_popcnt64_win32_unused;

#endif
