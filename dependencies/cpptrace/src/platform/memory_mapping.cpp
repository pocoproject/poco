#include "platform/memory_mapping.hpp"

#include "utils/optional.hpp"
#include "utils/utils.hpp"

#ifndef _MSC_VER

#if IS_WINDOWS
 #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
 #endif
 #include <windows.h>
#else
 #include <sys/mman.h>
 #include <unistd.h>
 #if IS_APPLE
  #include <mach/mach.h>
  #ifdef CPPTRACE_HAS_MACH_VM
   #include <mach/mach_vm.h>
  #endif
 #else
  #include <fstream>
  #include <ios>
 #endif
#endif

CPPTRACE_BEGIN_NAMESPACE
namespace detail {
    #if IS_WINDOWS
    int get_page_size() {
        SYSTEM_INFO info;
        GetSystemInfo(&info);
        return info.dwPageSize;
    }
    int mprotect_page_and_return_old_protections(void* page, int page_size, int protections) {
        DWORD old_protections;
        if(!VirtualProtect(page, page_size, protections, &old_protections)) {
            throw internal_error(
                "VirtualProtect call failed: {}",
                std::system_error(GetLastError(), std::system_category()).what()
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
            throw internal_error(
                "VirtualAlloc call failed: {}",
                std::system_error(GetLastError(), std::system_category()).what()
            );
        }
        return page;
    }
    #else
    int get_page_size() {
        #if defined(_SC_PAGESIZE)
         return sysconf(_SC_PAGESIZE);
        #else
         return getpagesize();
        #endif
    }
    #if IS_APPLE
    int get_page_protections(void* page) {
        // https://stackoverflow.com/a/12627784/15675011
        #ifdef CPPTRACE_HAS_MACH_VM
        mach_vm_size_t vmsize;
        mach_vm_address_t address = (mach_vm_address_t)page;
        #else
        vm_size_t vmsize;
        vm_address_t address = (vm_address_t)page;
        #endif
        vm_region_basic_info_data_t info;
        mach_msg_type_number_t info_count =
            sizeof(size_t) == 8 ? VM_REGION_BASIC_INFO_COUNT_64 : VM_REGION_BASIC_INFO_COUNT;
        memory_object_name_t object;
        kern_return_t status =
        #ifdef CPPTRACE_HAS_MACH_VM
        mach_vm_region
        #else
        vm_region_64
        #endif
        (
            mach_task_self(),
            &address,
            &vmsize,
            VM_REGION_BASIC_INFO,
            (vm_region_info_t)&info,
            &info_count,
            &object
        );
        if(status == KERN_INVALID_ADDRESS) {
            throw internal_error("vm_region failed with KERN_INVALID_ADDRESS");
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
    // Code for reading /proc/self/maps
    // Unfortunately this is the canonical and only way to get memory permissions on linux
    // It comes with some surprising behaviors. Because it's a pseudo-file and maps could update at any time, reads of
    // the file can tear. The surprising observable behavior here is overlapping ranges:
    // - https://unix.stackexchange.com/questions/704987/overlapping-address-ranges-in-proc-maps
    // - https://stackoverflow.com/questions/59737950/what-is-the-correct-way-to-get-a-consistent-snapshot-of-proc-pid-smaps
    // Additional info:
    //   Note: reading /proc/PID/maps or /proc/PID/smaps is inherently racy (consistent
    //   output can be achieved only in the single read call).
    //   This typically manifests when doing partial reads of these files while the
    //   memory map is being modified.  Despite the races, we do provide the following
    //   guarantees:
    //
    //   1) The mapped addresses never go backwards, which implies no two
    //      regions will ever overlap.
    //   2) If there is something at a given vaddr during the entirety of the
    //      life of the smaps/maps walk, there will be some output for it.
    //
    //   https://www.kernel.org/doc/Documentation/filesystems/proc.txt
    // Ideally we could do everything as a single read() call but I don't think that's practical, especially given that
    // the kernel has limited buffers internally. While we shouldn't be modifying mapped memory while reading
    // /proc/self/maps here, it's theoretically possible that we could allocate and that could go to the OS for more
    // pages.
    // While reading this is inherently racy, as far as I can tell tears don't happen within a line but they can happen
    // between lines.
    // The code that writes /proc/pid/maps:
    // - https://github.com/torvalds/linux/blob/3d0ebc36b0b3e8486ceb6e08e8ae173aaa6d1221/fs/proc/task_mmu.c#L304-L365

    struct address_range {
        uintptr_t low;
        uintptr_t high;
        int perms;
        bool operator<(const address_range& other) const {
            return low < other.low;
        }
    };

    // returns nullopt on eof
    optional<address_range> read_map_entry(std::ifstream& stream) {
        uintptr_t start;
        uintptr_t stop;
        stream>>start;
        stream.ignore(1); // dash
        stream>>stop;
        if(stream.eof()) {
            return nullopt;
        }
        if(stream.fail()) {
            throw internal_error("Failure reading /proc/self/maps");
        }
        stream.ignore(1); // space
        char r, w, x; // there's a private/shared flag after these but we don't need it
        stream>>r>>w>>x;
        if(stream.fail() || stream.eof()) {
            throw internal_error("Failure reading /proc/self/maps");
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
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return address_range{start, stop, perms};
    }

    // returns a vector or nullopt if a tear is detected
    optional<std::vector<address_range>> try_load_mapped_region_info() {
        std::ifstream stream("/proc/self/maps");
        stream>>std::hex;
        std::vector<address_range> ranges;
        while(auto entry = read_map_entry(stream)) {
            const auto& range = entry.unwrap();
            VERIFY(range.low <= range.high);
            if(!ranges.empty()) {
                const auto& last_range = ranges.back();
                if(range.low < last_range.high) {
                    return nullopt;
                }
            }
            ranges.push_back(range);
        }
        return ranges;
    }

    // we can allocate during try_load_mapped_region_info, in theory that could cause a tear
    optional<std::vector<address_range>> try_load_mapped_region_info_with_retries(int n) {
        VERIFY(n > 0);
        for(int i = 0; i < n; i++) {
            if(auto info = try_load_mapped_region_info()) {
                return info;
            }
        }
        throw internal_error("Couldn't successfully load /proc/self/maps after {} retries", n);
    }

    const std::vector<address_range>& load_mapped_region_info() {
        static std::vector<address_range> regions;
        static bool has_loaded = false;
        if(!has_loaded) {
            has_loaded = true;
            if(auto info = try_load_mapped_region_info_with_retries(2)) {
                regions = std::move(info).unwrap();
            }
        }
        return regions;
    }

    int get_page_protections(void* page) {
        const auto& mapped_region_info = load_mapped_region_info();
        auto it = first_less_than_or_equal(
            mapped_region_info.begin(),
            mapped_region_info.end(),
            reinterpret_cast<uintptr_t>(page),
            [](uintptr_t a, const address_range& b) {
                return a < b.low;
            }
        );
        if(it == mapped_region_info.end()) {
            throw internal_error(
                "Failed to find mapping for {>16:0h} in /proc/self/maps",
                reinterpret_cast<uintptr_t>(page)
            );
        }
        return it->perms;
    }
    #endif
    void mprotect_page(void* page, int page_size, int protections) {
        if(mprotect(page, page_size, protections) != 0) {
            throw internal_error("mprotect call failed: {}", strerror(errno));
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
            throw internal_error("mmap call failed: {}", strerror(errno));
        }
        return page;
    }
    #endif
}
CPPTRACE_END_NAMESPACE

#endif
