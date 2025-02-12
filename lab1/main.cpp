#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

namespace fs = std::filesystem;

bool is_cpp_file(const fs::directory_entry &entry);
std::vector<std::pair<std::string, std::regex>> get_expressions();

void process_file(
    const fs::directory_entry &entry,
    const std::vector<std::pair<std::string, std::regex>> &regexprs);

int main(int argc, char const *argv[]) {
  fs::path dir = "folly";

  auto regexprs = get_expressions();

  try {
    for (auto &&entry : fs::recursive_directory_iterator(dir)) {
      if (is_cpp_file(entry)) {
        process_file(entry, regexprs);
      }
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}

bool is_cpp_file(const fs::directory_entry &entry) {
  return entry.is_regular_file() && entry.path().extension() == ".cpp";
}

std::vector<std::pair<std::string, std::regex>> get_expressions() {
  return {
      {"Keyword",
       std::regex(
           R"(\b(alignas|alignof|and|and_eq|asm|auto|bitand|bitor|bool|break|case|"
        "catch|char|char8_t|char16_t|char32_t|class|compl|concept|const|"
        "consteval|constexpr|constinit|const_cast|continue|co_await|co_return|co_"
        "yield|default|delete|do|double|dynamic_cast|else|enum|explicit|export|"
        "extern|false|float|for|friend|goto|if|inline|int|long|mutable|namespace|"
        "new|noexcept|not|not_eq|nullptr|operator|or|or_eq|private|protected|"
        "public|register|reinterpret_cast|requires|return|short|signed|sizeof|"
        "static|static_assert|static_cast|struct|switch|template|this|thread_"
        "local|throw|true|try|typedef|typeid|typename|union|unsigned|using|"
        "virtual|void|volatile|wchar_t|while|xor|xor_eq)\b)")},
      {"Identyfiers", std::regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)")},
      {"Integral literals",
       std::regex(
           R"(\b(0[bB][01']+|[0-9']+|0[0-7']+|0[xX][0-9a-fA-F']+)([uU]?[lL]{0,2}|[lL]{0,2}[uU]?)\b)")},
           {"Directives", std::regex(R"(^\s*#\s*.*\s\b.*$)")},
           {"String literals", std::regex(R"((L|LR|u8|u|U)?(\".*\"))")}};
}

template <typename Iter>
void print_lexemes(std::ostream &os, Iter beg, Iter end) {
  for (auto i = beg; i != end; ++i) {
    os << '{' << (*i).str() << "}, ";
  }
  os << '\n';
}

void process_file(
    const fs::directory_entry &entry,
    const std::vector<std::pair<std::string, std::regex>> &regexprs) {

  std::ifstream file(entry.path());
  std::string line;

  while (std::getline(file, line)) {
    std::cout << "In line{ " << line << " }\n";

    for (auto &&[lexeme_type, expr] : regexprs) {
      auto beg = std::sregex_iterator(line.begin(), line.end(), expr);
      auto end = std::sregex_iterator();
      auto count = std::distance(beg, end);

      if (count > 0) {
        std::cout << count << ' ' << lexeme_type << ": ";
        print_lexemes(std::cout, beg, end);
      }
    }

    std::cout << '\n';
  }
}