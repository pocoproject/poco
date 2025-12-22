#include <iostream>
#include <string>
#include <optional>
#include <version>

using namespace std::literals;

int main()
{
  std::optional<std::string> option;
  std::string str = "Try to compile C++20"s;
  std::cout << str << " "s << option.has_value() << '\n';
  return 0;
}
