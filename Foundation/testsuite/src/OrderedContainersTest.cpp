//
// OrderedContainersTest.cpp
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "OrderedContainersTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"

#ifdef POCO_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#endif // POCO_COMPILER_MSVC


using Poco::OrderedMap;
using Poco::OrderedSet;
using Poco::Exception;


namespace {

	static std::size_t nb_custom_allocs = 0;

	template<typename T>
	class custom_allocator
	{
	public:
		using value_type = T;
		using pointer = T*;
		using const_pointer = const T*;
		using reference = T &;
		using const_reference = const T &;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		using propagate_on_container_move_assignment = std::true_type;


		template<typename U>
		struct rebind
		{
			using other = custom_allocator<U>;
		};

		custom_allocator() = default;

		custom_allocator(const custom_allocator &) = default;

		template<typename U>
		custom_allocator(const custom_allocator<U> &)
		{
		}

		pointer address(reference x) const noexcept
		{
			return &x;
		}

		const_pointer address(const_reference x) const noexcept
		{
			return &x;
		}

		pointer allocate(size_type n, const void* /*hint*/ = 0)
		{
			nb_custom_allocs++;

			pointer ptr = static_cast<pointer>(std::malloc(n * sizeof(T)));
			if(ptr == nullptr)
			{
				throw std::bad_alloc();
			}

			return ptr;
		}

		void deallocate(T*p, size_type /*n*/)
		{
			std::free(p);
		}

		size_type max_size() const noexcept
		{
			return std::numeric_limits<size_type>::max() / sizeof(value_type);
		}

		template<typename U, typename... Args>
		void construct(U*p, Args &&... args)
		{
			::new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
		}

		template<typename U>
		void destroy(U*p)
		{
			p->~U();
		}
	};


	template<class T, class U>
	bool operator==(const custom_allocator<T> &, const custom_allocator<U> &)
	{
		return true;
	}

	template<class T, class U>
	bool operator!=(const custom_allocator<T> &, const custom_allocator<U> &)
	{
		return false;
	}

}


OrderedContainersTest::OrderedContainersTest(const std::string& rName): CppUnit::TestCase(rName)
{
}


OrderedContainersTest::~OrderedContainersTest()
{
}


void OrderedContainersTest::testMapInsert()
{
	testMapFuncFwd(OrderedMap, testMapInsertImpl);
}


void OrderedContainersTest::testRangeInsert()
{
	// insert x values in vector, range insert x-15 values from vector to map, check values
	const int nb_values = 1000;
	std::vector<std::pair<int, int>> values;
	for(int i = 0; i < nb_values; i++)
		values.push_back(std::make_pair(i, i+1));


	OrderedMap<int, int> map = {{-1, 0}, {-2, 0}};
	map.insert(values.begin() + 10, values.end() - 5);

	assertEquals(map.size(), 987);

	assertEquals(map.values_container()[0].first, -1);
	assertEquals(map.values_container()[0].second, 0);

	assertEquals(map.values_container()[1].first, -2);
	assertEquals(map.values_container()[1].second, 0);

	for(int i = 10, j = 2; i < nb_values - 5; i++, j++)
	{
		assertEquals(map.values_container()[j].first, i);
		assertEquals(map.values_container()[j].second, i+1);
	}

}


void OrderedContainersTest::testInsertWithHint()
{
	OrderedMap<int, int> map{{1, 0}, {2, 1}, {3, 2}};

	// Wrong hint
	assertTrue(map.insert(map.find(2), std::make_pair(3, 4)) == map.find(3));

	// Good hint
	assertTrue(map.insert(map.find(2), std::make_pair(2, 4)) == map.find(2));

	// end() hint
	assertTrue(map.insert(map.find(10), std::make_pair(2, 4)) == map.find(2));

	assertEquals(map.size(), 3);

	// end() hint, new value
	assertEquals(map.insert(map.find(10), std::make_pair(4, 3))->first, 4);

	// Wrong hint, new value
	assertEquals(map.insert(map.find(2), std::make_pair(5, 4))->first, 5);

	assertEquals(map.size(), 5);
}


void OrderedContainersTest::testEmplace()
{
	OrderedMap<std::int64_t, move_only_test> map;
	OrderedMap<std::int64_t, move_only_test>::iterator it;
	bool inserted;


	std::tie(it, inserted) = map.emplace(std::piecewise_construct,
	std::forward_as_tuple(10),
	std::forward_as_tuple(1));
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));
	assertTrue(inserted);


	std::tie(it, inserted) = map.emplace(std::piecewise_construct,
	std::forward_as_tuple(10),
	std::forward_as_tuple(3));
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));
	assertTrue(!inserted);
}


void OrderedContainersTest::testTryEmplace()
{
	OrderedMap<std::int64_t, move_only_test> map;
	OrderedMap<std::int64_t, move_only_test>::iterator it;
	bool inserted;


	std::tie(it, inserted) = map.try_emplace(10, 1);
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));
	assertTrue(inserted);


	std::tie(it, inserted) = map.try_emplace(10, 3);
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));
	assertTrue(!inserted);
}


void OrderedContainersTest::testTryEmplace2()
{
	OrderedMap<std::string, move_only_test> map;
	OrderedMap<std::string, move_only_test>::iterator it;
	bool inserted;

	const std::size_t nb_values = 1000;
	for(std::size_t i = 0; i < nb_values; i++) {
		std::tie(it, inserted) = map.try_emplace(utils::get_key<std::string>(i), i);

		assertEquals(it->first, utils::get_key<std::string>(i));
		assert(it->second == move_only_test(i));
		assertTrue(inserted);
	}
	assertEquals(map.size(), nb_values);

	for(std::size_t i = 0; i < nb_values; i++) {
		std::tie(it, inserted) = map.try_emplace(utils::get_key<std::string>(i), i + 1);

		assertEquals(it->first, utils::get_key<std::string>(i));
		assert(it->second == move_only_test(i));
		assertTrue(!inserted);
	}

	for(std::size_t i = 0; i < nb_values; i++) {
		it = map.find(utils::get_key<std::string>(i));

		assertEquals(it->first, utils::get_key<std::string>(i));
		assert(it->second == move_only_test(i));
	}
}


void OrderedContainersTest::testTryEmplaceHint()
{
	OrderedMap<std::int64_t, move_only_test> map(0);

	// end() hint, new value
	auto it = map.try_emplace(map.find(10), 10, 1);
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));

	// Good hint
	it = map.try_emplace(map.find(10), 10, 3);
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));

	// Wrong hint, new value
	it = map.try_emplace(map.find(10), 1, 3);
	assertEquals(it->first, 1);
	assert(it->second == move_only_test(3));
}


void OrderedContainersTest::testInsertOrAssign()
{
	OrderedMap<std::int64_t, move_only_test> map;
	OrderedMap<std::int64_t, move_only_test>::iterator it;
	bool inserted;


	std::tie(it, inserted) = map.insert_or_assign(10, move_only_test(1));
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));
	assertTrue(inserted);


	std::tie(it, inserted) = map.insert_or_assign(10, move_only_test(3));
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(3));
	assertTrue(!inserted);
}


void OrderedContainersTest::testInsertOrAssignHint()
{
	OrderedMap<std::int64_t, move_only_test> map(0);

	// end() hint, new value
	auto it = map.insert_or_assign(map.find(10), 10, move_only_test(1));
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(1));

	// Good hint
	it = map.insert_or_assign(map.find(10), 10, move_only_test(3));
	assertEquals(it->first, 10);
	assert(it->second == move_only_test(3));

	// Bad hint, new value
	it = map.insert_or_assign(map.find(10), 1, move_only_test(3));
	assertEquals(it->first, 1);
	assert(it->second == move_only_test(3));
}


void OrderedContainersTest::testInsertAtPosition()
{
	OrderedMap<std::string, int> map = {{"Key2", 2}, {"Key4", 4}, {"Key6", 6}};

	map.insert_at_position(map.begin(), {"Key1", 1});
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key4", 4}, {"Key6", 6}}));

	map.insert_at_position(map.nth(2), {"Key3", 3});
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key3", 3},
		{"Key4", 4}, {"Key6", 6}}));

	map.insert_at_position(map.end(), {"Key7", 7});
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key3", 3},
		{"Key4", 4}, {"Key6", 6}, {"Key7", 7}}));

	map.insert_at_position(map.nth(4), {"Key5", 5});
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key3", 3},
		{"Key4", 4}, {"Key5", 5}, {"Key6", 6}, {"Key7", 7}}));
}


void OrderedContainersTest::testTryEmplaceAtPosition()
{
	OrderedMap<std::string, int> map = {{"Key2", 2}, {"Key4", 4}, {"Key6", 6}};

	map.try_emplace_at_position(map.begin(), "Key1", 1);
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key4", 4}, {"Key6", 6}}));

	map.try_emplace_at_position(map.nth(2), "Key3", 3);
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key3", 3},
		{"Key4", 4}, {"Key6", 6}}));

	map.try_emplace_at_position(map.end(), "Key7", 7);
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key3", 3},
		{"Key4", 4}, {"Key6", 6}, {"Key7", 7}}));

	map.try_emplace_at_position(map.nth(4), "Key5", 5);
	assertTrue(map == (OrderedMap<std::string, int>{{"Key1", 1}, {"Key2", 2}, {"Key3", 3},
		{"Key4", 4}, {"Key5", 5}, {"Key6", 6}, {"Key7", 7}}));
}


void OrderedContainersTest::testRangeEraseAll()
{
	// insert x values, delete all
	using HMap = OrderedMap<std::string, std::int64_t>;

	const std::size_t nb_values = 1000;
	HMap map = utils::get_filled_hash_map<HMap>(nb_values);

	auto it = map.erase(map.begin(), map.end());
	assertTrue(it == map.end());
	assertTrue(map.empty());
}


void OrderedContainersTest::testRangeErase()
{
	// insert x values, delete all except 10 first and 10 last value
	using HMap = OrderedMap<std::string, std::int64_t>;

	const std::size_t nb_values = 1000;
	HMap map = utils::get_filled_hash_map<HMap>(nb_values);

	auto it = map.erase(map.begin() + 10, map.end() - 10);
	assertTrue(it == map.end() - 10);
	assertEquals(map.size(), 20);
	assertEquals(std::distance(map.begin(), map.end()), 20);

	for(std::size_t i = 0; i < 10; i++) {
		assertEquals(map.at(utils::get_key<std::string>(i)), utils::get_value<std::int64_t>(i));
		assertEquals(map.at(utils::get_key<std::string>(nb_values - 10 + i)),
				utils::get_value<std::int64_t>(nb_values - 10 + i));
	}
}


void OrderedContainersTest::testMapEraseLoop()
{
	testMapFuncFwd(OrderedMap, testMapEraseLoopImpl);
}


void OrderedContainersTest::testMapInsertEraseInsert()
{
	testMapFuncFwd(OrderedMap, testMapInsertEraseInsertImpl);
}


void OrderedContainersTest::testRangeEraseSameIterators()
{
	const std::size_t nb_values = 100;
	auto map = utils::get_filled_hash_map<OrderedMap<std::int64_t, std::int64_t>>(nb_values);

	OrderedMap<std::int64_t, std::int64_t>::const_iterator it_const = map.cbegin();
	std::advance(it_const, 10);

	OrderedMap<std::int64_t, std::int64_t>::iterator it_mutable = map.erase(it_const, it_const);
	assertTrue(it_const == it_mutable);
	assertTrue(map.mutable_iterator(it_const) == it_mutable);
	assertEquals(map.size(), 100);

	it_mutable.value() = -100;
	assertEquals(it_const.value(), -100);
}


void OrderedContainersTest::testUnorderedErase()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{1, 10}, {2, 20}, {3, 30},
		{4, 40}, {5, 50}, {6, 60}};
	assertEquals(map.size(), 6);


	assertEquals(map.unordered_erase(3), 1);
	assertEquals(map.size(), 5);

	assertEquals(map.unordered_erase(0), 0);
	assertEquals(map.size(), 5);

	auto it = map.begin();
	while(it != map.end()) {
		it = map.unordered_erase(it);
	}

	assertEquals(map.size(), 0);
}


void OrderedContainersTest::testCompare()
{
	const OrderedMap<std::string, int> map = {{"D", 1}, {"L", 2}, {"A", 3}};

	assertTrue(map == (OrderedMap<std::string, int>{{"D", 1}, {"L", 2}, {"A", 3}}));
	assertTrue(map != (OrderedMap<std::string, int>{{"L", 2}, {"D", 1}, {"A", 3}}));


	assertTrue(map < (OrderedMap<std::string, int>{{"D", 1}, {"L", 2}, {"B", 3}}));
	assertTrue(map <= (OrderedMap<std::string, int>{{"D", 1}, {"L", 2}, {"B", 3}}));
	assertTrue(map <= (OrderedMap<std::string, int>{{"D", 1}, {"L", 2}, {"A", 3}}));

	assertTrue(map > (OrderedMap<std::string, int>{{"D", 1}, {"K", 2}, {"A", 3}}));
	assertTrue(map >= (OrderedMap<std::string, int>{{"D", 1}, {"K", 2}, {"A", 3}}));
	assertTrue(map >= (OrderedMap<std::string, int>{{"D", 1}, {"L", 2}, {"A", 3}}));
}



void OrderedContainersTest::testClear()
{
	// insert x values, clear map, insert new values
	using HMap = OrderedMap<std::int64_t, std::int64_t>;

	const std::size_t nb_values = 1000;
	auto map = utils::get_filled_hash_map<HMap>(nb_values);

	map.clear();
	assertEquals(map.size(), 0);
	assertEquals(std::distance(map.begin(), map.end()), 0);

	map.insert({5, -5});
	map.insert({{1, -1}, {2, -1}, {4, -4}, {3, -3}});

	assertTrue(map == (HMap({{5, -5}, {1, -1}, {2, -1}, {4, -4}, {3, -3}})));
}


void OrderedContainersTest::testReverseIterator()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{1, 1}, {-2, 2}, {3, 3}};
	map[2] = 4;

	std::size_t i = 4;
	for(auto it = map.rbegin(); it != map.rend(); ++it) {
		assertEquals(it->second, i);
		i--;
	}

	i = 4;
	for(auto it = map.rcbegin(); it != map.rcend(); ++it) {
		assertEquals(it->second, i);
		i--;
	}
}


void OrderedContainersTest::testIteratorArithmetic()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{1, 10}, {2, 20}, {3, 30},
		{4, 40}, {5, 50}, {6, 60}};

	OrderedMap<std::int64_t, std::int64_t>::const_iterator it;
	OrderedMap<std::int64_t, std::int64_t>::const_iterator it2;

	it = map.cbegin();
	// it += n
	it += 3;
	assertEquals(it->second, 40);

	// it + n
	assertEquals((map.cbegin() + 3)->second, 40);
	// n + it
	assertEquals((3 + map.cbegin())->second, 40);

	it = map.cbegin() + 4;
	// it -= n
	it -= 2;
	assertEquals(it->second, 30);

	// it - n
	assertEquals((it - 1)->second, 20);

	it = map.cbegin() + 2;
	it2 = map.cbegin() + 4;
	// it - it
	assertEquals(it2 - it, 2);

	// it[n]
	assertEquals(map.cbegin()[2].second, 30);

	it = map.cbegin() + 1;
	// it[n]
	assertEquals(it[2].second, 40);

	it = map.cbegin();
	// it++
	it++;
	assertEquals((it++)->second, 20);

	it = map.cbegin();
	// ++it
	++it;
	assertEquals((++it)->second, 30);

	it = map.cend();
	// it--
	it--;
	assertEquals((it--)->second, 60);

	it = map.cend();
	// --it
	--it;
	assertEquals((--it)->second, 50);
}


void OrderedContainersTest::testIteratorComparators()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{1, 10}, {2, 20}, {3, 30},
		{4, 40}, {5, 50}, {6, 60}};

	OrderedMap<std::int64_t, std::int64_t>::const_iterator it;
	OrderedMap<std::int64_t, std::int64_t>::const_iterator it2;

	it = map.begin() + 1;
	it2 = map.end() - 1;

	assertTrue(it < it2);
	assertTrue(it <= it2);
	assertTrue(it2 > it);
	assertTrue(it2 >= it);

	it = map.begin() + 3;
	it2 = map.end() - 3;

	assertTrue(it == it2);
	assertTrue(it <= it2);
	assertTrue(it >= it2);
	assertTrue(it2 <= it);
	assertTrue(it2 >= it);
	assertTrue(!(it < it2));
	assertTrue(!(it > it2));
	assertTrue(!(it2 < it));
	assertTrue(!(it2 > it));
}


void OrderedContainersTest::testModifyValue()
{
	// insert x values, modify value of even keys, check values
	const std::size_t nb_values = 100;
	auto map = utils::get_filled_hash_map<OrderedMap<std::int64_t, std::int64_t>>(nb_values);

	for(auto it = map.begin(); it != map.end(); ++it)
	{
		if(it->first % 2 == 0) it.value() = -1;
	}

	for(auto& val : map)
	{
		if(val.first % 2 == 0)
			assertEquals(val.second, -1);
		else
			assert(val.second != -1);
	}
}


void OrderedContainersTest::testAssignOperator()
{
		OrderedMap<std::int64_t, std::int64_t> map = {{0, 10}, {-2, 20}};
		assertEquals(map.size(), 2);

		map = {{1, 3}, {2, 4}};
		assertEquals(map.size(), 2);
		assertEquals(map.at(1), 3);
		assertEquals(map.at(2), 4);
		assertTrue(map.find(0) == map.end());
}


void OrderedContainersTest::testMoveConstructor()
{
	// insert x values in map, move map into map_move, check map and map_move,
	// insert additional values in map_move, check map_move
	using HMap = OrderedMap<std::string, move_only_test>;

	const std::size_t nb_values = 100;
	HMap map = utils::get_filled_hash_map<HMap>(nb_values);
	HMap map_move(std::move(map));

	assertTrue(map_move == utils::get_filled_hash_map<HMap>(nb_values));
	assertTrue(map == (HMap()));



	for(std::size_t i = nb_values; i < nb_values*2; i++) {
		map_move.insert({utils::get_key<std::string>(i), utils::get_value<move_only_test>(i)});
	}

	assertEquals(map_move.size(), nb_values*2);
	assertTrue(map_move == utils::get_filled_hash_map<HMap>(nb_values*2));
}


void OrderedContainersTest::testMoveOperator()
{
	// insert x values in map, move map into map_move, check map and map_move,
	// insert additional values in map_move, check map_move
	using HMap = OrderedMap<std::string, move_only_test>;

	const std::size_t nb_values = 100;
	HMap map = utils::get_filled_hash_map<HMap>(nb_values);
	HMap map_move = utils::get_filled_hash_map<HMap>(1);
	map_move = std::move(map);

	assertTrue(map_move == utils::get_filled_hash_map<HMap>(nb_values));
	assertTrue(map == (HMap()));



	for(std::size_t i = nb_values; i < nb_values*2; i++) {
		map_move.insert({utils::get_key<std::string>(i), utils::get_value<move_only_test>(i)});
	}

	assertEquals(map_move.size(), nb_values*2);
	assertTrue(map_move == utils::get_filled_hash_map<HMap>(nb_values*2));
}


void OrderedContainersTest::testReassignMovedObjectMoveConstructor()
{
	using HMap = OrderedMap<std::string, std::string>;

	HMap map = {{"Key1", "Value1"}, {"Key2", "Value2"}, {"Key3", "Value3"}};
	HMap map_move(std::move(map));

	assertEquals(map_move.size(), 3);
	assertEquals(map.size(), 0);

	map = {{"Key4", "Value4"}, {"Key5", "Value5"}};
	assertTrue(map == (HMap({{"Key4", "Value4"}, {"Key5", "Value5"}})));
}


void OrderedContainersTest::testReassignMovedObjectMoveOperator()
{
	using HMap = OrderedMap<std::string, std::string>;

	HMap map = {{"Key1", "Value1"}, {"Key2", "Value2"}, {"Key3", "Value3"}};
	HMap map_move = std::move(map);

	assertEquals(map_move.size(), 3);
	assertEquals(map.size(), 0);

	map = {{"Key4", "Value4"}, {"Key5", "Value5"}};
	assertTrue(map == (HMap({{"Key4", "Value4"}, {"Key5", "Value5"}})));
}


void OrderedContainersTest::testCopyConstructorOperator()
{
		using HMap = OrderedMap<std::string, std::string, mod_hash<9>>;

	const std::size_t nb_values = 100;
	HMap map = utils::get_filled_hash_map<HMap>(nb_values);

	HMap map_copy = map;
	HMap map_copy2(map);
	HMap map_copy3;
	map_copy3 = map;

	assertTrue(map == map_copy);
	map.clear();

	assertTrue(map_copy == map_copy2);
	assertTrue(map_copy == map_copy3);
}


void OrderedContainersTest::testAt()
{
	// insert x values, use at for known and unknown values.
	OrderedMap<std::int64_t, std::int64_t> map = {{0, 10}, {-2, 20}};

	assertEquals(map.at(0), 10);
	assertEquals(map.at(-2), 20);
	try
	{
		map.at(1);
		fail("must throw out of range");
	}
	catch (std::out_of_range&) {}
}


void OrderedContainersTest::testEqualRange()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{0, 10}, {-2, 20}};

	auto it_pair = map.equal_range(0);
	assertTrue(std::distance(it_pair.first, it_pair.second) == 1);
	assertEquals(it_pair.first->second, 10);

	it_pair = map.equal_range(1);
	assertTrue(it_pair.first == it_pair.second);
	assertTrue(it_pair.first == map.end());
}


void OrderedContainersTest::testData()
{
	OrderedMap<std::int64_t, std::int64_t, std::hash<std::int64_t>, std::equal_to<std::int64_t>,
			std::allocator<std::pair<std::int64_t, std::int64_t>>,
			std::vector<std::pair<std::int64_t, std::int64_t>>> map = {{1, -1}, {2, -2}, {4, -4}, {3, -3}};

	assertTrue(map.data() == map.values_container().data());
}


void OrderedContainersTest::testAccessOperator()
{
	// insert x values, use at for known and unknown values.
	OrderedMap<std::int64_t, std::int64_t> map = {{0, 10}, {-2, 20}};

	assertEquals(map[0], 10);
	assertEquals(map[-2], 20);
	assertEquals(map[2], std::int64_t());

	assertEquals(map.size(), 3);
}


void OrderedContainersTest::testSwap()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{1, 10}, {8, 80}, {3, 30}};
	OrderedMap<std::int64_t, std::int64_t> map2 = {{4, 40}, {5, 50}};

	using std::swap;
	swap(map, map2);

	assertTrue(map == (OrderedMap<std::int64_t, std::int64_t>{{4, 40}, {5, 50}}));
	assertTrue(map2 == (OrderedMap<std::int64_t, std::int64_t>{{1, 10}, {8, 80}, {3, 30}}));
}


void OrderedContainersTest::testFrontBack()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{1, 10}, {2, 20}};
	map.insert({0, 0});

	assertTrue(map.front() == (std::pair<std::int64_t, std::int64_t>(1, 10)));
	assertTrue(map.back() == (std::pair<std::int64_t, std::int64_t>(0, 0)));


	map.clear();
	map.insert({3, 30});
	assertTrue(map.front() == (std::pair<std::int64_t, std::int64_t>(3, 30)));
	assertTrue(map.back() == (std::pair<std::int64_t, std::int64_t>(3, 30)));
}


void OrderedContainersTest::testNth()
{
	OrderedMap<std::int64_t, std::int64_t> map = {{1, 10}, {2, 20}};
	map.insert({0, 0});

	assertTrue(map.nth(0) != map.end());
	assertTrue(*map.nth(0) == (std::pair<std::int64_t, std::int64_t>(1, 10)));

	assertTrue(map.nth(1) != map.end());
	assertTrue(*map.nth(1) == (std::pair<std::int64_t, std::int64_t>(2, 20)));

	assertTrue(map.nth(2) != map.end());
	assertTrue(*map.nth(2) == (std::pair<std::int64_t, std::int64_t>(0, 0)));

	assertTrue(map.nth(3) == map.end());


	map.clear();
	assertTrue(map.nth(0) == map.end());
}


void OrderedContainersTest::testHeterogeneousLookups()
{
	struct hash_ptr
	{
		std::size_t operator()(const std::unique_ptr<int>& p) const {
			return std::hash<std::uintptr_t>()(reinterpret_cast<std::uintptr_t>(p.get()));
		}

		std::size_t operator()(std::uintptr_t p) const {
			return std::hash<std::uintptr_t>()(p);
		}

		std::size_t operator()(const int* const& p) const {
			return std::hash<std::uintptr_t>()(reinterpret_cast<std::uintptr_t>(p));
		}
	};

	struct equal_to_ptr {
		using is_transparent = std::true_type;

		bool operator()(const std::unique_ptr<int>& p1, const std::unique_ptr<int>& p2) const {
			return p1 == p2;
		}

		bool operator()(const std::unique_ptr<int>& p1, std::uintptr_t p2) const {
			return reinterpret_cast<std::uintptr_t>(p1.get()) == p2;
		}

		bool operator()(std::uintptr_t p1, const std::unique_ptr<int>& p2) const {
			return p1 == reinterpret_cast<std::uintptr_t>(p2.get());
		}

		bool operator()(const std::unique_ptr<int>& p1, const int* const& p2) const {
			return p1.get() == p2;
		}

		bool operator()(const int* const& p1, const std::unique_ptr<int>& p2) const {
			return p1 == p2.get();
		}
	};

	std::unique_ptr<int> ptr1(new int(1));
	std::unique_ptr<int> ptr2(new int(2));
	std::unique_ptr<int> ptr3(new int(3));
	int other = -1;

	const std::uintptr_t addr1 = reinterpret_cast<std::uintptr_t>(ptr1.get());
	const int* const addr2 = ptr2.get();
	const int* const addr_unknown = &other;

	OrderedMap<std::unique_ptr<int>, int, hash_ptr, equal_to_ptr> map;
	map.insert({std::move(ptr1), 4});
	map.insert({std::move(ptr2), 5});
	map.insert({std::move(ptr3), 6});

	assertEquals(map.size(), 3);

	assertEquals(map.at(addr1), 4);
	assertEquals(map.at(addr2), 5);
	try
	{
		map.at(addr_unknown);
		fail("must throw");
	}
	catch (std::out_of_range&) {}

	assertTrue(map.find(addr1) != map.end());
	assertEquals(*map.find(addr1)->first, 1);

	assertTrue(map.find(addr2) != map.end());
	assertEquals(*map.find(addr2)->first, 2);

	assertTrue(map.find(addr_unknown) == map.end());

	assertEquals(map.count(addr1), 1);
	assertEquals(map.count(addr2), 1);
	assertEquals(map.count(addr_unknown), 0);

	assertEquals(map.erase(addr1), 1);
	assertEquals(map.unordered_erase(addr2), 1);
	assertEquals(map.erase(addr_unknown), 0);
	assertEquals(map.unordered_erase(addr_unknown), 0);

	assertEquals(map.size(), 1);
}


void OrderedContainersTest::testEmptyMap()
{
	OrderedMap<std::string, int> map(0);

	assertEquals(map.size(), 0);
	assertTrue(map.empty());

	assertTrue(map.begin() == map.end());
	assertTrue(map.begin() == map.cend());
	assertTrue(map.cbegin() == map.cend());

	assertTrue(map.find("") == map.end());
	assertTrue(map.find("test") == map.end());

	assertEquals(map.count(""), 0);
	assertEquals(map.count("test"), 0);

	try
	{
		map.at("");
		fail ("must throw");
	}
	catch (std::out_of_range&) {}
	try
	{
		map.at("test");
		fail ("must throw");
	}
	catch (std::out_of_range&) {}

	auto range = map.equal_range("test");
	assertTrue(range.first == range.second);

	assertEquals(map.erase("test"), 0);
	assertTrue(map.erase(map.begin(), map.end()) == map.end());

	assertEquals(map["new value"], int{});
}


void OrderedContainersTest::testPrecalculatedHash()
{
	OrderedMap<int, int, std::hash<int>> map = {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}};
	const OrderedMap<int, int> map_const = map;

	/**
	 * find
	 */
	assertTrue(map.find(3, map.hash_function()(3)) != map.end());
	assertEquals(map.find(3, map.hash_function()(3))->second, -3);

	assertTrue(map_const.find(3, map_const.hash_function()(3)) != map_const.end());
	assertEquals(map_const.find(3, map_const.hash_function()(3))->second, -3);

	assertTrue(map.hash_function()(2) != map.hash_function()(3));
	assertTrue(map.find(3, map.hash_function()(2)) == map.end());

	/**
	 * at
	 */
	assertEquals(map.at(3, map.hash_function()(3)), -3);
	assertEquals(map_const.at(3, map_const.hash_function()(3)), -3);

	assertTrue(map.hash_function()(2) != map.hash_function()(3));
	try
	{
		map.at(3, map.hash_function()(2));
	} catch(std::out_of_range&) {}

	/**
	 * count
	 */
	assertEquals(map.count(3, map.hash_function()(3)), 1);
	assertEquals(map_const.count(3, map_const.hash_function()(3)), 1);

	assertTrue(map.hash_function()(2) != map.hash_function()(3));
	assertEquals(map.count(3, map.hash_function()(2)), 0);

	/**
	 * equal_range
	 */
	auto it_range = map.equal_range(3, map.hash_function()(3));
	assertTrue(std::distance(it_range.first, it_range.second) == 1);
	assertEquals(it_range.first->second, -3);

	auto it_range_const = map_const.equal_range(3, map_const.hash_function()(3));
	assertTrue(std::distance(it_range_const.first, it_range_const.second) == 1);
	assertEquals(it_range_const.first->second, -3);

	it_range = map.equal_range(3, map.hash_function()(2));
	assertTrue(map.hash_function()(2) != map.hash_function()(3));
	assertEquals(std::distance(it_range.first, it_range.second), 0);

	/**
	 * erase
	 */
	assertEquals(map.erase(3, map.hash_function()(3)), 1);

	assertTrue(map.hash_function()(2) != map.hash_function()(4));
	assertEquals(map.erase(4, map.hash_function()(2)), 0);
}


void OrderedContainersTest::testSetInsert()
{
	testSetFuncFwd(OrderedSet, testSetInsertImpl);
}


void OrderedContainersTest::testCustomAllocator1()
{
	nb_custom_allocs = 0;

	OrderedMap<int, int, std::hash<int>, std::equal_to<int>,
				custom_allocator<std::pair<int, int>>> map;

	const int nb_elements = 10000;
	for(int i = 0; i < nb_elements; i++)
	{
		map.insert({i, i*2});
	}

	assertTrue(nb_custom_allocs != 0);
}


void OrderedContainersTest::setUp()
{
}


void OrderedContainersTest::tearDown()
{
}


CppUnit::Test* OrderedContainersTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("OrderedContainersTest");

	CppUnit_addTest(pSuite, OrderedContainersTest, testMapInsert);
	CppUnit_addTest(pSuite, OrderedContainersTest, testRangeInsert);
	CppUnit_addTest(pSuite, OrderedContainersTest, testInsertWithHint);
	CppUnit_addTest(pSuite, OrderedContainersTest, testEmplace);
	CppUnit_addTest(pSuite, OrderedContainersTest, testTryEmplace);
	CppUnit_addTest(pSuite, OrderedContainersTest, testTryEmplace2);
	CppUnit_addTest(pSuite, OrderedContainersTest, testTryEmplaceHint);
	CppUnit_addTest(pSuite, OrderedContainersTest, testInsertOrAssign);
	CppUnit_addTest(pSuite, OrderedContainersTest, testInsertOrAssignHint);
	CppUnit_addTest(pSuite, OrderedContainersTest, testInsertAtPosition);
	CppUnit_addTest(pSuite, OrderedContainersTest, testTryEmplaceAtPosition);
	CppUnit_addTest(pSuite, OrderedContainersTest, testRangeErase);
	CppUnit_addTest(pSuite, OrderedContainersTest, testMapEraseLoop);
	CppUnit_addTest(pSuite, OrderedContainersTest, testMapInsertEraseInsert);
	CppUnit_addTest(pSuite, OrderedContainersTest, testRangeEraseAll);
	CppUnit_addTest(pSuite, OrderedContainersTest, testRangeEraseSameIterators);
	CppUnit_addTest(pSuite, OrderedContainersTest, testUnorderedErase);
	CppUnit_addTest(pSuite, OrderedContainersTest, testCompare);
	CppUnit_addTest(pSuite, OrderedContainersTest, testClear);
	CppUnit_addTest(pSuite, OrderedContainersTest, testReverseIterator);
	CppUnit_addTest(pSuite, OrderedContainersTest, testIteratorArithmetic);
	CppUnit_addTest(pSuite, OrderedContainersTest, testIteratorComparators);
	CppUnit_addTest(pSuite, OrderedContainersTest, testModifyValue);
	CppUnit_addTest(pSuite, OrderedContainersTest, testAssignOperator);
	CppUnit_addTest(pSuite, OrderedContainersTest, testMoveConstructor);
	CppUnit_addTest(pSuite, OrderedContainersTest, testMoveOperator);
	CppUnit_addTest(pSuite, OrderedContainersTest, testReassignMovedObjectMoveConstructor);
	CppUnit_addTest(pSuite, OrderedContainersTest, testReassignMovedObjectMoveOperator);
	CppUnit_addTest(pSuite, OrderedContainersTest, testCopyConstructorOperator);
	CppUnit_addTest(pSuite, OrderedContainersTest, testAt);
	CppUnit_addTest(pSuite, OrderedContainersTest, testEqualRange);
	CppUnit_addTest(pSuite, OrderedContainersTest, testData);
	CppUnit_addTest(pSuite, OrderedContainersTest, testAccessOperator);
	CppUnit_addTest(pSuite, OrderedContainersTest, testSwap);
	CppUnit_addTest(pSuite, OrderedContainersTest, testFrontBack);
	CppUnit_addTest(pSuite, OrderedContainersTest, testNth);
	CppUnit_addTest(pSuite, OrderedContainersTest, testHeterogeneousLookups);
	CppUnit_addTest(pSuite, OrderedContainersTest, testEmptyMap);
	CppUnit_addTest(pSuite, OrderedContainersTest, testPrecalculatedHash);
	CppUnit_addTest(pSuite, OrderedContainersTest, testSetInsert);
	CppUnit_addTest(pSuite, OrderedContainersTest, testCustomAllocator1);

	return pSuite;
}

#ifdef POCO_COMPILER_MSVC
#pragma warning(pop)
#endif // POCO_COMPILER_MSVC
