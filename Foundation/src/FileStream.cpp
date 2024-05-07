//
// FileStream.cpp
//
// Library: Foundation
// Package: Streams
// Module:  FileStream
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/FileStream.h"
#include "Poco/Exception.h"
#if defined(POCO_OS_FAMILY_WINDOWS)
#include "FileStream_WIN32.cpp"
#else
#include "FileStream_POSIX.cpp"
#endif


namespace Poco {


FileIOS::FileIOS()
{
	poco_ios_init(&_buf);
}


FileIOS::~FileIOS()
{
}


void FileIOS::open(const std::string& path, std::ios::openmode mode)
{
	clear();
	_buf.open(path, mode);
}


void FileIOS::openHandle(NativeHandle handle, std::ios::openmode mode)
{
	clear();
	_buf.openHandle(handle, mode);
}


void FileIOS::close()
{
	if (!_buf.close())
	{
		setstate(ios_base::badbit);
	}
}


FileStreamBuf* FileIOS::rdbuf()
{
	return &_buf;
}


FileIOS::NativeHandle FileIOS::nativeHandle() const
{
	return _buf.nativeHandle();
}


Poco::UInt64 FileIOS::size() const
{
	return _buf.size();
}


void FileIOS::flushToDisk()
{
	_buf.flushToDisk();
}


FileInputStream::FileInputStream():
	std::istream(&_buf)
{
}


FileInputStream::FileInputStream(const std::string& path, std::ios::openmode mode):
	std::istream(&_buf)
{
	open(path, mode);
}


FileInputStream::~FileInputStream()
{
}


void FileInputStream::open(const std::string& path, std::ios::openmode mode)
{
	FileIOS::open(path, mode | std::ios::in);
}


FileOutputStream::FileOutputStream():
	std::ostream(&_buf)
{
}


FileOutputStream::FileOutputStream(const std::string& path, std::ios::openmode mode):
	std::ostream(&_buf)
{
	open(path, mode);
}


FileOutputStream::~FileOutputStream()
{
}


void FileOutputStream::open(const std::string& path, std::ios::openmode mode)
{
	FileIOS::open(path, mode | std::ios::out);
}


FileStream::FileStream():
	std::iostream(&_buf)
{
}


FileStream::FileStream(const std::string& path, std::ios::openmode mode):
	std::iostream(&_buf)
{
	open(path, mode);
}


FileStream::~FileStream()
{
}


void FileStream::open(const std::string& path, std::ios::openmode mode)
{
	FileIOS::open(path, mode);
}


} // namespace Poco
