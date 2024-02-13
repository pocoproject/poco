#include "benchmark/benchmark.h"

#include "benchmark_utils.h"
#include "queries.h"

int main(int argc, char** argv) {
  // Create parse and tokenize benchmarks for TPC-H queries.
  const auto tpch_queries = getTPCHQueries();
  for (const auto& query : tpch_queries) {
    std::string p_name = query.first + "-parse";
    benchmark::RegisterBenchmark(p_name.c_str(), &BM_ParseBenchmark, query.second);
    std::string t_name = query.first + "-tokenize";
    benchmark::RegisterBenchmark(t_name.c_str(), &BM_TokenizeBenchmark, query.second);
  }

  // Create parse and tokenize benchmarks for all queries in sql_queries array.
  for (unsigned i = 0; i < sql_queries.size(); ++i) {
    const auto& query = sql_queries[i];
    std::string p_name = getQueryName(i) + "-parse";
    benchmark::RegisterBenchmark(p_name.c_str(), &BM_ParseBenchmark, query.second);

    std::string t_name = getQueryName(i) + "-tokenize";
    benchmark::RegisterBenchmark(t_name.c_str(), &BM_TokenizeBenchmark, query.second);
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
}
