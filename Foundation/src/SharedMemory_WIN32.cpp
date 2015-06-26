//
// SharedMemoryImpl.cpp
//
// $Id: //poco/1.4/Foundation/src/SharedMemory_WIN32.cpp#1 $
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
#include "Poco/Exception.h"
#include "Poco/File.h"
#if defined (POCO_WIN32_UTF8)
#include "Poco/UnicodeConverter.h"
#endif
#include "Poco/UnWindows.h"


namespace Poco {


SharedMemoryImpl::SharedMemoryImpl(const std::string& name, std::size_t size, SharedMemory::AccessMode mode, const void*, bool):
	_name(name),
	_memHandle(INVALID_HANDLE_VALUE),
	_fileHandle(INVALID_HANDLE_VALUE),
	_size(static_cast<DWORD>(size)),
	_mode(PAGE_READONLY),
	_address(0)
{
	if (mode == SharedMemory::AM_WRITE)
		_mode = PAGE_READWRITE;

#if defined (POCO_WIN32_UTF8)
	std::wstring utf16name;
	UnicodeConverter::toUTF16(_name, utf16name);
	_memHandle = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, _mode, 0, _size, utf16name.c_str());
#else
	_memHandle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, _mode, 0, _size, _name.c_str());
#endif

	if (!_memHandle)
		throw SystemException("Cannot create shared memory object", _name);

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

#if defined (POCO_WIN32_UTF8)
	std::wstring utf16name;
	UnicodeConverter::toUTF16(_name, utf16name);
	_fileHandle = CreateFileW(utf16name.c_str(), fileMode, shareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
	_fileHandle = CreateFileA(_name.c_str(), fileMode, shareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#endif

	if (_fileHandle == INVALID_HANDLE_VALUE)
		throw OpenFileException("Cannot open memory mapped file", _name);

	_memHandle = CreateFileMapping(_fileHandle, NULL, _mode, 0, 0, NULL);
	if (!_memHandle)
	{
		CloseHandle(_fileHandle);
		_fileHandle = INVALID_HANDLE_VALUE;
		throw SystemException("Cannot map file into shared memory", _name);
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
		throw SystemException("Cannot map shared memory object", _name);

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
