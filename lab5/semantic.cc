#include "semantic.h"
#include "cool-tree.h"
#include "visitors.h"

#include <unordered_map>
#include <unordered_set>

using StringUSet = std::unordered_set<std::string>;
using StringUMap = std::unordered_map<std::string, std::string>;

Semantic::Semantic(Classes parse_result)
    : classes_(parse_result)
{
}

void Semantic::error(std::string_view msg, int line)
{
    std::cerr << "line: " << line << " semantic error: " << msg << '\n';
    err_count_++;
}

void Semantic::checkClassDuplicates(class__class *cls)
{
    auto class_name = GetNameVisitor::get(cls);
    auto result = classes_names_.insert(class_name);

    if (!result.second)
    {
        error("class '" + class_name + "' already exist", cls->get_line_number());
    }
}

void Semantic::analysis()
{
    for (int i = classes_->first(); classes_->more(i); i = classes_->next(i))
    {
        class__class *current_class = dynamic_cast<class__class *>(classes_->nth(i));
        checkClassDuplicates(current_class);
    }
}
