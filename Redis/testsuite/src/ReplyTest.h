//
// ReplyTest.h
//
// Library: Redis
// Package: Redis
// Module:  ReplyTest
//
// Definition of the ReplyTest class.
//
// Copyright (c) 2026, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef ReplyTest_INCLUDED
#define ReplyTest_INCLUDED


#include "Poco/Redis/Redis.h"
#include "CppUnit/TestCase.h"


class ReplyTest: public CppUnit::TestCase
	/// Reads replies from a loopback peer; needs no Redis server.
{
public:
	ReplyTest() = delete;
	explicit ReplyTest(const std::string& name);
	~ReplyTest() override;

	void testBulkString();
	void testEmptyBulkString();
	void testNullBulkString();
	void testLargeBulkString();
	void testTruncatedBulkString();
	void testTruncatedLargeBulkString();
	void testBulkStringWithoutTerminator();
	void testTruncatedLine();
	void testNestingAtLimit();
	void testNestingTooDeep();
	void testNestingCountsDepth();
	void testConsecutiveReplies();

	void setUp() override;
	void tearDown() override;

	static CppUnit::Test* suite();
};


#endif // ReplyTest_INCLUDED
