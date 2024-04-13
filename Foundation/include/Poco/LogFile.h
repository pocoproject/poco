//
// LogFile.h
//
// Library: Foundation
// Package: Logging
// Module:  LogFile
//
// Definition of the LogFile class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_LogFile_INCLUDED
#define Foundation_LogFile_INCLUDED

#include "Poco/Foundation.h"
#include "Poco/Timestamp.h"
#include "Poco/FileStream.h"

namespace Poco {


class Foundation_API LogFile
	/// This class is used by FileChannel to work
	/// with a log file.
{
public:
	LogFile(const std::string& path);
		/// Creates the LogFile.

	~LogFile();
		/// Destroys the LogFile.

	void write(const std::string& text, bool flush = true);
		/// Writes the given text to the log file.
		/// If flush is true, the text will be immediately
		/// flushed to the file.

	UInt64 size() const;
		/// Returns the current size in bytes of the log file.

	Timestamp creationDate() const;
		/// Returns the date and time the log file was created.

	const std::string& path() const;
		/// Returns the path given in the constructor.

private:
	std::string _path;
	mutable Poco::FileOutputStream _str;
	Timestamp _creationDate;
	UInt64 _size;
};


} // namespace Poco


#endif // Foundation_LogFile_INCLUDED
