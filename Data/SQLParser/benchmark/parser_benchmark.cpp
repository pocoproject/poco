
#include <chrono>
#include <sstream>
#include "benchmark/benchmark.h"

#include "SQLParser.h"
#include "parser/bison_parser.h"
#include "parser/flex_lexer.h"

#include "benchmark_utils.h"

// Benchmark the influence of increasing size of the query, while
// the number of tokens remains unchanged.
static void BM_CharacterCount(benchmark::State& st) {
  const size_t querySize = st.range(0);

  // Base query has size of 18 characters.
  std::string query = "SELECT %name% FROM test;";

  const uint pad = querySize - 18;
  const std::string filler = std::string(pad, 'a');
  query.replace(7, 6, filler);

  st.counters["num_tokens"] = getNumTokens(query);
  st.counters["num_chars"] = query.size();
  while (st.KeepRunning()) {
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);
  }
}
BENCHMARK(BM_CharacterCount)
  ->RangeMultiplier(1 << 2)
  ->Ranges({{1 << 5, 1 << 15},
            {5, 5}});

// Benchmark the influence of increasing number of tokens, while
// the number of characters remains unchanged.
static void BM_ConditionalTokens(benchmark::State& st) {
  const size_t targetSize = st.range(0);
  const size_t numTokens = st.range(1);

  // Base query contains 6 tokens.
  std::string query = "SELECT * FROM test";

  // Create conditional.
  std::stringstream condStream;
  size_t missingTokens = numTokens - 4;
  if (missingTokens > 0) {
    condStream << " WHERE a";
    missingTokens -= 2;

    while (missingTokens > 0) {
      condStream << " AND a";
      missingTokens -= 2;
    }
  }

  query += condStream.str();

  if (targetSize >= query.size()) {
    const size_t pad = targetSize - query.size();
    const std::string filler = std::string(pad, 'a');
    query.replace(7, 1, filler);

  } else {
    // Query can't be the same length as in the other benchmarks.
    // Running this will result in unusable data.
    fprintf(stderr, "Too many tokens. Query too long for benchmark char limit (%lu > %lu).\n",
      query.size(), targetSize);
    return;
  }

  st.counters["num_tokens"] = getNumTokens(query);
  st.counters["num_chars"] = query.size();
  while (st.KeepRunning()) {
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);
    if (!result.isValid()) st.SkipWithError("Parsing failed!");
  }
}
BENCHMARK(BM_ConditionalTokens)
  ->RangeMultiplier(1 << 2)
  ->Ranges({{1 << 14, 1 << 14},
            {1 << 2, 1 << 11}});



