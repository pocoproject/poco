//
// UnWindows.h
//
// $Id: //poco/1.4/Foundation/include/Poco/UnWindows.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  UnWindows
//
// A wrapper around the <windows.h> header file that #undef's some
// of the macros for function names defined by <windows.h> that
// are a frequent source of conflicts (e.g., GetUserName).
//
// Remember, that most of the WIN32 API functions come in two variants,
// an Unicode variant (e.g., GetUserNameA) and an ASCII variant (GetUserNameW).
// There is also a macro (GetUserName) that's either defined to be the Unicode
// name or the ASCII name, depending on whether the UNICODE macro is #define'd
// or not. POCO always calls the Unicode or ASCII functions directly (depending
// on whether POCO_WIN32_UTF8 is #define'd or not), so the macros are not ignored.
//
// These macro definitions are a frequent case of problems and naming conflicts,
// especially for C++ programmers. Say, you define a class with a member function named
// GetUserName. Depending on whether "Poco/UnWindows.h" has been included by a particular
// translation unit or not, this might be changed to GetUserNameA/GetUserNameW, or not.
// While, due to naming conventions used, this is less of a problem in POCO, some
// of the users of POCO might use a different naming convention where this can become
// a problem.
//
// To disable the #undef's, compile POCO with the POCO_NO_UNWINDOWS macro #define'd.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_UnWindows_INCLUDED
#define Foundation_UnWindows_INCLUDED


// Reduce bloat
#if defined(_WIN32)
	#if !defined(WIN32_LEAN_AND_MEAN) && !defined(POCO_BLOATED_WIN32)
		#define WIN32_LEAN_AND_MEAN
	#endif
#endif


#if defined(_WIN32_WINNT) && (_WIN32_WINNT < 0x0501)
	#error Unsupported Windows version.
#elif !defined(_WIN32_WINNT)
	// define minimum supported
	#define _WIN32_WINNT 0x0501
#endif


#include <windows.h>


#if !defined(POCO_NO_UNWINDOWS)
// A list of annoying macros to #undef.
// Extend as required.
#undef GetBinaryType
#undef GetShortPathName
#undef GetLongPathName
#undef GetEnvironmentStrings
#undef SetEnvironmentStrings
#undef FreeEnvironmentStrings
#undef FormatMessage
#undef EncryptFile
#undef DecryptFile
#undef CreateMutex
#undef OpenMutex
#undef CreateEvent
#undef OpenEvent
#undef CreateSemaphore
#undef OpenSemaphore
#undef LoadLibrary
#undef GetModuleFileName
#undef CreateProcess
#undef GetCommandLine
#undef GetEnvironmentVariable
#undef SetEnvironmentVariable
#undef ExpandEnvironmentStrings
#undef OutputDebugString
#undef FindResource
#undef UpdateResource
#undef FindAtom
#undef AddAtom
#undef GetSystemDirectory
#undef GetTempPath
#undef GetTempFileName
#undef SetCurrentDirectory
#undef GetCurrentDirectory
#undef CreateDirectory
#undef RemoveDirectory
#undef CreateFile
#undef DeleteFile
#undef SearchPath
#undef CopyFile
#undef MoveFile
#undef ReplaceFile
#undef GetComputerName
#undef SetComputerName
#undef GetUserName
#undef LogonUser
#undef GetVersion
#undef GetObject
#endif // POCO_NO_UNWINDOWS

#endif // Foundation_UnWindows_INCLUDED
