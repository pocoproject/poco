//
// DirectoryIterator.cpp
//
// Library: Foundation
// Package: Filesystem
// Module:  DirectoryIterator
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DirectoryIterator.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "DirectoryIterator_WIN32.cpp"
#elif defined(POCO_OS_FAMILY_UNIX)
#include "DirectoryIterator_UNIX.cpp"
#endif


namespace Poco {


DirectoryIterator::DirectoryIterator(): _pImpl(0)
{
}


DirectoryIterator::DirectoryIterator(const std::string& pathString): _path(pathString), _pImpl(new DirectoryIteratorImpl(pathString))
{
	_path.makeDirectory();
	_path.setFileName(_pImpl->get());
	_file = _path;
}


DirectoryIterator::DirectoryIterator(const DirectoryIterator& iterator): _path(iterator._path), _pImpl(iterator._pImpl)
{
	if (_pImpl)
	{
		_pImpl->duplicate();
		_file = _path;
	}
}


DirectoryIterator::DirectoryIterator(const File& file): _path(file.path()), _pImpl(new DirectoryIteratorImpl(file.path()))
{
	_path.makeDirectory();
	_path.setFileName(_pImpl->get());
	_file = _path;
}


DirectoryIterator::DirectoryIterator(const Path& otherPath): _path(otherPath), _pImpl(new DirectoryIteratorImpl(otherPath.toString()))
{
	_path.makeDirectory();
	_path.setFileName(_pImpl->get());
	_file = _path;
}


DirectoryIterator::~DirectoryIterator()
{
	if (_pImpl) _pImpl->release();
}


DirectoryIterator& DirectoryIterator::operator = (const DirectoryIterator& it)
{
	if (_pImpl) _pImpl->release();
	_pImpl = it._pImpl;
	if (_pImpl)
	{
		_pImpl->duplicate();
		_path = it._path;
		_file = _path;
	}
	return *this;
}


DirectoryIterator& DirectoryIterator::operator = (const File& file)
{
	if (_pImpl) _pImpl->release();
	_pImpl = new DirectoryIteratorImpl(file.path());
	_path.parseDirectory(file.path());
	_path.setFileName(_pImpl->get());
	_file = _path;
	return *this;
}


DirectoryIterator& DirectoryIterator::operator = (const Path& otherPath)
{
	if (_pImpl) _pImpl->release();
	_pImpl = new DirectoryIteratorImpl(otherPath.toString());
	_path = otherPath;
	_path.makeDirectory();
	_path.setFileName(_pImpl->get());
	_file = _path;
	return *this;
}


DirectoryIterator& DirectoryIterator::operator = (const std::string& pathString)
{
	if (_pImpl) _pImpl->release();
	_pImpl = new DirectoryIteratorImpl(pathString);
	_path.parseDirectory(pathString);
	_path.setFileName(_pImpl->get());
	_file = _path;
	return *this;
}


DirectoryIterator& DirectoryIterator::operator ++ ()
{
	if (_pImpl)
	{
		_path.setFileName(_pImpl->next());
		_file = _path;
	}
	return *this;
}


DirectoryIterator DirectoryIterator::operator ++ (int /*dummy*/)
{
	if (_pImpl)
	{
		_path.setFileName(_pImpl->next());
		_file = _path;
	}
	return *this;
}


} // namespace Poco
