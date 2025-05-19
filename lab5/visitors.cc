#include "visitors.h"

#include <string>

std::string GetNameVisitor::get(tree_node *node)
{
    GetNameVisitor visitor;
    node->accept(visitor);
    return visitor.name;
}

std::string GetParentVisitor::get(tree_node *node)
{
    GetParentVisitor visitor;
    node->accept(visitor);
    return visitor.name;
}