module;

#define ENABLE_ZIP

#ifdef ENABLE_ZIP
#include "Poco/Zip/Add.h"
#include "Poco/Zip/AutoDetectStream.h"
#include "Poco/Zip/Compress.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/Delete.h"
#include "Poco/Zip/Keep.h"
#include "Poco/Zip/ParseCallback.h"
#include "Poco/Zip/PartialStream.h"
#include "Poco/Zip/Rename.h"
#include "Poco/Zip/Replace.h"
#include "Poco/Zip/SkipCallback.h"
#include "Poco/Zip/ZipArchive.h"
#include "Poco/Zip/ZipArchiveInfo.h"
#include "Poco/Zip/ZipCommon.h"
#include "Poco/Zip/ZipDataInfo.h"
#include "Poco/Zip/ZipException.h"
#include "Poco/Zip/ZipFileInfo.h"
#include "Poco/Zip/Zip.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "Poco/Zip/ZipManipulator.h"
#include "Poco/Zip/ZipOperation.h"
#include "Poco/Zip/ZipStream.h"
#include "Poco/Zip/ZipUtil.h"
#endif

export module Poco.Zip;

export namespace Poco::Zip {
	#ifdef ENABLE_ZIP
	using Poco::Zip::Add;
	using Poco::Zip::AutoDetectIOS;
	using Poco::Zip::AutoDetectInputStream;
	using Poco::Zip::AutoDetectStreamBuf;
	using Poco::Zip::Compress;
	using Poco::Zip::Decompress;
	using Poco::Zip::Delete;
	using Poco::Zip::Keep;
	using Poco::Zip::ParseCallback;
	using Poco::Zip::PartialIOS;
	using Poco::Zip::PartialInputStream;
	using Poco::Zip::PartialOutputStream;
	using Poco::Zip::PartialStreamBuf;
	using Poco::Zip::Rename;
	using Poco::Zip::Replace;
	using Poco::Zip::SkipCallback;
	using Poco::Zip::ZipArchive;
	using Poco::Zip::ZipArchiveInfo;
	using Poco::Zip::ZipArchiveInfo64;
	using Poco::Zip::ZipCommon;
	using Poco::Zip::ZipDataInfo;
	using Poco::Zip::ZipDataInfo64;
	using Poco::Zip::ZipException;
	using Poco::Zip::ZipFileInfo;
	using Poco::Zip::ZipIOS;
	using Poco::Zip::ZipInputStream;
	using Poco::Zip::ZipLocalFileHeader;
	using Poco::Zip::ZipManipulationException;
	using Poco::Zip::ZipManipulator;
	using Poco::Zip::ZipOperation;
	using Poco::Zip::ZipOutputStream;
	using Poco::Zip::ZipStreamBuf;
	using Poco::Zip::ZipUtil;
	#endif
}
