//
// LoggerTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "LoggerTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Logger.h"
#include "Poco/AutoPtr.h"
#include "TestChannel.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/NullChannel.h"
#include <atomic>
#include <functional>
#include <thread>
#include <memory>
#include <vector>

using Poco::Logger;
using Poco::Channel;
using Poco::Formatter;
using Poco::Message;
using Poco::AutoPtr;
using Poco::PatternFormatter;
using Poco::FormattingChannel;
using Poco::NullChannel;
using Poco::Event;
using Poco::Thread;


namespace
{
	class Bystander
		/// A thread that logs when it is asked to, which shows whether
		/// logging is possible while the thread that asked is busy
		/// with something else.
	{
	public:
		using Ptr = std::shared_ptr<Bystander>;

		explicit Bystander(std::function<void()> logOne):
			_thread([this, logOne = std::move(logOne)]
			{
				if (_asked.tryWait(TIMEOUT))
				{
					logOne();
					_logged.set();
				}
			})
		{
		}

		~Bystander()
		{
			_asked.set();
			_thread.join();
		}

		void ask()
			/// Asks the thread to log and waits for that, but not
			/// for long: logging is not possible if it takes longer.
		{
			_asked.set();
			_couldLog = _logged.tryWait(TIMEOUT);
		}

		bool couldLog() const
			/// Returns true if the thread has logged in time
			/// when it was asked to.
		{
			return _couldLog;
		}

	private:
		static constexpr long TIMEOUT = 10000;

		Event _asked;
		Event _logged;
		std::atomic<bool> _couldLog{false};
		std::thread _thread;
	};


	class LastWordsChannel: public Channel
		/// A channel that asks a bystander to log while it is destroyed.
	{
	public:
		explicit LastWordsChannel(Bystander::Ptr pBystander):
			_pBystander(std::move(pBystander))
		{
		}

		void log(const Message&) override
		{
		}

	protected:
		~LastWordsChannel() override
		{
			_pBystander->ask();
		}

	private:
		Bystander::Ptr _pBystander;
	};


	class LastWordsFormatter: public Formatter
		/// A formatter that asks a bystander to log while it is destroyed.
	{
	public:
		explicit LastWordsFormatter(Bystander::Ptr pBystander):
			_pBystander(std::move(pBystander))
		{
		}

		~LastWordsFormatter() override
		{
			_pBystander->ask();
		}

		void format(const Message& msg, std::string& text) override
		{
			text = msg.getText();
		}

	private:
		Bystander::Ptr _pBystander;
	};

	struct Watch
		/// What a test shares with a channel or a formatter that it watches.
	{
		using Ptr = std::shared_ptr<Watch>;

		static constexpr long TIMEOUT = 10000;

		void messageArrived()
			/// Called by the watched object for every message. Holds the
			/// message there until the test lets it go on, if the test
			/// wants that.
		{
			++messages;
			arrived.set();
			if (holding) goOn.tryWait(TIMEOUT);
			if (destroyed) destroyedInUse = true;
		}

		Event arrived;
			/// A message has arrived in the watched object.

		Event goOn;
			/// Set by the test: the message that is held may go on.

		std::atomic<bool> holding{false};
			/// Messages are held in the watched object.

		std::atomic<int> messages{0};
			/// The messages that have arrived in the watched object.

		std::atomic<bool> destroyed{false};
			/// The watched object is destroyed.

		std::atomic<bool> destroyedInUse{false};
			/// The watched object was destroyed with a message in it.
	};


	class WatchedChannel: public Channel
		/// A channel that tells a test of its messages and of its destruction.
	{
	public:
		explicit WatchedChannel(Watch::Ptr pWatch):
			_pWatch(std::move(pWatch))
		{
		}

		void log(const Message&) override
		{
			// Kept apart from the channel, which the test expects to be
			// there for as long as the message is in it.
			Watch::Ptr pWatch = _pWatch;
			pWatch->messageArrived();
		}

	protected:
		~WatchedChannel() override
		{
			_pWatch->destroyed = true;
		}

	private:
		Watch::Ptr _pWatch;
	};


	class WatchedFormatter: public Formatter
		/// A formatter that tells a test of its messages and of its destruction.
	{
	public:
		explicit WatchedFormatter(Watch::Ptr pWatch):
			_pWatch(std::move(pWatch))
		{
		}

		~WatchedFormatter() override
		{
			_pWatch->destroyed = true;
		}

		void format(const Message& msg, std::string& text) override
		{
			Watch::Ptr pWatch = _pWatch;
			pWatch->messageArrived();
			text = msg.getText();
		}

	private:
		Watch::Ptr _pWatch;
	};


	class SelfReplacingChannel: public Channel
		/// A channel that replaces itself in its logger while it logs.
	{
	public:
		SelfReplacingChannel(Logger& logger, Watch::Ptr pWatch):
			_logger(logger),
			_pWatch(std::move(pWatch))
		{
		}

		void log(const Message&) override
		{
			Watch::Ptr pWatch = _pWatch;
			_logger.setChannel(new NullChannel);
			pWatch->messageArrived();
		}

	protected:
		~SelfReplacingChannel() override
		{
			_pWatch->destroyed = true;
		}

	private:
		Logger& _logger;
		Watch::Ptr _pWatch;
	};


	class Sender
		/// A thread that logs. It is joined when the test leaves it
		/// behind, which a failed assertion does as well.
	{
	public:
		explicit Sender(std::function<void()> log):
			_thread(std::move(log))
		{
		}

		~Sender()
		{
			join();
		}

		void join()
		{
			if (_thread.joinable()) _thread.join();
		}

	private:
		std::thread _thread;
	};


	struct LastWords
		/// Logs when the thread that it belongs to ends.
	{
		~LastWords()
		{
			if (say) say();
		}

		std::function<void()> say;
	};


	thread_local LastWords lastWords;
}


LoggerTest::LoggerTest(const std::string& name): CppUnit::TestCase(name)
{
}


LoggerTest::~LoggerTest()
{
}


void LoggerTest::testLogger()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel);

	assertTrue (root.getLevel() == Message::PRIO_INFORMATION);
	assertTrue (root.is(Message::PRIO_INFORMATION));
	assertTrue (root.fatal());
	assertTrue (root.critical());
	assertTrue (root.error());
	assertTrue (root.warning());
	assertTrue (root.notice());
	assertTrue (root.information());
	assertTrue (!root.debug());
	assertTrue (!root.trace());

	root.information("Informational message");
	assertTrue (pChannel->list().size() == 1);
	root.warning("Warning message");
	assertTrue (pChannel->list().size() == 2);
	root.debug("Debug message");
	assertTrue (pChannel->list().size() == 2);

	Logger& logger1 = Logger::get("Logger1");
	Logger& logger2 = Logger::get("Logger2");
	Logger& logger11 = Logger::get("Logger1.Logger1");
	Logger& logger12 = Logger::get("Logger1.Logger2");
	Logger& logger21 = Logger::get("Logger2.Logger1");
	Logger& logger22 = Logger::get("Logger2.Logger2");

	std::vector<std::string> loggers;
	Logger::names(loggers);
	assertTrue (loggers.size() == 7);
	assertTrue (loggers[0] == "");
	assertTrue (loggers[1] == "Logger1");
	assertTrue (loggers[2] == "Logger1.Logger1");
	assertTrue (loggers[3] == "Logger1.Logger2");
	assertTrue (loggers[4] == "Logger2");
	assertTrue (loggers[5] == "Logger2.Logger1");
	assertTrue (loggers[6] == "Logger2.Logger2");

	Logger::setLevel("Logger1", Message::PRIO_DEBUG);
	assertTrue (logger1.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_DEBUG));
	assertTrue (logger12.is(Message::PRIO_DEBUG));
	assertTrue (!logger2.is(Message::PRIO_DEBUG));
	assertTrue (!logger21.is(Message::PRIO_DEBUG));
	assertTrue (!logger22.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_INFORMATION));
	assertTrue (logger12.is(Message::PRIO_INFORMATION));
	assertTrue (logger21.is(Message::PRIO_INFORMATION));
	assertTrue (logger22.is(Message::PRIO_INFORMATION));

	Logger::setLevel("Logger2.Logger1", Message::PRIO_ERROR);
	assertTrue (logger1.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_DEBUG));
	assertTrue (logger12.is(Message::PRIO_DEBUG));
	assertTrue (!logger21.is(Message::PRIO_DEBUG));
	assertTrue (!logger22.is(Message::PRIO_DEBUG));
	assertTrue (logger11.is(Message::PRIO_INFORMATION));
	assertTrue (logger12.is(Message::PRIO_INFORMATION));
	assertTrue (logger21.is(Message::PRIO_ERROR));
	assertTrue (logger22.is(Message::PRIO_INFORMATION));

	Logger::setLevel("", Message::PRIO_WARNING);
	assertTrue (root.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger1.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger11.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger12.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger1.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger21.getLevel() == Message::PRIO_WARNING);
	assertTrue (logger22.getLevel() == Message::PRIO_WARNING);

	AutoPtr<TestChannel> pChannel2 = new TestChannel;
	Logger::setChannel("Logger2", pChannel2);
	assertTrue (pChannel.get()  == root.getChannel().get());
	assertTrue (pChannel.get()  == logger1.getChannel().get());
	assertTrue (pChannel.get()  == logger11.getChannel().get());
	assertTrue (pChannel.get()  == logger12.getChannel().get());
	assertTrue (pChannel2.get() == logger2.getChannel().get());
	assertTrue (pChannel2.get() == logger21.getChannel().get());
	assertTrue (pChannel2.get() == logger22.getChannel().get());

	root.setLevel(Message::PRIO_TRACE);
	pChannel->list().clear();
	root.trace("trace");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_TRACE);
	pChannel->list().clear();
	root.debug("debug");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_DEBUG);
	pChannel->list().clear();
	root.information("information");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_INFORMATION);
	pChannel->list().clear();
	root.notice("notice");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_NOTICE);
	pChannel->list().clear();
	root.warning("warning");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_WARNING);
	pChannel->list().clear();
	root.error("error");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_ERROR);
	pChannel->list().clear();
	root.critical("critical");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_CRITICAL);
	pChannel->list().clear();
	root.fatal("fatal");
	assertTrue (pChannel->list().begin()->getPriority() == Message::PRIO_FATAL);

	root.setLevel("1");
	assertTrue (root.getLevel() == Message::PRIO_FATAL);
	root.setLevel("8");
	assertTrue (root.getLevel() == Message::PRIO_TRACE);
	try
	{
		root.setLevel("0");
		assertTrue (0);
	}
	catch(Poco::InvalidArgumentException&)
	{
	}
	try
	{
		root.setLevel("9");
		assertTrue (0);
	}
	catch(Poco::InvalidArgumentException&)
	{
	}

}


void LoggerTest::testFormat()
{
	std::string str = Logger::format("$0$1", "foo", "bar");
	assertTrue (str == "foobar");
	str = Logger::format("foo$0", "bar");
	assertTrue (str == "foobar");
	str = Logger::format("the amount is $$ $0", "100");
	assertTrue (str == "the amount is $ 100");
	str = Logger::format("$0$1$2", "foo", "bar");
	assertTrue (str == "foobar");
	str = Logger::format("$foo$0", "bar");
	assertTrue (str == "$foobar");
	str = Logger::format("$0", "1");
	assertTrue (str == "1");
	str = Logger::format("$0$1", "1", "2");
	assertTrue (str == "12");
	str = Logger::format("$0$1$2", "1", "2", "3");
	assertTrue (str == "123");
	str = Logger::format("$0$1$2$3", "1", "2", "3", "4");
	assertTrue (str == "1234");
}

void LoggerTest::testFormatAny()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel);

	root.error("%s%s", std::string("foo"), std::string("bar"));
	assertTrue (pChannel->getLastMessage().getText() == "foobar");

	root.error("foo%s", std::string("bar"));
	assertTrue (pChannel->getLastMessage().getText() == "foobar");

	root.error("the amount is %% %d", 100);
	assertTrue (pChannel->getLastMessage().getText() == "the amount is % 100");

	root.error("%d", 1);
	assertTrue (pChannel->getLastMessage().getText() == "1");

	root.error("%d%d", 1, 2);
	assertTrue (pChannel->getLastMessage().getText() == "12");

	root.error("%d%d%d", 1, 2, 3);
	assertTrue (pChannel->getLastMessage().getText() == "123");

	root.error("%d%d%d%d", 1, 2, 3, 4);
	assertTrue (pChannel->getLastMessage().getText() == "1234");

	root.error("%d%d%d%d%d", 1, 2, 3, 4, 5);
	assertTrue (pChannel->getLastMessage().getText() == "12345");

	root.error("%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6);
	assertTrue (pChannel->getLastMessage().getText() == "123456");

	root.error("%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7);
	assertTrue (pChannel->getLastMessage().getText() == "1234567");

	root.error("%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8);
	assertTrue (pChannel->getLastMessage().getText() == "12345678");

	root.error("%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9);
	assertTrue (pChannel->getLastMessage().getText() == "123456789");

	root.error("%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	assertTrue (pChannel->getLastMessage().getText() == "12345678910");

	root.error("%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
	assertTrue (pChannel->getLastMessage().getText() == "1234567891011");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	assertTrue (pChannel->getLastMessage().getText() == "123456789101112");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
	assertTrue (pChannel->getLastMessage().getText() == "12345678910111213");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
	assertTrue (pChannel->getLastMessage().getText() == "1234567891011121314");

	root.error("%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	assertTrue (pChannel->getLastMessage().getText() == "123456789101112131415");
}


void LoggerTest::testDump()
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	Logger& root = Logger::root();
	root.setChannel(pChannel);
	root.setLevel(Message::PRIO_INFORMATION);

	char buffer1[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
	root.dump("test", buffer1, sizeof(buffer1));
	assertTrue (pChannel->list().empty());

	root.setLevel(Message::PRIO_DEBUG);
	root.dump("test", buffer1, sizeof(buffer1));

	std::string msg = pChannel->list().begin()->getText();
	assertTrue (msg == "test\n0000  00 01 02 03 04 05                                 ......");
	pChannel->clear();

	char buffer2[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
	};
	root.dump("", buffer2, sizeof(buffer2));
	msg = pChannel->list().begin()->getText();
	assertTrue (msg == "0000  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  ................");
	pChannel->clear();

	char buffer3[] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
		0x20, 0x41, 0x42, 0x1f, 0x7f, 0x7e
	};
	root.dump("", buffer3, sizeof(buffer3));
	msg = pChannel->list().begin()->getText();
	assertTrue (msg == "0000  00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  ................\n"
				   "0010  20 41 42 1F 7F 7E                                  AB..~");
	pChannel->clear();
}

namespace ThreadNameTestStrings {
const std::string loggerName = "Logger";
const std::string threadName = "ThreadName";
const std::string message = "Test message";
}

template <typename ThreadFactory>
std::string LoggerTest::doTestFormatThreadName(ThreadFactory makeThread)
{
	AutoPtr<TestChannel> pChannel = new TestChannel;
	AutoPtr<PatternFormatter> fmt = new PatternFormatter("%s:%I:%T:%q:%t");
	AutoPtr<FormattingChannel> pFmtChannel = new FormattingChannel(fmt, pChannel);

	Logger& logger = Logger::get(ThreadNameTestStrings::loggerName);
	logger.setChannel(pFmtChannel);
	logger.setLevel(Message::PRIO_INFORMATION);

	Event ev;
	auto thr = makeThread(
		ThreadNameTestStrings::threadName,
		[&ev, &logger] {
			logger.information(ThreadNameTestStrings::message);
			ev.set();
		});
	ev.wait();

	thr->join();

	const std::string logMsg = pChannel->getLastMessage().getText();
	std::vector<std::string> parts;
	std::size_t p = 0;
	while (p < logMsg.size())
	{
		auto q = logMsg.find(':', p);
		if (q == std::string::npos)
		{
			q = logMsg.size();
		}
		parts.push_back(logMsg.substr(p, q - p));
		p = q + 1;
	}
	assertTrue (parts.size() >= 5);
	assertEqual( ThreadNameTestStrings::loggerName, parts[0] );
	assertEqual( ThreadNameTestStrings::threadName, parts[2] );
	assertEqual( "I", parts[3] );
	assertEqual( ThreadNameTestStrings::message, parts[4] );

	return parts[1];
}


void LoggerTest::testFormatThreadName()
{
	Thread thr(ThreadNameTestStrings::threadName);
	std::string expectedTid = std::to_string(thr.id());

	std::string actualTid = doTestFormatThreadName(
		[&thr](std::string, auto body) {
			thr.startFunc(std::move(body));
			return &thr;
		}
	);
	assertEqual( expectedTid, actualTid );
}


void LoggerTest::testFormatStdThreadName()
{
#ifndef POCO_NO_THREADNAME
	std::unique_ptr<std::thread> thrPtr;
	std::string expectedTid;
	std::string actualTid = doTestFormatThreadName(
		[&thrPtr, &expectedTid](std::string name, auto bodyIn) {
			thrPtr = std::make_unique<std::thread>(
				[name, body = std::move(bodyIn), &expectedTid] {
					expectedTid = std::to_string(Thread::currentOsTid());
					Thread::setCurrentName(name);
					body();
				}
			);
			return thrPtr.get();
		}
	);
	assertEqual( expectedTid, actualTid );
#endif
}

void LoggerTest::testLoggerRefSurvivesShutdown()
{
	// Simulate a singleton caching a Logger& obtained before shutdown.
	Logger& cached = Logger::get("TestLogger.Cached");
	AutoPtr<TestChannel> pChannel = new TestChannel;
	cached.setChannel(pChannel);
	cached.setLevel(Message::PRIO_INFORMATION);
	cached.information("before shutdown");
	assertTrue (pChannel->list().size() == 1);

	Logger::shutdown();

	// The cached reference must still be valid; logging through it
	// must be a safe no-op (channel detached).
	cached.information("post-shutdown message");
	cached.log(Message("x", "y", Message::PRIO_ERROR));
	assertTrue (cached.getChannel().isNull());
	assertTrue (pChannel->list().size() == 1);

	// get() with the same name returns the same (muted) instance.
	Logger& again = Logger::get("TestLogger.Cached");
	assertTrue (&again == &cached);
	assertTrue (again.getChannel().isNull());
}


void LoggerTest::testConcurrentChannelReplacement()
{
	Logger& logger = Logger::get("TestLogger.ConcurrentReplace");
	logger.setLevel(Message::PRIO_INFORMATION);

	std::atomic<bool> stop{false};
	std::vector<std::thread> threads;

	// Worker threads logging concurrently
	for (int i = 0; i < 4; ++i)
	{
		threads.emplace_back([&logger, &stop, i]() {
			while (!stop)
			{
				logger.information("concurrent message from thread " + std::to_string(i));
			}
		});
	}

	// Channel replacement thread
	for (int i = 0; i < 200; ++i)
	{
		AutoPtr<NullChannel> pChan = new NullChannel;
		logger.setChannel(pChan);
		std::this_thread::yield();
	}

	stop = true;
	for (auto& t : threads)
	{
		t.join();
	}
	logger.setChannel(nullptr);
}


void LoggerTest::testConcurrentShutdown()
{
	Logger& logger = Logger::get("TestLogger.ConcurrentShutdown");
	AutoPtr<NullChannel> pChannel = new NullChannel;
	logger.setChannel(pChannel);
	logger.setLevel(Message::PRIO_INFORMATION);

	std::atomic<bool> stop{false};
	std::vector<std::thread> threads;

	for (int i = 0; i < 4; ++i)
	{
		threads.emplace_back([&logger, &stop]() {
			while (!stop)
			{
				logger.information("message during shutdown");
			}
		});
	}

	for (int i = 0; i < 50; ++i)
	{
		Logger::shutdown();
		logger.setChannel(new NullChannel);
	}

	stop = true;
	for (auto& t : threads)
	{
		t.join();
	}
	Logger::shutdown();
}


void LoggerTest::testLogDuringDestructionOfReplaced()
{
	// The destructor of a channel or of a formatter may log, and so may
	// any other thread while that destructor runs.
	AutoPtr<FormattingChannel> pFormattingChannel = new FormattingChannel;
	pFormattingChannel->setChannel(new NullChannel);
	Logger& logger = Logger::get("TestLogger.DestructionOfReplaced");
	logger.setLevel(Message::PRIO_INFORMATION);
	logger.setChannel(pFormattingChannel);

	auto logOne = [&logger] { logger.information("logged while the one replaced is destroyed"); };

	auto pFormatterBystander = std::make_shared<Bystander>(logOne);
	pFormattingChannel->setFormatter(new LastWordsFormatter(pFormatterBystander));
	pFormattingChannel->setFormatter(nullptr);
	assertTrue (pFormatterBystander->couldLog());

	auto pDestinationBystander = std::make_shared<Bystander>(logOne);
	pFormattingChannel->setChannel(new LastWordsChannel(pDestinationBystander));
	pFormattingChannel->setChannel(new NullChannel);
	assertTrue (pDestinationBystander->couldLog());

	auto pChannelBystander = std::make_shared<Bystander>(logOne);
	logger.setChannel(new LastWordsChannel(pChannelBystander));
	logger.setChannel(pFormattingChannel);
	assertTrue (pChannelBystander->couldLog());

	logger.setChannel(nullptr);
}


void LoggerTest::testGetDuringDestructionOfDetached()
{
	// While a channel that shutdown() or the static setChannel() has
	// detached is destroyed, any thread may ask for a logger.
	Logger& logger = Logger::get("TestLogger.DestructionOfDetached");
	auto getOne = [] { Logger::get("TestLogger.AskedFor").information("asked for while a channel is destroyed"); };

	auto pShutdownBystander = std::make_shared<Bystander>(getOne);
	logger.setChannel(new LastWordsChannel(pShutdownBystander));
	Logger::shutdown();
	assertTrue (pShutdownBystander->couldLog());

	auto pSetChannelBystander = std::make_shared<Bystander>(getOne);
	logger.setChannel(new LastWordsChannel(pSetChannelBystander));
	Logger::setChannel("TestLogger.DestructionOfDetached", nullptr);
	assertTrue (pSetChannelBystander->couldLog());

	auto pDestroyBystander = std::make_shared<Bystander>(getOne);
	Logger& loggerToDestroy = Logger::get("TestLogger.DestructionOnDestroy");
	loggerToDestroy.setChannel(new LastWordsChannel(pDestroyBystander));
	Logger::destroy("TestLogger.DestructionOnDestroy");
	assertTrue (pDestroyBystander->couldLog());
}


void LoggerTest::testConcurrentSetLevel()
{
	Logger& logger = Logger::get("TestLogger.ConcurrentLevel");
	logger.setChannel(new NullChannel);

	std::atomic<bool> stop{false};
	std::vector<std::thread> threads;

	for (int i = 0; i < 4; ++i)
	{
		threads.emplace_back([&logger, &stop]() {
			while (!stop)
			{
				logger.information("message while the level changes");
			}
		});
	}

	for (int i = 0; i < 1000; ++i)
	{
		logger.setLevel(i % 2 == 0 ? Message::PRIO_ERROR : Message::PRIO_DEBUG);
		std::this_thread::yield();
	}

	stop = true;
	for (auto& t : threads)
	{
		t.join();
	}
	logger.setChannel(nullptr);
}


void LoggerTest::testReplacedChannelReleased()
{
	// With no message on its way, a channel that is replaced or detached
	// is released at once. A thread that has logged and ended does not
	// hold it back.
	Logger& logger = Logger::get("TestLogger.ReplacedReleased");
	logger.setLevel(Message::PRIO_INFORMATION);

	auto pReplaced = std::make_shared<Watch>();
	logger.setChannel(new WatchedChannel(pReplaced));
	logger.information("through before the channel is replaced");
	Sender sender([&logger] { logger.information("through as well, from a thread that ends"); });
	sender.join();
	assertEqual (2, pReplaced->messages.load());
	assertTrue (!pReplaced->destroyed);
	logger.setChannel(new NullChannel);
	assertTrue (pReplaced->destroyed);

	auto pReplacedByName = std::make_shared<Watch>();
	logger.setChannel(new WatchedChannel(pReplacedByName));
	Logger::setChannel("TestLogger.ReplacedReleased", new NullChannel);
	assertTrue (pReplacedByName->destroyed);

	auto pDetached = std::make_shared<Watch>();
	logger.setChannel(new WatchedChannel(pDetached));
	Logger::shutdown();
	assertTrue (pDetached->destroyed);

	AutoPtr<FormattingChannel> pFormattingChannel = new FormattingChannel;
	auto pFormatter = std::make_shared<Watch>();
	pFormattingChannel->setFormatter(new WatchedFormatter(pFormatter));
	pFormattingChannel->setFormatter(nullptr);
	assertTrue (pFormatter->destroyed);

	auto pDestination = std::make_shared<Watch>();
	pFormattingChannel->setChannel(new WatchedChannel(pDestination));
	pFormattingChannel->setChannel(nullptr);
	assertTrue (pDestination->destroyed);
}


void LoggerTest::testReplaceChannelWithMessageOnItsWay()
{
	// A channel that is replaced while a message is on its way through
	// it is kept until that message is through.
	Logger& logger = Logger::get("TestLogger.MessageOnItsWay");
	logger.setLevel(Message::PRIO_INFORMATION);

	auto pReplaced = std::make_shared<Watch>();
	pReplaced->holding = true;
	logger.setChannel(new WatchedChannel(pReplaced));

	Sender sender([&logger] { logger.information("on its way while the channel is replaced"); });
	assertTrue (pReplaced->arrived.tryWait(Watch::TIMEOUT));
	logger.setChannel(new NullChannel);
	assertTrue (!pReplaced->destroyed);

	pReplaced->goOn.set();
	sender.join();
	assertTrue (pReplaced->destroyed);
	assertTrue (!pReplaced->destroyedInUse);

	logger.setChannel(nullptr);
}


void LoggerTest::testReplaceFormatterWithMessageOnItsWay()
{
	// The same for the formatter and for the destination of a
	// FormattingChannel that a logger passes its messages to.
	AutoPtr<FormattingChannel> pFormattingChannel = new FormattingChannel;
	Logger& logger = Logger::get("TestLogger.FormatterMessageOnItsWay");
	logger.setLevel(Message::PRIO_INFORMATION);
	logger.setChannel(pFormattingChannel);
	auto logOne = [&logger] { logger.information("on its way while a part of the channel is replaced"); };

	{
		auto pFormatter = std::make_shared<Watch>();
		pFormatter->holding = true;
		pFormattingChannel->setFormatter(new WatchedFormatter(pFormatter));
		pFormattingChannel->setChannel(new NullChannel);

		Sender sender(logOne);
		assertTrue (pFormatter->arrived.tryWait(Watch::TIMEOUT));
		pFormattingChannel->setFormatter(nullptr);
		assertTrue (!pFormatter->destroyed);

		pFormatter->goOn.set();
		sender.join();
		assertTrue (pFormatter->destroyed);
		assertTrue (!pFormatter->destroyedInUse);
	}

	{
		auto pDestination = std::make_shared<Watch>();
		pDestination->holding = true;
		pFormattingChannel->setChannel(new WatchedChannel(pDestination));

		Sender sender(logOne);
		assertTrue (pDestination->arrived.tryWait(Watch::TIMEOUT));
		pFormattingChannel->setChannel(new NullChannel);
		assertTrue (!pDestination->destroyed);

		pDestination->goOn.set();
		sender.join();
		assertTrue (pDestination->destroyed);
		assertTrue (!pDestination->destroyedInUse);
	}

	logger.setChannel(nullptr);
}


void LoggerTest::testReplacedChannelNotKeptByLaterMessage()
{
	// A channel that was replaced is kept for the messages that were on
	// their way at that moment, and not for one that set out later.
	Logger& logger = Logger::get("TestLogger.LaterMessage");
	logger.setLevel(Message::PRIO_INFORMATION);
	auto logOne = [&logger] { logger.information("on its way"); };

	auto pReplaced = std::make_shared<Watch>();
	pReplaced->holding = true;
	auto pCurrent = std::make_shared<Watch>();
	pCurrent->holding = true;

	logger.setChannel(new WatchedChannel(pReplaced));
	Sender earlier(logOne);
	assertTrue (pReplaced->arrived.tryWait(Watch::TIMEOUT));

	logger.setChannel(new WatchedChannel(pCurrent));
	Sender later(logOne);
	assertTrue (pCurrent->arrived.tryWait(Watch::TIMEOUT));
	assertTrue (!pReplaced->destroyed);

	pReplaced->goOn.set();
	earlier.join();
	assertTrue (pReplaced->destroyed);
	assertTrue (!pReplaced->destroyedInUse);
	assertTrue (!pCurrent->destroyed);

	pCurrent->goOn.set();
	later.join();
	logger.setChannel(nullptr);
	assertTrue (pCurrent->destroyed);
}


void LoggerTest::testChannelReplacesItself()
{
	// A channel may replace itself in its logger while it logs: it is
	// kept until its message is through.
	Logger& logger = Logger::get("TestLogger.ReplacesItself");
	logger.setLevel(Message::PRIO_INFORMATION);

	auto pWatch = std::make_shared<Watch>();
	logger.setChannel(new SelfReplacingChannel(logger, pWatch));
	logger.information("makes the channel replace itself");
	assertEqual (1, pWatch->messages.load());
	assertTrue (pWatch->destroyed);
	assertTrue (!pWatch->destroyedInUse);

	logger.setChannel(nullptr);
}


void LoggerTest::testLogWhileThreadEnds()
{
	// A thread may log while it ends, from the destructor of an object
	// of its own, whether that object was made before the thread logged
	// for the first time or after.
	Logger& logger = Logger::get("TestLogger.ThreadEnds");
	logger.setLevel(Message::PRIO_INFORMATION);
	auto pWatch = std::make_shared<Watch>();
	logger.setChannel(new WatchedChannel(pWatch));
	auto logLast = [&logger] { logger.information("logged while the thread ends"); };

	Sender madeBefore([&logger, logLast]
	{
		lastWords.say = logLast;
		logger.information("logged by the thread");
	});
	madeBefore.join();
	assertEqual (2, pWatch->messages.load());

	Sender madeAfter([&logger, logLast]
	{
		logger.information("logged by the thread");
		lastWords.say = logLast;
	});
	madeAfter.join();
	assertEqual (4, pWatch->messages.load());

	// the threads that have ended hold nothing back
	logger.setChannel(nullptr);
	assertTrue (pWatch->destroyed);
}


void LoggerTest::setUp()
{
	Logger::shutdown();
}


void LoggerTest::tearDown()
{
}


CppUnit::Test* LoggerTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LoggerTest");

	CppUnit_addTest(pSuite, LoggerTest, testLogger);
	CppUnit_addTest(pSuite, LoggerTest, testFormat);
	CppUnit_addTest(pSuite, LoggerTest, testFormatAny);
	CppUnit_addTest(pSuite, LoggerTest, testDump);
	CppUnit_addTest(pSuite, LoggerTest, testFormatThreadName);
	CppUnit_addTest(pSuite, LoggerTest, testFormatStdThreadName);
	CppUnit_addTest(pSuite, LoggerTest, testLoggerRefSurvivesShutdown);
	CppUnit_addTest(pSuite, LoggerTest, testConcurrentChannelReplacement);
	CppUnit_addTest(pSuite, LoggerTest, testConcurrentShutdown);
	CppUnit_addTest(pSuite, LoggerTest, testLogDuringDestructionOfReplaced);
	CppUnit_addTest(pSuite, LoggerTest, testGetDuringDestructionOfDetached);
	CppUnit_addTest(pSuite, LoggerTest, testConcurrentSetLevel);
	CppUnit_addTest(pSuite, LoggerTest, testReplacedChannelReleased);
	CppUnit_addTest(pSuite, LoggerTest, testReplaceChannelWithMessageOnItsWay);
	CppUnit_addTest(pSuite, LoggerTest, testReplaceFormatterWithMessageOnItsWay);
	CppUnit_addTest(pSuite, LoggerTest, testReplacedChannelNotKeptByLaterMessage);
	CppUnit_addTest(pSuite, LoggerTest, testChannelReplacesItself);
	CppUnit_addTest(pSuite, LoggerTest, testLogWhileThreadEnds);

	return pSuite;
}
