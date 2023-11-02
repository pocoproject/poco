//
// PIDFile.cpp
//
// Library: Foundation
// Package: Processes
// Module:  PIDFile
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:    BSL-1.0
//


#include "Poco/PIDFile.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Process.h"
#include "Poco/FileStream.h"
#include <fstream>


using Poco::Path;
using Poco::File;
using Poco::Process;
using Poco::FileInputStream;
using Poco::FileOutputStream;


namespace Poco {


PIDFile::PIDFile()
{
}


PIDFile::PIDFile(const std::string& fileName, bool write):
	_fileName(fileName)
{
	if (write) create();
}


PIDFile::~PIDFile()
{
	destroy();
}


void PIDFile::setName(const std::string& fileName)
{
	destroy();
	_fileName = fileName;
	create();
}


void PIDFile::create()
{
	if (!_fileName.empty())
	{
		Path p(getFileName(_fileName));
		if (!File(p.makeParent()).exists())
			File(p).createDirectories();
		_pid = static_cast<int>(Process::id());
		FileOutputStream fos(_fileName);
		fos << _pid; fos.close();
	}
}


void PIDFile::destroy()
{
	if (!_fileName.empty())
	{
		File f(_fileName);
		if (f.exists()) f.remove();
		_fileName.clear();
	}
	_pid = INVALID_PID;
}


bool PIDFile::exists() const
{
	if (File(_fileName).exists())
	{
		FileInputStream fis(_fileName);
		int fPID = 0;
		if (fis.peek() != std::ifstream::traits_type::eof())
			fis >> fPID;
		return fPID == _pid;
	}
	return false;
}


bool PIDFile::contains(const std::string& fileName, int pid)
{
	if (File(fileName).exists())
	{
		FileInputStream fis(fileName);
		int fPID = 0;
		if (fis.peek() != std::ifstream::traits_type::eof())
			fis >> fPID;
		return fPID == pid;
	}
	return false;
}


std::string& PIDFile::getFileName(std::string& pidFile)
{
	Path p(pidFile);
	pidFile = p.makeAbsolute().toString();
	return pidFile;
}


} // namespace Poco
