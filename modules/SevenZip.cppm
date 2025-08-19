module;

#define ENABLE_SEVENZIP
#include "Poco/SevenZip/Archive.h"
#include "Poco/SevenZip/ArchiveEntry.h"
#include "Poco/SevenZip/SevenZip.h"
#ifdef ENABLE_SEVENZIP

#endif

export module Poco.SevenZip;

export namespace Poco::SevenZip {
	#ifdef ENABLE_SEVENZIP
	using Poco::SevenZip::Archive;
	using Poco::SevenZip::ArchiveEntry;
	using Poco::SevenZip::ArchiveImpl;
	#endif
}
