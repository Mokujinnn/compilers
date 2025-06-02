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

std::string GetExpressionTypeVisitor::get(tree_node *node)
{
    GetExpressionTypeVisitor visitor;
    node->accept(visitor);
    return visitor.type;
}

Expressions GetExpressionsVisitor::get(tree_node *node)
{
    GetExpressionsVisitor visitor;
    node->accept(visitor);
    return visitor.exprs;
}

Expression GetExpressionVisitor::get(tree_node *node)
{
    GetExpressionVisitor visitor;
    node->accept(visitor);
    return visitor.expr;
}