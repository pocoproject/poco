#ifndef __BENCHMARK_UTILS_H__
#define __BENCHMARK_UTILS_H__

#include "benchmark/benchmark.h"

size_t getNumTokens(const std::string& query);

void BM_TokenizeBenchmark(benchmark::State& st, const std::string& query);

void BM_ParseBenchmark(benchmark::State& st, const std::string& query);

std::string readFileContents(const std::string& file_path);




#define TIME_DIFF(end, start)\
  std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

#define NOW()\
  std::chrono::high_resolution_clock::now();

#define PARSE_QUERY_BENCHMARK(name, query)\
  static void name(benchmark::State& st) {\
    BM_ParseBenchmark(st, query);\
  }\
  BENCHMARK(name);

#define TOKENIZE_QUERY_BENCHMARK(name, query)\
  static void name(benchmark::State& st) {\
    BM_TokenizeBenchmark(st, query);\
  }\
  BENCHMARK(name);


#define BENCHMARK_QUERY(test_name, query)\
  TOKENIZE_QUERY_BENCHMARK(test_name##Tokenize, query)\
  PARSE_QUERY_BENCHMARK(test_name##Parse, query)


#endif