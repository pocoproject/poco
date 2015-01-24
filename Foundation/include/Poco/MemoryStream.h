//
// MemoryStream.h
//
// $Id: //poco/1.4/Foundation/include/Poco/MemoryStream.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  MemoryStream
//
// Definition of MemoryStreamBuf, MemoryInputStream, MemoryOutputStream
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_MemoryStream_INCLUDED
#define Foundation_MemoryStream_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/StreamUtil.h"
#include <streambuf>
#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>


namespace Poco {


template <typename charT, typename traits>
class BasicMemoryStreamBuf: public std::basic_streambuf<charT, traits>
	/// BasicMemoryStreamBuf is a simple implementation of a 
	/// stream buffer for reading and writing from a memory area.
	///
	/// This streambuf only supports unidirectional streams.
	/// In other words, the BasicMemoryStreamBuf can be
	/// used for the implementation of an istream or an
	/// ostream, but not for an iostream.
{
protected:
	typedef charT char_type;
	typedef typename traits::int_type int_type;
	typedef typename traits::pos_type pos_type;
	typedef typename traits::off_type off_type;
	typedef traits traits_type;

public:
	BasicMemoryStreamBuf(char_type* pBuffer, std::streamsize bufferSize):
		_pBuffer(pBuffer),
		_bufferSize(bufferSize)
	{
		this->setg(_pBuffer, _pBuffer, _pBuffer + _bufferSize);
		this->setp(_pBuffer, _pBuffer + _bufferSize);
	}

	~BasicMemoryStreamBuf()
	{
	}

	virtual int_type overflow(int_type /*c*/)
	{
		return traits_type::eof();
	}

	virtual int_type underflow()
	{
		return traits_type::eof();
	}

	virtual pos_type seekoff(off_type off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out)
	{
		const pos_type fail = off_type(-1);
		off_type newoff = off_type(-1);

		if ((which & std::ios_base::in) != 0)
		{
			// Position the input sequence

			if (gptr() == 0)
				return fail;

			switch (way)
			{
			case std::ios_base::beg:
				newoff = 0;
				break;
			case std::ios_base::cur:
				// cur is not valid if both in and out are specified (Condition 3)
				if ((which & std::ios_base::out) != 0)
					return fail;
				newoff = gptr() - eback();
				break;
			case std::ios_base::end:
				newoff = egptr() - eback();
				break;
			}

			if ((newoff + off) < 0 || (egptr() - eback()) < (newoff + off))
				return fail;
			this->setg(eback(), eback() + newoff + off, egptr());
		}

		if ((which & std::ios_base::out) != 0)
		{
			if (pptr() == 0)
				return fail;

			switch (way)
			{
			case std::ios_base::beg:
				newoff = 0;
				break;
			case std::ios_base::cur:
				// cur is not valid if both in and out are specified (Condition 3)
				if ((which & std::ios_base::in) != 0)
					return fail;
				newoff = pptr() - pbase();
				break;
			case std::ios_base::end:
				newoff = epptr() - pbase();
				break;
			}

			if (newoff + off < 0 || (epptr() - pbase()) < newoff + off)
				return fail;
			this->pbump((int)(newoff + off - (pptr() - pbase())));
		}

		return newoff;
	}

	virtual int sync()
	{
		return 0;
	}
	
	std::streamsize charsWritten() const
	{
		return static_cast<std::streamsize>(this->pptr() - this->pbase());
	}

	void reset()
		/// Resets the buffer so that current read and write positions
		/// will be set to the beginning of the buffer.
	{
		this->setg(_pBuffer, _pBuffer, _pBuffer + _bufferSize);
		this->setp(_pBuffer, _pBuffer + _bufferSize);
	}
		
private:
	char_type*      _pBuffer;
	std::streamsize _bufferSize;

	BasicMemoryStreamBuf();
	BasicMemoryStreamBuf(const BasicMemoryStreamBuf&);
	BasicMemoryStreamBuf& operator = (const BasicMemoryStreamBuf&);
};


//
// We provide an instantiation for char
//
typedef BasicMemoryStreamBuf<char, std::char_traits<char> > MemoryStreamBuf;


class Foundation_API MemoryIOS: public virtual std::ios
	/// The base class for MemoryInputStream and MemoryOutputStream.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	MemoryIOS(char* pBuffer, std::streamsize bufferSize);
		/// Creates the basic stream.
		
	~MemoryIOS();
		/// Destroys the stream.

	MemoryStreamBuf* rdbuf();
		/// Returns a pointer to the underlying streambuf.
		
protected:
	MemoryStreamBuf _buf;
};


class Foundation_API MemoryInputStream: public MemoryIOS, public std::istream
	/// An input stream for reading from a memory area.
{
public:
	MemoryInputStream(const char* pBuffer, std::streamsize bufferSize);
		/// Creates a MemoryInputStream for the given memory area,
		/// ready for reading.
	
	~MemoryInputStream();
		/// Destroys the MemoryInputStream.
};


class Foundation_API MemoryOutputStream: public MemoryIOS, public std::ostream
	/// An input stream for reading from a memory area.
{
public:
	MemoryOutputStream(char* pBuffer, std::streamsize bufferSize);
		/// Creates a MemoryOutputStream for the given memory area,
		/// ready for writing.
	
	~MemoryOutputStream();
		/// Destroys the MemoryInputStream.

	std::streamsize charsWritten() const;
		/// Returns the number of chars written to the buffer.
};


//
// inlines
//
inline MemoryStreamBuf* MemoryIOS::rdbuf()
{
	return &_buf;
}


inline std::streamsize MemoryOutputStream::charsWritten() const
{
	return _buf.charsWritten();
}


} // namespace Poco


#endif // Foundation_MemoryStream_INCLUDED
