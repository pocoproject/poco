#include "benchmark_utils.h"

#include <fstream>
#include <iostream>

#include "SQLParser.h"

size_t getNumTokens(const std::string& query) {
  std::vector<int16_t> tokens;
  hsql::SQLParser::tokenize(query, &tokens);
  return tokens.size();
}

void BM_TokenizeBenchmark(benchmark::State& st, const std::string& query) {
  st.counters["num_tokens"] = getNumTokens(query);
  st.counters["num_chars"] = query.size();

  while (st.KeepRunning()) {
    std::vector<int16_t> tokens(512);
    hsql::SQLParser::tokenize(query, &tokens);
  }
}

void BM_ParseBenchmark(benchmark::State& st, const std::string& query) {
  st.counters["num_tokens"] = getNumTokens(query);
  st.counters["num_chars"] = query.size();

  while (st.KeepRunning()) {
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);
    if (!result.isValid()) {
      std::cout << query << std::endl;
      std::cout << result.errorMsg() << std::endl;
      st.SkipWithError("Parsing failed!");
    }
  }
}

std::string readFileContents(const std::string& file_path) {
  std::ifstream t(file_path.c_str());
  std::string text((std::istreambuf_iterator<char>(t)),
                   std::istreambuf_iterator<char>());
  return text;
}
