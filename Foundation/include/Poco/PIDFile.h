//
// PIDFile.h
//
// Library: Foundation
// Package: Processes
// Module:  PIDFile
//
// Definition of the PIDFile class.
//
// Copyright (c) 2023, Applied Informatics Software Engineering GmbH.
// Aleph ONE Software Engineering d.o.o.,
// and Contributors.
//
// SPDX-License-Identifier:    BSL-1.0
//


#ifndef Foundation_PIDFile_INCLUDED
#define Foundation_PIDFile_INCLUDED


#include "Poco/Foundation.h"
#include <memory>


namespace Poco {


class Foundation_API PIDFile
	/// A utility class, creating process ID file on
	/// construction and deleting it on destruction.
{
public:
	using Ptr = std::unique_ptr<PIDFile>;

	static const int INVALID_PID = -1;

	PIDFile();
		/// Creates the PIDFile.

	PIDFile(const std::string& fileName, bool write = true);
		/// Creates the PIDFile.
		/// If `fileName` is not empty, creates the PID file.
		/// If `write` is true, the file is written.

	~PIDFile();
		/// Destroys the PIDFile.
		/// If fileName is not empty, deletes the PID file.

	const std::string& getName() const;
		/// Returns the file name.

	void setName(const std::string& fileName);
		/// Sets the file name.

	void create();
		/// Creates the file and writes PID into it.

	void destroy();
		/// Deletes the PID file and invalidates the held PID.

	int getPID() const;
		/// Returns the PID.

	bool exists() const;
		/// Returns true if PID file exists and its content is
		/// equal to the held PID.

	static bool contains(const std::string& fileName, int pid);
		/// Returns true if the `fileName` contains the given `pid`.

	static std::string& getFileName(std::string& pidFile);
		/// Returns the file name.

private:

	std::string _fileName;
	int _pid = INVALID_PID;
};


//
// inlines
//

inline const std::string& PIDFile::getName() const
{
	return _fileName;
}


inline int PIDFile::getPID() const
{
	return _pid;
}


} // namespace Poco


#endif // Foundation_ProcessRunner_INCLUDED
