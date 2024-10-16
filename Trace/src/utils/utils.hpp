#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <ios>
#include <memory>
#include <new>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "utils/common.hpp"
#include "utils/error.hpp"
#include "utils/microfmt.hpp"

#if IS_WINDOWS
 #include <windows.h>
 #include <io.h>
#else
 #include <sys/stat.h>
 #include <unistd.h>
#endif

namespace cpptrace {
namespace detail {
    inline bool isatty(int fd) {
        #if IS_WINDOWS
         return _isatty(fd);
        #else
         return ::isatty(fd);
        #endif
    }

    inline int fileno(std::FILE* stream) {
        #if IS_WINDOWS
         return _fileno(stream);
        #else
         return ::fileno(stream);
        #endif
    }

    inline std::vector<std::string> split(const std::string& str, const std::string& delims) {
        std::vector<std::string> vec;
        std::size_t old_pos = 0;
        std::size_t pos = 0;
        while((pos = str.find_first_of(delims, old_pos)) != std::string::npos) {
            vec.emplace_back(str.substr(old_pos, pos - old_pos));
            old_pos = pos + 1;
        }
        vec.emplace_back(str.substr(old_pos));
        return vec;
    }

    template<typename C>
    inline std::string join(const C& container, const std::string& delim) {
        auto iter = std::begin(container);
        auto end = std::end(container);
        std::string str;
        if(std::distance(iter, end) > 0) {
            str += *iter;
            while(++iter != end) {
                str += delim;
                str += *iter;
            }
        }
        return str;
    }

    // first value in a sorted range such that *it <= value
    template<typename ForwardIt, typename T>
    ForwardIt first_less_than_or_equal(ForwardIt begin, ForwardIt end, const T& value) {
        auto it = std::upper_bound(begin, end, value);
        // it is first > value, we want first <= value
        if(it != begin) {
            return --it;
        }
        return end;
    }

    // first value in a sorted range such that *it <= value
    template<typename ForwardIt, typename T, typename Compare>
    ForwardIt first_less_than_or_equal(ForwardIt begin, ForwardIt end, const T& value, Compare compare) {
        auto it = std::upper_bound(begin, end, value, compare);
        // it is first > value, we want first <= value
        if(it != begin) {
            return --it;
        }
        return end;
    }

    constexpr const char* const whitespace = " \t\n\r\f\v";

    inline std::string trim(const std::string& str) {
        if(str.empty()) {
            return "";
        }
        const std::size_t left = str.find_first_not_of(whitespace);
        const std::size_t right = str.find_last_not_of(whitespace) + 1;
        return str.substr(left, right - left);
    }

    inline bool is_little_endian() {
        std::uint16_t num = 0x1;
        const auto* ptr = (std::uint8_t*)&num;
        return ptr[0] == 1;
    }

    // Modified from
    // https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
    template<typename T, std::size_t N>
    struct byte_swapper;

    template<typename T>
    struct byte_swapper<T, 1> {
        T operator()(T val) {
            return val;
        }
    };

    template<typename T>
    struct byte_swapper<T, 2> {
        T operator()(T val) {
            return (((val >> 8) & 0xff) | ((val & 0xff) << 8));
        }
    };

    template<typename T>
    struct byte_swapper<T, 4> {
        T operator()(T val) {
            return (((val & 0xff000000) >> 24) |
                    ((val & 0x00ff0000) >>  8) |
                    ((val & 0x0000ff00) <<  8) |
                    ((val & 0x000000ff) << 24));
        }
    };

    template<typename T>
    struct byte_swapper<T, 8> {
        T operator()(T val) {
            return (((val & 0xff00000000000000ULL) >> 56) |
                    ((val & 0x00ff000000000000ULL) >> 40) |
                    ((val & 0x0000ff0000000000ULL) >> 24) |
                    ((val & 0x000000ff00000000ULL) >> 8 ) |
                    ((val & 0x00000000ff000000ULL) << 8 ) |
                    ((val & 0x0000000000ff0000ULL) << 24) |
                    ((val & 0x000000000000ff00ULL) << 40) |
                    ((val & 0x00000000000000ffULL) << 56));
        }
    };

    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    T byteswap(T value) {
        return byte_swapper<T, sizeof(T)>{}(value);
    }

    inline void enable_virtual_terminal_processing_if_needed() noexcept {
        // enable colors / ansi processing if necessary
        #if IS_WINDOWS
         // https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#example-of-enabling-virtual-terminal-processing
         #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
          constexpr DWORD ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x4;
         #endif
         HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
         DWORD dwMode = 0;
         if(hOut == INVALID_HANDLE_VALUE) return;
         if(!GetConsoleMode(hOut, &dwMode)) return;
         if(dwMode != (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
         if(!SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) return;
        #endif
    }

    constexpr unsigned n_digits(unsigned value) noexcept {
        return value < 10 ? 1 : 1 + n_digits(value / 10);
    }
    static_assert(n_digits(1) == 1, "n_digits utility producing the wrong result");
    static_assert(n_digits(9) == 1, "n_digits utility producing the wrong result");
    static_assert(n_digits(10) == 2, "n_digits utility producing the wrong result");
    static_assert(n_digits(11) == 2, "n_digits utility producing the wrong result");
    static_assert(n_digits(1024) == 4, "n_digits utility producing the wrong result");

    struct nullopt_t {};

    static constexpr nullopt_t nullopt;

    template<
        typename T,
        typename std::enable_if<!std::is_same<typename std::decay<T>::type, void>::value, int>::type = 0
    >
    class optional {
        union {
            char x;
            T uvalue;
        };

        bool holds_value = false;

    public:
        optional() noexcept {}

        optional(nullopt_t) noexcept {}

        ~optional() {
            reset();
        }

        optional(const optional& other) : holds_value(other.holds_value) {
            if(holds_value) {
                new (static_cast<void*>(std::addressof(uvalue))) T(other.uvalue);
            }
        }

        optional(optional&& other)
            noexcept(std::is_nothrow_move_constructible<T>::value)
            : holds_value(other.holds_value)
        {
            if(holds_value) {
                new (static_cast<void*>(std::addressof(uvalue))) T(std::move(other.uvalue));
            }
        }

        optional& operator=(const optional& other) {
            optional copy(other);
            swap(copy);
            return *this;
        }

        optional& operator=(optional&& other)
            noexcept(std::is_nothrow_move_assignable<T>::value && std::is_nothrow_move_constructible<T>::value)
        {
            reset();
            if(other.holds_value) {
                new (static_cast<void*>(std::addressof(uvalue))) T(std::move(other.uvalue));
                holds_value = true;
            }
            return *this;
        }

        template<
            typename U = T,
            typename std::enable_if<!std::is_same<typename std::decay<U>::type, optional<T>>::value, int>::type = 0
        >
        optional(U&& value) : holds_value(true) {
            new (static_cast<void*>(std::addressof(uvalue))) T(std::forward<U>(value));
        }

        template<
            typename U = T,
            typename std::enable_if<!std::is_same<typename std::decay<U>::type, optional<T>>::value, int>::type = 0
        >
        optional& operator=(U&& value) {
            optional o(std::forward<U>(value));
            swap(o);
            return *this;
        }

        optional& operator=(nullopt_t) noexcept {
            reset();
            return *this;
        }

        void swap(optional& other) noexcept {
            if(holds_value && other.holds_value) {
                std::swap(uvalue, other.uvalue);
            } else if(holds_value && !other.holds_value) {
                new (&other.uvalue) T(std::move(uvalue));
                uvalue.~T();
            } else if(!holds_value && other.holds_value) {
                new (static_cast<void*>(std::addressof(uvalue))) T(std::move(other.uvalue));
                other.uvalue.~T();
            }
            std::swap(holds_value, other.holds_value);
        }

        bool has_value() const {
            return holds_value;
        }

        explicit operator bool() const {
            return holds_value;
        }

        void reset() {
            if(holds_value) {
                uvalue.~T();
            }
            holds_value = false;
        }

        NODISCARD T& unwrap() & {
            ASSERT(holds_value, "Optional does not contain a value");
            return uvalue;
        }

        NODISCARD const T& unwrap() const & {
            ASSERT(holds_value, "Optional does not contain a value");
            return uvalue;
        }

        NODISCARD T&& unwrap() && {
            ASSERT(holds_value, "Optional does not contain a value");
            return std::move(uvalue);
        }

        NODISCARD const T&& unwrap() const && {
            ASSERT(holds_value, "Optional does not contain a value");
            return std::move(uvalue);
        }

        template<typename U>
        NODISCARD T value_or(U&& default_value) const & {
            return holds_value ? uvalue : static_cast<T>(std::forward<U>(default_value));
        }

        template<typename U>
        NODISCARD T value_or(U&& default_value) && {
            return holds_value ? std::move(uvalue) : static_cast<T>(std::forward<U>(default_value));
        }
    };

    extern std::atomic_bool absorb_trace_exceptions;

    template<typename T, typename E, typename std::enable_if<!std::is_same<T, E>::value, int>::type = 0>
    class Result {
        union {
            T value_;
            E error_;
        };
        enum class member { value, error };
        member active;
    public:
        Result(T&& value) : value_(std::move(value)), active(member::value) {}
        Result(E&& error) : error_(std::move(error)), active(member::error) {
            if(!absorb_trace_exceptions.load()) {
                std::fprintf(stderr, "%s\n", unwrap_error().what());
            }
        }
        Result(T& value) : value_(T(value)), active(member::value) {}
        Result(E& error) : error_(E(error)), active(member::error) {
            if(!absorb_trace_exceptions.load()) {
                std::fprintf(stderr, "%s\n", unwrap_error().what());
            }
        }
        Result(Result&& other) : active(other.active) {
            if(other.active == member::value) {
                new (&value_) T(std::move(other.value_));
            } else {
                new (&error_) E(std::move(other.error_));
            }
        }
        ~Result() {
            if(active == member::value) {
                value_.~T();
            } else {
                error_.~E();
            }
        }

        bool has_value() const {
            return active == member::value;
        }

        bool is_error() const {
            return active == member::error;
        }

        explicit operator bool() const {
            return has_value();
        }

        NODISCARD optional<T> value() const & {
            return has_value() ? value_ : nullopt;
        }

        NODISCARD optional<E> error() const & {
            return is_error() ? error_ : nullopt;
        }

        NODISCARD optional<T> value() && {
            return has_value() ? std::move(value_) : nullopt;
        }

        NODISCARD optional<E> error() && {
            return is_error() ? std::move(error_) : nullopt;
        }

        NODISCARD T& unwrap_value() & {
            ASSERT(has_value(), "Result does not contain a value");
            return value_;
        }

        NODISCARD const T& unwrap_value() const & {
            ASSERT(has_value(), "Result does not contain a value");
            return value_;
        }

        NODISCARD T unwrap_value() && {
            ASSERT(has_value(), "Result does not contain a value");
            return std::move(value_);
        }

        NODISCARD E& unwrap_error() & {
            ASSERT(is_error(), "Result does not contain an error");
            return error_;
        }

        NODISCARD const E& unwrap_error() const & {
            ASSERT(is_error(), "Result does not contain an error");
            return error_;
        }

        NODISCARD E unwrap_error() && {
            ASSERT(is_error(), "Result does not contain an error");
            return std::move(error_);
        }

        template<typename U>
        NODISCARD T value_or(U&& default_value) const & {
            return has_value() ? value_ : static_cast<T>(std::forward<U>(default_value));
        }

        template<typename U>
        NODISCARD T value_or(U&& default_value) && {
            return has_value() ? std::move(value_) : static_cast<T>(std::forward<U>(default_value));
        }

        void drop_error() const {
            if(is_error()) {
                std::fprintf(stderr, "%s\n", unwrap_error().what());
            }
        }
    };

    struct monostate {};

    // TODO: Re-evaluate use of off_t
    template<typename T, typename std::enable_if<std::is_trivial<T>::value, int>::type = 0>
    Result<T, internal_error> load_bytes(std::FILE* object_file, off_t offset) {
        T object;
        if(std::fseek(object_file, offset, SEEK_SET) != 0) {
            return internal_error("fseek error");
        }
        if(std::fread(&object, sizeof(T), 1, object_file) != 1) {
            return internal_error("fread error");
        }
        return object;
    }

    // shamelessly stolen from stackoverflow
    inline bool directory_exists(const std::string& path) {
        #if IS_WINDOWS
         DWORD dwAttrib = GetFileAttributesA(path.c_str());
         return dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
        #else
         struct stat sb;
         return stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
        #endif
    }

    inline std::string basename(const std::string& path) {
        // Assumes no trailing /'s
        auto pos = path.rfind('/');
        if(pos == std::string::npos) {
            return path;
        } else {
            return path.substr(pos + 1);
        }
    }

    // A way to cast to unsigned long long without "warning: useless cast to type"
    template<typename T>
    unsigned long long to_ull(T t) {
        return static_cast<unsigned long long>(t);
    }
    template<typename T>
    frame_ptr to_frame_ptr(T t) {
        return static_cast<frame_ptr>(t);
    }

    // A way to cast to U without "warning: useless cast to type"
    template<typename U, typename V>
    U to(V v) {
        return static_cast<U>(v);
    }

    // TODO: Rework some stuff here. Not sure deleters should be optional or moved.
    // Also allow file_wrapper file = std::fopen(object_path.c_str(), "rb");
    template<
        typename T,
        typename D
        // workaround a msvc bug https://developercommunity.visualstudio.com/t/MSVC-1938331290-preview-fails-to-comp/10505565
        #if !defined(_MSC_VER) || _MSC_VER != 1938
         ,
         typename std::enable_if<
             std::is_same<decltype(std::declval<D>()(std::declval<T>())), void>::value,
             int
         >::type = 0,
         typename std::enable_if<
             std::is_standard_layout<T>::value && std::is_trivial<T>::value,
             int
         >::type = 0,
         typename std::enable_if<
             std::is_nothrow_move_constructible<T>::value,
             int
         >::type = 0
        #endif
    >
    class raii_wrapper {
        T obj;
        optional<D> deleter;
    public:
        raii_wrapper(T obj, D deleter) : obj(obj), deleter(deleter) {}
        raii_wrapper(raii_wrapper&& other) noexcept : obj(std::move(other.obj)), deleter(std::move(other.deleter)) {
            other.deleter = nullopt;
        }
        raii_wrapper(const raii_wrapper&) = delete;
        raii_wrapper& operator=(raii_wrapper&&) = delete;
        raii_wrapper& operator=(const raii_wrapper&) = delete;
        ~raii_wrapper() {
            if(deleter.has_value()) {
                deleter.unwrap()(obj);
            }
        }
        operator T&() {
            return obj;
        }
        operator const T&() const {
            return obj;
        }
        T& get() {
            return obj;
        }
        const T& get() const {
            return obj;
        }
    };

    template<
        typename T,
        typename D
        // workaround a msvc bug https://developercommunity.visualstudio.com/t/MSVC-1938331290-preview-fails-to-comp/10505565
        #if !defined(_MSC_VER) || _MSC_VER != 1938
         ,
         typename std::enable_if<
             std::is_same<decltype(std::declval<D>()(std::declval<T>())), void>::value,
             int
         >::type = 0,
         typename std::enable_if<
             std::is_standard_layout<T>::value && std::is_trivial<T>::value,
             int
         >::type = 0
        #endif
    >
    raii_wrapper<typename std::remove_reference<T>::type, D> raii_wrap(T obj, D deleter) {
        return raii_wrapper<typename std::remove_reference<T>::type, D>(obj, deleter);
    }

    inline void file_deleter(std::FILE* ptr) {
        if(ptr) {
            fclose(ptr);
        }
    }

    using file_wrapper = raii_wrapper<std::FILE*, void(*)(std::FILE*)>;

    template<typename T>
    class maybe_owned {
        std::unique_ptr<T> owned;
        T* ptr;
    public:
        maybe_owned(T* ptr) : ptr(ptr) {}
        maybe_owned(std::unique_ptr<T>&& owned) : owned(std::move(owned)), ptr(this->owned.get()) {}
        T* operator->() {
            return ptr;
        }
    };
}
}

#endif
