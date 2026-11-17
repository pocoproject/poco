//
// ChannelTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ChannelTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/SplitterChannel.h"
#include "Poco/AsyncChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/Message.h"
#include "Poco/Formatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/PatternFormatter.h"
#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/StreamChannel.h"
#include "Poco/Event.h"
#include "Poco/ErrorHandler.h"
#include "Poco/Exception.h"
#include "Poco/Mutex.h"
#include "Poco/NumberParser.h"
#include "Poco/String.h"
#include "TestChannel.h"
#include <atomic>
#include <limits>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>


using Poco::SplitterChannel;
using Poco::AsyncChannel;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::StreamChannel;
using Poco::Channel;
using Poco::Formatter;
using Poco::PatternFormatter;
using Poco::Logger;
using Poco::Message;
using Poco::AutoPtr;
using Poco::Event;
using Poco::Thread;
using Poco::Runnable;


class SimpleFormatter : public Formatter
{
public:
	void format(const Message& msg, std::string& text)
	{
		text = msg.getSource();
		text.append(": ");
		text.append(msg.getText());
	}
};


class LogRunnable : public Runnable
{
public:
	LogRunnable(AutoPtr<AsyncChannel> pAsync) :
		_pAsync(pAsync),
		_stop(false)
	{
	}

	void run()
	{
		Message msg;
		while (!_stop) _pAsync->log(msg);
	}

	void stop()
	{
		_stop = true;
	}

private:
	AutoPtr<AsyncChannel> _pAsync;
	std::atomic<bool> _stop;
};


namespace
{
	struct Tally
		/// The messages that all the channels of a test have got.
	{
		std::atomic<int> formatted{0};
		std::atomic<int> plain{0};
		std::atomic<int> other{0};
	};


	class TallyChannel : public Channel
		/// Counts the messages it gets: those that a SimpleFormatter
		/// has formatted, those that came unformatted, and the rest.
	{
	public:
		explicit TallyChannel(Tally& tally) :
			_tally(tally)
		{
		}

		void log(const Message& msg) override
		{
			if (msg.getText() == "Source: Text") ++_tally.formatted;
			else if (msg.getText() == "Text") ++_tally.plain;
			else ++_tally.other;
		}

	private:
		Tally& _tally;
	};


	class HoldingFormatter: public Formatter
		/// A formatter that holds a message until the test lets it go on.
	{
	public:
		static constexpr long TIMEOUT = 10000;

		void format(const Message& msg, std::string& text) override
		{
			arrived.set();
			(void) goOn.tryWait(TIMEOUT);
			text = msg.getText();
		}

		Event arrived;
		Event goOn;
	};


	class GenerationFormatter: public Formatter
		/// Writes the generation of the configuration it belongs to.
	{
	public:
		explicit GenerationFormatter(int generation):
			_generation(generation)
		{
		}

		void format(const Message&, std::string& text) override
		{
			text = std::to_string(_generation);
		}

	private:
		int _generation;
	};


	class FailingChannel: public Channel
		/// Throws a WriteFileException for the first messages it gets and
		/// records the text of the others, and the thread of every call.
	{
	public:
		FailingChannel(int failures, int messages):
			_failures(failures),
			_messages(messages)
		{
		}

		void log(const Message& msg) override
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			_threads.insert(Thread::currentOsTid());
			if (++_calls == _messages) _allCalled.set();
			if (_calls <= _failures) throw Poco::WriteFileException("full.log");
			_texts.push_back(msg.getText());
		}

		[[nodiscard]] bool waitAllCalled(long milliseconds)
		{
			return _allCalled.tryWait(milliseconds);
		}

		[[nodiscard]] int calls() const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _calls;
		}

		[[nodiscard]] std::size_t threadCount() const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _threads.size();
		}

		[[nodiscard]] std::vector<std::string> texts() const
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			return _texts;
		}

	private:
		mutable Poco::FastMutex _mutex;
		Event _allCalled;
		std::set<long> _threads;
		std::vector<std::string> _texts;
		int _failures;
		int _messages;
		int _calls = 0;
	};


	class CountingErrorHandler: public Poco::ErrorHandler
	{
	public:
		void exception(const Poco::Exception&) override
		{
			++_count;
		}

		void exception(const std::exception&) override
		{
			++_count;
		}

		void exception() override
		{
			++_count;
		}

		[[nodiscard]] int count() const
		{
			return _count.load();
		}

	private:
		std::atomic<int> _count{0};
	};


	class LoggingErrorHandler: public Poco::ErrorHandler
		/// Counts its calls and logs a message through the given channel in each.
		/// Given a release event, it signals that it was entered and waits for
		/// the event before it logs.
	{
	public:
		LoggingErrorHandler(Channel& channel, int calls, Event* pRelease = nullptr):
			_channel(channel),
			_pRelease(pRelease),
			_calls(calls)
		{
		}

		LoggingErrorHandler(const LoggingErrorHandler&) = delete;
		LoggingErrorHandler& operator = (const LoggingErrorHandler&) = delete;

		void exception(const Poco::Exception&) override
		{
			report();
		}

		void exception(const std::exception&) override
		{
			report();
		}

		void exception() override
		{
			report();
		}

		[[nodiscard]] bool waitAllCalled(long milliseconds)
		{
			return _allCalled.tryWait(milliseconds);
		}

		[[nodiscard]] bool waitEntered(long milliseconds)
		{
			return _entered.tryWait(milliseconds);
		}

		[[nodiscard]] int count() const
		{
			return _count.load();
		}

	private:
		void report()
		{
			constexpr long TIMEOUT = 10000;

			if (_pRelease != nullptr)
			{
				_entered.set();
				(void) _pRelease->tryWait(TIMEOUT);
			}
			_channel.log(Message("Handler", "Target failed", Message::PRIO_ERROR));
			if (++_count == _calls) _allCalled.set();
		}

		Channel& _channel;
		Event* _pRelease;
		Event _entered;
		Event _allCalled;
		std::atomic<int> _count{0};
		int _calls;
	};


	class GenerationChannel: public Channel
		/// Counts the messages that a formatter of an older generation
		/// than its own formatted: pairs of a formatter and a channel
		/// that never were.
	{
	public:
		GenerationChannel(int generation, std::atomic<int>& torn):
			_generation(generation),
			_torn(torn)
		{
		}

		void log(const Message& msg) override
		{
			if (std::stoi(msg.getText()) < _generation) ++_torn;
		}

	private:
		int _generation;
		std::atomic<int>& _torn;
	};


	struct MoveTally
		/// The messages that a MoveCountingChannel has got.
	{
		int copied = 0;
		int moved = 0;
		bool inPlace = false;
		Message last;
	};


	class MoveCountingChannel: public Channel
		/// Counts the messages it gets as rvalues and as const references,
		/// and keeps a copy of the last one. log(Message&&) also notes
		/// whether it got the very message that the test logged.
	{
	public:
		explicit MoveCountingChannel(MoveTally& tally, const Message* pOriginal = nullptr);
		MoveCountingChannel() = delete;
		MoveCountingChannel(const MoveCountingChannel&) = delete;
		MoveCountingChannel& operator = (const MoveCountingChannel&) = delete;

		void log(const Message& msg) override;
		void log(Message&& msg) override;

	protected:
		~MoveCountingChannel() override = default;

	private:
		MoveTally& _tally;
		const Message* _pOriginal;
	};


	MoveCountingChannel::MoveCountingChannel(MoveTally& tally, const Message* pOriginal):
		_tally(tally),
		_pOriginal(pOriginal)
	{
	}


	void MoveCountingChannel::log(const Message& msg)
	{
		++_tally.copied;
		_tally.last = msg;
	}


	void MoveCountingChannel::log(Message&& msg)
	{
		++_tally.moved;
		_tally.inPlace = (&msg == _pOriginal);
		_tally.last = std::move(msg);
	}


	struct DropTally
		/// The messages that a DropCountingChannel has got.
	{
		int delivered = 0;
		int reported = 0;
	};


	class DropCountingChannel: public Channel
		/// Counts the messages it gets and adds up the numbers of
		/// dropped messages that an AsyncChannel reports.
	{
	public:
		explicit DropCountingChannel(DropTally& tally);
		DropCountingChannel() = delete;

		void log(const Message& msg) override;

	protected:
		~DropCountingChannel() override = default;

	private:
		DropTally& _tally;
	};


	DropCountingChannel::DropCountingChannel(DropTally& tally):
		_tally(tally)
	{
	}


	void DropCountingChannel::log(const Message& msg)
	{
		static const std::string PREFIX("Dropped ");
		const std::string& text = msg.getText();
		if (Poco::startsWith(text, PREFIX))
			_tally.reported += Poco::NumberParser::parse(text.substr(PREFIX.size(), text.find(' ', PREFIX.size()) - PREFIX.size()));
		else
			++_tally.delivered;
	}


	class LogBackChannel: public Channel
		/// Logs one further message through a logger for every message
		/// that is not itself such a further message.
	{
	public:
		explicit LogBackChannel(Logger& logger);
		LogBackChannel() = delete;

		void log(const Message& msg) override;

	protected:
		~LogBackChannel() override = default;

	private:
		Logger& _logger;
	};


	LogBackChannel::LogBackChannel(Logger& logger):
		_logger(logger)
	{
	}


	void LogBackChannel::log(const Message& msg)
	{
		static const std::string BACK("Back");
		if (msg.getText() != BACK) _logger.information(BACK);
	}
}


ChannelTest::ChannelTest(const std::string& name) : CppUnit::TestCase(name)
{
}


ChannelTest::~ChannelTest()
{
}


void ChannelTest::testSplitter()
{
	AutoPtr<TestChannel> pChannel1 = new TestChannel;
	AutoPtr<TestChannel> pChannel2 = new TestChannel;
	AutoPtr<SplitterChannel> pSplitter = new SplitterChannel;
	pSplitter->addChannel(pChannel1);
	pSplitter->addChannel(pChannel2);
	Message msg;
	pSplitter->log(msg);
	assertTrue(pChannel1->list().size() == 1);
	assertTrue(pChannel2->list().size() == 1);
}

void ChannelTest::testSplitterAddSameChannelTwice()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<SplitterChannel> pSplitter = new SplitterChannel;
	pSplitter->addChannel(pChannel);
	pSplitter->addChannel(pChannel);

	assertTrue(pSplitter->count() == 1);

	Message msg;
	pSplitter->log(msg);

	pSplitter->removeChannel(pChannel);

	assertTrue(pSplitter->count() == 0);
}

void ChannelTest::testAsync()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(pChannel);
	LogRunnable lr(pAsync);
	pAsync->open();
	Thread t;
	t.start(lr);
	Message msg;
	pAsync->log(msg);
	pAsync->log(msg);
	pAsync->close();
	lr.stop();
	t.join();
	assertTrue(pChannel->list().size() >= 2);
}


void ChannelTest::testAsyncConcurrentReplacement()
{
	// Messages are logged while another thread replaces the target
	// channel and a third asks for it: each one gets to a target.
	constexpr int MESSAGES = 5000;

	Tally tally;
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(new TallyChannel(tally));

	std::atomic<bool> stop(false);
	std::atomic<int> logged(0);
	std::thread logging([&]()
	{
		Message msg("Source", "Text", Message::PRIO_INFORMATION);
		for (int i = 0; i < MESSAGES; ++i)
		{
			pAsync->log(msg);
			++logged;
		}
	});
	std::thread asking([&]()
	{
		while (!stop)
		{
			Channel::Ptr pChannel = pAsync->getChannel();
		}
	});

	while (logged < MESSAGES)
	{
		pAsync->setChannel(new TallyChannel(tally));
	}
	logging.join();
	stop = true;
	asking.join();
	pAsync->close();

	assertEqual(MESSAGES, tally.plain.load());
}


void ChannelTest::testAsyncTargetThrows()
{
	constexpr int FAILURES = 3;
	constexpr int MESSAGES = 10;
	constexpr long TIMEOUT = 10000;

	CountingErrorHandler handler;
	Poco::ErrorHandler* pOldHandler = Poco::ErrorHandler::set(&handler);
	AutoPtr<FailingChannel> pChannel = new FailingChannel(FAILURES, MESSAGES);
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(pChannel);
	for (int i = 0; i < MESSAGES; ++i)
	{
		pAsync->log(Message("Source", std::to_string(i), Message::PRIO_ERROR));
	}
	const bool allCalled = pChannel->waitAllCalled(TIMEOUT);

	// The closing thread holds a reference, so that a close() that does not
	// return is left behind instead of hanging the destructor of the channel.
	Thread closing;
	closing.startFunc([pAsync]() mutable { pAsync->close(); });
	const bool closed = closing.tryJoin(TIMEOUT);
	Poco::ErrorHandler::set(pOldHandler);

	assertTrue(allCalled);
	assertTrue(closed);
	assertEqual(FAILURES, handler.count());
	assertEqual(1, static_cast<int>(pChannel->threadCount()));
	const std::vector<std::string> texts = pChannel->texts();
	assertEqual(MESSAGES - FAILURES, static_cast<int>(texts.size()));
	for (int i = FAILURES; i < MESSAGES; ++i)
	{
		assertEqual(std::to_string(i), texts[i - FAILURES]);
	}
}


void ChannelTest::testAsyncErrorHandlerLogsToChannel()
{
	// The ErrorHandler logs to the channel whose target failed.
	constexpr int MESSAGES = 5;
	constexpr long TIMEOUT = 10000;

	AutoPtr<FailingChannel> pChannel = new FailingChannel(std::numeric_limits<int>::max(), MESSAGES);
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(pChannel);
	LoggingErrorHandler handler(*pAsync, MESSAGES);
	Poco::ErrorHandler* pOldHandler = Poco::ErrorHandler::set(&handler);
	for (int i = 0; i < MESSAGES; ++i)
	{
		pAsync->log(Message("Source", std::to_string(i), Message::PRIO_ERROR));
	}
	const bool allCalled = handler.waitAllCalled(TIMEOUT);

	// A message that the handler logged and the channel queued would fail
	// again and call the handler once more before close() returns.
	Thread closing;
	closing.startFunc([pAsync]() mutable { pAsync->close(); });
	const bool closed = closing.tryJoin(TIMEOUT);
	Poco::ErrorHandler::set(pOldHandler);

	assertTrue(allCalled);
	assertTrue(closed);
	assertEqual(MESSAGES, pChannel->calls());
	assertEqual(MESSAGES, handler.count());
}


void ChannelTest::testAsyncReopen()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(pChannel);
	pAsync->open();
	pAsync->log(Message("Source", "first", Message::PRIO_INFORMATION));
	pAsync->close();
	pAsync->open();
	pAsync->log(Message("Source", "second", Message::PRIO_INFORMATION));
	pAsync->close();

	assertTrue(pChannel->list().size() == 2);
	assertTrue(pChannel->list().back().getText() == "second");
}


void ChannelTest::testAsyncCloseDeliversQueued()
{
	constexpr int MESSAGES = 10000;

	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(pChannel);
	for (int i = 0; i < MESSAGES; ++i)
	{
		pAsync->log(Message("Source", "Text", Message::PRIO_INFORMATION));
	}
	pAsync->close();

	assertEqual(MESSAGES, static_cast<int>(pChannel->list().size()));
}


void ChannelTest::testAsyncCloseWhileLogging()
{
	// Messages are logged while close() runs: none of them starts the
	// background thread again, so nothing reaches the target after close().
	// The test guards against log() reopening a closed channel; the race in
	// which a message logged during close() started the thread again depends
	// on timing and cannot be reproduced deterministically.
	constexpr int THREADS = 4;
	constexpr int MESSAGES = 100000;
	constexpr long TIMEOUT = 10000;

	Tally tally;
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(new TallyChannel(tally));

	Event loggedOne;
	std::vector<std::thread> threads;
	for (int i = 0; i < THREADS; ++i)
	{
		threads.emplace_back([&]()
		{
			Message msg("Source", "Text", Message::PRIO_INFORMATION);
			for (int j = 0; j < MESSAGES; ++j)
			{
				pAsync->log(msg);
				loggedOne.set();
			}
		});
	}

	const bool logging = loggedOne.tryWait(TIMEOUT);
	pAsync->close();
	const int delivered = tally.plain;
	for (auto& t : threads)
	{
		t.join();
	}
	// A background thread started again would deliver the queued messages;
	// 200 ms is long enough for it to deliver at least one of them.
	Thread::sleep(200);

	assertTrue(logging);
	assertEqual(delivered, tally.plain.load());
}


void ChannelTest::testAsyncCloseWhileTargetLogs()
{
	// The target channel logs back into the AsyncChannel while close()
	// waits for the background thread to deliver the queued messages.
	// The threads hold the channel themselves: after a failure they may
	// still be blocked in it.
	// This is a stress test: a message logged back must pass the check of the
	// closed state before close() takes the mutex, so the test detects a
	// deadlock with high but not certain probability.
	constexpr int ROUNDS = 100;
	constexpr int MESSAGES = 1000;
	constexpr long TIMEOUT = 10000;

	Logger& logger = Logger::get("ChannelTest.LogBack");
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(new LogBackChannel(logger));
	logger.setChannel(pAsync);
	logger.setLevel(Message::PRIO_INFORMATION);

	bool closed = true;
	for (int round = 0; round < ROUNDS && closed; ++round)
	{
		pAsync->open();
		Thread logging;
		logging.startFunc([pAsync]() mutable
		{
			Message msg("Source", "Text", Message::PRIO_INFORMATION);
			for (int i = 0; i < MESSAGES; ++i)
			{
				pAsync->log(msg);
			}
		});
		Thread closing;
		closing.startFunc([pAsync]() mutable
		{
			pAsync->close();
		});
		closed = closing.tryJoin(TIMEOUT) && logging.tryJoin(TIMEOUT);
	}

	assertTrue(closed);
	Logger::destroy("ChannelTest.LogBack");
}


void ChannelTest::testAsyncDropCountConcurrent()
{
	// Several threads log into a queue of one message: every message is
	// either delivered or counted in a report of dropped messages.
	constexpr int THREADS = 4;
	constexpr int MESSAGES = 10000;

	DropTally tally;
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(new DropCountingChannel(tally));
	pAsync->setProperty("queueSize", "1");

	std::vector<std::thread> threads;
	for (int i = 0; i < THREADS; ++i)
	{
		threads.emplace_back([&]()
		{
			Message msg("Source", "Text", Message::PRIO_INFORMATION);
			for (int j = 0; j < MESSAGES; ++j)
			{
				pAsync->log(msg);
			}
		});
	}
	for (auto& t : threads)
	{
		t.join();
	}

	// Drops are reported with the next message that the queue takes, so the
	// last message is logged without a limit to report the remaining drops.
	pAsync->setProperty("queueSize", "none");
	pAsync->log(Message("Source", "Text", Message::PRIO_INFORMATION));
	pAsync->close();

	assertEqual(THREADS * MESSAGES + 1, tally.delivered + tally.reported);
}


void ChannelTest::testAsyncAppThreadLogsWhileHandling()
{
	// Another thread logs while the background thread reports a failure of
	// the target to the ErrorHandler: its message is delivered, and the
	// message that the ErrorHandler logs from the background thread is dropped.
	constexpr long TIMEOUT = 10000;

	AutoPtr<FailingChannel> pChannel = new FailingChannel(1, 2);
	AutoPtr<AsyncChannel> pAsync = new AsyncChannel(pChannel);
	Event release;
	LoggingErrorHandler handler(*pAsync, 1, &release);
	Poco::ErrorHandler* pOldHandler = Poco::ErrorHandler::set(&handler);
	pAsync->log(Message("Source", "first", Message::PRIO_INFORMATION));
	const bool entered = handler.waitEntered(TIMEOUT);
	pAsync->log(Message("Source", "second", Message::PRIO_INFORMATION));
	release.set();
	const bool deliveredSecond = pChannel->waitAllCalled(TIMEOUT);
	pAsync->close();
	Poco::ErrorHandler::set(pOldHandler);

	assertTrue(entered);
	assertTrue(deliveredSecond);
	assertEqual(2, pChannel->calls());
	assertEqual(1, handler.count());
}


void ChannelTest::testFormatting()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<Formatter> pFormatter = new SimpleFormatter;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(pFormatter, pChannel);
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	pFormatterChannel->log(msg);
	assertTrue(pChannel->list().size() == 1);
	assertTrue(pChannel->list().begin()->getText() == "Source: Text");
}


void ChannelTest::testFormattingConcurrentReplacement()
{
	// Messages are logged while another thread replaces the formatter and
	// the destination: each one is formatted by a formatter or by none,
	// and gets to a destination. Lvalues and rvalues take turns, so that
	// both overloads of log() meet the replacement.
	constexpr int THREADS = 4;
	constexpr int ROUNDS = 1000;
	constexpr long TIMEOUT = 10000;

	Tally tally;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(new SimpleFormatter, new TallyChannel(tally));

	std::atomic<bool> stop(false);
	std::atomic<int> logged(0);
	std::atomic<int> failed(0);
	Event loggedOne;
	std::vector<std::thread> threads;
	for (int i = 0; i < THREADS; ++i)
	{
		threads.emplace_back([&]()
		{
			Message msg("Source", "Text", Message::PRIO_INFORMATION);
			bool asRvalue = false;
			while (!stop)
			{
				try
				{
					if (asRvalue) pFormatterChannel->log(Message(msg));
					else pFormatterChannel->log(msg);
					asRvalue = !asRvalue;
					++logged;
				}
				catch (...)
				{
					++failed;
				}
				loggedOne.set();
			}
		});
	}

	// Replacing takes turns with logging, so that every round meets
	// messages that are being logged.
	int rounds = 0;
	while (rounds < ROUNDS && loggedOne.tryWait(TIMEOUT))
	{
		pFormatterChannel->setFormatter(rounds % 2 == 0 ? new SimpleFormatter : nullptr);
		pFormatterChannel->setChannel(new TallyChannel(tally));
		++rounds;
	}
	stop = true;
	for (auto& t : threads)
	{
		t.join();
	}

	assertEqual(ROUNDS, rounds);
	assertEqual(0, failed.load());
	assertEqual(0, tally.other.load());
	assertEqual(logged.load(), tally.formatted + tally.plain);
}


void ChannelTest::testFormattingMessageKeepsItsPair()
{
	// A message that is on its way keeps the formatter and the channel it
	// started with, as one pair: the channel is replaced while the
	// formatter holds a message, and the message still goes to the old one.
	AutoPtr<HoldingFormatter> pFormatter = new HoldingFormatter;
	AutoPtr<TestChannel> pOldChannel = new TestChannel;
	AutoPtr<TestChannel> pNewChannel = new TestChannel;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(pFormatter, pOldChannel);

	std::thread sender([&]()
	{
		pFormatterChannel->log(Message("Source", "Text", Message::PRIO_INFORMATION));
	});
	const bool arrived = pFormatter->arrived.tryWait(HoldingFormatter::TIMEOUT);
	pFormatterChannel->setChannel(pNewChannel);
	pFormatter->goOn.set();
	sender.join();

	assertTrue(arrived);
	assertTrue(pOldChannel->list().size() == 1);
	assertTrue(pNewChannel->list().empty());
}


void ChannelTest::testFormattingPairNeverTorn()
{
	// Messages are logged while another thread replaces first the formatter
	// and then the channel, generation after generation. A message is
	// formatted by the formatter and sent to the channel of one moment, so
	// none that an older formatter formatted reaches a newer channel.
	// Lvalues and rvalues take turns, so that both overloads of log()
	// meet the replacement.
	constexpr int THREADS = 4;
	constexpr int ROUNDS = 1000;
	constexpr long TIMEOUT = 10000;

	std::atomic<int> torn(0);
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(new GenerationFormatter(0), new GenerationChannel(0, torn));

	std::atomic<bool> stop(false);
	Event loggedOne;
	std::vector<std::thread> threads;
	for (int i = 0; i < THREADS; ++i)
	{
		threads.emplace_back([&]()
		{
			Message msg("Source", "Text", Message::PRIO_INFORMATION);
			bool asRvalue = false;
			while (!stop)
			{
				if (asRvalue) pFormatterChannel->log(Message(msg));
				else pFormatterChannel->log(msg);
				asRvalue = !asRvalue;
				loggedOne.set();
			}
		});
	}

	int rounds = 0;
	while (rounds < ROUNDS && loggedOne.tryWait(TIMEOUT))
	{
		++rounds;
		pFormatterChannel->setFormatter(new GenerationFormatter(rounds));
		pFormatterChannel->setChannel(new GenerationChannel(rounds, torn));
	}
	stop = true;
	for (auto& t : threads)
	{
		t.join();
	}

	assertEqual(ROUNDS, rounds);
	assertEqual(0, torn.load());
}


void ChannelTest::testFormattingRvalueInPlace()
{
	// An rvalue message is formatted in place and passed on as the same
	// object, with its source, priority and parameters.
	MoveTally tally;
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	msg.set("k", "v");
	AutoPtr<MoveCountingChannel> pChannel = new MoveCountingChannel(tally, &msg);
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(new PatternFormatter("%s: %p: %t [%[k]]"), pChannel);
	pFormatterChannel->log(std::move(msg));

	assertEqual(1, tally.moved);
	assertEqual(0, tally.copied);
	assertTrue(tally.inPlace);
	assertTrue(tally.last.getText() == "Source: Information: Text [v]");
	assertTrue(tally.last.getSource() == "Source");
	assertTrue(tally.last.getPriority() == Message::PRIO_INFORMATION);
	assertTrue(tally.last.get("k") == "v");
}


void ChannelTest::testFormattingRvalueWithoutFormatter()
{
	MoveTally tally;
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	AutoPtr<MoveCountingChannel> pChannel = new MoveCountingChannel(tally, &msg);
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel;
	pFormatterChannel->setChannel(pChannel);
	pFormatterChannel->log(std::move(msg));

	assertEqual(1, tally.moved);
	assertEqual(0, tally.copied);
	assertTrue(tally.inPlace);
	assertTrue(tally.last.getText() == "Text");
}


void ChannelTest::testFormattingConstLeavesMessage()
{
	MoveTally tally;
	const Message msg("Source", "Text", Message::PRIO_INFORMATION);
	AutoPtr<MoveCountingChannel> pChannel = new MoveCountingChannel(tally, &msg);
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(new SimpleFormatter, pChannel);
	pFormatterChannel->log(msg);

	assertTrue(msg.getText() == "Text");
	assertFalse(tally.inPlace);
	assertTrue(tally.last.getText() == "Source: Text");
}


void ChannelTest::testFormattingRvalueFromLogger()
{
	// Without a formatter, only the overload that the FormattingChannel
	// calls decides whether the message arrives as an rvalue.
	MoveTally tally;
	AutoPtr<MoveCountingChannel> pChannel = new MoveCountingChannel(tally);
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel;
	pFormatterChannel->setChannel(pChannel);
	Logger& logger = Logger::get("ChannelTest.FormattingRvalue");
	logger.setLevel(Message::PRIO_INFORMATION);
	logger.setChannel(pFormatterChannel);
	logger.information("x");
	Logger::destroy("ChannelTest.FormattingRvalue");

	assertEqual(1, tally.moved);
	assertEqual(0, tally.copied);
	assertTrue(tally.last.getText() == "x");
}


void ChannelTest::testConsole()
{
	AutoPtr<ConsoleChannel> pChannel = new ConsoleChannel;
	AutoPtr<Formatter> pFormatter = new SimpleFormatter;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(pFormatter, pChannel);
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	pFormatterChannel->log(msg);
}


void ChannelTest::testStream()
{
	std::ostringstream str;
	AutoPtr<StreamChannel> pChannel = new StreamChannel(str);
	AutoPtr<Formatter> pFormatter = new SimpleFormatter;
	AutoPtr<FormattingChannel> pFormatterChannel = new FormattingChannel(pFormatter, pChannel);
	Message msg("Source", "Text", Message::PRIO_INFORMATION);
	pFormatterChannel->log(msg);
	assertTrue(str.str().find("Source: Text") == 0);
}


void ChannelTest::setUp()
{
}


void ChannelTest::tearDown()
{
}


CppUnit::Test* ChannelTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ChannelTest");

	CppUnit_addTest(pSuite, ChannelTest, testSplitter);
	CppUnit_addTest(pSuite, ChannelTest, testSplitterAddSameChannelTwice);
	CppUnit_addTest(pSuite, ChannelTest, testAsync);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncConcurrentReplacement);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncTargetThrows);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncErrorHandlerLogsToChannel);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncReopen);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncCloseDeliversQueued);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncCloseWhileLogging);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncCloseWhileTargetLogs);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncDropCountConcurrent);
	CppUnit_addTest(pSuite, ChannelTest, testAsyncAppThreadLogsWhileHandling);
	CppUnit_addTest(pSuite, ChannelTest, testFormatting);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingConcurrentReplacement);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingMessageKeepsItsPair);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingPairNeverTorn);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingRvalueInPlace);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingRvalueWithoutFormatter);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingConstLeavesMessage);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingRvalueFromLogger);
	CppUnit_addTest(pSuite, ChannelTest, testConsole);
	CppUnit_addTest(pSuite, ChannelTest, testStream);

	return pSuite;
}
