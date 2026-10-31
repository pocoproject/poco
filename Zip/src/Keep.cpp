//
// Keep.cpp
//
// Library: Zip
// Package: Manipulation
// Module:  Keep
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Zip/Keep.h"
#include "Poco/Zip/Compress.h"
#include "Poco/Buffer.h"
#include "Poco/StreamCopier.h"


namespace Poco::Zip {


Keep::Keep(const ZipLocalFileHeader& hdr):
	_hdr(hdr)
{
}


Keep::Keep(const ZipLocalFileHeader& hdr, const ZipFileInfo& info):
	_hdr(hdr),
	_info(info)
{
}


void Keep::execute(Compress& c, std::istream& input)
{
	c.addFileRaw(input, _hdr, _hdr.getFileName(), _info);
}


} // namespace Poco::Zip
