#include "semantic.h"
#include "cool-tree.h"
#include "visitors.h"

#include <unordered_map>
#include <unordered_set>

using StringUSet = std::unordered_set<std::string>;
using StringUMap = std::unordered_map<std::string, std::string>;

Semantic::Semantic(program_class *ast)
    : ast_(ast)
{
}

void SemanticContext::error(std::string_view msg)
{
    std::cerr << "Semantic error: " << msg << '\n';
    err_count++;
}

void Semantic::checkClassDuplicates()
{
    // for (int i = classes_->first(); classes_->more(i); i = classes_->next(i))
    // {
    //     class__class *current_class = dynamic_cast<class__class *>(classes_->nth(i));
    //     auto class_name = GetNameVisitor::get(current_class);
    //     auto result = context_.classes_names.insert(class_name);

    //     if (!result.second)
    //     {
    //         error("class '" + class_name + "' already exist");
    //     }
    // }
}

void Semantic::analysis()
{
    CheckClassDuplicatesVisior check_duplicates(context_);

    check_duplicates.visit(*ast_);
}
