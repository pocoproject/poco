//
// ProcessOptions.h
//
// Library: Foundation
// Package: Processes
// Module:  ProcessOptions
//
// Definition of the ProcessOptions class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_ProcessOptions_INCLUDED
#define Foundation_ProcessOptions_INCLUDED


#include "Poco/Foundation.h"


namespace Poco {


enum ProcessOptions
	/// Options to configure child process behavior.
{
	PROCESS_CLOSE_STDIN = 1,
		/// Causes the child process STDIN to be closed.

	PROCESS_CLOSE_STDOUT = 2,
		/// Causes the child process STDOUT to be closed.

	PROCESS_CLOSE_STDERR = 4,
		/// Causes the child process STDERR to be closed.
};


} // namespace Poco


#endif // Foundation_ProcessOptions_INCLUDED
