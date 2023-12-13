//
// WinRegistryKey.cpp
//
// Library: Util
// Package: Windows
// Module:  WinRegistryKey
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Util/WinRegistryKey.h"
#include "Poco/Exception.h"
#include "Poco/Buffer.h"
#include "Poco/UnicodeConverter.h"


using Poco::SystemException;
using Poco::NotFoundException;
using Poco::InvalidArgumentException;


namespace Poco {
namespace Util {


namespace
{
	class AutoHandle
	{
	public:
		AutoHandle(HMODULE h):
			_h(h)
		{
		}

		~AutoHandle()
		{
			FreeLibrary(_h);
		}

		HMODULE handle()
		{
			return _h;
		}

	private:
		HMODULE _h;
	};
}


WinRegistryKey::WinRegistryKey(const std::string& key, bool readOnly, REGSAM extraSam):
	_hKey(0),
	_readOnly(readOnly),
	_extraSam(extraSam)
{
	std::string::size_type pos = key.find('\\');
	if (pos != std::string::npos)
	{
		std::string rootKey = key.substr(0, pos);
		_hRootKey = handleFor(rootKey);
		_subKey   = key.substr(pos + 1);
	}
	else throw InvalidArgumentException("Not a valid registry key", key);
}


WinRegistryKey::WinRegistryKey(HKEY hRootKey, const std::string& subKey, bool readOnly, REGSAM extraSam):
	_hRootKey(hRootKey),
	_subKey(subKey),
	_hKey(0),
	_readOnly(readOnly),
	_extraSam(extraSam)
{
}


WinRegistryKey::~WinRegistryKey()
{
	close();
}


void WinRegistryKey::setString(const std::string& name, const std::string& value)
{
	open();
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	std::wstring uvalue;
	Poco::UnicodeConverter::toUTF16(value, uvalue);
	if (RegSetValueExW(_hKey, uname.c_str(), 0, REG_SZ, (CONST BYTE*) uvalue.c_str(), (DWORD) (uvalue.size() + 1)*sizeof(wchar_t)) != ERROR_SUCCESS)
		handleSetError(name);
}


std::string WinRegistryKey::getString(const std::string& name)
{
	open();
	DWORD type;
	DWORD size;
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegQueryValueExW(_hKey, uname.c_str(), NULL, &type, NULL, &size) != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ && type != REG_LINK))
		throw NotFoundException(key(name));
	if (size > 0)
	{
		DWORD len = size/2;
		Poco::Buffer<wchar_t> buffer(len + 1);
		RegQueryValueExW(_hKey, uname.c_str(), NULL, NULL, (BYTE*) buffer.begin(), &size);
		buffer[len] = 0;
		std::wstring uresult(buffer.begin());
		std::string result;
		Poco::UnicodeConverter::toUTF8(uresult, result);
		return result;
	}
	return std::string();
}


void WinRegistryKey::setStringExpand(const std::string& name, const std::string& value)
{
	open();
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	std::wstring uvalue;
	Poco::UnicodeConverter::toUTF16(value, uvalue);
	if (RegSetValueExW(_hKey, uname.c_str(), 0, REG_EXPAND_SZ, (CONST BYTE*) uvalue.c_str(), (DWORD) (uvalue.size() + 1)*sizeof(wchar_t)) != ERROR_SUCCESS)
		handleSetError(name);
}


std::string WinRegistryKey::getStringExpand(const std::string& name)
{
	open();
	DWORD type;
	DWORD size;
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegQueryValueExW(_hKey, uname.c_str(), NULL, &type, NULL, &size) != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ && type != REG_LINK))
		throw NotFoundException(key(name));
	if (size > 0)
	{
		DWORD len = size/2;
		Poco::Buffer<wchar_t> buffer(len + 1);
		RegQueryValueExW(_hKey, uname.c_str(), NULL, NULL, (BYTE*) buffer.begin(), &size);
		buffer[len] = 0;

		wchar_t temp;
		DWORD expSize = ExpandEnvironmentStringsW(buffer.begin(), &temp, 1);
		Poco::Buffer<wchar_t> expBuffer(expSize);
		ExpandEnvironmentStringsW(buffer.begin(), expBuffer.begin(), expSize);
		std::string result;
		UnicodeConverter::toUTF8(expBuffer.begin(), result);

		return result;
	}
	return std::string();
}



void WinRegistryKey::setBinary( const std::string& name, const std::vector<char>& value )
{
	open();
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegSetValueExW(_hKey, uname.c_str(), 0, REG_BINARY, (CONST BYTE*) &value[0], (DWORD) value.size()) != ERROR_SUCCESS)
		handleSetError(name);
}


std::vector<char> WinRegistryKey::getBinary( const std::string& name )
{
	open();
	DWORD type;
	DWORD size;
	std::vector<char> result;

	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegQueryValueExW(_hKey, uname.c_str(), NULL, &type, NULL, &size) != ERROR_SUCCESS || type != REG_BINARY)
		throw NotFoundException(key(name));
	if (size > 0)
	{
		result.resize(size);
		RegQueryValueExW(_hKey, uname.c_str(), NULL, NULL, (BYTE*) &result[0], &size);
	}
	return result;
}


void WinRegistryKey::setInt(const std::string& name, int value)
{
	open();
	DWORD data = value;
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegSetValueExW(_hKey, uname.c_str(), 0, REG_DWORD, (CONST BYTE*) &data, sizeof(data)) != ERROR_SUCCESS)
		handleSetError(name);
}


int WinRegistryKey::getInt(const std::string& name)
{
	open();
	DWORD type;
	DWORD data;
	DWORD size = sizeof(data);
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegQueryValueExW(_hKey, uname.c_str(), NULL, &type, (BYTE*) &data, &size) != ERROR_SUCCESS || (type != REG_DWORD && type != REG_DWORD_BIG_ENDIAN))
		throw NotFoundException(key(name));
	return data;
}


#if defined(POCO_HAVE_INT64)


void WinRegistryKey::setInt64(const std::string& name, Poco::Int64 value)
{
	open();
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegSetValueExW(_hKey, uname.c_str(), 0, REG_QWORD, (CONST BYTE*) &value, sizeof(value)) != ERROR_SUCCESS)
		handleSetError(name);
}

Poco::Int64 WinRegistryKey::getInt64(const std::string& name)
{
	open();
	DWORD type;
	Poco::Int64 data;
	DWORD size = sizeof(data);
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegQueryValueExW(_hKey, uname.c_str(), NULL, &type, (BYTE*) &data, &size) != ERROR_SUCCESS || type != REG_QWORD)
		throw NotFoundException(key(name));
	return data;
}


#endif // POCO_HAVE_INT64


void WinRegistryKey::deleteValue(const std::string& name)
{
	open();
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegDeleteValueW(_hKey, uname.c_str()) != ERROR_SUCCESS)
		throw NotFoundException(key(name));
}


void WinRegistryKey::deleteKey()
{
	Keys keys;
	subKeys(keys);
	close();
	for (const auto& k: keys)
	{
		std::string subKey(_subKey);
		subKey += "\\";
		subKey += k;
		WinRegistryKey subRegKey(_hRootKey, subKey, _readOnly, _extraSam);
		subRegKey.deleteKey();
	}

	// NOTE: RegDeleteKeyEx is only available on Windows XP 64-bit SP3, Windows Vista or later.
	// We cannot call it directly as this would prevent the code running on Windows XP 32-bit.
	// Therefore, if we need to call RegDeleteKeyEx (_extraSam != 0) we need to check for
	// its existence in ADVAPI32.DLL and call it indirectly.
	std::wstring usubKey;
	Poco::UnicodeConverter::toUTF16(_subKey, usubKey);

	typedef LONG (WINAPI *RegDeleteKeyExWFunc)(HKEY hKey, const wchar_t* lpSubKey, REGSAM samDesired, DWORD Reserved);
	if (_extraSam != 0)
	{
		AutoHandle advAPI32(LoadLibraryW(L"ADVAPI32.DLL"));
		if (advAPI32.handle())
		{
			RegDeleteKeyExWFunc pRegDeleteKeyExW = reinterpret_cast<RegDeleteKeyExWFunc>(GetProcAddress(advAPI32.handle() , "RegDeleteKeyExW"));
			if (pRegDeleteKeyExW)
			{
				if ((*pRegDeleteKeyExW)(_hRootKey, usubKey.c_str(), _extraSam, 0) != ERROR_SUCCESS)
					throw NotFoundException(key());
				return;
			}
		}
	}

	if (RegDeleteKeyW(_hRootKey, usubKey.c_str()) != ERROR_SUCCESS)
		throw NotFoundException(key());
}


bool WinRegistryKey::exists()
{
	HKEY hKey;
	std::wstring usubKey;
	Poco::UnicodeConverter::toUTF16(_subKey, usubKey);
	if (RegOpenKeyExW(_hRootKey, usubKey.c_str(), 0, KEY_READ | _extraSam, &hKey) == ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return true;
	}
	return false;
}


WinRegistryKey::Type WinRegistryKey::type(const std::string& name)
{
	open();
	DWORD type = REG_NONE;
	DWORD size;
	std::wstring uname;
	Poco::UnicodeConverter::toUTF16(name, uname);
	if (RegQueryValueExW(_hKey, uname.c_str(), NULL, &type, NULL, &size) != ERROR_SUCCESS)
		throw NotFoundException(key(name));

	Type aType = (Type)type;
	return aType;
}


bool WinRegistryKey::exists(const std::string& name)
{
	bool exists = false;
	HKEY hKey;
	std::wstring usubKey;
	Poco::UnicodeConverter::toUTF16(_subKey, usubKey);
	if (RegOpenKeyExW(_hRootKey, usubKey.c_str(), 0, KEY_READ | _extraSam, &hKey) == ERROR_SUCCESS)
	{
		std::wstring uname;
		Poco::UnicodeConverter::toUTF16(name, uname);
		exists = RegQueryValueExW(hKey, uname.c_str(), NULL, NULL, NULL, NULL) == ERROR_SUCCESS;
		RegCloseKey(hKey);
	}
	return exists;
}


void WinRegistryKey::open()
{
	if (!_hKey)
	{
		std::wstring usubKey;
		Poco::UnicodeConverter::toUTF16(_subKey, usubKey);
		if (_readOnly)
		{
			if (RegOpenKeyExW(_hRootKey, usubKey.c_str(), 0, KEY_READ | _extraSam, &_hKey) != ERROR_SUCCESS)
				throw NotFoundException("Cannot open registry key: ", key());
		}
		else
		{
			if (RegCreateKeyExW(_hRootKey, usubKey.c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE | _extraSam, NULL, &_hKey, NULL) != ERROR_SUCCESS)
				throw SystemException("Cannot open registry key: ", key());
		}
	}
}


void WinRegistryKey::close()
{
	if (_hKey)
	{
		RegCloseKey(_hKey);
		_hKey = 0;
	}
}


std::string WinRegistryKey::key() const
{
	std::string result;
	if (_hRootKey == HKEY_CLASSES_ROOT)
		result = "HKEY_CLASSES_ROOT";
#if defined(HKEY_CURRENT_CONFIG)
	else if (_hRootKey == HKEY_CURRENT_CONFIG)
		result = "HKEY_CURRENT_CONFIG";
#endif
	else if (_hRootKey == HKEY_CURRENT_USER)
		result = "HKEY_CURRENT_USER";
	else if (_hRootKey == HKEY_LOCAL_MACHINE)
		result = "HKEY_LOCAL_MACHINE";
	else if (_hRootKey == HKEY_USERS)
		result = "HKEY_USERS";
#if defined(HKEY_PERFORMANCE_DATA)
	else if (_hRootKey == HKEY_PERFORMANCE_DATA)
		result = "HKEY_PERFORMANCE_DATA";
#endif
	else
		result = "(UNKNOWN)";
	result += '\\';
	result += _subKey;
	return result;
}


std::string WinRegistryKey::key(const std::string& valueName) const
{
	std::string result = key();
	if (!valueName.empty())
	{
		result += '\\';
		result += valueName;
	}
	return result;
}


HKEY WinRegistryKey::handle()
{
	open();
	return _hKey;
}


HKEY WinRegistryKey::handleFor(const std::string& rootKey)
{
	if (rootKey == "HKEY_CLASSES_ROOT")
		return HKEY_CLASSES_ROOT;
#if defined(HKEY_CURRENT_CONFIG)
	else if (rootKey == "HKEY_CURRENT_CONFIG")
		return HKEY_CURRENT_CONFIG;
#endif
	else if (rootKey == "HKEY_CURRENT_USER")
		return HKEY_CURRENT_USER;
	else if (rootKey == "HKEY_LOCAL_MACHINE")
		return HKEY_LOCAL_MACHINE;
	else if (rootKey == "HKEY_USERS")
		return HKEY_USERS;
#if defined(HKEY_PERFORMANCE_DATA)
	else if (rootKey == "HKEY_PERFORMANCE_DATA")
		return HKEY_PERFORMANCE_DATA;
#endif
	else
		throw InvalidArgumentException("Not a valid root key", rootKey);
}


void WinRegistryKey::handleSetError(const std::string& name)
{
	std::string msg = "Failed to set registry value";
	throw SystemException(msg, key(name));
}


void WinRegistryKey::subKeys(WinRegistryKey::Keys& keys)
{
	open();

	DWORD subKeyCount = 0;
	DWORD valueCount = 0;

	if (RegQueryInfoKey(_hKey, NULL, NULL, NULL, &subKeyCount, NULL, NULL, &valueCount, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
		return;

	wchar_t buf[256];
	DWORD bufSize = sizeof(buf)/sizeof(wchar_t);
	for (DWORD i = 0; i< subKeyCount; ++i)
	{
		if (RegEnumKeyExW(_hKey, i, buf, &bufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			std::wstring uname(buf);
			std::string name;
			Poco::UnicodeConverter::toUTF8(uname, name);
			keys.push_back(name);
		}
		bufSize = sizeof(buf)/sizeof(wchar_t);
	}
}


void WinRegistryKey::values(WinRegistryKey::Values& vals)
{
	open();

	DWORD valueCount = 0;

	if (RegQueryInfoKey(_hKey, NULL, NULL, NULL, NULL, NULL, NULL, &valueCount, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
		return ;

	wchar_t buf[256];
	DWORD bufSize = sizeof(buf)/sizeof(wchar_t);
	for (DWORD i = 0; i< valueCount; ++i)
	{
		if (RegEnumValueW(_hKey, i, buf, &bufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			std::wstring uname(buf);
			std::string name;
			Poco::UnicodeConverter::toUTF8(uname, name);
			vals.push_back(name);
		}
		bufSize = sizeof(buf)/sizeof(wchar_t);
	}
}


} } // namespace Poco::Util
