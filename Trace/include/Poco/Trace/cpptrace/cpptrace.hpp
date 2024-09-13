#ifndef CPPTRACE_HPP
#define CPPTRACE_HPP

#include <cstdint>
#include <exception>
#include <limits>
#include <iosfwd>
#include <string>
#include <system_error>
#include <type_traits>
#include <utility>
#include <vector>

#ifdef _WIN32
#define CPPTRACE_EXPORT_ATTR __declspec(dllexport)
#define CPPTRACE_IMPORT_ATTR __declspec(dllimport)
#else
#define CPPTRACE_EXPORT_ATTR __attribute__((visibility("default")))
#define CPPTRACE_IMPORT_ATTR __attribute__((visibility("default")))
#endif

#ifdef CPPTRACE_STATIC_DEFINE
#  define CPPTRACE_EXPORT
#  define CPPTRACE_NO_EXPORT
#else
#  ifndef CPPTRACE_EXPORT
#    ifdef cpptrace_lib_EXPORTS
        /* We are building this library */
#      define CPPTRACE_EXPORT CPPTRACE_EXPORT_ATTR
#    else
        /* We are using this library */
#      define CPPTRACE_EXPORT CPPTRACE_IMPORT_ATTR
#    endif
#  endif
#endif

#ifndef CPPTRACE_NO_STD_FORMAT
 #if __cplusplus >= 202002L
  #ifdef __has_include
   #if __has_include(<format>)
    #define CPPTRACE_STD_FORMAT
    #include <format>
   #endif
  #endif
 #endif
#endif

#ifdef _MSC_VER
 #define CPPTRACE_FORCE_NO_INLINE __declspec(noinline)
#else
 #define CPPTRACE_FORCE_NO_INLINE __attribute__((noinline))
#endif

#ifdef _MSC_VER
#pragma warning(push)
// warning C4251: using non-dll-exported type in dll-exported type, firing on std::vector<frame_ptr> and others for some
// reason
// 4275 is the same thing but for base classes
#pragma warning(disable: 4251; disable: 4275)
#endif

namespace cpptrace {
    struct object_trace;
    struct stacktrace;

    // Some type sufficient for an instruction pointer, currently always an alias to std::uintptr_t
    using frame_ptr = std::uintptr_t;

    struct CPPTRACE_EXPORT raw_trace {
        std::vector<frame_ptr> frames;
        static raw_trace current(std::size_t skip = 0);
        static raw_trace current(std::size_t skip, std::size_t max_depth);
        object_trace resolve_object_trace() const;
        stacktrace resolve() const;
        void clear();
        bool empty() const noexcept;

        using iterator = std::vector<frame_ptr>::iterator;
        using const_iterator = std::vector<frame_ptr>::const_iterator;
        inline iterator begin() noexcept { return frames.begin(); }
        inline iterator end() noexcept { return frames.end(); }
        inline const_iterator begin() const noexcept { return frames.begin(); }
        inline const_iterator end() const noexcept { return frames.end(); }
        inline const_iterator cbegin() const noexcept { return frames.cbegin(); }
        inline const_iterator cend() const noexcept { return frames.cend(); }
    };

    struct CPPTRACE_EXPORT object_frame {
        frame_ptr raw_address;
        frame_ptr object_address;
        std::string object_path;
    };

    struct CPPTRACE_EXPORT object_trace {
        std::vector<object_frame> frames;
        static object_trace current(std::size_t skip = 0);
        static object_trace current(std::size_t skip, std::size_t max_depth);
        stacktrace resolve() const;
        void clear();
        bool empty() const noexcept;

        using iterator = std::vector<object_frame>::iterator;
        using const_iterator = std::vector<object_frame>::const_iterator;
        inline iterator begin() noexcept { return frames.begin(); }
        inline iterator end() noexcept { return frames.end(); }
        inline const_iterator begin() const noexcept { return frames.begin(); }
        inline const_iterator end() const noexcept { return frames.end(); }
        inline const_iterator cbegin() const noexcept { return frames.cbegin(); }
        inline const_iterator cend() const noexcept { return frames.cend(); }
    };

    // This represents a nullable integer type
    // The max value of the type is used as a sentinel
    // This is used over std::optional because the library is C++11 and also std::optional is a bit heavy-duty for this
    // use.
    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    struct nullable {
        T raw_value;
        nullable& operator=(T value) {
            raw_value = value;
            return *this;
        }
        bool has_value() const noexcept {
            return raw_value != (std::numeric_limits<T>::max)();
        }
        T& value() noexcept {
            return raw_value;
        }
        const T& value() const noexcept {
            return raw_value;
        }
        T value_or(T alternative) const noexcept {
            return has_value() ? raw_value : alternative;
        }
        void swap(nullable& other) noexcept {
            std::swap(raw_value, other.raw_value);
        }
        void reset() noexcept {
            raw_value = (std::numeric_limits<T>::max)();
        }
        bool operator==(const nullable& other) const noexcept {
            return raw_value == other.raw_value;
        }
        bool operator!=(const nullable& other) const noexcept {
            return raw_value != other.raw_value;
        }
        constexpr static nullable null() noexcept {
            return { (std::numeric_limits<T>::max)() };
        }
    };

    struct CPPTRACE_EXPORT stacktrace_frame {
        frame_ptr raw_address;
        frame_ptr object_address;
        nullable<std::uint32_t> line;
        nullable<std::uint32_t> column;
        std::string filename;
        std::string symbol;
        bool is_inline;

        bool operator==(const stacktrace_frame& other) const {
            return raw_address == other.raw_address
                && object_address == other.object_address
                && line == other.line
                && column == other.column
                && filename == other.filename
                && symbol == other.symbol;
        }

        bool operator!=(const stacktrace_frame& other) const {
            return !operator==(other);
        }

        object_frame get_object_info() const;

        std::string to_string() const;
        friend std::ostream& operator<<(std::ostream& stream, const stacktrace_frame& frame);
    };

    struct CPPTRACE_EXPORT stacktrace {
        std::vector<stacktrace_frame> frames;
        static stacktrace current(std::size_t skip = 0);
        static stacktrace current(std::size_t skip, std::size_t max_depth);
        void print() const;
        void print(std::ostream& stream) const;
        void print(std::ostream& stream, bool color) const;
        void print_with_snippets() const;
        void print_with_snippets(std::ostream& stream) const;
        void print_with_snippets(std::ostream& stream, bool color) const;
        void clear();
        bool empty() const noexcept;
        std::string to_string(bool color = false) const;
        friend std::ostream& operator<<(std::ostream& stream, const stacktrace& trace);

        using iterator = std::vector<stacktrace_frame>::iterator;
        using const_iterator = std::vector<stacktrace_frame>::const_iterator;
        inline iterator begin() noexcept { return frames.begin(); }
        inline iterator end() noexcept { return frames.end(); }
        inline const_iterator begin() const noexcept { return frames.begin(); }
        inline const_iterator end() const noexcept { return frames.end(); }
        inline const_iterator cbegin() const noexcept { return frames.cbegin(); }
        inline const_iterator cend() const noexcept { return frames.cend(); }
    private:
        void print(std::ostream& stream, bool color, bool newline_at_end, const char* header) const;
        void print_with_snippets(std::ostream& stream, bool color, bool newline_at_end, const char* header) const;
        friend void print_terminate_trace();
    };

    CPPTRACE_EXPORT raw_trace generate_raw_trace(std::size_t skip = 0);
    CPPTRACE_EXPORT raw_trace generate_raw_trace(std::size_t skip, std::size_t max_depth);
    CPPTRACE_EXPORT object_trace generate_object_trace(std::size_t skip = 0);
    CPPTRACE_EXPORT object_trace generate_object_trace(std::size_t skip, std::size_t max_depth);
    CPPTRACE_EXPORT stacktrace generate_trace(std::size_t skip = 0);
    CPPTRACE_EXPORT stacktrace generate_trace(std::size_t skip, std::size_t max_depth);

    // Path max isn't so simple, so I'm choosing 4096 which seems to encompass what all major OS's expect and should be
    // fine in all reasonable cases.
    // https://eklitzke.org/path-max-is-tricky
    // https://insanecoding.blogspot.com/2007/11/pathmax-simply-isnt.html
    #define CPPTRACE_PATH_MAX 4096

    // safe tracing interface
    // signal-safe
    CPPTRACE_EXPORT std::size_t safe_generate_raw_trace(
        frame_ptr* buffer,
        std::size_t size,
        std::size_t skip = 0
    );
    // signal-safe
    CPPTRACE_EXPORT std::size_t safe_generate_raw_trace(
        frame_ptr* buffer,
        std::size_t size,
        std::size_t skip,
        std::size_t max_depth
    );
    struct CPPTRACE_EXPORT safe_object_frame {
        frame_ptr raw_address;
        frame_ptr address_relative_to_object_start; // base must still be added
        char object_path[CPPTRACE_PATH_MAX + 1];
        // To be called outside a signal handler. Not signal safe.
        object_frame resolve() const;
    };
    // signal-safe
    CPPTRACE_EXPORT void get_safe_object_frame(frame_ptr address, safe_object_frame* out);
    CPPTRACE_EXPORT bool can_signal_safe_unwind();

    // utilities:
    CPPTRACE_EXPORT std::string demangle(const std::string& name);
    CPPTRACE_EXPORT std::string get_snippet(
        const std::string& path,
        std::size_t line,
        std::size_t context_size,
        bool color = false
    );
    CPPTRACE_EXPORT bool isatty(int fd);

    CPPTRACE_EXPORT extern const int stdin_fileno;
    CPPTRACE_EXPORT extern const int stderr_fileno;
    CPPTRACE_EXPORT extern const int stdout_fileno;

    CPPTRACE_EXPORT void register_terminate_handler();

    // configuration:
    CPPTRACE_EXPORT void absorb_trace_exceptions(bool absorb);
    CPPTRACE_EXPORT void enable_inlined_call_resolution(bool enable);

    enum class cache_mode {
        // Only minimal lookup tables
        prioritize_memory = 0,
        // Build lookup tables but don't keep them around between trace calls
        hybrid = 1,
        // Build lookup tables as needed
        prioritize_speed = 2
    };

    namespace experimental {
        CPPTRACE_EXPORT void set_cache_mode(cache_mode mode);
    }

    // tracing exceptions:
    namespace detail {
        // This is a helper utility, if the library weren't C++11 an std::variant would be used
        class CPPTRACE_EXPORT lazy_trace_holder {
            bool resolved;
            union {
                raw_trace trace;
                stacktrace resolved_trace;
            };
        public:
            // constructors
            lazy_trace_holder() : resolved(false), trace() {}
            explicit lazy_trace_holder(raw_trace&& _trace) : resolved(false), trace(std::move(_trace)) {}
            explicit lazy_trace_holder(stacktrace&& _resolved_trace) : resolved(true), resolved_trace(std::move(_resolved_trace)) {}
            // logistics
            lazy_trace_holder(const lazy_trace_holder& other);
            lazy_trace_holder(lazy_trace_holder&& other) noexcept;
            lazy_trace_holder& operator=(const lazy_trace_holder& other);
            lazy_trace_holder& operator=(lazy_trace_holder&& other) noexcept;
            ~lazy_trace_holder();
            // access
            const raw_trace& get_raw_trace() const;
            stacktrace& get_resolved_trace();
            const stacktrace& get_resolved_trace() const;
        private:
            void clear();
        };

        CPPTRACE_EXPORT raw_trace get_raw_trace_and_absorb(std::size_t skip, std::size_t max_depth);
        CPPTRACE_EXPORT raw_trace get_raw_trace_and_absorb(std::size_t skip = 0);
    }

    // Interface for a traced exception object
    class CPPTRACE_EXPORT exception : public std::exception {
    public:
        const char* what() const noexcept override = 0;
        virtual const char* message() const noexcept = 0;
        virtual const stacktrace& trace() const noexcept = 0;
    };

    // Cpptrace traced exception object
    // I hate to have to expose anything about implementation detail but the idea here is that
    class CPPTRACE_EXPORT lazy_exception : public exception {
        mutable detail::lazy_trace_holder trace_holder;
        mutable std::string what_string;

    public:
        explicit lazy_exception(
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) : trace_holder(std::move(trace)) {}
        // std::exception
        const char* what() const noexcept override;
        // cpptrace::exception
        const char* message() const noexcept override;
        const stacktrace& trace() const noexcept override;
    };

    class CPPTRACE_EXPORT exception_with_message : public lazy_exception {
        mutable std::string user_message;

    public:
        explicit exception_with_message(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept : lazy_exception(std::move(trace)), user_message(std::move(message_arg)) {}

        const char* message() const noexcept override;
    };

    class CPPTRACE_EXPORT logic_error : public exception_with_message {
    public:
        explicit logic_error(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT domain_error : public exception_with_message {
    public:
        explicit domain_error(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT invalid_argument : public exception_with_message {
    public:
        explicit invalid_argument(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT length_error : public exception_with_message {
    public:
        explicit length_error(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT out_of_range : public exception_with_message {
    public:
        explicit out_of_range(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT runtime_error : public exception_with_message {
    public:
        explicit runtime_error(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT range_error : public exception_with_message {
    public:
        explicit range_error(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT overflow_error : public exception_with_message {
    public:
        explicit overflow_error(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT underflow_error : public exception_with_message {
    public:
        explicit underflow_error(
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : exception_with_message(std::move(message_arg), std::move(trace)) {}
    };

    class CPPTRACE_EXPORT nested_exception : public lazy_exception {
        std::exception_ptr ptr;
        mutable std::string message_value;
    public:
        explicit nested_exception(
            const std::exception_ptr& exception_ptr,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept
            : lazy_exception(std::move(trace)), ptr(exception_ptr) {}

        const char* message() const noexcept override;
        std::exception_ptr nested_ptr() const noexcept;
    };

    class CPPTRACE_EXPORT system_error : public runtime_error {
        std::error_code ec;
    public:
        explicit system_error(
            int error_code,
            std::string&& message_arg,
            raw_trace&& trace = detail::get_raw_trace_and_absorb()
        ) noexcept;
        const std::error_code& code() const noexcept;
    };

    // [[noreturn]] must come first due to old clang
    [[noreturn]] CPPTRACE_EXPORT void rethrow_and_wrap_if_needed(std::size_t skip = 0);
}

#if defined(CPPTRACE_STD_FORMAT) && defined(__cpp_lib_format)
 template <>
 struct std::formatter<cpptrace::stacktrace_frame> : std::formatter<std::string> {
     auto format(cpptrace::stacktrace_frame frame, format_context& ctx) const {
         return formatter<string>::format(frame.to_string(), ctx);
     }
 };

 template <>
 struct std::formatter<cpptrace::stacktrace> : std::formatter<std::string> {
     auto format(cpptrace::stacktrace trace, format_context& ctx) const {
         return formatter<string>::format(trace.to_string(), ctx);
     }
 };
#endif

// Exception wrapper utilities
#define CPPTRACE_WRAP_BLOCK(statements) do { \
        try { \
            statements \
        } catch(...) { \
            ::cpptrace::rethrow_and_wrap_if_needed(); \
        } \
    } while(0)

#define CPPTRACE_WRAP(expression) [&] () -> decltype((expression)) { \
        try { \
            return expression; \
        } catch(...) { \
            ::cpptrace::rethrow_and_wrap_if_needed(1); \
        } \
    } ()

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
