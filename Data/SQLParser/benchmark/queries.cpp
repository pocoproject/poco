#include "queries.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <regex>

#include "benchmark_utils.h"

namespace filesystem = std::filesystem;

std::string getQueryName(unsigned i) {
  if (sql_queries[i].first.empty()) {
    std::string name = "#" + std::to_string(i + 1);
    return name;
  }
  return std::string("") + sql_queries[i].first;
}

std::vector<SQLQuery> getQueriesFromDirectory(const std::string& dir_path) {
  std::regex query_file_regex("\\.sql$");
  std::vector<std::string> files;

  for (auto& entry : filesystem::directory_iterator(dir_path)) {
    if (filesystem::is_regular_file(entry)) {
      std::string path_str = filesystem::path(entry);

      if (std::regex_search(path_str, query_file_regex)) {
        files.push_back(path_str);
      }
    }
  }

  std::sort(files.begin(), files.end());

  std::vector<SQLQuery> queries;
  for (const std::string& file_path : files) {
    const filesystem::path p(file_path);
    const std::string query = readFileContents(file_path);
    queries.emplace_back(p.filename(), query);
  }
  return queries;
}

std::vector<SQLQuery> getTPCHQueries() {
  return getQueriesFromDirectory("test/queries/");
}
