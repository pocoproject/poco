//
// StreamUtil.h
//
// Library: Foundation
// Package: Streams
// Module:  StreamUtil
//
// Stream implementation support.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_StreamUtil_INCLUDED
#define Foundation_StreamUtil_INCLUDED


#include "Poco/Foundation.h"
#include <ios>
#include <iostream>


// poco_ios_init
//
// This is a workaround for a bug in the Dinkumware
// implementation of iostreams.
//
// Calling basic_ios::init() multiple times for the
// same basic_ios instance results in a memory leak
// caused by the ios' locale being allocated more than
// once, each time overwriting the old pointer.
// This usually occurs in the following scenario:
//
// class MyStreamBuf: public std::streambuf
// {
//     ...
// };
//
// class MyIOS: public virtual std::ios
// {
// public:
//     MyIOS()
//     {
//         init(&_buf);
//     }
// protected:
//     MyStreamBuf _buf;
// };
//
// class MyIStream: public MyIOS, public std::istream
// {
//     ...
// };
//
// In this scenario, std::ios::init() is called twice
// (the first time by the MyIOS constructor, the second
// time by the std::istream constructor), resulting in
// two locale objects being allocated, the pointer second
// one overwriting the pointer to the first one and thus
// causing a memory leak.
//
// The workaround is to call init() only once for each
// stream object - by the istream, ostream or iostream
// constructor, and not calling init() in ios-derived
// base classes.
//
// Some stream implementations, however, require that
// init() is called in the MyIOS constructor.
// Therefore we replace each call to init()with
// the poco_ios_init macro defined below.


#if !defined(POCO_IOS_INIT_HACK)
	// Microsoft Visual Studio with Dinkumware STL (but not STLport)
#	if defined(_MSC_VER)&& (!defined(_STLP_MSVC) || defined(_STLP_NO_OWN_IOSTREAMS))
#		define POCO_IOS_INIT_HACK 1
    // QNX with Dinkumware but not GNU C++ Library
#	elif defined(__QNX__) && !defined(__GLIBCPP__)
#		define POCO_IOS_INIT_HACK 1
#	endif
#endif


#if defined(POCO_IOS_INIT_HACK)
#	define poco_ios_init(buf)
#else
#	define poco_ios_init(buf) init(buf)
#endif


namespace Poco {


class StreamSaver
	/// Preserves and restores stream state using RAII.
	///
	/// Usage example:
	///
	/// void func(std::ostream &os)
	/// {
	///     StreamSaver(os); // preserve the original stream state
	///     int i = 42;
	///     os << std::hex << i;
	///     // ~StreamSaver() restores the original stream state
	/// }
	///
{
public:
	explicit  StreamSaver(std::ios& s):
		_stream(s), _flags(s.flags()), _precision(s.precision()),
		_width(s.width()), _rdstate(s.rdstate()),
		_exceptions(s.exceptions()), _tie(s.tie()),
		_rdbuf(s.rdbuf()), _fill(s.fill())
#ifndef POCO_NO_LOCALE
		, _locale(s.getloc())
#endif
		{}

	~StreamSaver()
	{
#ifndef POCO_NO_LOCALE
		_stream.imbue(_locale);
#endif
		_stream.fill(_fill);
		_stream.rdbuf(_rdbuf);
		_stream.tie(_tie);
		_stream.exceptions(_exceptions);
		_stream.clear(_rdstate);
		_stream.width(_width);
		_stream.precision(_precision);
		_stream.flags(_flags);
	}

private:
	StreamSaver();
	StreamSaver(const StreamSaver&);
	StreamSaver(StreamSaver&&);
	StreamSaver& operator=(const StreamSaver&);
	StreamSaver& operator=(StreamSaver&&);

	std::ios&           _stream;
	std::ios::fmtflags  _flags;
	std::streamsize     _precision;
	std::streamsize     _width;
	std::ios::iostate   _rdstate;
	std::ios::iostate   _exceptions;
	std::ostream*       _tie;
	std::streambuf*     _rdbuf;
	std::ios::char_type _fill;
#ifndef POCO_NO_LOCALE
	std::locale         _locale;
#endif
};


}
#endif // Foundation_StreamUtil_INCLUDED
