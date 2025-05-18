#include "visitors.h"

std::string GetNameVisitor::get(tree_node *node)
{
    GetNameVisitor visitor;
    node->accept(visitor);
    return visitor.name;
}