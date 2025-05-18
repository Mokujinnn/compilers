#pragma once

#include "cool-tree.h"

class GetNameVisitor : public Visitor
{
public:
    std::string name;

    static std::string get(tree_node *node);

public:
    void visit(class__class &node) override
    {
        name = std::string(node.name->get_string());
    }

    void visit(method_class &node) override
    {
        name = std::string(node.name->get_string());
    }

    void visit(attr_class &node) override
    {
        name = std::string(node.name->get_string());
    }

    void visit(formal_class &node) override
    {
        name = std::string(node.name->get_string());
    }

    void visit(let_class &node) override
    {
        name = std::string(node.identifier->get_string());
    }

    void visit(dispatch_class &node) override
    {
        name = std::string(node.name->get_string());
    }

    void visit(object_class &node) override
    {
        name = std::string(node.name->get_string());
    }

    void visit(bool_const_class &node) override
    {
        name = std::to_string(node.val);
    }

    void visit(string_const_class &node) override
    {
        name = std::string(node.token->get_string());
    }
};