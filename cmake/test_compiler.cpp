#include <iostream>
#include <string>
#include <optional>

using namespace std::literals;

int main()
{
  std::optional<std::string> option;
  std::string str = "Try to compile"s;
  std::cout << str << " "s << option.has_value() << '\n';
  return 0;
}
