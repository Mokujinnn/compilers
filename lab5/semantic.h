#pragma once

#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "cool-tree.h"
#include "cool-tree.handcode.h"
#include "tree.h"

struct SemanticContext
{
    int err_count{0};

    const std::unordered_set<std::string> dont_inherit{"Bool", "Int", "String", "SELF_TYPE"};
    std::unordered_set<std::string> classes_names{"Bool", "Int", "String", "SELF_TYPE", "Object"};
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> methods_;
    std::unordered_map<std::string, std::string> classes_hierarchy_;

    void error(std::string_view msg);
};

class Semantic
{
private:
    program_class *ast_;
    SemanticContext context_;

public:
    Semantic(program_class *ast);
    ~Semantic() = default;

    void analysis();
};
