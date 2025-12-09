//
// SharedLibrary_UNIX.cpp
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


#include "Poco/SharedLibrary_UNIX.h"
#include "Poco/Exception.h"
#include <dlfcn.h>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

#if defined(__APPLE__)
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#elif defined(__linux__)
#include <elf.h>
#endif


// Note: cygwin is missing RTLD_LOCAL, set it to 0
#if defined(__CYGWIN__) && !defined(RTLD_LOCAL)
#define RTLD_LOCAL 0
#endif


namespace Poco {


FastMutex SharedLibraryImpl::_mutex;


SharedLibraryImpl::SharedLibraryImpl()
{
	_handle = nullptr;
}


SharedLibraryImpl::~SharedLibraryImpl()
{
}


void SharedLibraryImpl::loadImpl(const std::string& path, int flags)
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle) throw LibraryAlreadyLoadedException(path);
	int realFlags = RTLD_LAZY;
	if (flags & SHLIB_LOCAL_IMPL)
		realFlags |= RTLD_LOCAL;
	else
		realFlags |= RTLD_GLOBAL;
	_handle = dlopen(path.c_str(), realFlags);
	if (!_handle)
	{
		const char* err = dlerror();
		std::string errMsg = err ? std::string(err) : path;

		// Try to identify missing dependencies
		std::vector<std::string> missingDeps = findMissingDependenciesImpl(path);
		if (!missingDeps.empty())
		{
			errMsg += "; missing dependencies: ";
			for (size_t i = 0; i < missingDeps.size(); ++i)
			{
				if (i > 0) errMsg += ", ";
				errMsg += missingDeps[i];
			}
		}

		throw LibraryLoadException(errMsg);
	}
	_path = path;
}


void SharedLibraryImpl::unloadImpl()
{
	FastMutex::ScopedLock lock(_mutex);

	if (_handle)
	{
		dlclose(_handle);
		_handle = nullptr;
	}
}


bool SharedLibraryImpl::isLoadedImpl() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _handle != nullptr;
}


void* SharedLibraryImpl::findSymbolImpl(const std::string& name)
{
	FastMutex::ScopedLock lock(_mutex);

	void* result = nullptr;
	if (_handle)
	{
		result = dlsym(_handle, name.c_str());
	}
	return result;
}


const std::string& SharedLibraryImpl::getPathImpl() const
{
	return _path;
}


std::string SharedLibraryImpl::suffixImpl()
{
#if defined(__APPLE__)
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.dylib";
	#else
		return ".dylib";
	#endif
#elif defined(hpux) || defined(_hpux)
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.sl";
	#else
		return ".sl";
	#endif
#elif defined(__CYGWIN__)
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.dll";
	#else
		return ".dll";
	#endif
#else
	#if defined(_DEBUG) && !defined(POCO_NO_SHARED_LIBRARY_DEBUG_SUFFIX)
		return "d.so";
	#else
		return ".so";
	#endif
#endif
}


bool SharedLibraryImpl::setSearchPathImpl(const std::string&)
{
	return false;
}


#if defined(__APPLE__)


std::vector<std::string> SharedLibraryImpl::findMissingDependenciesImpl(const std::string& path)
{
	std::vector<std::string> missingDeps;

	std::ifstream file(path, std::ios::binary);
	if (!file)
	{
		missingDeps.push_back(path);
		return missingDeps;
	}

	try
	{
		uint32_t magic;
		file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
		if (!file)
			return missingDeps;

		bool needsSwap = false;
		uint32_t ncmds = 0;
		uint32_t sizeofcmds = 0;

		if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		{
			// Fat binary - skip to first architecture
			bool fatSwap = (magic == FAT_CIGAM);
			fat_header fatHeader;
			file.seekg(0);
			file.read(reinterpret_cast<char*>(&fatHeader), sizeof(fatHeader));
			if (!file)
				return missingDeps;

			uint32_t narch = fatSwap ? __builtin_bswap32(fatHeader.nfat_arch) : fatHeader.nfat_arch;
			if (narch == 0)
				return missingDeps;

			fat_arch arch;
			file.read(reinterpret_cast<char*>(&arch), sizeof(arch));
			if (!file)
				return missingDeps;

			uint32_t offset = fatSwap ? __builtin_bswap32(arch.offset) : arch.offset;
			file.seekg(offset);
			file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
			if (!file)
				return missingDeps;
		}

		if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		{
			needsSwap = (magic == MH_CIGAM_64);
			file.seekg(-static_cast<std::streamoff>(sizeof(magic)), std::ios::cur);
			mach_header_64 header;
			file.read(reinterpret_cast<char*>(&header), sizeof(header));
			if (!file)
				return missingDeps;
			ncmds = needsSwap ? __builtin_bswap32(header.ncmds) : header.ncmds;
			sizeofcmds = needsSwap ? __builtin_bswap32(header.sizeofcmds) : header.sizeofcmds;
		}
		else if (magic == MH_MAGIC || magic == MH_CIGAM)
		{
			needsSwap = (magic == MH_CIGAM);
			file.seekg(-static_cast<std::streamoff>(sizeof(magic)), std::ios::cur);
			mach_header header;
			file.read(reinterpret_cast<char*>(&header), sizeof(header));
			if (!file)
				return missingDeps;
			ncmds = needsSwap ? __builtin_bswap32(header.ncmds) : header.ncmds;
			sizeofcmds = needsSwap ? __builtin_bswap32(header.sizeofcmds) : header.sizeofcmds;
		}
		else
		{
			return missingDeps;
		}

		// Read all load commands
		std::vector<char> cmdsBuffer(sizeofcmds);
		file.read(cmdsBuffer.data(), sizeofcmds);
		if (!file)
			return missingDeps;

		const char* ptr = cmdsBuffer.data();
		const char* end = ptr + sizeofcmds;

		for (uint32_t i = 0; i < ncmds && ptr < end; ++i)
		{
			const load_command* cmd = reinterpret_cast<const load_command*>(ptr);
			uint32_t cmdType = needsSwap ? __builtin_bswap32(cmd->cmd) : cmd->cmd;
			uint32_t cmdSize = needsSwap ? __builtin_bswap32(cmd->cmdsize) : cmd->cmdsize;

			if (cmdSize < sizeof(load_command) || ptr + cmdSize > end)
				break;

			if (cmdType == LC_LOAD_DYLIB || cmdType == LC_LOAD_WEAK_DYLIB || cmdType == LC_REEXPORT_DYLIB)
			{
				const dylib_command* dylibCmd = reinterpret_cast<const dylib_command*>(ptr);
				uint32_t nameOffset = needsSwap ? __builtin_bswap32(dylibCmd->dylib.name.offset) : dylibCmd->dylib.name.offset;

				if (nameOffset < cmdSize)
				{
					const char* libPath = ptr + nameOffset;
					std::string libName(libPath);

					// Check if library can be loaded
					void* handle = dlopen(libName.c_str(), RTLD_LAZY | RTLD_NOLOAD);
					if (!handle)
					{
						handle = dlopen(libName.c_str(), RTLD_LAZY);
						if (!handle)
						{
							// Extract just the library name for cleaner output
							size_t lastSlash = libName.rfind('/');
							if (lastSlash != std::string::npos)
								missingDeps.push_back(libName.substr(lastSlash + 1));
							else
								missingDeps.push_back(libName);
						}
						else
						{
							dlclose(handle);
						}
					}
				}
			}

			ptr += cmdSize;
		}
	}
	catch (...)
	{
		// If we get an exception while parsing, just return what we have
	}

	return missingDeps;
}


#elif defined(__linux__)


std::vector<std::string> SharedLibraryImpl::findMissingDependenciesImpl(const std::string& path)
{
	std::vector<std::string> missingDeps;

	std::ifstream file(path, std::ios::binary);
	if (!file)
	{
		missingDeps.push_back(path);
		return missingDeps;
	}

	try
	{
		unsigned char ident[EI_NIDENT];
		file.read(reinterpret_cast<char*>(ident), EI_NIDENT);
		if (!file)
			return missingDeps;

		if (ident[EI_MAG0] != ELFMAG0 || ident[EI_MAG1] != ELFMAG1 ||
			ident[EI_MAG2] != ELFMAG2 || ident[EI_MAG3] != ELFMAG3)
			return missingDeps;

		bool is64 = (ident[EI_CLASS] == ELFCLASS64);
		file.seekg(0);

		std::vector<std::string> neededLibs;

		if (is64)
		{
			Elf64_Ehdr ehdr;
			file.read(reinterpret_cast<char*>(&ehdr), sizeof(ehdr));
			if (!file)
				return missingDeps;

			// Find the dynamic section and string table
			Elf64_Off shoff = ehdr.e_shoff;
			uint16_t shnum = ehdr.e_shnum;
			uint16_t shentsize = ehdr.e_shentsize;

			if (shoff == 0 || shnum == 0)
				return missingDeps;

			// Read section headers
			std::vector<Elf64_Shdr> sections(shnum);
			file.seekg(shoff);
			for (uint16_t i = 0; i < shnum; ++i)
			{
				file.read(reinterpret_cast<char*>(&sections[i]), sizeof(Elf64_Shdr));
				if (!file)
					return missingDeps;
				if (shentsize > sizeof(Elf64_Shdr))
					file.seekg(shentsize - sizeof(Elf64_Shdr), std::ios::cur);
			}

			// Find .dynamic and .dynstr sections
			Elf64_Shdr* dynSection = nullptr;
			Elf64_Shdr* dynstrSection = nullptr;

			for (auto& sec : sections)
			{
				if (sec.sh_type == SHT_DYNAMIC)
					dynSection = &sec;
				else if (sec.sh_type == SHT_STRTAB && dynstrSection == nullptr)
					dynstrSection = &sec;
			}

			// Use the string table linked from .dynamic section
			if (dynSection && dynSection->sh_link < shnum)
				dynstrSection = &sections[dynSection->sh_link];

			if (!dynSection || !dynstrSection)
				return missingDeps;

			// Read string table
			std::vector<char> strtab(dynstrSection->sh_size);
			file.seekg(dynstrSection->sh_offset);
			file.read(strtab.data(), dynstrSection->sh_size);
			if (!file)
				return missingDeps;

			// Read dynamic entries
			file.seekg(dynSection->sh_offset);
			size_t numEntries = dynSection->sh_size / sizeof(Elf64_Dyn);
			for (size_t i = 0; i < numEntries; ++i)
			{
				Elf64_Dyn dyn;
				file.read(reinterpret_cast<char*>(&dyn), sizeof(dyn));
				if (!file)
					break;

				if (dyn.d_tag == DT_NULL)
					break;

				if (dyn.d_tag == DT_NEEDED)
				{
					if (dyn.d_un.d_val < strtab.size())
						neededLibs.push_back(&strtab[dyn.d_un.d_val]);
				}
			}
		}
		else
		{
			Elf32_Ehdr ehdr;
			file.read(reinterpret_cast<char*>(&ehdr), sizeof(ehdr));
			if (!file)
				return missingDeps;

			Elf32_Off shoff = ehdr.e_shoff;
			uint16_t shnum = ehdr.e_shnum;
			uint16_t shentsize = ehdr.e_shentsize;

			if (shoff == 0 || shnum == 0)
				return missingDeps;

			std::vector<Elf32_Shdr> sections(shnum);
			file.seekg(shoff);
			for (uint16_t i = 0; i < shnum; ++i)
			{
				file.read(reinterpret_cast<char*>(&sections[i]), sizeof(Elf32_Shdr));
				if (!file)
					return missingDeps;
				if (shentsize > sizeof(Elf32_Shdr))
					file.seekg(shentsize - sizeof(Elf32_Shdr), std::ios::cur);
			}

			Elf32_Shdr* dynSection = nullptr;
			Elf32_Shdr* dynstrSection = nullptr;

			for (auto& sec : sections)
			{
				if (sec.sh_type == SHT_DYNAMIC)
					dynSection = &sec;
				else if (sec.sh_type == SHT_STRTAB && dynstrSection == nullptr)
					dynstrSection = &sec;
			}

			if (dynSection && dynSection->sh_link < shnum)
				dynstrSection = &sections[dynSection->sh_link];

			if (!dynSection || !dynstrSection)
				return missingDeps;

			std::vector<char> strtab(dynstrSection->sh_size);
			file.seekg(dynstrSection->sh_offset);
			file.read(strtab.data(), dynstrSection->sh_size);
			if (!file)
				return missingDeps;

			file.seekg(dynSection->sh_offset);
			size_t numEntries = dynSection->sh_size / sizeof(Elf32_Dyn);
			for (size_t i = 0; i < numEntries; ++i)
			{
				Elf32_Dyn dyn;
				file.read(reinterpret_cast<char*>(&dyn), sizeof(dyn));
				if (!file)
					break;

				if (dyn.d_tag == DT_NULL)
					break;

				if (dyn.d_tag == DT_NEEDED)
				{
					if (dyn.d_un.d_val < strtab.size())
						neededLibs.push_back(&strtab[dyn.d_un.d_val]);
				}
			}
		}

		// Check which libraries are missing
		for (const auto& lib : neededLibs)
		{
			void* handle = dlopen(lib.c_str(), RTLD_LAZY | RTLD_NOLOAD);
			if (!handle)
			{
				handle = dlopen(lib.c_str(), RTLD_LAZY);
				if (!handle)
				{
					missingDeps.push_back(lib);
				}
				else
				{
					dlclose(handle);
				}
			}
		}
	}
	catch (...)
	{
		// If we get an exception while parsing, just return what we have
	}

	return missingDeps;
}


#else


std::vector<std::string> SharedLibraryImpl::findMissingDependenciesImpl(const std::string&)
{
	// Unsupported platform - return empty list
	return std::vector<std::string>();
}


#endif


} // namespace Poco
