#pragma once

#include "cool-tree.h"
#include "semantic.h"

#include <string>

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

class GetParentVisitor : public Visitor
{
public:
    std::string name;

    static std::string get(tree_node *node);

public:
    void visit(class__class &node) override
    {
        name = std::string(node.parent->get_string());
    }
};

class SemanticVisior : public Visitor
{
protected:
    SemanticContext &context_;

public:
    SemanticVisior(SemanticContext &context)
        : context_(context)
    {
    }
};

class CheckClassDuplicatesVisior : public SemanticVisior
{
private:
    using Base = SemanticVisior;

public:
    using Base::Base;

    void visit(program_class &node) override
    {
        for (int i = node.classes->first(); node.classes->more(i); i = node.classes->next(i))
        {
            class__class *current_class = dynamic_cast<class__class *>(node.classes->nth(i));
            current_class->accept(*this);
        }
    }

    void visit(class__class &node) override
    {
        auto class_name = GetNameVisitor::get(&node);
        auto result = context_.classes_names.insert(class_name);

        if (!result.second)
        {
            context_.error("class '" + class_name + "' already exist");
        }
    }
};