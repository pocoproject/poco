//
// Cancellation.h
//
// Library: AI
// Package: Core
// Module:  Cancellation
//
// Definition of the Cancellation class.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef AI_Cancellation_INCLUDED
#define AI_Cancellation_INCLUDED


#include "Poco/AI/AI.h"
#include "Poco/Mutex.h"
#include <atomic>
#include <functional>
#include <map>


namespace Poco::AI {


class AI_API Cancellation
	/// A cancellation token for one model call or one agent run, which another
	/// thread may cancel. Every call gets its own token, so cancelling one call
	/// never touches another that shares the provider.
	///
	/// An operation that blocks registers an abort with a Request for as long
	/// as it runs; cancel() sets the flag and calls every abort that is
	/// registered, e.g. those of the model call and of the tool that run at
	/// the same time. Thread-safe.
	///
	/// An abort must not throw: cancel() passes over what one throws, so that
	/// the others still run, and a Request whose abort throws in its
	/// constructor unregisters it before the exception leaves.
{
public:
	class AI_API Request
		/// Registers an abort with a token for the lifetime of the object.
	{
	public:
		POCO_NODISCARD_CTOR Request(Cancellation& cancellation, std::function<void()> abort);
			/// Registers abort. If the token is cancelled already, calls it at once.

		~Request();
			/// Unregisters the abort. Once the destructor has returned, the abort
			/// neither runs nor will be called.

	private:
		Request(const Request&) = delete;
		Request& operator = (const Request&) = delete;

		Cancellation& _cancellation;
		Poco::UInt64 _id;
	};

	Cancellation();
		/// Creates a token that is not cancelled.

	~Cancellation();
		/// Destroys the token.

	void cancel();
		/// Cancels: sets the flag and calls the aborts that are registered.
		/// May be called more than once, from any thread.

	[[nodiscard]] bool cancelled() const;
		/// Returns true once cancel() has been called.

private:
	Cancellation(const Cancellation&) = delete;
	Cancellation& operator = (const Cancellation&) = delete;

	mutable Poco::FastMutex _mutex;
	std::atomic<bool> _cancelled{false};
	std::map<Poco::UInt64, std::function<void()>> _aborts;
	Poco::UInt64 _nextId = 0;
};


//
// inlines
//
inline bool Cancellation::cancelled() const
{
	return _cancelled.load();
}


} // namespace Poco::AI


#endif // AI_Cancellation_INCLUDED
