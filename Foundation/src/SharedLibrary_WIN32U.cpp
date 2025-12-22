//
// SharedLibrary_WIN32U.cpp
//
// Library: Foundation
// Package: SharedLibrary
// Module:  SharedLibrary
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/SharedLibrary_WIN32U.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Path.h"
#include "Poco/UnWindows.h"
#include "Poco/Error.h"
#include "Poco/Format.h"
#include "Poco/String.h"
#include <vector>
#include <string>


namespace Poco {


FastMutex SharedLibraryImpl::_mutex;


SharedLibraryImpl::SharedLibraryImpl()
{
	_handle = 0;
}


SharedLibraryImpl::~SharedLibraryImpl()
{
}


void SharedLibraryImpl::loadImpl(const std::string& path, int /*flags*/)
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle) throw LibraryAlreadyLoadedException(_path);
	DWORD flags(0);

	Path p(path);
	if (p.isAbsolute()) flags |= LOAD_WITH_ALTERED_SEARCH_PATH;

	std::wstring upath;
	UnicodeConverter::toUTF16(path, upath);
	_handle = LoadLibraryExW(upath.c_str(), 0, flags);
	if (!_handle)
	{
		DWORD errn = Error::last();
		std::string err;
		Poco::format(err, "Error %lu while loading [%s]: [%s]", errn, path, Poco::trim(Error::getMessage(errn)));

		// Try to identify missing dependencies
		std::vector<std::string> missingDeps = findMissingDependenciesImpl(path);
		if (!missingDeps.empty())
		{
			err += "; missing dependencies: ";
			for (size_t i = 0; i < missingDeps.size(); ++i)
			{
				if (i > 0) err += ", ";
				err += missingDeps[i];
			}
		}

		throw LibraryLoadException(err);
	}
	_path = path;
}


void SharedLibraryImpl::unloadImpl()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle)
	{
		FreeLibrary((HMODULE) _handle);
		_handle = 0;
	}
	_path.clear();
}


bool SharedLibraryImpl::isLoadedImpl() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _handle != 0;
}


void* SharedLibraryImpl::findSymbolImpl(const std::string& name)
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle)
	{
		return (void*) GetProcAddress((HMODULE) _handle, name.c_str());
	}
	else return 0;
}


const std::string& SharedLibraryImpl::getPathImpl() const
{
	return _path;
}


std::string SharedLibraryImpl::suffixImpl()
{
#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
	return "d.dll";
#else
	return ".dll";
#endif
}


bool SharedLibraryImpl::setSearchPathImpl(const std::string& path)
{
#if _WIN32_WINNT >= 0x0502
	std::wstring wpath;
	Poco::UnicodeConverter::toUTF16(path, wpath);
	return SetDllDirectoryW(wpath.c_str()) != 0;
#else
	return false;
#endif
}


std::vector<std::string> SharedLibraryImpl::findMissingDependenciesImpl(const std::string& path)
{
	std::vector<std::string> missingDeps;

	std::wstring wpath;
	UnicodeConverter::toUTF16(path, wpath);

	HANDLE hFile = CreateFileW(wpath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		missingDeps.push_back(path);
		return missingDeps;
	}

	HANDLE hMapping = CreateFileMappingW(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (!hMapping)
	{
		CloseHandle(hFile);
		return missingDeps;
	}

	LPVOID pBase = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0);
	if (!pBase)
	{
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return missingDeps;
	}

	auto parseImports = [&]()
	{
		PIMAGE_DOS_HEADER pDosHeader = static_cast<PIMAGE_DOS_HEADER>(pBase);
		if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
			return;

		PIMAGE_NT_HEADERS pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
			static_cast<BYTE*>(pBase) + pDosHeader->e_lfanew);
		if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
			return;

		DWORD importDirRVA = 0;
		DWORD importDirSize = 0;

		if (pNtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		{
			PIMAGE_NT_HEADERS32 pNtHeaders32 = reinterpret_cast<PIMAGE_NT_HEADERS32>(pNtHeaders);
			if (pNtHeaders32->OptionalHeader.NumberOfRvaAndSizes > IMAGE_DIRECTORY_ENTRY_IMPORT)
			{
				importDirRVA = pNtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
				importDirSize = pNtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
			}
		}
		else if (pNtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
		{
			PIMAGE_NT_HEADERS64 pNtHeaders64 = reinterpret_cast<PIMAGE_NT_HEADERS64>(pNtHeaders);
			if (pNtHeaders64->OptionalHeader.NumberOfRvaAndSizes > IMAGE_DIRECTORY_ENTRY_IMPORT)
			{
				importDirRVA = pNtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
				importDirSize = pNtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size;
			}
		}

		if (importDirRVA == 0 || importDirSize == 0)
			return;

		// Convert RVA to file offset by finding the section containing the import directory
		PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeaders);
		DWORD importDirOffset = 0;
		for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
		{
			if (importDirRVA >= pSectionHeader[i].VirtualAddress &&
				importDirRVA < pSectionHeader[i].VirtualAddress + pSectionHeader[i].Misc.VirtualSize)
			{
				importDirOffset = importDirRVA - pSectionHeader[i].VirtualAddress + pSectionHeader[i].PointerToRawData;
				break;
			}
		}

		if (importDirOffset == 0)
			return;

		PIMAGE_IMPORT_DESCRIPTOR pImportDesc = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(
			static_cast<BYTE*>(pBase) + importDirOffset);

		// Get the directory containing the DLL for search path
		Path dllPath(path);
		std::string dllDir = dllPath.parent().toString();
		std::wstring wdllDir;
		UnicodeConverter::toUTF16(dllDir, wdllDir);

		while (pImportDesc->Name != 0)
		{
			// Convert the Name RVA to file offset
			DWORD nameRVA = pImportDesc->Name;
			DWORD nameOffset = 0;
			for (WORD i = 0; i < pNtHeaders->FileHeader.NumberOfSections; ++i)
			{
				if (nameRVA >= pSectionHeader[i].VirtualAddress &&
					nameRVA < pSectionHeader[i].VirtualAddress + pSectionHeader[i].Misc.VirtualSize)
				{
					nameOffset = nameRVA - pSectionHeader[i].VirtualAddress + pSectionHeader[i].PointerToRawData;
					break;
				}
			}

			if (nameOffset != 0)
			{
				const char* dllName = reinterpret_cast<const char*>(static_cast<BYTE*>(pBase) + nameOffset);

				// Skip API set DLLs - these are virtual DLLs resolved by Windows at runtime
				// They start with "api-" or "ext-" (per Microsoft documentation)
				if (_strnicmp(dllName, "api-", 4) == 0 || _strnicmp(dllName, "ext-", 4) == 0)
				{
					++pImportDesc;
					continue;
				}

				// Check if the DLL can be found
				std::wstring wDllName;
				UnicodeConverter::toUTF16(std::string(dllName), wDllName);

				// Try to find the DLL using the standard search order
				HMODULE hMod = LoadLibraryExW(wDllName.c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
				if (!hMod)
				{
					// Try in the same directory as the original DLL
					std::wstring fullPath = wdllDir + wDllName;
					hMod = LoadLibraryExW(fullPath.c_str(), NULL, LOAD_LIBRARY_AS_DATAFILE);
				}

				if (!hMod)
				{
					missingDeps.push_back(dllName);
				}
				else
				{
					FreeLibrary(hMod);
				}
			}

			++pImportDesc;
		}
	};

	try
	{
		parseImports();
	}
	catch (...)
	{
		// If we get an exception while parsing, just return what we have
	}

	UnmapViewOfFile(pBase);
	CloseHandle(hMapping);
	CloseHandle(hFile);

	return missingDeps;
}


} // namespace Poco
