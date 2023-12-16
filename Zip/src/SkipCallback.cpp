//
// SkipCallback.cpp
//
// Library: Zip
// Package: Zip
// Module:  SkipCallback
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Zip/SkipCallback.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipUtil.h"
#include "Poco/Exception.h"


namespace Poco {
namespace Zip {


SkipCallback::SkipCallback()
= default;


SkipCallback::~SkipCallback()
= default;


bool SkipCallback::handleZipEntry(std::istream& zipStream, const ZipLocalFileHeader& hdr)
{
	if (!hdr.searchCRCAndSizesAfterData())
		zipStream.seekg(hdr.getCompressedSize(), std::ios_base::cur);
	else
		ZipUtil::syncDataDescriptor(zipStream, hdr.needsZip64());
	if (!zipStream.good()) throw Poco::IOException("Failed to seek on input stream");
	return true;
}


} } // namespace Poco::Zip
