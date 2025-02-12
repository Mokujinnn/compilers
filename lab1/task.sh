# keywords
grep -E "\b(alignas|alignof|and|and_eq|asm|auto|bitand|bitor|bool|break|case|catch|char|char8_t|char16_t|char32_t|class|compl|concept|const|consteval|constexpr|constinit|const_cast|continue|co_await|co_return|co_yield|default|delete|do|double|dynamic_cast|else|enum|explicit|export|extern|false|float|for|friend|goto|if|inline|int|long|mutable|namespace|new|noexcept|not|not_eq|nullptr|operator|or|or_eq|private|protected|public|register|reinterpret_cast|requires|return|short|signed|sizeof|static|static_assert|static_cast|struct|switch|template|this|thread_local|throw|true|try|typedef|typeid|typename|union|unsigned|using|virtual|void|volatile|wchar_t|while|xor|xor_eq)\b" folly/**/*.cpp

# identifers
grep -E "\b[a-zA-Z_][a-zA-Z0-9_]*\b" folly/**/*.cpp

# integral literals
grep -E "\b(0[bB][01']+|[1-9][0-9']+|0[0-7']+|0[xX][0-9a-fA-F']+)([uU]?[lL]{0,2}|[lL]{0,2}[uU]?)\b" folly/**/*.cpp

# directives
grep -E "^\s*#\s*.*\s\b.*$" folly/**/*.cpp

# strings
grep -E "\b(L|LR|u8|u|U)?(\".*\")"