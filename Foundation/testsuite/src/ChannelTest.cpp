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
#include "Poco/ConsoleChannel.h"
#include "Poco/StreamChannel.h"
#include "Poco/Event.h"
#include "TestChannel.h"
#include <atomic>
#include <sstream>
#include <thread>
#include <vector>


using Poco::SplitterChannel;
using Poco::AsyncChannel;
using Poco::FormattingChannel;
using Poco::ConsoleChannel;
using Poco::StreamChannel;
using Poco::Channel;
using Poco::Formatter;
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
	// and gets to a destination.
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
			while (!stop)
			{
				try
				{
					pFormatterChannel->log(msg);
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
	CppUnit_addTest(pSuite, ChannelTest, testFormatting);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingConcurrentReplacement);
	CppUnit_addTest(pSuite, ChannelTest, testConsole);
	CppUnit_addTest(pSuite, ChannelTest, testStream);

	return pSuite;
}
