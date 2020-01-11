//
// OrderedContainersTest.h
//
// Definition of the OrderedContainersTest class.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef OrderedContainersTest_INCLUDED
#define OrderedContainersTest_INCLUDED


#include "Poco/Foundation.h"
#include "CppUnit/TestCase.h"
#include "Poco/OrderedMap.h"
#include "Poco/OrderedSet.h"
#include "ordered_map_util.h"
#include <tuple>
#include <deque>

#ifdef POCO_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#endif // POCO_COMPILER_MSVC

class OrderedContainersTest: public CppUnit::TestCase
{
public:
	OrderedContainersTest(const std::string& name);
	~OrderedContainersTest();

	void testMapInsert();
	void testRangeInsert();
	void testInsertWithHint();
	void testEmplace();
	void testTryEmplace();
	void testTryEmplace2();
	void testTryEmplaceHint();
	void testInsertOrAssign();
	void testInsertOrAssignHint();
	void testInsertAtPosition();
	void testTryEmplaceAtPosition();
	void testRangeErase();
	void testMapEraseLoop();
	void testMapInsertEraseInsert();
	void testRangeEraseAll();
	void testRangeEraseSameIterators();
	void testUnorderedErase();
	void testCompare();
	void testClear();
	void testReverseIterator();
	void testIteratorArithmetic();
	void testIteratorComparators();
	void testModifyValue();
	void testAssignOperator();
	void testMoveConstructor();
	void testMoveOperator();
	void testReassignMovedObjectMoveConstructor();
	void testReassignMovedObjectMoveOperator();
	void testCopyConstructorOperator();
	void testAt();
	void testEqualRange();
	void testData();
	void testAccessOperator();
	void testSwap();
	void testFrontBack();
	void testNth();
	void testHeterogeneousLookups();
	void testEmptyMap();
	void testPrecalculatedHash();

	void testSetInsert();

	void testCustomAllocator1();

	void setUp();
	void tearDown();

	static CppUnit::Test* suite();

private:

#define testMapFuncFwd(cont, func) {                                                                \
									func<cont<std::int64_t, std::int64_t>>();                       \
									func<cont<std::int64_t, std::int64_t,                           \
											std::hash<std::int64_t>, std::equal_to<std::int64_t>,   \
											std::allocator<std::pair<std::int64_t, std::int64_t>>,  \
											std::deque<std::pair<std::int64_t, std::int64_t>>>>();  \
									func<cont<std::int64_t, std::int64_t,                           \
											std::hash<std::int64_t>, std::equal_to<std::int64_t>,   \
											std::allocator<std::pair<std::int64_t, std::int64_t>>,  \
											std::vector<std::pair<std::int64_t, std::int64_t>>>>(); \
									func<cont<std::string, std::string>>();                         \
									func<cont<std::string, std::string, mod_hash<9>>>();            \
								}
// needs significant CppUnit or test changes, maybe one day ...
//func<cont<move_only_test, move_only_test, mod_hash<9>>>();

#define testSetFuncFwd(cont, func) {                                                 \
                                    func<cont<std::int64_t, std::hash<std::int64_t>, \
                                                std::equal_to<std::int64_t>,         \
                                                std::allocator<std::int64_t>,        \
                                                std::deque<std::int64_t>>>();        \
                                    func<cont<std::int64_t, std::hash<std::int64_t>, \
                                            std::equal_to<std::int64_t>,             \
                                            std::allocator<std::int64_t>,            \
                                            std::vector<std::int64_t>>>();           \
                                    func<cont<std::int64_t, mod_hash<9>>>();         \
                                    func<cont<std::string>>();                       \
                                    func<cont<std::string, mod_hash<9>>>();          \
                                    }

	template <typename T>
	void testMapInsertImpl()
	{
		// insert x values, insert them again, check values through find, check order through iterator
		using key_tt = typename T::key_type;
		using value_tt = typename T::mapped_type;

		const std::size_t nb_values = 1000;

		T map;
		typename T::iterator it;
		bool inserted;

		for(std::size_t i = 0; i < nb_values; i++)
		{
			// Avoid sequential values by splitting the values with modulo
			const std::size_t insert_val = (i%2 == 0) ? i : nb_values + i;
			std::tie(it, inserted) = map.insert({utils::get_key<key_tt>(insert_val),
					utils::get_value<value_tt>(insert_val)});

			assertEquals(it->first, utils::get_key<key_tt>(insert_val));
			assertEquals(it->second, utils::get_value<value_tt>(insert_val));
			assertTrue(inserted);
		}
		assertEquals(map.size(), nb_values);

		for(std::size_t i = 0; i < nb_values; i++)
		{
			const std::size_t insert_val = (i%2 == 0)?i:nb_values + i;
			std::tie(it, inserted) = map.insert({utils::get_key<key_tt>(insert_val),
					utils::get_value<value_tt>(insert_val + 1)});

			assertEquals(it->first, utils::get_key<key_tt>(insert_val));
			assertEquals(it->second, utils::get_value<value_tt>(insert_val));
			assertTrue(!inserted);
		}
		assertEquals(map.size(), nb_values);

		for(std::size_t i = 0; i < nb_values; i++)
		{
			const std::size_t insert_val = (i%2 == 0)?i:nb_values + i;
			it = map.find(utils::get_key<key_tt>(insert_val));

			assertEquals(it->first, utils::get_key<key_tt>(insert_val));
			assertEquals(it->second, utils::get_value<value_tt>(insert_val));
		}

		std::size_t i = 0;
		for(const auto& key_value: map)
		{
			const std::size_t insert_val = (i%2 == 0)?i:nb_values + i;

			assertEquals(key_value.first, utils::get_key<key_tt>(insert_val));
			assertEquals(key_value.second, utils::get_value<value_tt>(insert_val));

			i++;
		}
	}

	template <typename T>
	void testMapEraseLoopImpl()
	{
		// insert x values, delete all one by one
		std::size_t nb_values = 1000;

		T map = utils::get_filled_hash_map<T>(nb_values);
		T map2 = utils::get_filled_hash_map<T>(nb_values);

		auto it = map.begin();
		// Use second map to check for key after delete as we may not copy the key with move-only types.
		auto it2 = map2.begin();
		while(it != map.end()) {
			it = map.erase(it);
			--nb_values;

			assertEquals(map.count(it2->first), 0);
			assertEquals(map.size(), nb_values);
			++it2;
		}

		assertTrue(map.empty());
	}

	template <typename T>
	void testMapInsertEraseInsertImpl()
	{
		// insert x/2 values, delete x/4 values, insert x/2 values, find each value, check order of values
		using key_tt = typename T::key_type; using value_tt = typename T:: mapped_type;

		const std::size_t nb_values = 2000;
		T map;
		typename T::iterator it;
		bool inserted;

		// Insert nb_values/2
		for(std::size_t i = 0; i < nb_values/2; i++)
		{
			std::tie(it, inserted) = map.insert({utils::get_key<key_tt>(i), utils::get_value<value_tt>(i)});

			assertEquals(it->first, utils::get_key<key_tt>(i));
			assertEquals(it->second, utils::get_value<value_tt>(i));
			assertTrue(inserted);
		}
		assertEquals(map.size(), nb_values/2);


		// Delete nb_values/4
		for(std::size_t i = 0; i < nb_values/2; i++)
		{
			if(i%2 == 0) {
				assertEquals(map.erase(utils::get_key<key_tt>(i)), 1);
			}
		}
		assertEquals(map.size(), nb_values/4);


		// Insert nb_values/2
		for(std::size_t i = nb_values/2; i < nb_values; i++)
		{
			std::tie(it, inserted) = map.insert({utils::get_key<key_tt>(i), utils::get_value<value_tt>(i)});

			assertEquals(it->first, utils::get_key<key_tt>(i));
			assertEquals(it->second, utils::get_value<value_tt>(i));
			assertTrue(inserted);
		}
		assertEquals(map.size(), nb_values-nb_values/4);


		// Find
		for(std::size_t i = 0; i < nb_values; i++)
		{
			if(i%2 == 0 && i < nb_values/2)
			{
				it = map.find(utils::get_key<key_tt>(i));
				assertTrue(it == map.cend());
			}
			else {
				it = map.find(utils::get_key<key_tt>(i));
				assertEquals(it->first, utils::get_key<key_tt>(i));
				assertEquals(it->second, utils::get_value<value_tt>(i));
			}
		}

		// Check order
		std::size_t i = 1;
		for(const auto& key_value: map)
		{
			if(i < nb_values/2)
			{
				assertEquals(key_value.first, utils::get_key<key_tt>(i));
				assertEquals(key_value.second, utils::get_value<value_tt>(i));

				i = std::min(i+2, nb_values/2);
			}
			else
			{
				assertEquals(key_value.first, utils::get_key<key_tt>(i));
				assertEquals(key_value.second, utils::get_value<value_tt>(i));

				i++;
			}
		}
	}

	template <typename T>
	void testSetInsertImpl()
	{
		// insert x values, insert them again, check values through find, check order through iterator
		using key_t = typename T::key_type;

		const std::size_t nb_values = 1000;

		T set;
		typename T::iterator it;
		bool inserted;

		for(std::size_t i = 0; i < nb_values; i++)
		{
			const std::size_t insert_val = (i%2 == 0)?i:nb_values + i;
			std::tie(it, inserted) = set.insert(utils::get_key<key_t>(insert_val));

			assertEquals(*it, utils::get_key<key_t>(insert_val));
			assertTrue(inserted);
		}
		assertEquals(set.size(), nb_values);

		for(std::size_t i = 0; i < nb_values; i++)
		{
			const std::size_t insert_val = (i%2 == 0)?i:nb_values + i;
			std::tie(it, inserted) = set.insert(utils::get_key<key_t>(insert_val));

			assertEquals(*it, utils::get_key<key_t>(insert_val));
			assertTrue(!inserted);
		}
		assertEquals(set.size(), nb_values);

		for(std::size_t i = 0; i < nb_values; i++)
		{
			const std::size_t insert_val = (i%2 == 0)?i:nb_values + i;
			it = set.find(utils::get_key<key_t>(insert_val));

			assertEquals(*it, utils::get_key<key_t>(insert_val));
		}

		std::size_t i = 0;
		for(const auto& value: set)
		{
			const std::size_t insert_val = (i%2 == 0)?i:nb_values + i;

			assertEquals(value, utils::get_key<key_t>(insert_val));

			i++;
		}
	}
};

#ifdef POCO_COMPILER_MSVC
#pragma warning(pop)
#endif // POCO_COMPILER_MSVC


#endif // OrderedContainersTest_INCLUDED
