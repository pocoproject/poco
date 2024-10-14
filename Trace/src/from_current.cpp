#include <cpptrace/cpptrace.hpp>
#define CPPTRACE_DONT_PREPARE_UNWIND_INTERCEPTOR_ON
#include <cpptrace/from_current.hpp>

#include <system_error>
#include <typeinfo>

#include "platform/platform.hpp"
#include "utils/common.hpp"
#include "utils/microfmt.hpp"
#include "utils/utils.hpp"

#ifndef _MSC_VER
 #include <array>
 #include <string.h>
 #if IS_WINDOWS
  #include <windows.h>
 #else
  #include <sys/mman.h>
  #include <unistd.h>
  #if IS_APPLE
   #include <mach/mach.h>
   #include <mach/mach_vm.h>
  #else
   #include <fstream>
   #include <iomanip>
  #endif
 #endif
#endif

namespace cpptrace {
    namespace detail {
        thread_local lazy_trace_holder current_exception_trace;

        CPPTRACE_FORCE_NO_INLINE void collect_current_trace(std::size_t skip) {
            current_exception_trace = lazy_trace_holder(cpptrace::generate_raw_trace(skip + 1));
        }

        #ifndef _MSC_VER
        // set only once by do_prepare_unwind_interceptor
        char (*intercept_unwind_handler)(std::size_t) = nullptr;

        CPPTRACE_FORCE_NO_INLINE
        bool intercept_unwind(const std::type_info*, const std::type_info*, void**, unsigned) {
            if(intercept_unwind_handler) {
                intercept_unwind_handler(1);
            }
            return false;
        }

        CPPTRACE_FORCE_NO_INLINE
        bool unconditional_exception_unwind_interceptor(const std::type_info*, const std::type_info*, void**, unsigned) {
            collect_current_trace(1);
            return false;
        }

        using do_catch_fn = decltype(intercept_unwind);

        unwind_interceptor::~unwind_interceptor() = default;
        unconditional_unwind_interceptor::~unconditional_unwind_interceptor() = default;

        #if IS_LIBSTDCXX
            constexpr size_t vtable_size = 11;
        #elif IS_LIBCXX
            constexpr size_t vtable_size = 10;
        #else
            #warning "Cpptrace from_current: Unrecognized C++ standard library, from_current() won't be supported"
            constexpr size_t vtable_size = 0;
        #endif

        #if IS_WINDOWS
        int get_page_size() {
            SYSTEM_INFO info;
            GetSystemInfo(&info);
            return info.dwPageSize;
        }
        constexpr auto memory_readonly = PAGE_READONLY;
        constexpr auto memory_readwrite = PAGE_READWRITE;
        int mprotect_page_and_return_old_protections(void* page, int page_size, int protections) {
            DWORD old_protections;
            if(!VirtualProtect(page, page_size, protections, &old_protections)) {
                throw std::runtime_error(
                    microfmt::format(
                        "VirtualProtect call failed: {}",
                        std::system_error(GetLastError(), std::system_category()).what()
                    )
                );
            }
            return old_protections;
        }
        void mprotect_page(void* page, int page_size, int protections) {
            mprotect_page_and_return_old_protections(page, page_size, protections);
        }
        void* allocate_page(int page_size) {
            auto page = VirtualAlloc(nullptr, page_size, MEM_COMMIT | MEM_RESERVE, memory_readwrite);
            if(!page) {
                throw std::runtime_error(
                    microfmt::format(
                        "VirtualAlloc call failed: {}",
                        std::system_error(GetLastError(), std::system_category()).what()
                    )
                );
            }
            return page;
        }
        #else
        int get_page_size() {
            return getpagesize();
        }
        constexpr auto memory_readonly = PROT_READ;
        constexpr auto memory_readwrite = PROT_READ | PROT_WRITE;
        #if IS_APPLE
        int get_page_protections(void* page) {
            // https://stackoverflow.com/a/12627784/15675011
            mach_vm_size_t vmsize;
            mach_vm_address_t address = (mach_vm_address_t)page;
            vm_region_basic_info_data_t info;
            mach_msg_type_number_t info_count =
                sizeof(size_t) == 8 ? VM_REGION_BASIC_INFO_COUNT_64 : VM_REGION_BASIC_INFO_COUNT;
            memory_object_name_t object;
            kern_return_t status = mach_vm_region(
                mach_task_self(),
                &address,
                &vmsize,
                VM_REGION_BASIC_INFO,
                (vm_region_info_t)&info,
                &info_count,
                &object
            );
            if(status == KERN_INVALID_ADDRESS) {
                throw std::runtime_error("vm_region failed with KERN_INVALID_ADDRESS");
            }
            int perms = 0;
            if(info.protection & VM_PROT_READ) {
                perms |= PROT_READ;
            }
            if(info.protection & VM_PROT_WRITE) {
                perms |= PROT_WRITE;
            }
            if(info.protection & VM_PROT_EXECUTE) {
                perms |= PROT_EXEC;
            }
            return perms;
        }
        #else
        int get_page_protections(void* page) {
            auto page_addr = reinterpret_cast<uintptr_t>(page);
            std::ifstream stream("/proc/self/maps");
            stream>>std::hex;
            while(!stream.eof()) {
                uintptr_t start;
                uintptr_t stop;
                stream>>start;
                stream.ignore(1); // dash
                stream>>stop;
                if(stream.eof()) {
                    break;
                }
                if(stream.fail()) {
                    throw std::runtime_error("Failure reading /proc/self/maps");
                }
                if(page_addr >= start && page_addr < stop) {
                    stream.ignore(1); // space
                    char r, w, x; // there's a private/shared flag after these but we don't need it
                    stream>>r>>w>>x;
                    if(stream.fail() || stream.eof()) {
                        throw std::runtime_error("Failure reading /proc/self/maps");
                    }
                    int perms = 0;
                    if(r == 'r') {
                        perms |= PROT_READ;
                    }
                    if(w == 'w') {
                        perms |= PROT_WRITE;
                    }
                    if(x == 'x') {
                        perms |= PROT_EXEC;
                    }
                    // std::cerr<<"--parsed: "<<std::hex<<start<<" "<<stop<<" "<<r<<w<<x<<std::endl;
                    return perms;
                }
                stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            throw std::runtime_error("Failed to find mapping with page in /proc/self/maps");
        }
        #endif
        void mprotect_page(void* page, int page_size, int protections) {
            if(mprotect(page, page_size, protections) != 0) {
                throw std::runtime_error(microfmt::format("mprotect call failed: {}", strerror(errno)));
            }
        }
        int mprotect_page_and_return_old_protections(void* page, int page_size, int protections) {
            auto old_protections = get_page_protections(page);
            mprotect_page(page, page_size, protections);
            return old_protections;
        }
        void* allocate_page(int page_size) {
            auto page = mmap(nullptr, page_size, memory_readwrite, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
            if(page == MAP_FAILED) {
                throw std::runtime_error(microfmt::format("mmap call failed: {}", strerror(errno)));
            }
            return page;
        }
        #endif

        void perform_typeinfo_surgery(const std::type_info& info, do_catch_fn* do_catch_function) {
            if(vtable_size == 0) { // set to zero if we don't know what standard library we're working with
                return;
            }
            void* type_info_pointer = const_cast<void*>(static_cast<const void*>(&info));
            void* type_info_vtable_pointer = *static_cast<void**>(type_info_pointer);
            // the type info vtable pointer points to two pointers inside the vtable, adjust it back
            type_info_vtable_pointer = static_cast<void*>(static_cast<void**>(type_info_vtable_pointer) - 2);

            // for libstdc++ the class type info vtable looks like
            // 0x7ffff7f89d18 <_ZTVN10__cxxabiv117__class_type_infoE>:    0x0000000000000000  0x00007ffff7f89d00
            //                                                            [offset           ][typeinfo pointer ]
            // 0x7ffff7f89d28 <_ZTVN10__cxxabiv117__class_type_infoE+16>: 0x00007ffff7dd65a0  0x00007ffff7dd65c0
            //                                                            [base destructor  ][deleting dtor    ]
            // 0x7ffff7f89d38 <_ZTVN10__cxxabiv117__class_type_infoE+32>: 0x00007ffff7dd8f10  0x00007ffff7dd8f10
            //                                                            [__is_pointer_p   ][__is_function_p  ]
            // 0x7ffff7f89d48 <_ZTVN10__cxxabiv117__class_type_infoE+48>: 0x00007ffff7dd6640  0x00007ffff7dd6500
            //                                                            [__do_catch       ][__do_upcast      ]
            // 0x7ffff7f89d58 <_ZTVN10__cxxabiv117__class_type_infoE+64>: 0x00007ffff7dd65e0  0x00007ffff7dd66d0
            //                                                            [__do_upcast      ][__do_dyncast     ]
            // 0x7ffff7f89d68 <_ZTVN10__cxxabiv117__class_type_infoE+80>: 0x00007ffff7dd6580  0x00007ffff7f8abe8
            //                                                            [__do_find_public_src][other         ]
            // In libc++ the layout is
            //  [offset           ][typeinfo pointer ]
            //  [base destructor  ][deleting dtor    ]
            //  [noop1            ][noop2            ]
            //  [can_catch        ][search_above_dst ]
            //  [search_below_dst ][has_unambiguous_public_base]
            // Relevant documentation/implementation:
            //  https://itanium-cxx-abi.github.io/cxx-abi/abi.html
            //  libstdc++
            //   https://github.com/gcc-mirror/gcc/blob/b13e34699c7d27e561fcfe1b66ced1e50e69976f/libstdc%252B%252B-v3/libsupc%252B%252B/typeinfo
            //   https://github.com/gcc-mirror/gcc/blob/b13e34699c7d27e561fcfe1b66ced1e50e69976f/libstdc%252B%252B-v3/libsupc%252B%252B/class_type_info.cc
            //  libc++
            //   https://github.com/llvm/llvm-project/blob/648f4d0658ab00cf1e95330c8811aaea9481a274/libcxx/include/typeinfo
            //   https://github.com/llvm/llvm-project/blob/648f4d0658ab00cf1e95330c8811aaea9481a274/libcxxabi/src/private_typeinfo.h

            // shouldn't be anything other than 4096 but out of an abundance of caution
            auto page_size = get_page_size();
            if(page_size <= 0 && (page_size & (page_size - 1)) != 0) {
                throw std::runtime_error(
                    microfmt::format("getpagesize() is not a power of 2 greater than zero (was {})", page_size)
                );
            }

            // allocate a page for the new vtable so it can be made read-only later
            // the OS cleans this up, no cleanup done here for it
            void* new_vtable_page = allocate_page(page_size);
            // make our own copy of the vtable
            memcpy(new_vtable_page, type_info_vtable_pointer, vtable_size * sizeof(void*));
            // ninja in the custom __do_catch interceptor
            auto new_vtable = static_cast<void**>(new_vtable_page);
            new_vtable[6] = reinterpret_cast<void*>(do_catch_function);
            // make the page read-only
            mprotect_page(new_vtable_page, page_size, memory_readonly);

            // make the vtable pointer for unwind_interceptor's type_info point to the new vtable
            auto type_info_addr = reinterpret_cast<uintptr_t>(type_info_pointer);
            auto page_addr = type_info_addr & ~(page_size - 1);
            // make sure the memory we're going to set is within the page
            if(type_info_addr - page_addr + sizeof(void*) > static_cast<unsigned>(page_size)) {
                throw std::runtime_error("pointer crosses page boundaries");
            }
            auto old_protections = mprotect_page_and_return_old_protections(
                reinterpret_cast<void*>(page_addr),
                page_size,
                memory_readwrite
            );
            *static_cast<void**>(type_info_pointer) = static_cast<void*>(new_vtable + 2);
            mprotect_page(reinterpret_cast<void*>(page_addr), page_size, old_protections);
        }

        void do_prepare_unwind_interceptor(char(*intercept_unwind_handler)(std::size_t)) {
            static bool did_prepare = false;
            if(!did_prepare) {
                cpptrace::detail::intercept_unwind_handler = intercept_unwind_handler;
                try {
                    perform_typeinfo_surgery(typeid(cpptrace::detail::unwind_interceptor), intercept_unwind);
                    perform_typeinfo_surgery(
                        typeid(cpptrace::detail::unconditional_unwind_interceptor),
                        unconditional_exception_unwind_interceptor
                    );
                } catch(std::exception& e) {
                    std::fprintf(
                        stderr,
                        "Cpptrace: Exception occurred while preparing from_current support: %s\n",
                        e.what()
                    );
                } catch(...) {
                    std::fprintf(stderr, "Cpptrace: Unknown exception occurred while preparing from_current support\n");
                }
                did_prepare = true;
            }
        }
        #endif
    }

    const raw_trace& raw_trace_from_current_exception() {
        return detail::current_exception_trace.get_raw_trace();
    }

    const stacktrace& from_current_exception() {
        return detail::current_exception_trace.get_resolved_trace();
    }
}
