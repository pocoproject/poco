#include "snippets/snippet.hpp"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <iostream>

#include "utils/common.hpp"
#include "utils/utils.hpp"

namespace cpptrace {
namespace detail {
    constexpr std::int64_t max_size = 1024 * 1024 * 10; // 10 MiB

    struct line_range {
        std::size_t begin;
        std::size_t end; // one past the end
    };

    class snippet_manager {
        bool loaded_contents;
        std::string contents;
        // 1-based indexing
        std::vector<line_range> line_table;
    public:
        snippet_manager(const std::string& path) : loaded_contents(false) {
            std::ifstream file;
            try {
                file.open(path, std::ios::ate);
                if(file.is_open()) {
                    std::ifstream::pos_type size = file.tellg();
                    if(size == std::ifstream::pos_type(-1) || size > max_size) {
                        return;
                    }
                    // else load file
                    file.seekg(0, std::ios::beg);
                    contents.resize(to<std::size_t>(size));
                    if(!file.read(&contents[0], size)) {
                        // error ...
                    }
                    build_line_table();
                    loaded_contents = true;
                }
            } catch(const std::ifstream::failure&) {
                // ...
            }
        }

        // takes a 1-index line
        std::string get_line(std::size_t line) const {
            if(!loaded_contents || line > line_table.size()) {
                return "";
            } else {
                return contents.substr(line_table[line].begin, line_table[line].end - line_table[line].begin);
            }
        }

        std::size_t num_lines() const {
            return line_table.size();
        }

        bool ok() const {
            return loaded_contents;
        }
    private:
        void build_line_table() {
            line_table.push_back({0, 0});
            std::size_t pos = 0; // stores the start of the current line
            while(true) {
                // find the end of the current line
                std::size_t terminator_pos = contents.find('\n', pos);
                if(terminator_pos == std::string::npos) {
                    line_table.push_back({pos, contents.size()});
                    break;
                } else {
                    std::size_t end_pos = terminator_pos; // one past the end of the current line
                    if(end_pos > 0 && contents[end_pos - 1] == '\r') {
                        end_pos--;
                    }
                    line_table.push_back({pos, end_pos});
                    pos = terminator_pos + 1;
                }
            }
        }
    };

    std::mutex snippet_manager_mutex;
    std::unordered_map<std::string, const snippet_manager> snippet_managers;

    const snippet_manager& get_manager(const std::string& path) {
        std::unique_lock<std::mutex> lock(snippet_manager_mutex);
        auto it = snippet_managers.find(path);
        if(it == snippet_managers.end()) {
            return snippet_managers.insert({path, snippet_manager(path)}).first->second;
        } else {
            return it->second;
        }
    }

    // how wide the margin for the line number should be
    constexpr std::size_t margin_width = 8;

    // 1-indexed line
    std::string get_snippet(const std::string& path, std::size_t target_line, std::size_t context_size, bool color) {
        const auto& manager = get_manager(path);
        if(!manager.ok()) {
            return "";
        }
        auto begin = target_line <= context_size + 1 ? 1 : target_line - context_size;
        auto original_begin = begin;
        auto end = std::min(target_line + context_size, manager.num_lines() - 1);
        std::vector<std::string> lines;
        for(auto line = begin; line <= end; line++) {
            lines.push_back(manager.get_line(line));
        }
        // trim blank lines
        while(begin < target_line && lines[begin - original_begin].empty()) {
            begin++;
        }
        while(end > target_line && lines[end - original_begin].empty()) {
            end--;
        }
        // make the snippet
        std::string snippet;
        for(auto line = begin; line <= end; line++) {
            if(color && line == target_line) {
                snippet += YELLOW;
            }
            auto line_str = std::to_string(line);
            snippet += microfmt::format("{>{}}: ", margin_width, line_str);
            if(color && line == target_line) {
                snippet += RESET;
            }
            snippet += lines[line - original_begin] + "\n";
        }
        return snippet;
    }
}
}
