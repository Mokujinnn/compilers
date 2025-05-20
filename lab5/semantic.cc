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

void Semantic::analysis()
{
    CheckClassDuplicatesVisior check_duplicates(context_);
    CheckClassMainVisitor check_main(context_);
    CheckUniqueFeaturesVisitor check_features(context_);
    CheckUniqueFormalsVisitor check_formals(context_);

    check_duplicates.visit(*ast_);
    check_main.visit(*ast_);
    check_features.visit(*ast_);
    check_formals.visit(*ast_);
}
