//
// SocketProactor.h
//
// Library: Net
// Package: Sockets
// Module:  SocketProactor
//
// Definition of the SocketProactor class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Net_SocketProactor_INCLUDED
#define Net_SocketProactor_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/Socket.h"
#include "Poco/Net/PollSet.h"
#include "Poco/Runnable.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/AutoPtr.h"
#include "Poco/Mutex.h"
#include "Poco/Activity.h"
#include "Poco/NotificationQueue.h"
#include "Poco/ErrorHandler.h"
#include <unordered_map>
#include <atomic>
#include <functional>
#include <deque>
#include <utility>
#include <memory>
#include <iostream>
#include <system_error>


namespace Poco {


class Thread;


namespace Net {


class Socket;
class Worker;


class Net_API SocketProactor final: public Poco::Runnable
	/// This class implements the proactor pattern.
	/// It may also contain a simple work executor (enabled by default),
	/// which executes submitted workload.
{
public:
	using Buffer = std::vector<std::uint8_t>;
	using Work = std::function<void()>;
	using Callback = std::function<void (const std::error_code& failure, int bytesReceived)>;

	static const int POLL_READ = PollSet::POLL_READ;
	static const int POLL_WRITE = PollSet::POLL_WRITE;
	static const int POLL_ERROR = PollSet::POLL_ERROR;

	static const Timestamp::TimeDiff PERMANENT_COMPLETION_HANDLER;

	explicit SocketProactor(bool worker = true);
		/// Creates the SocketProactor.

	explicit SocketProactor(const Poco::Timespan& timeout, bool worker = true);
		/// Creates the SocketProactor, using the given timeout.

	SocketProactor(const SocketProactor&) = delete;
	SocketProactor(SocketProactor&&) = delete;
	SocketProactor& operator=(const SocketProactor&) = delete;
	SocketProactor& operator=(SocketProactor&&) = delete;

	~SocketProactor() override;
		/// Destroys the SocketProactor.

	void addWork(const Work& ch, Timestamp::TimeDiff ms = PERMANENT_COMPLETION_HANDLER);
		/// Adds work to be executed after the next poll() completion.
		/// Function will be called until the specified expiration,
		/// which defaults to immediately, ie. expiration after the
		/// first invocation.

	void addWork(Work&& ch, Timestamp::TimeDiff ms = PERMANENT_COMPLETION_HANDLER, int pos = -1);
		/// Adds work to be executed after the next poll() completion.
		/// Function will be called until the specified expiration,
		/// which defaults to immediately, ie. expiration after the
		/// first invocation.

	void removeWork();
		/// Removes all scheduled work.

	int scheduledWork();
		/// Returns the number of scheduled functions.

	int removeScheduledWork(int count = -1);
		/// Removes the count scheduled functions
		/// from the front of the schedule queue.
		/// Default is removal of all scheduled functions.

	int permanentWork();
		/// Returns the number of permanent functions.

	int removePermanentWork(int count = -1);
		/// Removes the count permanent functions
		/// from the front of the schedule queue.
		/// Default is removal of all functions.

	int poll(int* pHandled = nullptr);
		/// Polls all registered sockets and calls their respective handlers.
		/// If pHandled is not null, after the call it contains the total number
		/// of read/write/error socket handlers called.
		/// Returns the number of completion handlers invoked.

	int runOne();
		/// Runs one handler, scheduled or permanent.
		/// If there are no available handlers, it blocks
		/// until the first handler is encountered and executed.
		/// Returns 1 on successful handler invocation, 0 on
		/// exception.

	void run() override;
		/// Runs the SocketProactor. The reactor will run
		/// until stop() is called (in a separate thread).

	void stop();
		/// Stops the SocketProactor.
		///
		/// The proactor will be stopped when the next event
		/// (including a timeout event) occurs.

	void wakeUp();
		/// Wakes up idle reactor.

	void wait();
		/// Blocks and waits for the scheduled I/O completion
		/// handlers loop to end.

	void setTimeout(const Poco::Timespan& timeout);
		/// Sets the timeout. 
		///
		/// If no other event occurs for the given timeout 
		/// interval, a timeout event is sent to all event listeners.
		///
		/// The default timeout is 250 milliseconds;
		///
		/// The timeout is passed to the Socket::select()
		/// method.

	Poco::Timespan getTimeout() const;
		/// Returns the timeout.

	void addSocket(const Socket& sock, int mode);
		/// Adds the socket to the poll set.

	void updateSocket(const Socket& sock, int mode);
		/// Updates the socket mode in the poll set.

	void removeSocket(const Socket& sock);
		/// Removes the socket from the poll set.

	void addReceiveFrom(Socket sock, Buffer& buf, SocketAddress& addr, Callback&& onCompletion);
		/// Adds the datagram socket and the completion handler to the I/O receive queue.

	void addSendTo(Socket sock, const Buffer& message, const SocketAddress& addr, Callback&& onCompletion);
		/// Adds the datagram socket and the completion handler to the I/O send queue.

	void addSendTo(Socket sock, Buffer&& message, const SocketAddress&& addr, Callback&& onCompletion);
		/// Adds the datagram socket and the completion handler to the I/O send queue.

	void addSend(Socket sock, Buffer* pMessage, SocketAddress* pAddr, Callback&& onCompletion, bool own = false);
		/// Adds the socket and the completion handler to the I/O send queue.
		/// For stream socket, pAddr can be nullptr.
		/// If `own` is true, message and address are deleted after the I/O completion.

	void addReceive(Socket sock, Buffer& buf, Callback&& onCompletion);
		/// Adds the stream socket and the completion handler to the I/O receive queue.

	void addSend(Socket sock, const Buffer& message, Callback&& onCompletion);
		/// Adds the stream socket and the completion handler to the I/O send queue.

	void addSend(Socket sock, Buffer&& message, Callback&& onCompletion);
		/// Adds the stream socket and the completion handler to the I/O send queue.

	bool hasSocketHandlers() const;
		/// Returns true if proactor had at least one I/O completion handler.

	bool has(const Socket& sock) const;
		/// Returns true if socket is registered with this proactor.

	bool isRunning() const;
		/// Returns true if this proactor is running

	bool ioCompletionInProgress() const;
		/// Returns true if there are not executed handlers from last IO.

private:
	void onShutdown();
		/// Called when the SocketProactor is about to terminate.

	int doWork(bool handleOne = false, bool expiredOnly = false);
		/// Runs the scheduled work.
		/// If handleOne is true, only the next scheduled function
		/// is called.
		/// If expiredOnly is true, only expired temporary functions
		/// are called.

	using MutexType = Poco::Mutex;
	using ScopedLock = MutexType::ScopedLock;

	static const long DEFAULT_MAX_TIMEOUT_MS = 250;

	struct Handler
		/// Handler struct holds the scheduled I/O.
		/// At the actual I/O, Buffer and SocketAddress
		/// are used appropriately, and deleted if owned.
		/// Callback is passed to the IOCompletion queue.
	{
		Buffer* _pBuf = nullptr;
		SocketAddress* _pAddr = nullptr;
		Callback _onCompletion = nullptr;
		bool _owner = false;
	};

	class IONotification: public Notification
		/// IONotification object is used to transfer
		/// the I/O completion handlers into the
		/// completion handlers queue.
	{
	public:
		IONotification() = delete;

		IONotification(Callback&& onCompletion, int bytes, const std::error_code& errorCode):
			_onCompletion(std::move(onCompletion)),
			_bytes(bytes),
			_errorCode(errorCode)
			/// Creates the IONotification.
		{
		}

		~IONotification() override = default;

		void call()
			/// Calls the completion handler.
		{
			_onCompletion(_errorCode, _bytes);
		};

	private:
		Callback _onCompletion;
		int _bytes;
		std::error_code _errorCode;
	};

	class IOCompletion
		/// IOCompletion utility class accompanies the
		/// SocketProactor and serves to execute I/O
		/// completion handlers in its own thread.
	{
	public:
		IOCompletion() = delete;

		explicit IOCompletion(int maxTimeout):
			_activity(this, &IOCompletion::run)
			/// Creates IOCompletion.
		{
			start();
		}

		~IOCompletion()
		{
			wakeUp();
		}

		void start()
			/// Starts the I/O completion execution.
		{
			_activity.start();
		}

		void stop()
			/// Stops the I/O completion execution.
		{
			_activity.stop();
			_nq.wakeUpAll();
		}

		void wait()
			/// Blocks until I/O execution completely stops.
		{
			_activity.wait();
		}

		void enqueue(Notification::Ptr pNotification)
			/// Enqueues I/O completion.
		{
			_nq.enqueueNotification(std::move(pNotification));
		}

		void wakeUp()
			/// Wakes up the I/O completion execution loop.
		{
			_nq.wakeUpAll();
		}

		int queueSize() const
		{
			return _nq.size();
		}

	private:
		bool runOne()
			/// Runs the next I/O completion handler in the queue.
		{
			auto* pNf = dynamic_cast<IONotification*>(_nq.waitDequeueNotification());
			if (_activity.isStopped()) return false;
			if (pNf)
			{
				try
				{
					pNf->call();
					pNf->release();
					return true;
				}
				catch (Exception& exc)
				{
					ErrorHandler::handle(exc);
				}
				catch (std::exception& exc)
				{
					ErrorHandler::handle(exc);
				}
				catch (...)
				{
					ErrorHandler::handle();
				}
			}
			return false;
		}

		void run()
			/// Continuously runs enqueued completion handlers.
		{
			while(!_activity.isStopped()) runOne();
		}

		Activity<IOCompletion> _activity;
		NotificationQueue _nq;
	};

	using IOHandlerList = std::deque<std::unique_ptr<Handler>>;
	using IOHandlerIt = IOHandlerList::iterator;
	using SubscriberMap = std::unordered_map<poco_socket_t, std::deque<std::unique_ptr<Handler>>>;

	void sleep(bool isAtWork);
		/// Sleep policy implementation.
		/// If there is currently any work being done,
		/// timeout is kept at zero (ie. no timeout),
		/// otherwise, the timeout is incremented and
		///  - trySleep() is called if proactor runs
		///    in a Poco::Thread, which is necessary
		///    for trySleep call to be interruptable
		/// or
		///  - sleep() is called (not interruptable)
		///
		/// The value of _timeout can grow up to
		/// _maxTimeout value.

	int error(Socket& sock);
		/// Enqueues the completion handlers and removes
		/// them from the handlers list after the operation
		/// successfully completes.

	bool hasHandlers(SubscriberMap& handlers, int sockfd);
	void deleteHandler(IOHandlerList& handlers, IOHandlerList::iterator& it);

	template <typename T>
	int errorImpl(Socket& sock, T& handlerMap, Poco::Mutex& mutex)
	{
		Poco::Mutex::ScopedLock l(mutex);
		auto hIt = handlerMap.find(sock.impl()->sockfd());
		if (hIt == handlerMap.end()) return 0;
		unsigned err = 0;
		sock.getOption(SOL_SOCKET, SO_ERROR, err);
		IOHandlerList& handlers = hIt->second;
		int handled = static_cast<int>(handlers.size());
		auto it = handlers.begin();
		auto end = handlers.end();
		while (it != end)
		{
			enqueueIONotification(std::move((*it)->_onCompletion), 0, err);
			deleteHandler(handlers, it);
			// end iterator is invalidated when the last member
			// is removed, so make sure we don't check for it
			if (handlers.empty()) break;
		}
		handled -= static_cast<int>(handlers.size());
		if (handled) _ioCompletion.wakeUp();
		return handled;
	}

	int send(Socket& sock);
		/// Calls the appropriate output function; enqueues
		/// the accompanying completion handler and removes
		/// it from the handlers list after the operation
		/// successfully completes.

	int receive(Socket& sock);
		/// Calls the appropriate input function; enqueues
		/// the accompanying completion handler and removes
		/// it from the handlers list after the operation
		/// successfully completes.

	void sendTo(SocketImpl& sock, IOHandlerIt& it);
		/// Sends data to the datagram socket and enqueues the
		/// accompanying completion handler.

	void send(SocketImpl& sock, IOHandlerIt& it);
		/// Sends data to the stream socket and enqueues the
		/// accompanying completion handler.

	void receiveFrom(SocketImpl& sock, IOHandlerIt& it, int available);
		/// Reads data from the datagram socket and enqueues the
		/// accompanying completion handler.

	void receive(SocketImpl& sock, IOHandlerIt& it, int available);
		/// Reads data from the stream socket and enqueues the
		/// accompanying completion handler.

	void enqueueIONotification(Callback&& onCompletion, int n, int err);
		/// Enqueues the completion handler into the I/O
		/// completion handler.

	Worker& worker();

	std::atomic<bool> _isRunning;
	std::atomic<bool> _isStopped;
	std::atomic<bool> _stop;
	long              _timeout;
	long              _maxTimeout;
	PollSet           _pollSet;
	Poco::Thread*     _pThread;

	SubscriberMap _readHandlers;
	SubscriberMap _writeHandlers;
	IOCompletion  _ioCompletion;
	Poco::Mutex   _writeMutex;
	Poco::Mutex   _readMutex;

	std::unique_ptr<Worker> _pWorker;
	friend class Worker;
};

//
// inlines
//

inline void SocketProactor::addSocket(const Socket& sock, int mode)
{
	_pollSet.add(sock, mode | PollSet::POLL_ERROR);
}


inline void SocketProactor::updateSocket(const Socket& sock, int mode)
{
	_pollSet.update(sock, mode);
}


inline void SocketProactor::removeSocket(const Socket& sock)
{
	_pollSet.remove(sock);
}


inline void SocketProactor::enqueueIONotification(Callback&& onCompletion, int n, int err)
{
	if (onCompletion)
	{
		_ioCompletion.enqueue(new IONotification(
				std::move(onCompletion), n,
				std::error_code(err, std::generic_category())));
	}
}


inline bool SocketProactor::isRunning() const
{
	return _isRunning;
}


} } // namespace Poco::Net


#endif // Net_SocketProactor_INCLUDED
