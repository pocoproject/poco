//
// SevenZip.cppm
//
// C++ module file
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

module;

#ifdef ENABLE_SEVENZIP
#include "Poco/SevenZip/Archive.h"
#include "Poco/SevenZip/ArchiveEntry.h"
#include "Poco/SevenZip/SevenZip.h"
#endif

export module Poco.SevenZip;

export namespace Poco::SevenZip {
	#ifdef ENABLE_SEVENZIP
	using Poco::SevenZip::Archive;
	using Poco::SevenZip::ArchiveEntry;
	using Poco::SevenZip::ArchiveImpl;
	#endif
}
