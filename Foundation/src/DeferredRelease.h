//
// DeferredRelease.h
//
// Library: Foundation
// Package: Core
// Module:  DeferredRelease
//
// Definition of the DeferredRelease class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_DeferredRelease_INCLUDED
#define Foundation_DeferredRelease_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include <vector>


namespace Poco {


class DeferredRelease
	/// Releases reference counted objects that readers on other threads
	/// may still use, once no such reader is left.
	///
	/// It is for a pointer that many threads read all the time and that
	/// is replaced rarely, like the channel of a Logger. Counting a
	/// reference for every use has all the readers write the same count,
	/// and guarding the pointer with a mutex has them wait for one
	/// another.
	///
	/// A reader is a Reader object for as long as it uses what it reads:
	/// it notes, in a record of its own thread, that the thread is
	/// reading, then loads the pointer and uses the object without
	/// counting a reference. Who replaces the pointer hands the object
	/// that was replaced to release(). It is released at once if no thread
	/// is reading, and otherwise as soon as every reader that was there at
	/// that moment is gone: a reader that came later cannot have loaded
	/// it.
	///
	/// The pointer that readers load must be a std::atomic. A reader
	/// loads it after its Reader was created, and who replaces it stores
	/// the new value before the object that it pointed to is handed to
	/// release(), both with the default memory order.
	///
	/// This class is internal to the Foundation library.
{
public:
	using Ptr = AutoPtr<RefCountedObject>;

	class Reader
		/// Marks its thread as reading for as long as it lives.
		/// The readers of a thread may be within one another.
	{
	public:
		Reader();
		~Reader();

		Reader(const Reader&) = delete;
		Reader& operator = (const Reader&) = delete;

	private:
		class ThreadState;

		ThreadState& _thread;
	};

	static void release(Ptr pObject);
		/// Releases the object when no reader can use it any more.
		/// Does nothing for a null pointer.

	static void release(std::vector<Ptr>& objects);
		/// Releases the objects when no reader can use them any more,
		/// and leaves the vector empty.

	DeferredRelease() = delete;
};


} // namespace Poco


#endif // Foundation_DeferredRelease_INCLUDED
