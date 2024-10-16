#ifdef CPPTRACE_UNWIND_WITH_DBGHELP

#include <cpptrace/cpptrace.hpp>
#include "unwind/unwind.hpp"
#include "utils/common.hpp"
#include "utils/utils.hpp"
#include "platform/dbghelp_syminit_manager.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>
#include <mutex>

#include <windows.h>
#include <dbghelp.h>

// Fucking windows headers
#ifdef min
 #undef min
#endif

namespace cpptrace {
namespace detail {
    #if IS_MSVC
    #pragma warning(push)
    #pragma warning(disable: 4740) // warning C4740: flow in or out of inline asm code suppresses global optimization
    #endif
    CPPTRACE_FORCE_NO_INLINE
    std::vector<frame_ptr> capture_frames(std::size_t skip, std::size_t max_depth) {
        skip++;
        // https://jpassing.com/2008/03/12/walking-the-stack-of-the-current-thread/

        // Get current thread context
        // GetThreadContext cannot be used on the current thread.
        // RtlCaptureContext doesn't work on i386
        CONTEXT context;
        #if defined(_M_IX86) || defined(__i386__)
        ZeroMemory(&context, sizeof(CONTEXT));
        context.ContextFlags = CONTEXT_CONTROL;
        #if IS_MSVC
        __asm {
            label:
            mov [context.Ebp], ebp;
            mov [context.Esp], esp;
            mov eax, [label];
            mov [context.Eip], eax;
        }
        #else
        asm(
            "label:\n\t"
            "mov{l %%ebp, %[cEbp] | %[cEbp], ebp};\n\t"
            "mov{l %%esp, %[cEsp] | %[cEsp], esp};\n\t"
            "mov{l $label, %%eax | eax, OFFSET label};\n\t"
            "mov{l %%eax, %[cEip] | %[cEip], eax};\n\t"
            : [cEbp] "=r" (context.Ebp),
              [cEsp] "=r" (context.Esp),
              [cEip] "=r" (context.Eip)
        );
        #endif
        #else
        RtlCaptureContext(&context);
        #endif
        // Setup current frame
        STACKFRAME64 frame;
        ZeroMemory(&frame, sizeof(STACKFRAME64));
        DWORD machine_type;
        #if defined(_M_IX86) || defined(__i386__)
        machine_type           = IMAGE_FILE_MACHINE_I386;
        frame.AddrPC.Offset    = context.Eip;
        frame.AddrPC.Mode      = AddrModeFlat;
        frame.AddrFrame.Offset = context.Ebp;
        frame.AddrFrame.Mode   = AddrModeFlat;
        frame.AddrStack.Offset = context.Esp;
        frame.AddrStack.Mode   = AddrModeFlat;
        #elif defined(_M_X64) || defined(__x86_64__)
        machine_type           = IMAGE_FILE_MACHINE_AMD64;
        frame.AddrPC.Offset    = context.Rip;
        frame.AddrPC.Mode      = AddrModeFlat;
        frame.AddrFrame.Offset = context.Rsp;
        frame.AddrFrame.Mode   = AddrModeFlat;
        frame.AddrStack.Offset = context.Rsp;
        frame.AddrStack.Mode   = AddrModeFlat;
        #elif defined(_M_IA64) || defined(__aarch64__)
        machine_type           = IMAGE_FILE_MACHINE_IA64;
        frame.AddrPC.Offset    = context.StIIP;
        frame.AddrPC.Mode      = AddrModeFlat;
        frame.AddrFrame.Offset = context.IntSp;
        frame.AddrFrame.Mode   = AddrModeFlat;
        frame.AddrBStore.Offset= context.RsBSP;
        frame.AddrBStore.Mode  = AddrModeFlat;
        frame.AddrStack.Offset = context.IntSp;
        frame.AddrStack.Mode   = AddrModeFlat;
        #else
        #error "Cpptrace: StackWalk64 not supported for this platform yet"
        #endif

        std::vector<frame_ptr> trace;

        // Dbghelp is is single-threaded, so acquire a lock.
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        // For some reason SymInitialize must be called before StackWalk64
        // Note that the code assumes that
        // SymInitialize( GetCurrentProcess(), NULL, TRUE ) has
        // already been called.
        //
        HANDLE proc = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();
        if(get_cache_mode() == cache_mode::prioritize_speed) {
            get_syminit_manager().init(proc);
        } else {
            if(!SymInitialize(proc, NULL, TRUE)) {
                throw internal_error("SymInitialize failed");
            }
        }
        while(trace.size() < max_depth) {
            if(
                !StackWalk64(
                    machine_type,
                    proc,
                    thread,
                    &frame,
                    machine_type == IMAGE_FILE_MACHINE_I386 ? NULL : &context,
                    NULL,
                    SymFunctionTableAccess64,
                    SymGetModuleBase64,
                    NULL
                )
            ) {
                // Either failed or finished walking
                break;
            }
            if(frame.AddrPC.Offset != 0) {
                // Valid frame
                if(skip) {
                    skip--;
                } else {
                    // On x86/x64/arm, as far as I can tell, the frame return address is always one after the call
                    // So we just decrement to get the pc back inside the `call` / `bl`
                    // This is done with _Unwind too but conditionally based on info from _Unwind_GetIPInfo.
                    trace.push_back(to_frame_ptr(frame.AddrPC.Offset) - 1);
                }
            } else {
                // base
                break;
            }
        }
        if(get_cache_mode() != cache_mode::prioritize_speed) {
            if(!SymCleanup(proc)) {
                throw internal_error("SymCleanup failed");
            }
        }
        return trace;
    }

    CPPTRACE_FORCE_NO_INLINE
    std::size_t safe_capture_frames(frame_ptr*, std::size_t, std::size_t, std::size_t) {
        // Can't safe trace with dbghelp
        return 0;
    }
    #if IS_MSVC
    #pragma warning(pop)
    #endif

    bool has_safe_unwind() {
        return false;
    }
}
}

#endif
