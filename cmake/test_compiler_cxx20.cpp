#include <atomic>
#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <version>

#if defined(_MSVC_LANG)
  #define POCO_CHECK_CPLUSPLUS _MSVC_LANG
#else
  #define POCO_CHECK_CPLUSPLUS __cplusplus
#endif
#if POCO_CHECK_CPLUSPLUS < 202002L
  #error "The compiler does not compile this file as C++20 or later."
#endif

using namespace std::literals;

// C++20 language and library features that POCO uses without a feature guard.
struct Guard
{
  [[nodiscard]] explicit Guard(int value): _value(value) {}
  int _value;
};

int main(int argc, char**)
{
  std::optional<std::string> option;
  std::string str = "Try to compile C++20"s;
  std::u8string u8str(str.begin(), str.end());
  std::filesystem::path path(u8str);
  std::atomic_flag flag;
  Guard guard(argc);
  if (flag.test()) [[unlikely]]
    return 1;
  std::cout << str << " "s << option.has_value() << path.empty() << guard._value << '\n';
  return 0;
}
