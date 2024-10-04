//
// DeflatingStream.cpp
//
// Library: Foundation
// Package: Streams
// Module:  ZLibStream
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DeflatingStream.h"
#include "Poco/Exception.h"
#include <memory>
#if defined(POCO_UNBUNDLED)
#include <zlib.h>
#else
#include "zlib.h"
#endif


namespace Poco {


DeflatingStreamBuf::DeflatingStreamBuf(std::istream& istr, StreamType type, int level):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in),
	_pIstr(&istr),
	_pOstr(0),
	_eof(false)
{
	std::unique_ptr<char[]> buffer(new char[DEFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = deflateInit2(pZstr.get(), level, Z_DEFLATED, 15 + (type == STREAM_GZIP ? 16 : 0), 8, Z_DEFAULT_STRATEGY);
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


DeflatingStreamBuf::DeflatingStreamBuf(std::istream& istr, int windowBits, int level):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in),
	_pIstr(&istr),
	_pOstr(0),
	_eof(false)
{
	std::unique_ptr<char[]> buffer(new char[DEFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = deflateInit2(pZstr.get(), level, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY);
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


DeflatingStreamBuf::DeflatingStreamBuf(std::ostream& ostr, StreamType type, int level):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::out),
	_pIstr(0),
	_pOstr(&ostr),
	_eof(false)
{
	std::unique_ptr<char[]> buffer(new char[DEFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = deflateInit2(pZstr.get(), level, Z_DEFLATED, 15 + (type == STREAM_GZIP ? 16 : 0), 8, Z_DEFAULT_STRATEGY);
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


DeflatingStreamBuf::DeflatingStreamBuf(std::ostream& ostr, int windowBits, int level):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::out),
	_pIstr(0),
	_pOstr(&ostr),
	_eof(false)
{
	std::unique_ptr<char[]> buffer(new char[DEFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = deflateInit2(pZstr.get(), level, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY);
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


DeflatingStreamBuf::~DeflatingStreamBuf()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
	delete [] _buffer;
	deflateEnd(_pZstr);
	delete _pZstr;
}


int DeflatingStreamBuf::close()
{
	BufferedStreamBuf::sync();
	_pIstr = 0;
	if (_pOstr)
	{
		if (_pZstr->next_out)
		{
			int rc = deflate(_pZstr, Z_FINISH);
			if (rc != Z_OK && rc != Z_STREAM_END) throw IOException(zError(rc));
			_pOstr->write(_buffer, DEFLATE_BUFFER_SIZE - _pZstr->avail_out);
			if (!_pOstr->good()) throw IOException("Failed writing deflated data to output stream");
			_pZstr->next_out  = (unsigned char*) _buffer;
			_pZstr->avail_out = DEFLATE_BUFFER_SIZE;
			while (rc != Z_STREAM_END)
			{
				rc = deflate(_pZstr, Z_FINISH);
				if (rc != Z_OK && rc != Z_STREAM_END) throw IOException(zError(rc));
				_pOstr->write(_buffer, DEFLATE_BUFFER_SIZE - _pZstr->avail_out);
				if (!_pOstr->good()) throw IOException("Failed writing deflated data to output stream");
				_pZstr->next_out  = (unsigned char*) _buffer;
				_pZstr->avail_out = DEFLATE_BUFFER_SIZE;
			}
		}
		_pOstr->flush();
		_pOstr = 0;
	}
	return 0;
}


int DeflatingStreamBuf::sync()
{
	if (BufferedStreamBuf::sync())
		return -1;

	if (_pOstr)
	{
		if (_pZstr->next_out)
		{
			int rc = deflate(_pZstr, Z_SYNC_FLUSH);
			if (rc != Z_OK) throw IOException(zError(rc));
			_pOstr->write(_buffer, DEFLATE_BUFFER_SIZE - _pZstr->avail_out);
			if (!_pOstr->good()) throw IOException("Failed writing deflated data to output stream");
			while (_pZstr->avail_out == 0)
			{
				_pZstr->next_out  = (unsigned char*) _buffer;
				_pZstr->avail_out = DEFLATE_BUFFER_SIZE;
				rc = deflate(_pZstr, Z_SYNC_FLUSH);
				if (rc != Z_OK) throw IOException(zError(rc));
				_pOstr->write(_buffer, DEFLATE_BUFFER_SIZE - _pZstr->avail_out);
				if (!_pOstr->good()) throw IOException("Failed writing deflated data to output stream");
			};
			_pZstr->next_out  = (unsigned char*) _buffer;
			_pZstr->avail_out = DEFLATE_BUFFER_SIZE;
		}
		// NOTE: This breaks the Zip library and causes corruption in some files.
		// See GH #1828
		// _pOstr->flush();
	}
	return 0;
}


int DeflatingStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	if (!_pIstr) return 0;
	if (_pZstr->avail_in == 0 && !_eof)
	{
		int n = 0;
		if (_pIstr->good())
		{
			_pIstr->read(_buffer, DEFLATE_BUFFER_SIZE);
			n = static_cast<int>(_pIstr->gcount());
		}
		if (n > 0)
		{
			_pZstr->next_in  = (unsigned char*) _buffer;
			_pZstr->avail_in = n;
		}
		else
		{
			_pZstr->next_in  = 0;
			_pZstr->avail_in = 0;
			_eof = true;
		}
	}
	_pZstr->next_out  = (unsigned char*) buffer;
	_pZstr->avail_out = static_cast<unsigned>(length);
	for (;;)
	{
		int rc = deflate(_pZstr, _eof ? Z_FINISH : Z_NO_FLUSH);
		if (_eof && rc == Z_STREAM_END)
		{
			_pIstr = 0;
			return static_cast<int>(length) - _pZstr->avail_out;
		}
		if (rc != Z_OK) throw IOException(zError(rc));
		if (_pZstr->avail_out == 0)
		{
			return static_cast<int>(length);
		}
		if (_pZstr->avail_in == 0)
		{
			int n = 0;
			if (_pIstr->good())
			{
				_pIstr->read(_buffer, DEFLATE_BUFFER_SIZE);
				n = static_cast<int>(_pIstr->gcount());
			}
			if (n > 0)
			{
				_pZstr->next_in  = (unsigned char*) _buffer;
				_pZstr->avail_in = n;
			}
			else
			{
				_pZstr->next_in  = 0;
				_pZstr->avail_in = 0;
				_eof = true;
			}
		}
	}
}


int DeflatingStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	if (length == 0 || !_pOstr) return 0;

	_pZstr->next_in   = (unsigned char*) buffer;
	_pZstr->avail_in  = static_cast<unsigned>(length);
	_pZstr->next_out  = (unsigned char*) _buffer;
	_pZstr->avail_out = DEFLATE_BUFFER_SIZE;
	for (;;)
	{
		int rc = deflate(_pZstr, Z_NO_FLUSH);
		if (rc != Z_OK) throw IOException(zError(rc));
		if (_pZstr->avail_out == 0)
		{
			_pOstr->write(_buffer, DEFLATE_BUFFER_SIZE);
			if (!_pOstr->good()) throw IOException("Failed writing deflated data to output stream");
			_pZstr->next_out  = (unsigned char*) _buffer;
			_pZstr->avail_out = DEFLATE_BUFFER_SIZE;
		}
		if (_pZstr->avail_in == 0)
		{
			_pOstr->write(_buffer, DEFLATE_BUFFER_SIZE - _pZstr->avail_out);
			if (!_pOstr->good()) throw IOException("Failed writing deflated data to output stream");
			_pZstr->next_out  = (unsigned char*) _buffer;
			_pZstr->avail_out = DEFLATE_BUFFER_SIZE;
			break;
		}
	}
	return static_cast<int>(length);
}


DeflatingIOS::DeflatingIOS(std::ostream& ostr, DeflatingStreamBuf::StreamType type, int level):
	_buf(ostr, type, level)
{
	poco_ios_init(&_buf);
}


DeflatingIOS::DeflatingIOS(std::ostream& ostr, int windowBits, int level):
	_buf(ostr, windowBits, level)
{
	poco_ios_init(&_buf);
}


DeflatingIOS::DeflatingIOS(std::istream& istr, DeflatingStreamBuf::StreamType type, int level):
	_buf(istr, type, level)
{
	poco_ios_init(&_buf);
}


DeflatingIOS::DeflatingIOS(std::istream& istr, int windowBits, int level):
	_buf(istr, windowBits, level)
{
	poco_ios_init(&_buf);
}


DeflatingIOS::~DeflatingIOS()
{
}


DeflatingStreamBuf* DeflatingIOS::rdbuf()
{
	return &_buf;
}


DeflatingOutputStream::DeflatingOutputStream(std::ostream& ostr, DeflatingStreamBuf::StreamType type, int level):
	std::ostream(&_buf),
	DeflatingIOS(ostr, type, level)
{
}


DeflatingOutputStream::DeflatingOutputStream(std::ostream& ostr, int windowBits, int level):
	std::ostream(&_buf),
	DeflatingIOS(ostr, windowBits, level)
{
}


DeflatingOutputStream::~DeflatingOutputStream()
{
}


int DeflatingOutputStream::close()
{
	return _buf.close();
}


int DeflatingOutputStream::sync()
{
	return _buf.pubsync();
}


DeflatingInputStream::DeflatingInputStream(std::istream& istr, DeflatingStreamBuf::StreamType type, int level):
	std::istream(&_buf),
	DeflatingIOS(istr, type, level)
{
}


DeflatingInputStream::DeflatingInputStream(std::istream& istr, int windowBits, int level):
	std::istream(&_buf),
	DeflatingIOS(istr, windowBits, level)
{
}


DeflatingInputStream::~DeflatingInputStream()
{
}


} // namespace Poco
