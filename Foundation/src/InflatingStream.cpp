//
// InflatingStream.cpp
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


#include "Poco/InflatingStream.h"
#include "Poco/Exception.h"
#include <cstring>
#include <memory>
#if defined(POCO_UNBUNDLED)
#include <zlib.h>
#else
#include "zlib.h"
#endif


namespace Poco {


InflatingStreamBuf::InflatingStreamBuf(std::istream& istr, StreamType type):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in),
	_pIstr(&istr),
	_pOstr(0),
	_eof(false),
	_check(type != STREAM_ZIP)
{
	std::unique_ptr<char[]> buffer(new char[INFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = inflateInit2(pZstr.get(), 15 + (type == STREAM_GZIP ? 16 : 0));
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


InflatingStreamBuf::InflatingStreamBuf(std::istream& istr, int windowBits):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::in),
	_pIstr(&istr),
	_pOstr(0),
	_eof(false),
	_check(false)
{
	std::unique_ptr<char[]> buffer(new char[INFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = inflateInit2(pZstr.get(), windowBits);
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


InflatingStreamBuf::InflatingStreamBuf(std::ostream& ostr, StreamType type):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::out),
	_pIstr(0),
	_pOstr(&ostr),
	_eof(false),
	_check(type != STREAM_ZIP)
{
	std::unique_ptr<char[]> buffer(new char[INFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = inflateInit2(pZstr.get(), 15 + (type == STREAM_GZIP ? 16 : 0));
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


InflatingStreamBuf::InflatingStreamBuf(std::ostream& ostr, int windowBits):
	BufferedStreamBuf(STREAM_BUFFER_SIZE, std::ios::out),
	_pIstr(0),
	_pOstr(&ostr),
	_eof(false),
	_check(false)
{
	std::unique_ptr<char[]> buffer(new char[INFLATE_BUFFER_SIZE]);

	std::unique_ptr<z_stream> pZstr = std::make_unique<z_stream>(z_stream{});
	int rc = inflateInit2(pZstr.get(), windowBits);
	if (rc != Z_OK)
	{
		throw IOException(zError(rc));
	}

	_pZstr = pZstr.release();
	_buffer = buffer.release();
}


InflatingStreamBuf::~InflatingStreamBuf()
{
	try
	{
		close();
	}
	catch (...)
	{
	}
	delete [] _buffer;
	inflateEnd(_pZstr);
	delete _pZstr;
}


int InflatingStreamBuf::close()
{
	sync();
	_pIstr = 0;
	_pOstr = 0;
	return 0;
}


void InflatingStreamBuf::reset()
{
	int rc = inflateReset(_pZstr);
	if (rc == Z_OK)
		_eof = false;
	else
		throw IOException(zError(rc));
}


int InflatingStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	if (_eof || !_pIstr) return 0;

	if (_pZstr->avail_in == 0)
	{
		int n = 0;
		if (_pIstr->good())
		{
			_pIstr->read(_buffer, INFLATE_BUFFER_SIZE);
			n = static_cast<int>(_pIstr->gcount());
		}
		_pZstr->next_in   = (unsigned char*) _buffer;
		_pZstr->avail_in  = n;
	}
	_pZstr->next_out  = (unsigned char*) buffer;
	_pZstr->avail_out = static_cast<unsigned>(length);
	for (;;)
	{
		int rc = inflate(_pZstr, Z_NO_FLUSH);
		if (rc == Z_DATA_ERROR && !_check)
		{
			if (_pZstr->avail_in == 0)
			{
				if (_pIstr->good())
					rc = Z_OK;
				else
					rc = Z_STREAM_END;
			}
		}
		if (rc == Z_STREAM_END)
		{
			_eof = true;
			return static_cast<int>(length) - _pZstr->avail_out;
		}
		if (rc != Z_OK) throw IOException(zError(rc));
		if (_pZstr->avail_out == 0)
			return static_cast<int>(length);
		if (_pZstr->avail_in == 0)
		{
			int n = 0;
			if (_pIstr->good())
			{
				_pIstr->read(_buffer, INFLATE_BUFFER_SIZE);
				n = static_cast<int>(_pIstr->gcount());
			}
			if (n > 0)
			{
				_pZstr->next_in  = (unsigned char*) _buffer;
				_pZstr->avail_in = n;
			}
			else return static_cast<int>(length) - _pZstr->avail_out;
		}
	}
}


int InflatingStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	if (length == 0 || !_pOstr) return 0;

	_pZstr->next_in   = (unsigned char*) buffer;
	_pZstr->avail_in  = static_cast<unsigned>(length);
	_pZstr->next_out  = (unsigned char*) _buffer;
	_pZstr->avail_out = INFLATE_BUFFER_SIZE;
	for (;;)
	{
		int rc = inflate(_pZstr, Z_NO_FLUSH);
		if (rc == Z_STREAM_END)
		{
			_pOstr->write(_buffer, INFLATE_BUFFER_SIZE - _pZstr->avail_out);
			if (!_pOstr->good()) throw IOException("Failed writing inflated data to output stream");
			break;
		}
		if (rc != Z_OK) throw IOException(zError(rc));
		if (_pZstr->avail_out == 0)
		{
			_pOstr->write(_buffer, INFLATE_BUFFER_SIZE);
			if (!_pOstr->good()) throw IOException("Failed writing inflated data to output stream");
			_pZstr->next_out  = (unsigned char*) _buffer;
			_pZstr->avail_out = INFLATE_BUFFER_SIZE;
		}
		if (_pZstr->avail_in == 0)
		{
			_pOstr->write(_buffer, INFLATE_BUFFER_SIZE - _pZstr->avail_out);
			if (!_pOstr->good()) throw IOException("Failed writing inflated data to output stream");
			_pZstr->next_out  = (unsigned char*) _buffer;
			_pZstr->avail_out = INFLATE_BUFFER_SIZE;
			break;
		}
	}
	return static_cast<int>(length);
}


int InflatingStreamBuf::sync()
{
	int n = BufferedStreamBuf::sync();
	if (!n && _pOstr) _pOstr->flush();
	return n;
}


InflatingIOS::InflatingIOS(std::ostream& ostr, InflatingStreamBuf::StreamType type):
	_buf(ostr, type)
{
	poco_ios_init(&_buf);
}


InflatingIOS::InflatingIOS(std::ostream& ostr, int windowBits):
	_buf(ostr, windowBits)
{
	poco_ios_init(&_buf);
}


InflatingIOS::InflatingIOS(std::istream& istr, InflatingStreamBuf::StreamType type):
	_buf(istr, type)
{
	poco_ios_init(&_buf);
}


InflatingIOS::InflatingIOS(std::istream& istr, int windowBits):
	_buf(istr, windowBits)
{
	poco_ios_init(&_buf);
}


InflatingIOS::~InflatingIOS()
{
}


InflatingStreamBuf* InflatingIOS::rdbuf()
{
	return &_buf;
}


InflatingOutputStream::InflatingOutputStream(std::ostream& ostr, InflatingStreamBuf::StreamType type):
	std::ostream(&_buf),
	InflatingIOS(ostr, type)
{
}


InflatingOutputStream::InflatingOutputStream(std::ostream& ostr, int windowBits):
	std::ostream(&_buf),
	InflatingIOS(ostr, windowBits)
{
}


InflatingOutputStream::~InflatingOutputStream()
{
}


int InflatingOutputStream::close()
{
	return _buf.close();
}


InflatingInputStream::InflatingInputStream(std::istream& istr, InflatingStreamBuf::StreamType type):
	std::istream(&_buf),
	InflatingIOS(istr, type)
{
}


InflatingInputStream::InflatingInputStream(std::istream& istr, int windowBits):
	std::istream(&_buf),
	InflatingIOS(istr, windowBits)
{
}


InflatingInputStream::~InflatingInputStream()
{
}


void InflatingInputStream::reset()
{
	_buf.reset();
	clear();
}


} // namespace Poco
