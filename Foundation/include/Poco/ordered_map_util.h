/**
* MIT License
*
* Copyright (c) 2017 Tessil
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef TSL_UTILS_H
#define TSL_UTILS_H


#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <utility>


template<unsigned int MOD>
class mod_hash {
public:
	template<typename T>
	std::size_t operator()(const T&  value) const {
		return std::hash<T>()(value) % MOD;
	}
};


class move_only_test {
public:
	explicit move_only_test(std::int64_t value) : m_value(new std::int64_t(value)) {
	}

	move_only_test(const move_only_test&) = delete;
	move_only_test(move_only_test&&) = default;
	move_only_test& operator=(const move_only_test&) = delete;
	move_only_test& operator=(move_only_test&&) = default;

	friend std::ostream& operator<<(std::ostream& stream, const move_only_test& value) {
		if (value.m_value == nullptr) {
			stream << "null";
		}
		else {
			stream << *value.m_value;
		}

		return stream;
	}

	friend bool operator==(const move_only_test& lhs, const move_only_test& rhs) {
		if (lhs.m_value == nullptr || rhs.m_value == nullptr) {
			return lhs.m_value == nullptr && rhs.m_value == nullptr;
		}
		else {
			return *lhs.m_value == *rhs.m_value;
		}
	}

	friend bool operator!=(const move_only_test& lhs, const move_only_test& rhs) {
		return !(lhs == rhs);
	}

	std::int64_t value() const {
		return *m_value;
	}

private:
	std::unique_ptr<std::int64_t> m_value;
};


namespace std {
	template<>
	struct hash<move_only_test> {
		std::size_t operator()(const move_only_test& val) const {
			return std::hash<std::int64_t>()(val.value());
		}
	};
}


class utils {
public:
	template<typename T>
	static T get_key(std::size_t counter);

	template<typename T>
	static T get_value(std::size_t counter);

	template<typename HMap>
	static HMap get_filled_hash_map(std::size_t nb_elements);
};



template<>
inline std::int64_t utils::get_key<std::int64_t>(std::size_t counter) {
	return counter;
}

template<>
inline std::string utils::get_key<std::string>(std::size_t counter) {
	return "Key " + std::to_string(counter);
}

template<>
inline move_only_test utils::get_key<move_only_test>(std::size_t counter) {
	return move_only_test(counter);
}




template<>
inline std::int64_t utils::get_value<std::int64_t>(std::size_t counter) {
	return counter * 2;
}

template<>
inline std::string utils::get_value<std::string>(std::size_t counter) {
	return "Value " + std::to_string(counter);
}

template<>
inline move_only_test utils::get_value<move_only_test>(std::size_t counter) {
	return move_only_test(counter * 2);
}



template<typename HMap>
inline HMap utils::get_filled_hash_map(std::size_t nb_elements) {
	using key_tt = typename HMap::key_type; using value_tt = typename HMap::mapped_type;

	HMap map;
	map.reserve(nb_elements);

	for (std::size_t i = 0; i < nb_elements; i++) {
		map.insert({ utils::get_key<key_tt>(i), utils::get_value<value_tt>(i) });
	}

	return map;
}

#endif
