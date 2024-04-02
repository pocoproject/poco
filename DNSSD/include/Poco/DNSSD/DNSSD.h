//
// DNSSD.h
//
// $Id: //poco/1.7/DNSSD/include/Poco/DNSSD/DNSSD.h#1 $
//
// Library: DNSSD
// Package: Core
// Module:  DNSSD
//
// Basic definitions for the Poco DNSSD library.
// This file must be the first file included by every other DNSSD
// header file.
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef DNSSD_DNSSD_INCLUDED
#define DNSSD_DNSSD_INCLUDED


#include "Poco/Poco.h"


//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DNSSD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DNSSD_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(DNSSD_EXPORTS)
		#define DNSSD_API __declspec(dllexport)
	#else
		#define DNSSD_API __declspec(dllimport)
	#endif
#endif


#if !defined(DNSSD_API)
	#define DNSSD_API
#endif


#if defined(_MSC_VER) && !defined(POCO_LIB_SUFFIX)
	#if defined(POCO_DLL)
		#if defined(_DEBUG)
			#define POCO_LIB_SUFFIX "d.lib"
		#else
			#define POCO_LIB_SUFFIX ".lib"
		#endif
	#elif defined(_DLL)
		#if defined(_DEBUG)
			#define POCO_LIB_SUFFIX "mdd.lib"
		#else
			#define POCO_LIB_SUFFIX "md.lib"
		#endif
	#else
		#if defined(_DEBUG)
			#define POCO_LIB_SUFFIX "mtd.lib"
		#else
			#define POCO_LIB_SUFFIX "mt.lib"
		#endif
	#endif
#endif


#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(DNSSD_EXPORTS)
		#pragma comment(lib, "PocoDNSSD" POCO_LIB_SUFFIX)
	#endif
#endif


namespace Poco {
namespace DNSSD {


enum HandleType
{
	SD_SERVICE_HANDLE = 1,
	SD_RECORD_HANDLE  = 2,
	SD_BROWSE_HANDLE  = 3
};


template <typename Base, int Type, Base Invalid>
class OpaqueHandle
{
public:
	enum
	{
		TYPE = Type
	};

	OpaqueHandle():
		_h(Invalid),
		_subtype(0)
	{
	}

	OpaqueHandle(Base h, int subtype = 0):
		_h(h),
		_subtype(subtype)
	{
	}

	template <typename T>
	OpaqueHandle(T h, int subtype = 0):
		_h(reinterpret_cast<Base>(h)),
		_subtype(subtype)
	{
	}

	~OpaqueHandle()
	{
	}

	int subtype() const
	{
		return _subtype;
	}

	template <typename T>
	T cast() const
	{
		return reinterpret_cast<T>(_h);
	}

	bool operator == (const OpaqueHandle& other) const
	{
		return _h == other._h;
	}

	bool operator != (const OpaqueHandle& other) const
	{
		return _h != other._h;
	}

	bool operator <= (const OpaqueHandle& other) const
	{
		return _h <= other._h;
	}

	bool operator < (const OpaqueHandle& other) const
	{
		return _h < other._h;
	}

	bool operator >= (const OpaqueHandle& other) const
	{
		return _h >= other._h;
	}

	bool operator > (const OpaqueHandle& other) const
	{
		return _h > other._h;
	}

	void reset()
	{
		_h = Invalid;
	}

	bool isValid() const
	{
		return _h != Invalid;
	}

	bool isNull() const
	{
		return _h == Invalid;
	}

private:
	Base _h;
	int  _subtype;
};


typedef OpaqueHandle<Poco::IntPtr, SD_SERVICE_HANDLE, 0> ServiceHandle;
typedef OpaqueHandle<Poco::IntPtr, SD_RECORD_HANDLE, 0> RecordHandle;
typedef OpaqueHandle<Poco::IntPtr, SD_BROWSE_HANDLE, 0> BrowseHandle;


} } // namespace Poco::DNSSD


#endif // DNSSD_DNSSD_INCLUDED
