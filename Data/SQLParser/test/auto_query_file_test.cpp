#include <stdio.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "SQLParser.h"
#include "thirdparty/microtest/microtest.h"

// Read all lines from the given file path. Skips comment lines.
std::vector<std::string> readlines(std::string path);

// Read the queries from all files that were supplied to the test
// through the -f argument. For all queries it is checked whether they
// can be parsed successfully.
TEST(AutoQueryFileTest) {
  const std::vector<std::string>& args = mt::Runtime::args();

  std::vector<std::string> query_files;

  // Parse command line arguments to retrieve query files.
  uint i = 1;
  for (; i < args.size(); ++i) {
    if (args[i] == "-f") {
      query_files.push_back(args[++i]);
    }
  }

  // Read list of queries from all input files.
  std::vector<std::string> lines;
  for (std::string path : query_files) {
    std::vector<std::string> tmp = readlines(path);
    lines.insert(lines.end(), tmp.begin(), tmp.end());
  }

  // Execute queries.
  size_t num_executed = 0;
  size_t num_failed = 0;
  for (std::string line : lines) {
    bool expected_result = true;
    std::string query = line;

    // If a line starts with '!' parsing is expected to fail.
    if (query.at(0) == '!') {
      expected_result = false;
      query = query.substr(1);
    }

    // Measuring the parsing time.
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // Parse the query.
    hsql::SQLParserResult result;
    hsql::SQLParser::parse(query, &result);

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    double us = elapsed_seconds.count() * 1000 * 1000;

    if (expected_result == result.isValid()) {
      printf("\033[0;32m{      ok} (%.1fus)\033[0m %s\n", us, line.c_str());
    } else {
      printf("\033[0;31m{  failed}\033[0m\n");
      printf("\t\033[0;31m%s (L%d:%d)\n\033[0m", result.errorMsg(), result.errorLine(), result.errorColumn());
      printf("\t%s\n", line.c_str());
      ++num_failed;
    }
    ++num_executed;
  }

  if (num_failed == 0) {
    printf("\033[0;32m{      ok} \033[0mAll %lu grammar tests completed successfully!\n", num_executed);
  } else {
    fprintf(stderr, "\033[0;31m{  failed} \033[0mSome grammar tests failed! %lu out of %lu tests failed!\n", num_failed,
            num_executed);
  }
  ASSERT_EQ(num_failed, 0);
}

std::vector<std::string> readlines(std::string path) {
  std::ifstream infile(path);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(infile, line)) {
    std::istringstream iss(line);

    // Skip comments.
    if (line[0] == '#' || (line[0] == '-' && line[1] == '-')) {
      continue;
    }

    lines.push_back(line);
  }
  return lines;
}
