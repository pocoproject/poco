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
	///
	/// A standard stream that is closed for the child process is cut off
	/// from that of the parent process. On POSIX platforms its descriptor
	/// does not stay free: it is attached to the null device, so that the
	/// first file or socket the child process opens does not become its
	/// standard input, output or error. Reading from a closed STDIN gives
	/// end of file, what is written to a closed STDOUT or STDERR is discarded.
{
	PROCESS_CLOSE_STDIN = 1,
		/// Causes the child process STDIN to be closed.

	PROCESS_CLOSE_STDOUT = 2,
		/// Causes the child process STDOUT to be closed.

	PROCESS_CLOSE_STDERR = 4,
		/// Causes the child process STDERR to be closed.

	PROCESS_KILL_TREE = 8,
		/// Ensures that all child processes (and their descendants) are
		/// terminated when the ProcessRunner is stopped or destroyed.
		///
		/// On Unix/Linux, the child is placed in a new process group via
		/// setpgid(0, 0). On stop, SIGINT is sent to the entire process
		/// group. If the group does not exit before the timeout expires,
		/// SIGKILL is sent to the entire group.
		///
		/// On Windows, creates a Job Object with JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE.
		/// On stop, a graceful termination signal (NamedEvent) is sent to
		/// the leader process only. Non-Poco children in the tree receive
		/// no graceful signal — they are terminated when the Job Object
		/// handle is closed after the leader exits or the timeout expires.

	PROCESS_NEW_GROUP = 16,
		/// Starts the child process in a process group of its own, so that
		/// it does not get what the terminal or the console of the parent
		/// sends to the group of the parent, such as Ctrl+C, and so that
		/// Process::requestTermination(pid, options) can ask it to end the
		/// way console programs understand.
		///
		/// On Unix/Linux, the child is placed in a new process group via
		/// setpgid(0, 0). A termination request is SIGINT, as for any
		/// process.
		///
		/// On Windows, the child is created with CREATE_NEW_PROCESS_GROUP.
		/// It shares the console of the parent when the parent has one, and
		/// has a console of its own, without a window, otherwise. A
		/// termination request is a CTRL_BREAK_EVENT to its group, which
		/// console programs take as a request to end, as they take Ctrl+C.
		/// ProcessRunner::stop() sends it before the timeout.
};


/// Exit codes used by the child process to signal pre-exec failures
/// back to the parent. These are chosen to avoid collision with common
/// application exit codes.
static constexpr int PROCESS_EXIT_EXEC_FAILED    = 72;
	/// execvp() failed (command not found, permission denied, etc.)

static constexpr int PROCESS_EXIT_SETPGID_FAILED = 73;
	/// setpgid(0, 0) failed in child (PROCESS_KILL_TREE or PROCESS_NEW_GROUP).
	/// The specific errno is lost (child address space).


} // namespace Poco


#endif // Foundation_ProcessOptions_INCLUDED
