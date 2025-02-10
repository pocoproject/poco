//
// BufferedBidirectionalStreamBuf.h
//
// Library: Foundation
// Package: Streams
// Module:  StreamBuf
//
// Definition of template BasicBufferedBidirectionalStreamBuf and class BufferedBidirectionalStreamBuf.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_BufferedBidirectionalStreamBuf_INCLUDED
#define Foundation_BufferedBidirectionalStreamBuf_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/BufferAllocator.h"
#include "Poco/StreamUtil.h"
#include <streambuf>
#include <iosfwd>
#include <ios>


namespace Poco {


template <typename ch, typename tr, typename ba = BufferAllocator<ch>>
class BasicBufferedBidirectionalStreamBuf: public std::basic_streambuf<ch, tr>
	/// This is an implementation of a buffered bidirectional
	/// streambuf that greatly simplifies the implementation of
	/// custom streambufs of various kinds.
	/// Derived classes only have to override the methods
	/// readFromDevice() or writeToDevice().
	///
	/// In contrast to BasicBufferedStreambuf, this class supports
	/// simultaneous read and write access, so in addition to
	/// istream and ostream this streambuf can also be used
	/// for implementing an iostream.
{
protected:
	using Base = std::basic_streambuf<ch, tr>;
	using IOS = std::basic_ios<ch, tr>;
	using char_type = ch;
	using char_traits = tr;
	using Allocator = ba;
	using int_type = typename Base::int_type;
	using pos_type = typename Base::pos_type;
	using off_type = typename Base::off_type;
	using openmode = typename IOS::openmode;

public:
	BasicBufferedBidirectionalStreamBuf(std::streamsize bufferSize, openmode mode):
		_bufsize(bufferSize),
		_pReadBuffer(Allocator::allocate(_bufsize)),
		_pWriteBuffer(Allocator::allocate(_bufsize)),
		_mode(mode)
	{
		resetBuffers();
	}

	~BasicBufferedBidirectionalStreamBuf() override
	{
		Allocator::deallocate(_pReadBuffer, _bufsize);
		Allocator::deallocate(_pWriteBuffer, _bufsize);
	}

	BasicBufferedBidirectionalStreamBuf(const BasicBufferedBidirectionalStreamBuf&) = delete;
	BasicBufferedBidirectionalStreamBuf& operator = (const BasicBufferedBidirectionalStreamBuf&) = delete;

	int_type overflow(int_type c) override
	{
		if (!(_mode & IOS::out)) return char_traits::eof();

		if (flushBuffer() == std::streamsize(-1)) return char_traits::eof();
		if (c != char_traits::eof())
		{
			*this->pptr() = char_traits::to_char_type(c);
			this->pbump(1);
		}

		return c;
	}

	int_type underflow() override
	{
		if (!(_mode & IOS::in)) return char_traits::eof();

		if (this->gptr() && (this->gptr() < this->egptr()))
			return char_traits::to_int_type(*this->gptr());

		int putback = int(this->gptr() - this->eback());
		if (putback > 4) putback = 4;

		char_traits::move(_pReadBuffer + (4 - putback), this->gptr() - putback, putback);

		int n = readFromDevice(_pReadBuffer + 4, _bufsize - 4);
		if (n <= 0) return char_traits::eof();

		this->setg(_pReadBuffer + (4 - putback), _pReadBuffer + 4, _pReadBuffer + 4 + n);

		// return next character
		return char_traits::to_int_type(*this->gptr());
	}

	int sync() override
	{
		if (this->pptr() && this->pptr() > this->pbase())
		{
			if (flushBuffer() == -1) return -1;
		}
		return 0;
	}

protected:
	void setMode(openmode mode)
	{
		_mode = mode;
	}

	openmode getMode() const
	{
		return _mode;
	}

	void resetBuffers()
	{
		this->setg(_pReadBuffer + 4, _pReadBuffer + 4, _pReadBuffer + 4);
		this->setp(_pWriteBuffer, _pWriteBuffer + _bufsize);
	}

	virtual bool resizeBuffer(std::streamsize bufferSize)
	{
		if (_bufsize != bufferSize)
		{
			Allocator::deallocate(_pReadBuffer, _bufsize);
			Allocator::deallocate(_pWriteBuffer, _bufsize);

			_bufsize = bufferSize;
			_pReadBuffer = Allocator::allocate(_bufsize);
			_pWriteBuffer = Allocator::allocate(_bufsize);
		}
		resetBuffers();

		return true;
	}

private:
	virtual int readFromDevice(char_type* /*buffer*/, std::streamsize /*length*/)
	{
		return 0;
	}

	virtual int writeToDevice(const char_type* /*buffer*/, std::streamsize /*length*/)
	{
		return 0;
	}

	int flushBuffer()
	{
		int n = int(this->pptr() - this->pbase());
		if (writeToDevice(this->pbase(), n) == n)
		{
			this->pbump(-n);
			return n;
		}
		return -1;
	}

	std::streamsize _bufsize {0};
	char_type*      _pReadBuffer {nullptr};
	char_type*      _pWriteBuffer {nullptr};
	openmode        _mode {0};
};


//
// We provide an instantiation for char.
//

#if defined(POCO_OS_FAMILY_WINDOWS)
extern template class BasicBufferedBidirectionalStreamBuf<char, std::char_traits<char>>;
#else
extern template class Foundation_API BasicBufferedBidirectionalStreamBuf<char, std::char_traits<char>>;
#endif

using BufferedBidirectionalStreamBuf = BasicBufferedBidirectionalStreamBuf<char, std::char_traits<char>>;

} // namespace Poco


#endif // Foundation_BufferedBidirectionalStreamBuf_INCLUDED
