//
// Keep.h
//
// Library: Zip
// Package: Manipulation
// Module:  Keep
//
// Definition of the Keep class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Zip_Keep_INCLUDED
#define Zip_Keep_INCLUDED


#include "Poco/Zip/Zip.h"
#include "Poco/Zip/ZipOperation.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipFileInfo.h"
#include <optional>


namespace Poco::Zip {


class Zip_API Keep: public ZipOperation
	/// Keep simply forwards the compressed data stream from the input ZipArchive
	/// to the output zip archive
{
public:
	Keep(const ZipLocalFileHeader& hdr);
		/// Creates the Keep object.

	Keep(const ZipLocalFileHeader& hdr, const ZipFileInfo& info);
		/// Creates the Keep object. The copied entry gets the
		/// attributes of the directory entry info.

	void execute(Compress& c, std::istream& input);
		///Adds a copy of the compressed input file to the ZipArchive

private:
	const ZipLocalFileHeader _hdr;
	const std::optional<ZipFileInfo> _info;
};


} // namespace Poco::Zip


#endif // Zip_Keep_INCLUDED
