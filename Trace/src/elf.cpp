#include "binary/elf.hpp"

#if IS_LINUX

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

#include <elf.h>

namespace cpptrace {
namespace detail {
    template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    T elf_byteswap_if_needed(T value, bool elf_is_little) {
        if(is_little_endian() == elf_is_little) {
            return value;
        } else {
            return byteswap(value);
        }
    }

    template<std::size_t Bits>
    static Result<std::uintptr_t, internal_error> elf_get_module_image_base_from_program_table(
        const std::string& object_path,
        std::FILE* file,
        bool is_little_endian
    ) {
        static_assert(Bits == 32 || Bits == 64, "Unexpected Bits argument");
        using Header = typename std::conditional<Bits == 32, Elf32_Ehdr, Elf64_Ehdr>::type;
        using PHeader = typename std::conditional<Bits == 32, Elf32_Phdr, Elf64_Phdr>::type;
        auto loaded_header = load_bytes<Header>(file, 0);
        if(loaded_header.is_error()) {
            return std::move(loaded_header).unwrap_error();
        }
        const Header& file_header = loaded_header.unwrap_value();
        if(file_header.e_ehsize != sizeof(Header)) {
            return internal_error("ELF file header size mismatch" + object_path);
        }
        // PT_PHDR will occur at most once
        // Should be somewhat reliable https://stackoverflow.com/q/61568612/15675011
        // It should occur at the beginning but may as well loop just in case
        for(int i = 0; i < file_header.e_phnum; i++) {
            auto loaded_ph = load_bytes<PHeader>(file, file_header.e_phoff + file_header.e_phentsize * i);
            if(loaded_ph.is_error()) {
                return std::move(loaded_ph).unwrap_error();
            }
            const PHeader& program_header = loaded_ph.unwrap_value();
            if(elf_byteswap_if_needed(program_header.p_type, is_little_endian) == PT_PHDR) {
                return elf_byteswap_if_needed(program_header.p_vaddr, is_little_endian) -
                       elf_byteswap_if_needed(program_header.p_offset, is_little_endian);
            }
        }
        // Apparently some objects like shared objects can end up missing this header. 0 as a base seems correct.
        return 0;
    }

    Result<std::uintptr_t, internal_error> elf_get_module_image_base(const std::string& object_path) {
        auto file = raii_wrap(std::fopen(object_path.c_str(), "rb"), file_deleter);
        if(file == nullptr) {
            return internal_error("Unable to read object file " + object_path);
        }
        // Initial checks/metadata
        auto magic = load_bytes<std::array<char, 4>>(file, 0);
        if(magic.is_error()) {
            return std::move(magic).unwrap_error();
        }
        if(magic.unwrap_value() != (std::array<char, 4>{0x7F, 'E', 'L', 'F'})) {
            return internal_error("File is not ELF " + object_path);
        }
        auto ei_class = load_bytes<std::uint8_t>(file, 4);
        if(ei_class.is_error()) {
            return std::move(ei_class).unwrap_error();
        }
        bool is_64 = ei_class.unwrap_value() == 2;
        auto ei_data = load_bytes<std::uint8_t>(file, 5);
        if(ei_data.is_error()) {
            return std::move(ei_data).unwrap_error();
        }
        bool is_little_endian = ei_data.unwrap_value() == 1;
        auto ei_version = load_bytes<std::uint8_t>(file, 6);
        if(ei_version.is_error()) {
            return std::move(ei_version).unwrap_error();
        }
        if(ei_version.unwrap_value() != 1) {
            return internal_error("Unexpected ELF version " + object_path);
        }
        // get image base
        if(is_64) {
            return elf_get_module_image_base_from_program_table<64>(object_path, file, is_little_endian);
        } else {
            return elf_get_module_image_base_from_program_table<32>(object_path, file, is_little_endian);
        }
    }

}
}

#endif
