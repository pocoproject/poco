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
#include "Poco/File.h"
#include "Poco/Exception.h"


namespace Poco {


FileStreamBuf::FileStreamBuf():
	BufferedBidirectionalStreamBuf(BUFFER_SIZE, std::ios::in | std::ios::out),
	_handle(INVALID_HANDLE_VALUE),
	_pos(0)
{
}


FileStreamBuf::~FileStreamBuf()
{
	close();
}


void FileStreamBuf::open(const std::string& path, std::ios::openmode mode)
{
	poco_assert (_handle == INVALID_HANDLE_VALUE);

	_path = path;
	_pos = 0;
	setMode(mode);
	resetBuffers();

	DWORD access = 0;
	if (mode & std::ios::in)
		access |= GENERIC_READ;
	if (mode & std::ios::out)
		access |= GENERIC_WRITE;

	DWORD shareMode = FILE_SHARE_READ;
	if (!(mode & std::ios::out))
		shareMode |= FILE_SHARE_WRITE;

	DWORD creationDisp = OPEN_EXISTING;
	if (mode & std::ios::trunc)
		creationDisp = CREATE_ALWAYS;
	else if (mode & std::ios::out)
		creationDisp = OPEN_ALWAYS;

	DWORD flags = FILE_ATTRIBUTE_NORMAL;

	std::wstring utf16Path;
	FileImpl::convertPath(path, utf16Path);
	_handle = CreateFileW(utf16Path.c_str(), access, shareMode, NULL, creationDisp, flags, NULL);

	if (_handle == INVALID_HANDLE_VALUE)
		File::handleLastError(_path);

	if ((mode & std::ios::ate) || (mode & std::ios::app))
		seekoff(0, std::ios::end, mode);
}


void FileStreamBuf::openHandle(NativeHandle handle, std::ios::openmode mode)
{
	poco_assert(_handle == INVALID_HANDLE_VALUE);
	poco_assert(handle != INVALID_HANDLE_VALUE);

	_pos = 0;
	setMode(mode);
	resetBuffers();

	_handle = handle;

	if ((mode & std::ios::ate) || (mode & std::ios::app))
		seekoff(0, std::ios::end, mode);
}


int FileStreamBuf::readFromDevice(char* buffer, std::streamsize length)
{
	if (INVALID_HANDLE_VALUE == _handle || !(getMode() & std::ios::in))
		return -1;

	if (getMode() & std::ios::out)
		sync();

	DWORD bytesRead(0);
	BOOL rc = ReadFile(_handle, buffer, static_cast<DWORD>(length), &bytesRead, NULL);
	if (rc == 0)
	{
		if (GetLastError() == ERROR_BROKEN_PIPE)
		{
			// Read from closed pipe -> treat as EOF
			return 0;
		}
		File::handleLastError(_path);
	}

	_pos += bytesRead;

	return static_cast<int>(bytesRead);
}


int FileStreamBuf::writeToDevice(const char* buffer, std::streamsize length)
{
	if (INVALID_HANDLE_VALUE == _handle || !(getMode() & std::ios::out))
		return -1;

	if (getMode() & std::ios::app)
	{
		LARGE_INTEGER li;
		li.QuadPart = 0;
		li.LowPart  = SetFilePointer(_handle, li.LowPart, &li.HighPart, FILE_END);
		if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
			File::handleLastError(_path);
		_pos = li.QuadPart;
	}

	DWORD bytesWritten(0);
	BOOL rc = WriteFile(_handle, buffer, static_cast<DWORD>(length), &bytesWritten, NULL);
	if (rc == 0)
		File::handleLastError(_path);

	_pos += bytesWritten;

	return static_cast<int>(bytesWritten);
}


bool FileStreamBuf::close()
{
	bool success = true;
	if (_handle != INVALID_HANDLE_VALUE)
	{
		try
		{
			if (getMode() & std::ios::out)
				sync();
		}
		catch (...)
		{
			success = false;
		}
		CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
	return success;
}


std::streampos FileStreamBuf::seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode)
{
	if (INVALID_HANDLE_VALUE == _handle || !(getMode() & mode))
		return -1;

	if (getMode() & std::ios::out)
		sync();

	std::streamoff adj;
	if (mode & std::ios::in)
		adj = static_cast<std::streamoff>(egptr() - gptr());
	else
		adj = 0;

	resetBuffers();

	DWORD offset = FILE_BEGIN;
	if (dir == std::ios::cur)
	{
		offset = FILE_CURRENT;
		off -= adj;
	}
	else if (dir == std::ios::end)
	{
		offset = FILE_END;
	}

	LARGE_INTEGER li;
	li.QuadPart = off;
	li.LowPart  = SetFilePointer(_handle, li.LowPart, &li.HighPart, offset);

	if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
		File::handleLastError(_path);
	_pos = li.QuadPart;
	return std::streampos(static_cast<std::streamoff>(_pos));
}


std::streampos FileStreamBuf::seekpos(std::streampos pos, std::ios::openmode mode)
{
	if (INVALID_HANDLE_VALUE == _handle || !(getMode() & mode))
		return -1;

	if (getMode() & std::ios::out)
		sync();

	resetBuffers();

	LARGE_INTEGER li;
	li.QuadPart = pos;
	li.LowPart  = SetFilePointer(_handle, li.LowPart, &li.HighPart, FILE_BEGIN);

	if (li.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
		File::handleLastError(_path);
	_pos = li.QuadPart;
	return std::streampos(static_cast<std::streamoff>(_pos));
}


void FileStreamBuf::flushToDisk()
{
	if (getMode() & std::ios::out)
	{
		sync();
		if (FlushFileBuffers(_handle) == 0)
			File::handleLastError(_path);
	}
}


FileStreamBuf::NativeHandle FileStreamBuf::nativeHandle() const
{
	return _handle;
}

Poco::UInt64 FileStreamBuf::size() const
{
	LARGE_INTEGER result;
	result.QuadPart = 0;
	DWORD high = 0;
	result.LowPart = ::GetFileSize(_handle, &high);
	if (high > 0)
	{
		result.HighPart = high;
	}
	return result.QuadPart;
}


} // namespace Poco
