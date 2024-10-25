#ifdef CPPTRACE_GET_SYMBOLS_WITH_LIBDWARF

#include "symbols/dwarf/resolver.hpp"

#include <cpptrace/cpptrace.hpp>
#include "symbols/dwarf/dwarf.hpp" // has dwarf #includes
#include "symbols/symbols.hpp"
#include "utils/common.hpp"
#include "utils/error.hpp"
#include "utils/utils.hpp"
#include "platform/path.hpp"
#include "platform/program_name.hpp" // For CPPTRACE_MAX_PATH
#include "binary/mach-o.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

// It's been tricky to piece together how to handle all this dwarf stuff. Some resources I've used are
// https://www.prevanders.net/libdwarf.pdf
// https://github.com/davea42/libdwarf-addr2line
// https://github.com/ruby/ruby/blob/master/addr2line.c

namespace cpptrace {
namespace detail {
namespace libdwarf {
    // printbugging as we go
    constexpr bool dump_dwarf = false;
    constexpr bool trace_dwarf = false;

    struct subprogram_entry {
        die_object die;
        Dwarf_Addr low;
        Dwarf_Addr high;
    };

    struct cu_entry {
        die_object die;
        Dwarf_Half dwversion;
        Dwarf_Addr low;
        Dwarf_Addr high;
    };

    struct line_entry {
        Dwarf_Addr low;
        // Dwarf_Addr high;
        // int i;
        Dwarf_Line line;
        optional<std::string> path;
        optional<std::uint32_t> line_number;
        optional<std::uint32_t> column_number;
        line_entry(Dwarf_Addr low, Dwarf_Line line) : low(low), line(line) {}
    };

    struct line_table_info {
        Dwarf_Unsigned version;
        Dwarf_Line_Context line_context;
        // sorted by low_addr
        // TODO: Make this optional at some point, it may not be generated if cache mode switches during program exec...
        std::vector<line_entry> line_entries;
    };

    class dwarf_resolver;

    // used to describe data from an upstream binary to a resolver for the .dwo
    struct skeleton_info {
        die_object cu_die;
        Dwarf_Half dwversion;
        dwarf_resolver& resolver;
    };

    class dwarf_resolver : public symbol_resolver {
        std::string object_path;
        Dwarf_Debug dbg = nullptr;
        bool ok = false;
        // .debug_aranges cache
        Dwarf_Arange* aranges = nullptr;
        Dwarf_Signed arange_count = 0;
        // Map from CU -> Line context
        std::unordered_map<Dwarf_Off, line_table_info> line_tables;
        // Map from CU -> Sorted subprograms vector
        std::unordered_map<Dwarf_Off, std::vector<subprogram_entry>> subprograms_cache;
        // Vector of ranges and their corresponding CU offsets
        std::vector<cu_entry> cu_cache;
        bool generated_cu_cache = false;
        // Map from CU -> {srcfiles, count}
        std::unordered_map<Dwarf_Off, std::pair<char**, Dwarf_Signed>> srcfiles_cache;
        // Map from CU -> split full cu resolver
        std::unordered_map<Dwarf_Off, std::unique_ptr<dwarf_resolver>> split_full_cu_resolvers;
        // info for resolving a dwo object
        optional<skeleton_info> skeleton;

    private:
        // Error handling helper
        // For some reason R (*f)(Args..., void*)-style deduction isn't possible, seems like a bug in all compilers
        // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56190
        template<
            typename... Args,
            typename... Args2,
            typename std::enable_if<
                std::is_same<
                    decltype(
                        (void)std::declval<int(Args...)>()(std::forward<Args2>(std::declval<Args2>())..., nullptr)
                    ),
                    void
                >::value,
                int
            >::type = 0
        >
        int wrap(int (*f)(Args...), Args2&&... args) const {
            Dwarf_Error error = nullptr;
            int ret = f(std::forward<Args2>(args)..., &error);
            if(ret == DW_DLV_ERROR) {
                handle_dwarf_error(dbg, error);
            }
            return ret;
        }

    public:
        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        dwarf_resolver(const std::string& object_path_, optional<skeleton_info> split_ = nullopt)
            : object_path(object_path_),
              skeleton(std::move(split_))
        {
            // use a buffer when invoking dwarf_init_path, which allows it to automatically find debuglink or dSYM
            // sources
            bool use_buffer = true;
            // for universal / fat mach-o files
            unsigned universal_number = 0;
            #if IS_APPLE
            if(directory_exists(object_path + ".dSYM")) {
                // Possibly depends on the build system but a obj.cpp.o.dSYM/Contents/Resources/DWARF/obj.cpp.o can be
                // created alongside .o files. These are text files containing directives, as opposed to something we
                // can actually use
                std::string dsym_resource = object_path + ".dSYM/Contents/Resources/DWARF/" + basename(object_path);
                if(file_is_mach_o(dsym_resource)) {
                    object_path = std::move(dsym_resource);
                }
                use_buffer = false; // we resolved dSYM above as appropriate
            }
            auto result = macho_is_fat(object_path);
            if(result.is_error()) {
                result.drop_error();
            } else if(result.unwrap_value()) {
                auto obj = mach_o::open_mach_o(object_path);
                if(!obj) {
                    ok = false;
                    return;
                }
                universal_number = obj.unwrap_value().get_fat_index();
            }
            #endif

            // Giving libdwarf a buffer for a true output path is needed for its automatic resolution of debuglink and
            // dSYM files. We don't utilize the dSYM logic here, we just care about debuglink.
            std::unique_ptr<char[]> buffer;
            if(use_buffer) {
                buffer = std::unique_ptr<char[]>(new char[CPPTRACE_MAX_PATH]);
            }
            auto ret = wrap(
                dwarf_init_path_a,
                object_path.c_str(),
                buffer.get(),
                CPPTRACE_MAX_PATH,
                DW_GROUPNUMBER_ANY,
                universal_number,
                nullptr,
                nullptr,
                &dbg
            );
            if(ret == DW_DLV_OK) {
                ok = true;
            } else if(ret == DW_DLV_NO_ENTRY) {
                // fail, no debug info
                ok = false;
            } else {
                ok = false;
                PANIC("Unknown return code from dwarf_init_path");
            }

            if(skeleton) {
                VERIFY(wrap(dwarf_set_tied_dbg, dbg, skeleton.unwrap().resolver.dbg) == DW_DLV_OK);
            }

            if(ok) {
                // Check for .debug_aranges for fast lookup
                wrap(dwarf_get_aranges, dbg, &aranges, &arange_count);
            }
        }

        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        ~dwarf_resolver() override {
            // TODO: Maybe redundant since dwarf_finish(dbg); will clean up the line stuff anyway but may as well just
            // for thoroughness
            for(auto& entry : line_tables) {
                dwarf_srclines_dealloc_b(entry.second.line_context);
            }
            for(auto& entry : srcfiles_cache) {
                dwarf_dealloc(dbg, entry.second.first, DW_DLA_LIST);
            }
            // subprograms_cache needs to be destroyed before dbg otherwise there will be another use after free
            subprograms_cache.clear();
            split_full_cu_resolvers.clear();
            skeleton.reset();
            if(aranges) {
                dwarf_dealloc(dbg, aranges, DW_DLA_LIST);
            }
            cu_cache.clear();
            dwarf_finish(dbg);
        }

        dwarf_resolver(const dwarf_resolver&) = delete;
        dwarf_resolver& operator=(const dwarf_resolver&) = delete;
        dwarf_resolver(dwarf_resolver&&) = delete;
        dwarf_resolver& operator=(dwarf_resolver&&) = delete;

    private:
        // walk all CU's in a dbg, callback is called on each die and should return true to
        // continue traversal
        void walk_compilation_units(const std::function<bool(const die_object&)>& fn) {
            // libdwarf keeps track of where it is in the file, dwarf_next_cu_header_d is statefull
            Dwarf_Unsigned next_cu_header;
            Dwarf_Half header_cu_type;
            while(true) {
                int ret = wrap(
                    dwarf_next_cu_header_d,
                    dbg,
                    true,
                    nullptr,
                    nullptr,
                    nullptr,
                    nullptr,
                    nullptr,
                    nullptr,
                    nullptr,
                    nullptr,
                    &next_cu_header,
                    &header_cu_type
                );
                if(ret == DW_DLV_NO_ENTRY) {
                    if(dump_dwarf) {
                        std::fprintf(stderr, "End walk_dbg\n");
                    }
                    return;
                }
                if(ret != DW_DLV_OK) {
                    PANIC("Unexpected return code from dwarf_next_cu_header_d");
                    return;
                }
                // 0 passed as the die to the first call of dwarf_siblingof_b immediately after dwarf_next_cu_header_d
                // to fetch the cu die
                die_object cu_die(dbg, nullptr);
                cu_die = cu_die.get_sibling();
                if(!cu_die) {
                    break;
                }
                if(!walk_die_list(cu_die, fn)) {
                    break;
                }
            }
            if(dump_dwarf) {
                std::fprintf(stderr, "End walk_compilation_units\n");
            }
        }

        void lazy_generate_cu_cache() {
            if(!generated_cu_cache) {
                walk_compilation_units([this] (const die_object& cu_die) {
                    Dwarf_Half offset_size = 0;
                    Dwarf_Half dwversion = 0;
                    dwarf_get_version_of_die(cu_die.get(), &dwversion, &offset_size);
                    if(skeleton) {
                        // NOTE: If we have a corresponding skeleton, we assume we have one CU matching the skeleton CU
                        // Precedence for this assumption is https://dwarfstd.org/doc/DWARF5.pdf#subsection.3.1.3
                        // TODO: Also assuming same dwversion
                        const auto& skeleton_cu = skeleton.unwrap().cu_die;
                        auto ranges_vec = skeleton_cu.get_rangelist_entries(skeleton_cu, dwversion);
                        for(auto range : ranges_vec) {
                            // TODO: Reduce cloning here
                            cu_cache.push_back({ cu_die.clone(), dwversion, range.first, range.second });
                        }
                        return false;
                    } else {
                        auto ranges_vec = cu_die.get_rangelist_entries(cu_die, dwversion);
                        for(auto range : ranges_vec) {
                            // TODO: Reduce cloning here
                            cu_cache.push_back({ cu_die.clone(), dwversion, range.first, range.second });
                        }
                        return true;
                    }
                });
                std::sort(cu_cache.begin(), cu_cache.end(), [] (const cu_entry& a, const cu_entry& b) {
                    return a.low < b.low;
                });
                generated_cu_cache = true;
            }
        }

        std::string subprogram_symbol(
            const die_object& die,
            Dwarf_Half dwversion
        ) {
            ASSERT(die.get_tag() == DW_TAG_subprogram || die.get_tag() == DW_TAG_inlined_subroutine);
            optional<std::string> name;
            if(auto linkage_name = die.get_string_attribute(DW_AT_linkage_name)) {
                name = std::move(linkage_name);
            } else if(auto linkage_name = die.get_string_attribute(DW_AT_MIPS_linkage_name)) {
                name = std::move(linkage_name);
            } else if(auto linkage_name = die.get_string_attribute(DW_AT_name)) {
                name = std::move(linkage_name);
            }
            if(name.has_value()) {
                return std::move(name).unwrap();
            } else {
                if(die.has_attr(DW_AT_specification)) {
                    die_object spec = die.resolve_reference_attribute(DW_AT_specification);
                    return subprogram_symbol(spec, dwversion);
                } else if(die.has_attr(DW_AT_abstract_origin)) {
                    die_object spec = die.resolve_reference_attribute(DW_AT_abstract_origin);
                    return subprogram_symbol(spec, dwversion);
                }
            }
            return "";
        }

        // despite (some) dwarf using 1-indexing, file_i should be the 0-based index
        std::string resolve_filename(const die_object& cu_die, Dwarf_Unsigned file_i) {
            // for split-dwarf line resolution happens in the skeleton
            if(skeleton) {
                return skeleton.unwrap().resolver.resolve_filename(skeleton.unwrap().cu_die, file_i);
            }
            std::string filename;
            if(get_cache_mode() == cache_mode::prioritize_memory) {
                char** dw_srcfiles;
                Dwarf_Signed dw_filecount;
                VERIFY(wrap(dwarf_srcfiles, cu_die.get(), &dw_srcfiles, &dw_filecount) == DW_DLV_OK);
                if(Dwarf_Signed(file_i) < dw_filecount) {
                    // dwarf is using 1-indexing
                    filename = dw_srcfiles[file_i];
                }
                dwarf_dealloc(cu_die.dbg, dw_srcfiles, DW_DLA_LIST);
            } else {
                auto off = cu_die.get_global_offset();
                auto it = srcfiles_cache.find(off);
                if(it == srcfiles_cache.end()) {
                    char** dw_srcfiles;
                    Dwarf_Signed dw_filecount;
                    VERIFY(wrap(dwarf_srcfiles, cu_die.get(), &dw_srcfiles, &dw_filecount) == DW_DLV_OK);
                    it = srcfiles_cache.insert(it, {off, {dw_srcfiles, dw_filecount}});
                }
                char** dw_srcfiles = it->second.first;
                Dwarf_Signed dw_filecount = it->second.second;
                if(Dwarf_Signed(file_i) < dw_filecount) {
                    // dwarf is using 1-indexing
                    filename = dw_srcfiles[file_i];
                }
            }
            return filename;
        }

        void get_inlines_info(
            const die_object& cu_die,
            const die_object& die,
            Dwarf_Addr pc,
            Dwarf_Half dwversion,
            std::vector<stacktrace_frame>& inlines
        ) {
            ASSERT(die.get_tag() == DW_TAG_subprogram || die.get_tag() == DW_TAG_inlined_subroutine);
            // get_inlines_info is recursive and recurses into dies with pc ranges matching the pc we're looking for,
            // however, because I wouldn't want anything stack overflowing I'm breaking the recursion out into a loop
            // while looping when we find the target die we need to be able to store a die somewhere that doesn't die
            // at the end of the list traversal, we'll use this as a holder for it
            die_object current_obj_holder(dbg, nullptr);
            optional<std::reference_wrapper<const die_object>> current_die = die;
            while(current_die.has_value()) {
                auto child = current_die.unwrap().get().get_child();
                if(!child) {
                    break;
                }
                optional<std::reference_wrapper<const die_object>> target_die;
                walk_die_list(
                    child,
                    [this, &cu_die, pc, dwversion, &inlines, &target_die, &current_obj_holder] (const die_object& die) {
                        if(die.get_tag() == DW_TAG_inlined_subroutine && die.pc_in_die(cu_die, dwversion, pc)) {
                            const auto name = subprogram_symbol(die, dwversion);
                            auto file_i = die.get_unsigned_attribute(DW_AT_call_file);
                            // TODO: Refactor.... Probably put logic in resolve_filename.
                            if(file_i) {
                                // for dwarf 2, 3, 4, and experimental line table version 0xfe06 1-indexing is used
                                // for dwarf 5 0-indexing is used
                                optional<std::reference_wrapper<line_table_info>> line_table_opt;
                                if(skeleton) {
                                    line_table_opt = skeleton.unwrap().resolver.get_line_table(
                                        skeleton.unwrap().cu_die
                                    );
                                } else {
                                    line_table_opt = get_line_table(cu_die);
                                }
                                if(line_table_opt) {
                                    auto& line_table = line_table_opt.unwrap().get();
                                    if(line_table.version != 5) {
                                        if(file_i.unwrap() == 0) {
                                            file_i.reset(); // 0 means no name to be found
                                        } else {
                                            // decrement to 0-based index
                                            file_i.unwrap()--;
                                        }
                                    }
                                } else {
                                    // silently continue
                                }
                            }
                            std::string file = file_i ? resolve_filename(cu_die, file_i.unwrap()) : "";
                            const auto line = die.get_unsigned_attribute(DW_AT_call_line);
                            const auto col = die.get_unsigned_attribute(DW_AT_call_column);
                            inlines.push_back(stacktrace_frame{
                                0,
                                0, // TODO: Could put an object address here...
                                {static_cast<std::uint32_t>(line.value_or(0))},
                                {static_cast<std::uint32_t>(col.value_or(0))},
                                file,
                                name,
                                true
                            });
                            current_obj_holder = die.clone();
                            target_die = current_obj_holder;
                            return false;
                        } else {
                            return true;
                        }
                    }
                );
                // recursing into the found target as-if by get_inlines_info(cu_die, die, pc, dwversion, inlines);
                current_die = target_die;
            }
        }

        std::string retrieve_symbol_for_subprogram(
            const die_object& cu_die,
            const die_object& die,
            Dwarf_Addr pc,
            Dwarf_Half dwversion,
            std::vector<stacktrace_frame>& inlines
        ) {
            ASSERT(die.get_tag() == DW_TAG_subprogram);
            const auto name = subprogram_symbol(die, dwversion);
            if(detail::should_resolve_inlined_calls()) {
                get_inlines_info(cu_die, die, pc, dwversion, inlines);
            }
            return name;
        }

        // returns true if this call found the symbol
        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        bool retrieve_symbol_walk(
            const die_object& cu_die,
            const die_object& die,
            Dwarf_Addr pc,
            Dwarf_Half dwversion,
            stacktrace_frame& frame,
            std::vector<stacktrace_frame>& inlines
        ) {
            bool found = false;
            walk_die_list(
                die,
                [this, &cu_die, pc, dwversion, &frame, &inlines, &found] (const die_object& die) {
                    if(dump_dwarf) {
                        std::fprintf(
                            stderr,
                            "-------------> %08llx %s %s\n",
                            to_ull(die.get_global_offset()),
                            die.get_tag_name(),
                            die.get_name().c_str()
                        );
                    }
                    if(!(die.get_tag() == DW_TAG_namespace || die.pc_in_die(cu_die, dwversion, pc))) {
                        if(dump_dwarf) {
                            std::fprintf(stderr, "pc not in die\n");
                        }
                    } else {
                        if(trace_dwarf) {
                            std::fprintf(
                                stderr,
                                "%s %08llx %s\n",
                                die.get_tag() == DW_TAG_namespace ? "pc maybe in die (namespace)" : "pc in die",
                                to_ull(die.get_global_offset()),
                                die.get_tag_name()
                            );
                        }
                        if(die.get_tag() == DW_TAG_subprogram) {
                            frame.symbol = retrieve_symbol_for_subprogram(cu_die, die, pc, dwversion, inlines);
                            found = true;
                            return false;
                        }
                        auto child = die.get_child();
                        if(child) {
                            if(retrieve_symbol_walk(cu_die, child, pc, dwversion, frame, inlines)) {
                                found = true;
                                return false;
                            }
                        } else {
                            if(dump_dwarf) {
                                std::fprintf(stderr, "(no child)\n");
                            }
                        }
                    }
                    return true;
                }
            );
            if(dump_dwarf) {
                std::fprintf(stderr, "End walk_die_list\n");
            }
            return found;
        }

        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        void preprocess_subprograms(
            const die_object& cu_die,
            const die_object& die,
            Dwarf_Half dwversion,
            std::vector<subprogram_entry>& vec
        ) {
            walk_die_list(
                die,
                [this, &cu_die, dwversion, &vec] (const die_object& die) {
                    switch(die.get_tag()) {
                        case DW_TAG_subprogram:
                            {
                                auto ranges_vec = die.get_rangelist_entries(cu_die, dwversion);
                                // TODO: Feels super inefficient and some day should maybe use an interval tree.
                                for(auto range : ranges_vec) {
                                    // TODO: Reduce cloning here
                                    vec.push_back({ die.clone(), range.first, range.second });
                                }
                                // Walk children to get things like lambdas
                                // TODO: Somehow find a way to get better names here? For gcc it's just "operator()"
                                // On clang it's better
                                auto child = die.get_child();
                                if(child) {
                                    preprocess_subprograms(cu_die, child, dwversion, vec);
                                }
                            }
                            break;
                        case DW_TAG_namespace:
                        case DW_TAG_structure_type:
                        case DW_TAG_class_type:
                        case DW_TAG_module:
                        case DW_TAG_imported_module:
                        case DW_TAG_compile_unit:
                            {
                                auto child = die.get_child();
                                if(child) {
                                    preprocess_subprograms(cu_die, child, dwversion, vec);
                                }
                            }
                            break;
                        default:
                            break;
                    }
                    return true;
                }
            );
            if(dump_dwarf) {
                std::fprintf(stderr, "End walk_die_list\n");
            }
        }

        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        void retrieve_symbol(
            const die_object& cu_die,
            Dwarf_Addr pc,
            Dwarf_Half dwversion,
            stacktrace_frame& frame,
            std::vector<stacktrace_frame>& inlines
        ) {
            if(get_cache_mode() == cache_mode::prioritize_memory) {
                retrieve_symbol_walk(cu_die, cu_die, pc, dwversion, frame, inlines);
            } else {
                auto off = cu_die.get_global_offset();
                auto it = subprograms_cache.find(off);
                if(it == subprograms_cache.end()) {
                    // TODO: Refactor. Do the sort in the preprocess function and return the vec directly.
                    std::vector<subprogram_entry> vec;
                    preprocess_subprograms(cu_die, cu_die, dwversion, vec);
                    std::sort(vec.begin(), vec.end(), [] (const subprogram_entry& a, const subprogram_entry& b) {
                        return a.low < b.low;
                    });
                    subprograms_cache.emplace(off, std::move(vec));
                    it = subprograms_cache.find(off);
                }
                auto& vec = it->second;
                auto vec_it = first_less_than_or_equal(
                    vec.begin(),
                    vec.end(),
                    pc,
                    [] (Dwarf_Addr pc, const subprogram_entry& entry) {
                        return pc < entry.low;
                    }
                );
                // If the vector has been empty this can happen
                if(vec_it != vec.end()) {
                    if(vec_it->die.pc_in_die(cu_die, dwversion, pc)) {
                        frame.symbol = retrieve_symbol_for_subprogram(cu_die, vec_it->die, pc, dwversion, inlines);
                    }
                } else {
                    ASSERT(vec.size() == 0, "Vec should be empty?");
                }
            }
        }

        // returns a reference to a CU's line table, may be invalidated if the line_tables map is modified
        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        optional<std::reference_wrapper<line_table_info>> get_line_table(const die_object& cu_die) {
            auto off = cu_die.get_global_offset();
            auto it = line_tables.find(off);
            if(it != line_tables.end()) {
                return it->second;
            } else {
                Dwarf_Unsigned version;
                Dwarf_Small table_count;
                Dwarf_Line_Context line_context;
                int ret = wrap(
                    dwarf_srclines_b,
                    cu_die.get(),
                    &version,
                    &table_count,
                    &line_context
                );
                static_assert(std::is_unsigned<decltype(table_count)>::value, "Expected unsigned Dwarf_Small");
                VERIFY(/*table_count >= 0 &&*/ table_count <= 2, "Unknown dwarf line table count");
                if(ret == DW_DLV_NO_ENTRY) {
                    // TODO: Failing silently for now
                    return nullopt;
                }
                VERIFY(ret == DW_DLV_OK);

                std::vector<line_entry> line_entries;

                if(get_cache_mode() == cache_mode::prioritize_speed) {
                    // build lookup table
                    Dwarf_Line* line_buffer = nullptr;
                    Dwarf_Signed line_count = 0;
                    Dwarf_Line* linebuf_actuals = nullptr;
                    Dwarf_Signed linecount_actuals = 0;
                    VERIFY(
                        wrap(
                            dwarf_srclines_two_level_from_linecontext,
                            line_context,
                            &line_buffer,
                            &line_count,
                            &linebuf_actuals,
                            &linecount_actuals
                        ) == DW_DLV_OK
                    );

                    // TODO: Make any attempt to note PC ranges? Handle line end sequence?
                    line_entries.reserve(line_count);
                    for(int i = 0; i < line_count; i++) {
                        Dwarf_Line line = line_buffer[i];
                        Dwarf_Addr low_addr = 0;
                        VERIFY(wrap(dwarf_lineaddr, line, &low_addr) == DW_DLV_OK);
                        // scan ahead for the last line entry matching this pc
                        int j;
                        for(j = i + 1; j < line_count; j++) {
                            Dwarf_Addr addr = 0;
                            VERIFY(wrap(dwarf_lineaddr, line_buffer[j], &addr) == DW_DLV_OK);
                            if(addr != low_addr) {
                                break;
                            }
                        }
                        line = line_buffer[j - 1];
                        // {
                        //     Dwarf_Unsigned line_number = 0;
                        //     VERIFY(wrap(dwarf_lineno, line, &line_number) == DW_DLV_OK);
                        //     frame.line = static_cast<std::uint32_t>(line_number);
                        //     char* filename = nullptr;
                        //     VERIFY(wrap(dwarf_linesrc, line, &filename) == DW_DLV_OK);
                        //     auto wrapper = raii_wrap(
                        //         filename,
                        //         [this] (char* str) { if(str) dwarf_dealloc(dbg, str, DW_DLA_STRING); }
                        //     );
                        //     frame.filename = filename;
                        //     printf("%s : %d\n", filename, line_number);
                        //     Dwarf_Bool is_line_end;
                        //     VERIFY(wrap(dwarf_lineendsequence, line, &is_line_end) == DW_DLV_OK);
                        //     if(is_line_end) {
                        //         puts("Line end");
                        //     }
                        // }
                        line_entries.push_back({
                            low_addr,
                            line
                        });
                        i = j - 1;
                    }
                    // sort lines
                    std::sort(line_entries.begin(), line_entries.end(), [] (const line_entry& a, const line_entry& b) {
                        return a.low < b.low;
                    });
                }

                it = line_tables.insert({off, {version, line_context, std::move(line_entries)}}).first;
                return it->second;
            }
        }

        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        void retrieve_line_info(
            const die_object& cu_die,
            Dwarf_Addr pc,
            stacktrace_frame& frame
        ) {
            // For debug fission the skeleton debug info will have the line table
            if(skeleton) {
                return skeleton.unwrap().resolver.retrieve_line_info(skeleton.unwrap().cu_die, pc, frame);
            }
            auto table_info_opt = get_line_table(cu_die);
            if(!table_info_opt) {
                return; // failing silently for now
            }
            auto& table_info = table_info_opt.unwrap().get();
            if(get_cache_mode() == cache_mode::prioritize_speed) {
                // Lookup in the table
                auto& line_entries = table_info.line_entries;
                auto table_it = first_less_than_or_equal(
                    line_entries.begin(),
                    line_entries.end(),
                    pc,
                    [] (Dwarf_Addr pc, const line_entry& entry) {
                        return pc < entry.low;
                    }
                );
                // If the vector has been empty this can happen
                if(table_it != line_entries.end()) {
                    Dwarf_Line line = table_it->line;
                    // line number
                    if(!table_it->line_number) {
                        Dwarf_Unsigned line_number = 0;
                        VERIFY(wrap(dwarf_lineno, line, &line_number) == DW_DLV_OK);
                        table_it->line_number = static_cast<std::uint32_t>(line_number);
                    }
                    frame.line = table_it->line_number.unwrap();
                    // column number
                    if(!table_it->column_number) {
                        Dwarf_Unsigned column_number = 0;
                        VERIFY(wrap(dwarf_lineoff_b, line, &column_number) == DW_DLV_OK);
                        table_it->column_number = static_cast<std::uint32_t>(column_number);
                    }
                    frame.column = table_it->column_number.unwrap();
                    // filename
                    if(!table_it->path) {
                        char* filename = nullptr;
                        VERIFY(wrap(dwarf_linesrc, line, &filename) == DW_DLV_OK);
                        auto wrapper = raii_wrap(
                            filename,
                            [this] (char* str) { if(str) dwarf_dealloc(dbg, str, DW_DLA_STRING); }
                        );
                        table_it->path = filename;
                    }
                    frame.filename = table_it->path.unwrap();
                }
            } else {
                Dwarf_Line_Context line_context = table_info.line_context;
                // walk for it
                Dwarf_Line* line_buffer = nullptr;
                Dwarf_Signed line_count = 0;
                Dwarf_Line* linebuf_actuals = nullptr;
                Dwarf_Signed linecount_actuals = 0;
                VERIFY(
                    wrap(
                        dwarf_srclines_two_level_from_linecontext,
                        line_context,
                        &line_buffer,
                        &line_count,
                        &linebuf_actuals,
                        &linecount_actuals
                    ) == DW_DLV_OK
                );
                Dwarf_Addr last_lineaddr = 0;
                Dwarf_Line last_line = nullptr;
                for(int i = 0; i < line_count; i++) {
                    Dwarf_Line line = line_buffer[i];
                    Dwarf_Addr lineaddr = 0;
                    VERIFY(wrap(dwarf_lineaddr, line, &lineaddr) == DW_DLV_OK);
                    Dwarf_Line found_line = nullptr;
                    if(pc == lineaddr) {
                        // Multiple PCs may correspond to a line, find the last one
                        found_line = line;
                        for(int j = i + 1; j < line_count; j++) {
                            Dwarf_Line line = line_buffer[j];
                            Dwarf_Addr lineaddr = 0;
                            VERIFY(wrap(dwarf_lineaddr, line, &lineaddr) == DW_DLV_OK);
                            if(pc == lineaddr) {
                                found_line = line;
                            }
                        }
                    } else if(last_line && pc > last_lineaddr && pc < lineaddr) {
                        // Guess that the last line had it
                        found_line = last_line;
                    }
                    if(found_line) {
                        Dwarf_Unsigned line_number = 0;
                        VERIFY(wrap(dwarf_lineno, found_line, &line_number) == DW_DLV_OK);
                        frame.line = static_cast<std::uint32_t>(line_number);
                        char* filename = nullptr;
                        VERIFY(wrap(dwarf_linesrc, found_line, &filename) == DW_DLV_OK);
                        auto wrapper = raii_wrap(
                            filename,
                            [this] (char* str) { if(str) dwarf_dealloc(dbg, str, DW_DLA_STRING); }
                        );
                        frame.filename = filename;
                    } else {
                        Dwarf_Bool is_line_end;
                        VERIFY(wrap(dwarf_lineendsequence, line, &is_line_end) == DW_DLV_OK);
                        if(is_line_end) {
                            last_lineaddr = 0;
                            last_line = nullptr;
                        } else {
                            last_lineaddr = lineaddr;
                            last_line = line;
                        }
                    }
                }
            }
        }

        struct cu_info {
            maybe_owned_die_object cu_die;
            Dwarf_Half dwversion;
        };

        // CU resolution has three paths:
        // - If aranges are present, the pc is looked up in aranges (falls through to next cases if not in aranges)
        // - If cache mode is prioritize memory, the CUs are walked for a match
        // - Otherwise a CU cache is built up and CUs are looked up in the map
        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        optional<cu_info> lookup_cu(Dwarf_Addr pc) {
            // Check for .debug_aranges for fast lookup
            if(aranges && !skeleton) { // don't bother under split dwarf
                // Try to find pc in aranges
                Dwarf_Arange arange;
                if(wrap(dwarf_get_arange, aranges, arange_count, pc, &arange) == DW_DLV_OK) {
                    // Address in table, load CU die
                    Dwarf_Off cu_die_offset;
                    VERIFY(wrap(dwarf_get_cu_die_offset, arange, &cu_die_offset) == DW_DLV_OK);
                    Dwarf_Die raw_die;
                    // Setting is_info = true for now, assuming in .debug_info rather than .debug_types
                    VERIFY(wrap(dwarf_offdie_b, dbg, cu_die_offset, true, &raw_die) == DW_DLV_OK);
                    die_object cu_die(dbg, raw_die);
                    Dwarf_Half offset_size = 0;
                    Dwarf_Half dwversion = 0;
                    VERIFY(dwarf_get_version_of_die(cu_die.get(), &dwversion, &offset_size) == DW_DLV_OK);
                    if(trace_dwarf) {
                        std::fprintf(stderr, "Found CU in aranges\n");
                        cu_die.print();
                    }
                    return cu_info{maybe_owned_die_object::owned(std::move(cu_die)), dwversion};
                }
            }
            // otherwise, or if not in aranges
            // one reason to fallback here is if the compilation has dwarf generated from different compilers and only
            // some of them generate aranges (e.g. static linking with cpptrace after specifying clang++ as the c++
            // compiler while the C compiler defaults to an older gcc)
            if(get_cache_mode() == cache_mode::prioritize_memory) {
                // walk for the cu and go from there
                optional<cu_info> info;
                walk_compilation_units([this, pc, &info] (const die_object& cu_die) {
                    Dwarf_Half offset_size = 0;
                    Dwarf_Half dwversion = 0;
                    dwarf_get_version_of_die(cu_die.get(), &dwversion, &offset_size);
                    //auto p = cu_die.get_pc_range(dwversion);
                    //cu_die.print();
                    //fprintf(stderr, "        %llx, %llx\n", p.first, p.second);
                    if(trace_dwarf) {
                        std::fprintf(stderr, "CU: %d %s\n", dwversion, cu_die.get_name().c_str());
                    }
                    // NOTE: If we have a corresponding skeleton, we assume we have one CU matching the skeleton CU
                    if(
                        (
                            skeleton
                            && skeleton.unwrap().cu_die.pc_in_die(
                                skeleton.unwrap().cu_die,
                                skeleton.unwrap().dwversion,
                                pc
                            )
                        ) || cu_die.pc_in_die(cu_die, dwversion, pc)
                    ) {
                        if(trace_dwarf) {
                            std::fprintf(
                                stderr,
                                "pc in die %08llx %s (now searching for %08llx)\n",
                                to_ull(cu_die.get_global_offset()),
                                cu_die.get_tag_name(),
                                to_ull(pc)
                            );
                        }
                        info = cu_info{maybe_owned_die_object::owned(cu_die.clone()), dwversion};
                        return false;
                    }
                    return true;
                });
                return info;
            } else {
                lazy_generate_cu_cache();
                // look up the cu
                auto vec_it = first_less_than_or_equal(
                    cu_cache.begin(),
                    cu_cache.end(),
                    pc,
                    [] (Dwarf_Addr pc, const cu_entry& entry) {
                        return pc < entry.low;
                    }
                );
                // TODO: Vec-it is already range-based, this range check is redundant
                // If the vector has been empty this can happen
                if(vec_it != cu_cache.end()) {
                    // TODO: Cache the range list?
                    // NOTE: If we have a corresponding skeleton, we assume we have one CU matching the skeleton CU
                    if(
                        (
                            skeleton
                            && skeleton.unwrap().cu_die.pc_in_die(
                                skeleton.unwrap().cu_die,
                                skeleton.unwrap().dwversion,
                                pc
                            )
                        ) || vec_it->die.pc_in_die(vec_it->die, vec_it->dwversion, pc)
                    ) {
                        return cu_info{maybe_owned_die_object::ref(vec_it->die), vec_it->dwversion};
                    }
                } else {
                    // I've had this happen for _start, where there is a cached CU for the object but _start is outside
                    // of the CU's PC range
                    // ASSERT(cu_cache.size() == 0, "Vec should be empty?");
                }
                return nullopt;
            }
        }

        optional<std::string> get_dwo_name(const die_object& cu_die) {
            if(auto dwo_name = cu_die.get_string_attribute(DW_AT_GNU_dwo_name)) {
                return dwo_name;
            } else if(auto dwo_name = cu_die.get_string_attribute(DW_AT_dwo_name)) {
                return dwo_name;
            } else {
                return nullopt;
            }
        }

        void perform_dwarf_fission_resolution(
            const die_object& cu_die,
            const optional<std::string>& dwo_name,
            const object_frame& object_frame_info,
            stacktrace_frame& frame,
            std::vector<stacktrace_frame>& inlines
        ) {
            // Split dwarf / debug fission / dwo is handled here
            // Location of the split full CU is a combination of DW_AT_dwo_name/DW_AT_GNU_dwo_name and DW_AT_comp_dir
            // https://gcc.gnu.org/wiki/DebugFission
            if(dwo_name) {
                // TODO: DWO ID?
                auto comp_dir = cu_die.get_string_attribute(DW_AT_comp_dir);
                Dwarf_Half offset_size = 0;
                Dwarf_Half dwversion = 0;
                dwarf_get_version_of_die(cu_die.get(), &dwversion, &offset_size);
                std::string path;
                if(is_absolute(dwo_name.unwrap())) {
                    path = dwo_name.unwrap();
                } else if(comp_dir) {
                    path = comp_dir.unwrap() + PATH_SEP + dwo_name.unwrap();
                } else {
                    // maybe default to dwo_name but for now not doing anything
                    return;
                }
                // todo: slight inefficiency in this copy-back strategy due to other frame members
                frame_with_inlines res;
                if(get_cache_mode() == cache_mode::prioritize_memory) {
                    dwarf_resolver resolver(
                        path,
                        skeleton_info{cu_die.clone(), dwversion, *this}
                    );
                    res = resolver.resolve_frame(object_frame_info);
                } else {
                    auto off = cu_die.get_global_offset();
                    auto it = split_full_cu_resolvers.find(off);
                    if(it == split_full_cu_resolvers.end()) {
                        it = split_full_cu_resolvers.emplace(
                            off,
                            std::unique_ptr<dwarf_resolver>(
                                new dwarf_resolver(
                                    path,
                                    skeleton_info{cu_die.clone(), dwversion, *this}
                                )
                            )
                        ).first;
                    }
                    res = it->second->resolve_frame(object_frame_info);
                }
                frame = std::move(res.frame);
                inlines = std::move(res.inlines);
            }
        }

        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        void resolve_frame_core(
            const object_frame& object_frame_info,
            stacktrace_frame& frame,
            std::vector<stacktrace_frame>& inlines
        ) {
            auto pc = object_frame_info.object_address;
            if(dump_dwarf) {
                std::fprintf(stderr, "%s\n", object_path.c_str());
                std::fprintf(stderr, "%llx\n", to_ull(pc));
            }
            optional<cu_info> cu = lookup_cu(pc);
            if(cu) {
                const auto& cu_die = cu.unwrap().cu_die.get();
                // gnu non-standard debug-fission may create non-skeleton CU DIEs and just add dwo attributes
                // clang emits dwo names in the split CUs, so guard against going down the dwarf fission path (which
                // doesn't infinitely recurse because it's not emitted as an absolute path and there's no comp dir but
                // it's good to guard against the infinite recursion anyway)
                auto dwo_name = get_dwo_name(cu_die);
                if(cu_die.get_tag() == DW_TAG_skeleton_unit || (dwo_name && !skeleton)) {
                    perform_dwarf_fission_resolution(cu_die, dwo_name, object_frame_info, frame, inlines);
                } else {
                    retrieve_line_info(cu_die, pc, frame);
                    retrieve_symbol(cu_die, pc, cu.unwrap().dwversion, frame, inlines);
                }
            }
        }

    public:
        CPPTRACE_FORCE_NO_INLINE_FOR_PROFILING
        frame_with_inlines resolve_frame(const object_frame& frame_info) override {
            if(!ok) {
                return {
                    {
                        frame_info.raw_address,
                        frame_info.object_address,
                        nullable<std::uint32_t>::null(),
                        nullable<std::uint32_t>::null(),
                        frame_info.object_path,
                        "",
                        false
                    },
                    {}
                };
            }
            stacktrace_frame frame = null_frame;
            frame.filename = frame_info.object_path;
            frame.raw_address = frame_info.raw_address;
            frame.object_address = frame_info.object_address;
            if(trace_dwarf) {
                std::fprintf(
                    stderr,
                    "Starting resolution for %s %08llx\n",
                    object_path.c_str(),
                    to_ull(frame_info.object_address)
                );
            }
            std::vector<stacktrace_frame> inlines;
            resolve_frame_core(
                frame_info,
                frame,
                inlines
            );
            return {std::move(frame), std::move(inlines)};
        }
    };

    std::unique_ptr<symbol_resolver> make_dwarf_resolver(const std::string& object_path) {
        return std::unique_ptr<dwarf_resolver>(new dwarf_resolver(object_path));
    }
}
}
}

#endif
