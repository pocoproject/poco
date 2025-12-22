#include <cpptrace/cpptrace.hpp>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <new>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "symbols/symbols.hpp"
#include "unwind/unwind.hpp"
#include "demangle/demangle.hpp"
#include "platform/exception_type.hpp"
#include "utils/common.hpp"
#include "utils/utils.hpp"
#include "binary/object.hpp"
#include "binary/safe_dl.hpp"
#include "snippets/snippet.hpp"

namespace cpptrace {
    CPPTRACE_FORCE_NO_INLINE
    raw_trace raw_trace::current(std::size_t skip) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return generate_raw_trace(skip + 1);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return raw_trace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    raw_trace raw_trace::current(std::size_t skip, std::size_t max_depth) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return generate_raw_trace(skip + 1, max_depth);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return raw_trace{};
        }
    }

    object_trace raw_trace::resolve_object_trace() const {
        try {
            return object_trace{detail::get_frames_object_info(frames)};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return object_trace{};
        }
    }

    stacktrace raw_trace::resolve() const {
        try {
            std::vector<stacktrace_frame> trace = detail::resolve_frames(frames);
            for(auto& frame : trace) {
                frame.symbol = detail::demangle(frame.symbol);
            }
            return {std::move(trace)};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return stacktrace{};
        }
    }

    void raw_trace::clear() {
        frames.clear();
    }

    bool raw_trace::empty() const noexcept {
        return frames.empty();
    }

    CPPTRACE_FORCE_NO_INLINE
    object_trace object_trace::current(std::size_t skip) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return generate_object_trace(skip + 1);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return object_trace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    object_trace object_trace::current(std::size_t skip, std::size_t max_depth) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return generate_object_trace(skip + 1, max_depth);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return object_trace{};
        }
    }

    stacktrace object_trace::resolve() const {
        try {
            std::vector<stacktrace_frame> trace = detail::resolve_frames(frames);
            for(auto& frame : trace) {
                frame.symbol = detail::demangle(frame.symbol);
            }
            return {std::move(trace)};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return stacktrace();
        }
    }

    void object_trace::clear() {
        frames.clear();
    }

    bool object_trace::empty() const noexcept {
        return frames.empty();
    }

    object_frame stacktrace_frame::get_object_info() const {
        return detail::get_frame_object_info(raw_address);
    }

    std::string stacktrace_frame::to_string() const {
        std::string str;
        if(is_inline) {
            str += microfmt::format("{<{}}", 2 * sizeof(frame_ptr) + 2, "(inlined)");
        } else {
            str += microfmt::format("0x{>{}:0h}", 2 * sizeof(frame_ptr), raw_address);
        }
        if(!symbol.empty()) {
            str += microfmt::format(" in {}", symbol);
        }
        if(!filename.empty()) {
            str += microfmt::format(" at {}", filename);
            if(line.has_value()) {
                str += microfmt::format(":{}", line.value());
                if(column.has_value()) {
                    str += microfmt::format(":{}", column.value());
                }
            }
        }
        return str;
    }

    std::ostream& operator<<(std::ostream& stream, const stacktrace_frame& frame) {
        return stream << frame.to_string();
    }

    CPPTRACE_FORCE_NO_INLINE
    stacktrace stacktrace::current(std::size_t skip) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return generate_trace(skip + 1);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return stacktrace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    stacktrace stacktrace::current(std::size_t skip, std::size_t max_depth) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return generate_trace(skip + 1, max_depth);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return stacktrace{};
        }
    }

    void stacktrace::print() const {
        print(std::cerr, true);
    }

    void stacktrace::print(std::ostream& stream) const {
        print(stream, true);
    }

    void stacktrace::print(std::ostream& stream, bool color) const {
        print(stream, color, true, nullptr);
    }

    void print_frame(
        std::ostream& stream,
        bool color,
        unsigned frame_number_width,
        std::size_t counter,
        const stacktrace_frame& frame
    ) {
        const auto reset   = color ? RESET : "";
        const auto green   = color ? GREEN : "";
        const auto yellow  = color ? YELLOW : "";
        const auto blue    = color ? BLUE : "";
        std::string line = microfmt::format("#{<{}} ", frame_number_width, counter);
        if(frame.is_inline) {
            line += microfmt::format("{<{}}", 2 * sizeof(frame_ptr) + 2, "(inlined)");
        } else {
            line += microfmt::format("{}0x{>{}:0h}{}", blue, 2 * sizeof(frame_ptr), frame.raw_address, reset);
        }
        if(!frame.symbol.empty()) {
            line += microfmt::format(" in {}{}{}", yellow, frame.symbol, reset);
        }
        if(!frame.filename.empty()) {
            line += microfmt::format(" at {}{}{}", green, frame.filename, reset);
            if(frame.line.has_value()) {
                line += microfmt::format(":{}{}{}", blue, frame.line.value(), reset);
                if(frame.column.has_value()) {
                    line += microfmt::format(":{}{}{}", blue, frame.column.value(), reset);
                }
            }
        }
        stream << line;
    }

    void stacktrace::print(std::ostream& stream, bool color, bool newline_at_end, const char* header) const {
        if(
            color && (
                (&stream == &std::cout && isatty(stdout_fileno)) || (&stream == &std::cerr && isatty(stderr_fileno))
            )
        ) {
            detail::enable_virtual_terminal_processing_if_needed();
        }
        stream << (header ? header : "Stack trace (most recent call first):") << '\n';
        std::size_t counter = 0;
        if(frames.empty()) {
            stream << "<empty trace>\n";
            return;
        }
        const auto frame_number_width = detail::n_digits(static_cast<int>(frames.size()) - 1);
        for(const auto& frame : frames) {
            print_frame(stream, color, frame_number_width, counter, frame);
            if(newline_at_end || &frame != &frames.back()) {
                stream << '\n';
            }
            counter++;
        }
    }

    void stacktrace::print_with_snippets() const {
        print_with_snippets(std::cerr, true);
    }

    void stacktrace::print_with_snippets(std::ostream& stream) const {
        print_with_snippets(stream, true);
    }

    void stacktrace::print_with_snippets(std::ostream& stream, bool color) const {
        print_with_snippets(stream, color, true, nullptr);
    }

    void stacktrace::print_with_snippets(std::ostream& stream, bool color, bool newline_at_end, const char* header) const {
        if(
            color && (
                (&stream == &std::cout && isatty(stdout_fileno)) || (&stream == &std::cerr && isatty(stderr_fileno))
            )
        ) {
            detail::enable_virtual_terminal_processing_if_needed();
        }
        stream << (header ? header : "Stack trace (most recent call first):") << '\n';
        std::size_t counter = 0;
        if(frames.empty()) {
            stream << "<empty trace>" << '\n';
            return;
        }
        const auto frame_number_width = detail::n_digits(static_cast<int>(frames.size()) - 1);
        for(const auto& frame : frames) {
            print_frame(stream, color, frame_number_width, counter, frame);
            if(newline_at_end || &frame != &frames.back()) {
                stream << '\n';
            }
            if(frame.line.has_value() && !frame.filename.empty()) {
                stream << detail::get_snippet(frame.filename, frame.line.value(), 2, color);
            }
            counter++;
        }
    }

    void stacktrace::clear() {
        frames.clear();
    }

    bool stacktrace::empty() const noexcept {
        return frames.empty();
    }

    std::string stacktrace::to_string(bool color) const {
        std::ostringstream oss;
        print(oss, color, false, nullptr);
        return std::move(oss).str();
    }

    std::ostream& operator<<(std::ostream& stream, const stacktrace& trace) {
        return stream << trace.to_string();
    }

    CPPTRACE_FORCE_NO_INLINE
    raw_trace generate_raw_trace(std::size_t skip) {
        try {
            return raw_trace{detail::capture_frames(skip + 1, SIZE_MAX)};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return raw_trace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    raw_trace generate_raw_trace(std::size_t skip, std::size_t max_depth) {
        try {
            return raw_trace{detail::capture_frames(skip + 1, max_depth)};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return raw_trace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    std::size_t safe_generate_raw_trace(frame_ptr* buffer, std::size_t size, std::size_t skip) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return detail::safe_capture_frames(buffer, size, skip + 1, SIZE_MAX);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return 0;
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    std::size_t safe_generate_raw_trace(
         frame_ptr* buffer,
         std::size_t size,
         std::size_t skip,
         std::size_t max_depth
    ) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return detail::safe_capture_frames(buffer, size, skip + 1, max_depth);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return 0;
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    object_trace generate_object_trace(std::size_t skip) {
        try {
            return object_trace{detail::get_frames_object_info(detail::capture_frames(skip + 1, SIZE_MAX))};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return object_trace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    object_trace generate_object_trace(std::size_t skip, std::size_t max_depth) {
        try {
            return object_trace{detail::get_frames_object_info(detail::capture_frames(skip + 1, max_depth))};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return object_trace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    stacktrace generate_trace(std::size_t skip) {
        try { // try/catch can never be hit but it's needed to prevent TCO
            return generate_trace(skip + 1, SIZE_MAX);
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return stacktrace{};
        }
    }

    CPPTRACE_FORCE_NO_INLINE
    stacktrace generate_trace(std::size_t skip, std::size_t max_depth) {
        try {
            std::vector<frame_ptr> frames = detail::capture_frames(skip + 1, max_depth);
            std::vector<stacktrace_frame> trace = detail::resolve_frames(frames);
            for(auto& frame : trace) {
                frame.symbol = detail::demangle(frame.symbol);
            }
            return {std::move(trace)};
        } catch(...) { // NOSONAR
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
            return stacktrace();
        }
    }

    object_frame safe_object_frame::resolve() const {
        return detail::resolve_safe_object_frame(*this);
    }

    void get_safe_object_frame(frame_ptr address, safe_object_frame* out) {
        detail::get_safe_object_frame(address, out);
    }

    bool can_signal_safe_unwind() {
        return detail::has_safe_unwind();
    }

    std::string demangle(const std::string& name) {
        return detail::demangle(name);
    }

    std::string get_snippet(const std::string& path, std::size_t line, std::size_t context_size, bool color) {
        return detail::get_snippet(path, line, context_size, color);
    }

    bool isatty(int fd) {
        return detail::isatty(fd);
    }

    extern const int stdin_fileno = detail::fileno(stdin);
    extern const int stdout_fileno = detail::fileno(stdout);
    extern const int stderr_fileno = detail::fileno(stderr);

    CPPTRACE_FORCE_NO_INLINE void print_terminate_trace() {
        try { // try/catch can never be hit but it's needed to prevent TCO
            generate_trace(1).print(
                std::cerr,
                isatty(stderr_fileno),
                true,
                "Stack trace to reach terminate handler (most recent call first):"
            );
        } catch(...) {
            if(!detail::should_absorb_trace_exceptions()) {
                throw;
            }
        }
    }

    [[noreturn]] void terminate_handler() {
        // TODO: Support std::nested_exception?
        try {
            auto ptr = std::current_exception();
            if(ptr == nullptr) {
                fputs("terminate called without an active exception", stderr);
                print_terminate_trace();
            } else {
                std::rethrow_exception(ptr);
            }
        } catch(cpptrace::exception& e) {
            microfmt::print(
                stderr,
                "Terminate called after throwing an instance of {}: {}\n",
                demangle(typeid(e).name()),
                e.message()
            );
            e.trace().print(std::cerr, isatty(stderr_fileno));
        } catch(std::exception& e) {
            microfmt::print(
                stderr, "Terminate called after throwing an instance of {}: {}\n", demangle(typeid(e).name()), e.what()
            );
            print_terminate_trace();
        } catch(...) {
            microfmt::print(
                stderr, "Terminate called after throwing an instance of {}\n", detail::exception_type_name()
            );
            print_terminate_trace();
        }
        std::flush(std::cerr);
        abort();
    }

    void register_terminate_handler() {
        std::set_terminate(terminate_handler);
    }

    namespace detail {
        std::atomic_bool absorb_trace_exceptions(true); // NOSONAR
        std::atomic_bool resolve_inlined_calls(true); // NOSONAR
        std::atomic<enum cache_mode> cache_mode(cache_mode::prioritize_speed); // NOSONAR
    }

    void absorb_trace_exceptions(bool absorb) {
        detail::absorb_trace_exceptions = absorb;
    }

     void enable_inlined_call_resolution(bool enable) {
        detail::resolve_inlined_calls = enable;
    }

    namespace experimental {
        void set_cache_mode(cache_mode mode) {
            detail::cache_mode = mode;
        }
    }

    namespace detail {
        bool should_absorb_trace_exceptions() {
            return absorb_trace_exceptions;
        }

        bool should_resolve_inlined_calls() {
            return resolve_inlined_calls;
        }

        enum cache_mode get_cache_mode() {
            return cache_mode;
        }

        CPPTRACE_FORCE_NO_INLINE
        raw_trace get_raw_trace_and_absorb(std::size_t skip, std::size_t max_depth) {
            try {
                return generate_raw_trace(skip + 1, max_depth);
            } catch(const std::exception& e) {
                if(!detail::should_absorb_trace_exceptions()) {
                    // TODO: Append to message somehow
                    std::fprintf(
                        stderr,
                        "Cpptrace: Exception occurred while resolving trace in cpptrace::exception object:\n%s\n",
                        e.what()
                    );
                }
                return raw_trace{};
            }
        }

        CPPTRACE_FORCE_NO_INLINE
        raw_trace get_raw_trace_and_absorb(std::size_t skip) {
            try { // try/catch can never be hit but it's needed to prevent TCO
                return get_raw_trace_and_absorb(skip + 1, SIZE_MAX);
            } catch(...) {
                if(!detail::should_absorb_trace_exceptions()) {
                    throw;
                }
                return raw_trace{};
            }
        }

        lazy_trace_holder::lazy_trace_holder(const lazy_trace_holder& other) : resolved(other.resolved) {
            if(other.resolved) {
                new (&resolved_trace) stacktrace(other.resolved_trace);
            } else {
                new (&trace) raw_trace(other.trace);
            }
        }
        lazy_trace_holder::lazy_trace_holder(lazy_trace_holder&& other) noexcept : resolved(other.resolved) {
            if(other.resolved) {
                new (&resolved_trace) stacktrace(std::move(other.resolved_trace));
            } else {
                new (&trace) raw_trace(std::move(other.trace));
            }
        }
        lazy_trace_holder& lazy_trace_holder::operator=(const lazy_trace_holder& other) {
            clear();
            resolved = other.resolved;
            if(other.resolved) {
                new (&resolved_trace) stacktrace(other.resolved_trace);
            } else {
                new (&trace) raw_trace(other.trace);
            }
            return *this;
        }
        lazy_trace_holder& lazy_trace_holder::operator=(lazy_trace_holder&& other) noexcept {
            clear();
            resolved = other.resolved;
            if(other.resolved) {
                new (&resolved_trace) stacktrace(std::move(other.resolved_trace));
            } else {
                new (&trace) raw_trace(std::move(other.trace));
            }
            return *this;
        }
        lazy_trace_holder::~lazy_trace_holder() {
            clear();
        }
        // access
        const raw_trace& lazy_trace_holder::get_raw_trace() const {
            if(resolved) {
                throw std::logic_error(
                    "cpptrace::detail::lazy_trace_holder::get_resolved_trace called on resolved holder"
                );
            }
            return trace;
        }
        stacktrace& lazy_trace_holder::get_resolved_trace() {
            if(!resolved) {
                raw_trace old_trace = std::move(trace);
                *this = lazy_trace_holder(stacktrace{});
                try {
                    if(!old_trace.empty()) {
                        resolved_trace = old_trace.resolve();
                    }
                } catch(const std::exception& e) {
                    if(!detail::should_absorb_trace_exceptions()) {
                        // TODO: Append to message somehow?
                        std::fprintf(
                            stderr,
                            "Exception occurred while resolving trace in cpptrace::detail::lazy_trace_holder:\n%s\n",
                            e.what()
                        );
                    }
                }
            }
            return resolved_trace;
        }
        const stacktrace& lazy_trace_holder::get_resolved_trace() const {
            if(!resolved) {
                throw std::logic_error(
                    "cpptrace::detail::lazy_trace_holder::get_resolved_trace called on unresolved const holder"
                );
            }
            return resolved_trace;
        }
        void lazy_trace_holder::clear() {
            if(resolved) {
                resolved_trace.~stacktrace();
            } else {
                trace.~raw_trace();
            }
        }
    }

    const char* lazy_exception::what() const noexcept {
        if(what_string.empty()) {
            what_string = message() + std::string(":\n") + trace_holder.get_resolved_trace().to_string();
        }
        return what_string.c_str();
    }

    const char* lazy_exception::message() const noexcept {
        return "cpptrace::lazy_exception";
    }

    const stacktrace& lazy_exception::trace() const noexcept {
        return trace_holder.get_resolved_trace();
    }

    const char* exception_with_message::message() const noexcept {
        return user_message.c_str();
    }

    system_error::system_error(int error_code, std::string&& message_arg, raw_trace&& trace) noexcept
        : runtime_error(
            message_arg + ": " + std::error_code(error_code, std::generic_category()).message(),
            std::move(trace)
          ),
          ec(std::error_code(error_code, std::generic_category())) {}

    const std::error_code& system_error::code() const noexcept {
        return ec;
    }

    const char* nested_exception::message() const noexcept {
        if(message_value.empty()) {
            try {
                std::rethrow_exception(ptr);
            } catch(std::exception& e) {
                message_value = std::string("Nested exception: ") + e.what();
            } catch(...) {
                message_value = "Nested exception holding instance of " + detail::exception_type_name();
            }
        }
        return message_value.c_str();
    }

    std::exception_ptr nested_exception::nested_ptr() const noexcept {
        return ptr;
    }

    CPPTRACE_FORCE_NO_INLINE
    void rethrow_and_wrap_if_needed(std::size_t skip) {
        try {
            std::rethrow_exception(std::current_exception());
        } catch(cpptrace::exception&) {
            throw; // already a cpptrace::exception
        } catch(...) {
            throw nested_exception(std::current_exception(), detail::get_raw_trace_and_absorb(skip + 1));
        }
    }
}
