//
// ByteOrderTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ByteOrderTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/ByteOrder.h"


using Poco::ByteOrder;
using Poco::Int8;
using Poco::UInt8;
using Poco::Int16;
using Poco::UInt16;
using Poco::Int32;
using Poco::UInt32;
#if defined(POCO_HAVE_INT64)
using Poco::Int64;
using Poco::UInt64;
#endif


ByteOrderTest::ByteOrderTest(const std::string& name): CppUnit::TestCase(name)
{
}


ByteOrderTest::~ByteOrderTest()
{
}


void ByteOrderTest::testByteOrderFlip()
{
	{
		Int8 norm = (Int8) 0xAB;
		Int8 flip = ByteOrder::flipBytes(norm);
		assertTrue (norm == flip);
	}
	{
		UInt8 norm = (UInt8) 0xAB;
		UInt8 flip = ByteOrder::flipBytes(norm);
		assertTrue (norm == flip);
	}
	{
		Int16 norm = (Int16) 0xAABB;
		Int16 flip = ByteOrder::flipBytes(norm);
		assertTrue (UInt16(flip) == 0xBBAA);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (flip == norm);
	}
	{
		UInt16 norm = (UInt16) 0xAABB;
		UInt16 flip = ByteOrder::flipBytes(norm);
		assertTrue (flip == 0xBBAA);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (flip == norm);
	}
	{
		Int32 norm = 0xAABBCCDD;
		Int32 flip = ByteOrder::flipBytes(norm);
		assertTrue (UInt32(flip) == 0xDDCCBBAA);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (flip == norm);
	}
	{
		UInt32 norm = 0xAABBCCDD;
		UInt32 flip = ByteOrder::flipBytes(norm);
		assertTrue (flip == 0xDDCCBBAA);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (flip == norm);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = (Int64(0x8899AABB) << 32) + 0xCCDDEEFF;
		Int64 flip = ByteOrder::flipBytes(norm);
		assertTrue (flip == (Int64(0xFFEEDDCC) << 32) + 0xBBAA9988);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (flip == norm);
	}
	{
		UInt64 norm = (UInt64(0x8899AABB) << 32) + 0xCCDDEEFF;
		UInt64 flip = ByteOrder::flipBytes(norm);
		assertTrue (flip == (UInt64(0xFFEEDDCC) << 32) + 0xBBAA9988);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (flip == norm);
	}
	#endif
}


void ByteOrderTest::testByteOrderBigEndian()
{
	//
	// all systems
	//
	{
		Int8 norm = 4;
		Int8 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt8 norm = 4;
		UInt8 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		Int8 norm = 4;
		Int8 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt8 norm = 4;
		UInt8 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
#if defined(POCO_ARCH_BIG_ENDIAN)
	//
	// big-endian systems
	//
	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm == flip);
	}
	#endif

	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm == flip);
	}
	#endif
#else
	//
	// little-endian systems
	//
	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::toBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#endif

	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::fromBigEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#endif
#endif
}


void ByteOrderTest::testByteOrderLittleEndian()
{
	// 
	// all systems
	//
	{
		Int8 norm = 4;
		Int8 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt8 norm = 4;
		UInt8 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		Int8 norm = 4;
		Int8 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt8 norm = 4;
		UInt8 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm == flip);
	}
#if defined(POCO_ARCH_LITTLE_ENDIAN)
	//
	// big-endian systems
	//
	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	#endif

	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm == flip);
	}
	#endif
#else
	//
	// little-endian systems
	//
	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::toLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#endif

	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::fromLittleEndian(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#endif
#endif
}


void ByteOrderTest::testByteOrderNetwork()
{
	{
		Int8 norm = 4;
		Int8 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt8 norm = 4;
		UInt8 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		Int8 norm = 4;
		Int8 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt8 norm = 4;
		UInt8 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
#if defined(POCO_ARCH_BIG_ENDIAN)
	//
	// big-endian systems
	//
	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm == flip);
	}
	#endif

	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm == flip);
	}
	#endif
#else
	//
	// little-endian systems
	//
	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::toNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#endif

	{
		Int16 norm = 4;
		Int16 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt16 norm = 4;
		UInt16 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		Int32 norm = 4;
		Int32 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt32 norm = 4;
		UInt32 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#if defined(POCO_HAVE_INT64)
	{
		Int64 norm = 4;
		Int64 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	{
		UInt64 norm = 4;
		UInt64 flip = ByteOrder::fromNetwork(norm);
		assertTrue (norm != flip);
		flip = ByteOrder::flipBytes(flip);
		assertTrue (norm == flip);
	}
	#endif
#endif
}


void ByteOrderTest::setUp()
{
}


void ByteOrderTest::tearDown()
{
}


CppUnit::Test* ByteOrderTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ByteOrderTest");

	CppUnit_addTest(pSuite, ByteOrderTest, testByteOrderFlip);
	CppUnit_addTest(pSuite, ByteOrderTest, testByteOrderBigEndian);
	CppUnit_addTest(pSuite, ByteOrderTest, testByteOrderLittleEndian);
	CppUnit_addTest(pSuite, ByteOrderTest, testByteOrderNetwork);

	return pSuite;
}
