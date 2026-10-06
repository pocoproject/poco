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
			while (!stop)
			{
				pFormatterChannel->log(msg);
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
	CppUnit_addTest(pSuite, ChannelTest, testFormattingMessageKeepsItsPair);
	CppUnit_addTest(pSuite, ChannelTest, testFormattingPairNeverTorn);
	CppUnit_addTest(pSuite, ChannelTest, testConsole);
	CppUnit_addTest(pSuite, ChannelTest, testStream);

	return pSuite;
}
