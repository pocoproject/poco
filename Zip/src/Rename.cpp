//
// Rename.cpp
//
// Library: Zip
// Package: Manipulation
// Module:  Rename
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <utility>

#include "Poco/Zip/Rename.h"
#include "Poco/Zip/Compress.h"


namespace Poco {
namespace Zip {


Rename::Rename(const ZipLocalFileHeader& hdr, std::string  newZipEntryName):
	_hdr(hdr),
	_newZipEntryName(std::move(newZipEntryName))
{
}


void Rename::execute(Compress& c, std::istream& input)
{
	c.addFileRaw(input, _hdr, _newZipEntryName);
}


} } // namespace Poco::Zip
