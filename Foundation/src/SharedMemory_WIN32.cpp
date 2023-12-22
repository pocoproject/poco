//
// SharedMemoryImpl.cpp
//
// Library: Foundation
// Package: Processes
// Module:  SharedMemoryImpl
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SharedMemory_WIN32.h"
#include "Poco/Error.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/Format.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/UnWindows.h"


namespace Poco {


SharedMemoryImpl::SharedMemoryImpl(const std::string& name, std::size_t size, SharedMemory::AccessMode mode, const void*, bool):
	_name(name),
	_memHandle(INVALID_HANDLE_VALUE),
	_fileHandle(INVALID_HANDLE_VALUE),
	_size(size),
	_mode(PAGE_READONLY),
	_address(0)
{
	if (mode == SharedMemory::AM_WRITE)
		_mode = PAGE_READWRITE;

	std::wstring utf16name;
	UnicodeConverter::toUTF16(_name, utf16name);
#ifdef _WIN64
	const DWORD dwMaxSizeLow = static_cast<DWORD>(_size & 0xFFFFFFFFULL);
	const DWORD dwMaxSizeHigh = static_cast<DWORD>((_size & (0xFFFFFFFFULL << 32)) >> 32);
#else
	if (_size > std::numeric_limits<DWORD>::max())
	{
		throw Poco::InvalidArgumentException(Poco::format("Requested shared memory size (%z) too large (max %lu)",
			_size, std::numeric_limits<DWORD>::max()));
	}
	const DWORD dwMaxSizeLow = static_cast<DWORD>(_size);
	const DWORD dwMaxSizeHigh = 0UL;
#endif
	_memHandle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, _mode, dwMaxSizeHigh, dwMaxSizeLow, utf16name.c_str());

	if (!_memHandle)
	{
		DWORD dwRetVal = GetLastError();
		int retVal = static_cast<int>(dwRetVal);

		if (_mode != PAGE_READONLY || dwRetVal != 5)
		{
			throw SystemException(Poco::format("Cannot create shared memory object %s [Error %d: %s]",
				_name, retVal, Error::getMessage(dwRetVal)), retVal);
		}

		_memHandle = OpenFileMappingW(PAGE_READONLY, FALSE, utf16name.c_str());
		if (!_memHandle)
		{
			dwRetVal = GetLastError();
			throw SystemException(Poco::format("Cannot open shared memory object %s [Error %d: %s]",
				_name, retVal, Error::getMessage(dwRetVal)), retVal);
		}
	}
	map();
}


SharedMemoryImpl::SharedMemoryImpl(const Poco::File& file, SharedMemory::AccessMode mode, const void*):
	_name(file.path()),
	_memHandle(INVALID_HANDLE_VALUE),
	_fileHandle(INVALID_HANDLE_VALUE),
	_size(0),
	_mode(PAGE_READONLY),
	_address(0)
{
	if (!file.exists() || !file.isFile())
		throw FileNotFoundException(_name);

	_size = static_cast<DWORD>(file.getSize());

	DWORD shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	DWORD fileMode  = GENERIC_READ;

	if (mode == SharedMemory::AM_WRITE)
	{
		_mode = PAGE_READWRITE;
		fileMode |= GENERIC_WRITE;
	}

	std::wstring utf16name;
	UnicodeConverter::toUTF16(_name, utf16name);
	_fileHandle = CreateFileW(utf16name.c_str(), fileMode, shareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (_fileHandle == INVALID_HANDLE_VALUE)
		throw OpenFileException("Cannot open memory mapped file", _name);

	_memHandle = CreateFileMapping(_fileHandle, NULL, _mode, 0, 0, NULL);
	if (!_memHandle)
	{
		DWORD dwRetVal = GetLastError();
		CloseHandle(_fileHandle);
		_fileHandle = INVALID_HANDLE_VALUE;
		throw SystemException(format("Cannot map file into shared memory %s [Error %d: %s]", _name, (int)dwRetVal, Error::getMessage(dwRetVal)));
	}
	map();
}


SharedMemoryImpl::~SharedMemoryImpl()
{
	unmap();
	close();
}


void SharedMemoryImpl::map()
{
	DWORD access = FILE_MAP_READ;
	if (_mode == PAGE_READWRITE)
		access = FILE_MAP_WRITE;
	LPVOID addr = MapViewOfFile(_memHandle, access, 0, 0, _size);
	if (!addr)
	{
		DWORD dwRetVal = GetLastError();
		throw SystemException(format("Cannot map shared memory object %s [Error %d: %s]", _name, (int)dwRetVal, Error::getMessage(dwRetVal)));
	}

	_address = static_cast<char*>(addr);
}


void SharedMemoryImpl::unmap()
{
	if (_address)
	{
		UnmapViewOfFile(_address);
		_address = 0;
		return;
	}
}


void SharedMemoryImpl::close()
{
	if (_memHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_memHandle);
		_memHandle = INVALID_HANDLE_VALUE;
	}

	if (_fileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_fileHandle);
		_fileHandle = INVALID_HANDLE_VALUE;
	}
}


} // namespace Poco
