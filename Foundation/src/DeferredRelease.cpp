//
// DeferredRelease.cpp
//
// Library: Foundation
// Package: Core
// Module:  DeferredRelease
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DeferredRelease.h"
#include "Poco/Types.h"
#include "Poco/Bugcheck.h"
#include <atomic>
#include <cstddef>
#include <forward_list>
#include <limits>
#include <list>
#include <mutex>
#include <new>


namespace Poco {


namespace
{
	// What every thread reads and hardly any writes is kept apart from
	// what a thread writes for every reader: threads that read do not
	// share a cache line that one of them writes.
	constexpr std::size_t CACHE_LINE = 128;


	struct alignas(CACHE_LINE) ThreadRecord
		/// What a thread tells the others: whether it is reading, and
		/// since when. Only its thread writes it.
	{
		std::atomic<Poco::UInt64> reading{0};
			/// 0, or the epoch at which the outermost reader of the
			/// thread began.

		bool taken = false;
			/// A thread has the record. Guarded by the mutex of the domain.
	};


	struct alignas(CACHE_LINE) Shared
	{
		std::atomic<Poco::UInt64> epoch{1};
			/// Counts the times objects were handed over for release.

		std::atomic<std::size_t> pending{0};
			/// The number of objects that wait for their readers to go.
	};


	Shared shared;
		/// Has no destructor to speak of, so that a thread may read
		/// while the static objects of the process are destroyed.


	class Domain
		/// The records of the threads and the objects that wait for
		/// their release.
	{
	public:
		static Domain& instance()
		{
			// Never destroyed, for the same reason: it is built in
			// storage that outlives every thread.
			alignas(Domain) static unsigned char storage[sizeof(Domain)];
			static Domain* const pDomain = ::new (static_cast<void*>(storage)) Domain;
			return *pDomain;
		}

		ThreadRecord& takeRecord()
			/// Returns a record for the calling thread: one that a thread
			/// has given back, or a new one. Records are never destroyed.
		{
			std::lock_guard<std::mutex> lock(_mutex);
			for (auto& record: _records)
			{
				if (!record.taken)
				{
					record.taken = true;
					return record;
				}
			}
			_records.emplace_front();
			_records.front().taken = true;
			return _records.front();
		}

		void giveBack(ThreadRecord& record)
		{
			std::lock_guard<std::mutex> lock(_mutex);
			record.taken = false;
		}

		void retire(DeferredRelease::Ptr* pObjects, std::size_t count)
			/// Takes the objects, and leaves null pointers in their place.
		{
			// Readers that began at this epoch or before may use the
			// objects; those that begin from now on cannot have loaded them.
			const Poco::UInt64 stamp = shared.epoch.fetch_add(1);

			std::lock_guard<std::mutex> lock(_mutex);
			for (std::size_t i = 0; i < count; ++i)
			{
				if (!pObjects[i]) continue;
				try
				{
					_retired.emplace_back();
				}
				catch (...)
				{
					// There is no room to note the object. It is left alone
					// for good rather than released under a reader.
					(void) pObjects[i].duplicate();
					pObjects[i].reset();
					continue;
				}
				_retired.back().pObject.swap(pObjects[i]);
				_retired.back().stamp = stamp;
			}
			shared.pending.store(_retired.size());
		}

		void reclaim()
			/// Releases the objects that no reader can use any more.
		{
			std::list<Retired> released;
			{
				std::lock_guard<std::mutex> lock(_mutex);
				if (_retired.empty()) return;

				Poco::UInt64 oldest = std::numeric_limits<Poco::UInt64>::max();
				for (const auto& record: _records)
				{
					const Poco::UInt64 reading = record.reading.load();
					if (reading != 0 && reading < oldest) oldest = reading;
				}
				// an object is kept for a reader that began at its epoch or before
				for (auto it = _retired.begin(); it != _retired.end();)
				{
					if (it->stamp < oldest)
						released.splice(released.end(), _retired, it++);
					else
						++it;
				}
				shared.pending.store(_retired.size());
			}
			// The objects are released here, with the mutex free again:
			// a destructor may read, or hand over an object of its own.
		}

	private:
		struct Retired
		{
			DeferredRelease::Ptr pObject;
			Poco::UInt64 stamp = 0;
				/// The epoch at which the object was handed over.
		};

		Domain() = default;

		std::mutex _mutex;
		std::forward_list<ThreadRecord> _records;
		std::list<Retired> _retired;
	};
}


class DeferredRelease::Reader::ThreadState
	/// What a thread keeps for itself.
{
public:
	static ThreadState& current()
		/// Returns the state of the calling thread. It needs no constructor
		/// and no destructor: it is there for a reader in the destructor
		/// of any other object of the thread.
	{
		static thread_local ThreadState state;
		return state;
	}

	void enter()
		/// A reader of the thread begins.
	{
		if (_depth == 0)
		{
			if (!_pRecord) takeRecord();
			_began = shared.epoch.load();
			// Stored before the reader loads a pointer, and seen for that by
			// whoever replaces the pointer and then looks for readers.
			_pRecord->reading.store(_began);
		}
		++_depth;
	}

	void leave() noexcept
		/// A reader of the thread is gone.
	{
		if (--_depth == 0)
		{
			_pRecord->reading.store(0);
			// An object that was handed over since this reader began may
			// have waited for it. A reader that began later is not what
			// an object waits for, and leaves the mutex alone.
			const bool awaited = shared.pending.load() != 0 && _began < shared.epoch.load();
			if (awaited || _ended) leaveWithWork(awaited);
		}
	}

private:
	void takeRecord()
	{
		_pRecord = &Domain::instance().takeRecord();
		if (!_ended)
		{
			struct AtThreadEnd
			{
				~AtThreadEnd()
				{
					current().threadEnds();
				}
			};
			// Constructed with the first reader of a thread, and so
			// destroyed when the thread ends.
			thread_local AtThreadEnd atThreadEnd;
			(void) atThreadEnd;
		}
	}

	void giveRecordBack() noexcept
	{
		ThreadRecord& record = *_pRecord;
		_pRecord = nullptr;
		try
		{
			Domain::instance().giveBack(record);
		}
		catch (...)
		{
			poco_unexpected();
		}
	}

	void threadEnds() noexcept
	{
		_ended = true;
		if (_depth == 0 && _pRecord) giveRecordBack();
	}

	void leaveWithWork(bool awaited) noexcept
	{
		// A reader in a thread that is ending: nobody would give the
		// record back later.
		if (_ended) giveRecordBack();
		if (awaited)
		{
			try
			{
				Domain::instance().reclaim();
			}
			catch (...)
			{
				poco_unexpected();
			}
		}
	}

	ThreadRecord* _pRecord = nullptr;
		/// The record of the thread in the domain, which owns it.

	Poco::UInt64 _began = 0;
		/// The epoch at which the outermost reader of the thread began.

	unsigned _depth = 0;
		/// The readers of the thread, one within the other.

	bool _ended = false;
		/// The thread is ending, and has given its record back.
};


DeferredRelease::Reader::Reader():
	_thread(ThreadState::current())
{
	_thread.enter();
}


DeferredRelease::Reader::~Reader()
{
	_thread.leave();
}


void DeferredRelease::release(Ptr pObject)
{
	if (!pObject) return;

	Domain& domain = Domain::instance();
	domain.retire(&pObject, 1);
	domain.reclaim();
}


void DeferredRelease::release(std::vector<Ptr>& objects)
{
	if (objects.empty()) return;

	Domain& domain = Domain::instance();
	domain.retire(objects.data(), objects.size());
	objects.clear();
	domain.reclaim();
}


} // namespace Poco
