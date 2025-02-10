//
// UnbufferedStreamBuf.h
//
// Library: Foundation
// Package: Streams
// Module:  StreamBuf
//
// Definition of template BasicUnbufferedStreamBuf and class UnbufferedStreamBuf.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_UnbufferedStreamBuf_INCLUDED
#define Foundation_UnbufferedStreamBuf_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/StreamUtil.h"
#include <streambuf>
#include <iosfwd>
#include <ios>


namespace Poco {


template <typename ch, typename tr>
class BasicUnbufferedStreamBuf: public std::basic_streambuf<ch, tr>
	/// This is an implementation of an unbuffered streambuf
	/// that greatly simplifies the implementation of
	/// custom streambufs of various kinds.
	/// Derived classes only have to override the methods
	/// readFromDevice() or writeToDevice().
{
protected:
	using Base = std::basic_streambuf<ch, tr>;
	using IOS = std::basic_ios<ch, tr>;
	using char_type = ch;
	using char_traits = tr;
	using int_type = typename Base::int_type;
	using pos_type = typename Base::pos_type;
	using off_type = typename Base::off_type;
	using openmode = typename IOS::openmode;

public:
	BasicUnbufferedStreamBuf():
		_pb(char_traits::eof()),
		_ispb(false)
	{
		this->setg(0, 0, 0);
		this->setp(0, 0);
	}

	~BasicUnbufferedStreamBuf() override = default;

	BasicUnbufferedStreamBuf(const BasicUnbufferedStreamBuf&) = delete;
	BasicUnbufferedStreamBuf& operator=(const BasicUnbufferedStreamBuf&) = delete;

	int_type overflow(int_type c) override
	{
		if (c != char_traits::eof())
			return writeToDevice(char_traits::to_char_type(c));
		else
			return c;
	}

	int_type underflow() override
	{
		if (_ispb)
		{
			return _pb;
		}
		else
		{
			int_type c = readFromDevice();
			if (c != char_traits::eof())
			{
				_ispb = true;
				_pb   = c;
			}
			return c;
		}
	}

	int_type uflow() override
	{
		if (_ispb)
		{
			_ispb = false;
			return _pb;
		}
		else
		{
			int_type c = readFromDevice();
			if (c != char_traits::eof())
			{
				_pb = c;
			}
			return c;
		}
	}

	int_type pbackfail(int_type c) override
	{
		if (_ispb)
		{
			return char_traits::eof();
		}
		else
		{
			_ispb = true;
			_pb   = c;
			return c;
		}
	}

	std::streamsize xsgetn(char_type* p, std::streamsize count) override
		/// Some platforms (for example, Compaq C++) have buggy implementations of
		/// xsgetn that handle null buffers incorrectly.
		/// Anyway, it does not hurt to provide an optimized implementation
		/// of xsgetn for this streambuf implementation.
	{
		std::streamsize copied = 0;
		while (count > 0)
		{
			int_type c = uflow();
			if (c == char_traits::eof()) break;
			*p++ = char_traits::to_char_type(c);
			++copied;
			--count;
		}
		return copied;
	}

protected:
	static int_type charToInt(char_type c)
	{
		return char_traits::to_int_type(c);
	}

private:
	virtual int_type readFromDevice()
	{
		return char_traits::eof();
	}

	virtual int_type writeToDevice(char_type)
	{
		return char_traits::eof();
	}

	int_type _pb;
	bool     _ispb;
};


//
// We provide an instantiation for char.
//

#if defined(POCO_OS_FAMILY_WINDOWS)
extern template class BasicUnbufferedStreamBuf<char, std::char_traits<char>>;
#else
extern template class Foundation_API BasicUnbufferedStreamBuf<char, std::char_traits<char>>;
#endif

using UnbufferedStreamBuf = BasicUnbufferedStreamBuf<char, std::char_traits<char>>;

} // namespace Poco


#endif // Foundation_UnbufferedStreamBuf_INCLUDED
