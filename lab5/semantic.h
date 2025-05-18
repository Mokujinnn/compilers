#pragma once

#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "cool-tree.handcode.h"
#include "tree.h"

class Semantic
{
    using StringUSet = std::unordered_set<std::string>;
    using StringUMap = std::unordered_map<std::string, std::string>;

private:
    Classes classes_;

    int err_count_{0};

    const StringUSet dont_inherit_{"Bool", "Int", "String", "SELF_TYPE"};
    StringUSet classes_names_{"Bool", "Int", "String", "SELF_TYPE", "Object"};

private:
    void error(std::string_view msg, int line);

    void checkClassDuplicates(class__class *cls);

public:
    Semantic(Classes parse_result);
    ~Semantic() = default;

    void analysis();
};
