#ifndef CPPTRACE_FROM_CURRENT_HPP
#define CPPTRACE_FROM_CURRENT_HPP

#include <cpptrace/cpptrace.hpp>

namespace cpptrace {
    CPPTRACE_EXPORT const raw_trace& raw_trace_from_current_exception();
    CPPTRACE_EXPORT const stacktrace& from_current_exception();

    namespace detail {
        // Trace switch is to prevent multiple tracing of stacks on call stacks with multiple catches that don't
        // immediately match
        inline bool& get_trace_switch() {
            static thread_local bool trace_switch = true;
            return trace_switch;
        }

        class CPPTRACE_EXPORT try_canary {
        public:
            ~try_canary() {
                // Fires when we exit a try block, either via normal means or during unwinding.
                // Either way: Flip the switch.
                get_trace_switch() = true;
            }
        };

        CPPTRACE_EXPORT CPPTRACE_FORCE_NO_INLINE void collect_current_trace(std::size_t skip);

        // this function can be void, however, a char return is used to prevent TCO of the collect_current_trace
        CPPTRACE_FORCE_NO_INLINE inline char exception_unwind_interceptor(std::size_t skip) {
            if(get_trace_switch()) {
                // Done during a search phase. Flip the switch off, no more traces until an unwind happens
                get_trace_switch() = false;
                collect_current_trace(skip + 1);
            }
            return 42;
        }

        #ifdef _MSC_VER
         CPPTRACE_FORCE_NO_INLINE inline int exception_filter() {
             exception_unwind_interceptor(1);
             return 0; // EXCEPTION_CONTINUE_SEARCH
         }
         CPPTRACE_FORCE_NO_INLINE inline int unconditional_exception_filter() {
             collect_current_trace(1);
             return 0; // EXCEPTION_CONTINUE_SEARCH
         }
        #else
         class CPPTRACE_EXPORT unwind_interceptor {
         public:
             virtual ~unwind_interceptor();
         };
         class CPPTRACE_EXPORT unconditional_unwind_interceptor {
         public:
             virtual ~unconditional_unwind_interceptor();
         };

         CPPTRACE_EXPORT void do_prepare_unwind_interceptor(char(*)(std::size_t));

         #ifndef CPPTRACE_DONT_PREPARE_UNWIND_INTERCEPTOR_ON
          __attribute__((constructor)) inline void prepare_unwind_interceptor() {
              // __attribute__((constructor)) inline functions can be called for every source file they're #included in
              // there is still only one copy of the inline function in the final executable, though
              // LTO can make the redundant constructs fire only once
              // do_prepare_unwind_interceptor prevents against multiple preparations however it makes sense to guard
              // against it here too as a fast path, not that this should matter for performance
              static bool did_prepare = false;
              if(!did_prepare) {
                 do_prepare_unwind_interceptor(exception_unwind_interceptor);
                 did_prepare = true;
              }
          }
         #endif
        #endif
    }
}

#ifdef _MSC_VER
 // this awful double-IILE is due to C2713 "You can't use structured exception handling (__try/__except) and C++
 // exception handling (try/catch) in the same function."
 #define CPPTRACE_TRY \
     try { \
         ::cpptrace::detail::try_canary cpptrace_try_canary; \
         [&]() { \
             __try { \
                 [&]() {
 #define CPPTRACE_CATCH(param) \
                 }(); \
             } __except(::cpptrace::detail::exception_filter()) {} \
         }(); \
     } catch(param)
 #define CPPTRACE_TRYZ \
     try { \
         [&]() { \
             __try { \
                 [&]() {
 #define CPPTRACE_CATCHZ(param) \
                 }(); \
             } __except(::cpptrace::detail::unconditional_exception_filter()) {} \
         }(); \
     } catch(param)
#else
 #define CPPTRACE_TRY \
     try { \
         _Pragma("GCC diagnostic push") \
         _Pragma("GCC diagnostic ignored \"-Wshadow\"") \
         ::cpptrace::detail::try_canary cpptrace_try_canary; \
         _Pragma("GCC diagnostic pop") \
         try {
 #define CPPTRACE_CATCH(param) \
         } catch(::cpptrace::detail::unwind_interceptor&) {} \
     } catch(param)
 #define CPPTRACE_TRYZ \
     try { \
         try {
 #define CPPTRACE_CATCHZ(param) \
         } catch(::cpptrace::detail::unconditional_unwind_interceptor&) {} \
     } catch(param)
#endif

#define CPPTRACE_CATCH_ALT(param) catch(param)

#ifdef CPPTRACE_UNPREFIXED_TRY_CATCH
 #define TRY CPPTRACE_TRY
 #define CATCH(param) CPPTRACE_CATCH(param)
 #define TRYZ CPPTRACE_TRYZ
 #define CATCHZ(param) CPPTRACE_CATCHZ(param)
 #define CATCH_ALT(param) CPPTRACE_CATCH_ALT(param)
#endif

#endif
