#pragma once

#include "cool-tree.h"
#include "semantic.h"

#include <functional>
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

class CheckClassMainVisitor : public SemanticVisior
{
private:
    using Base = SemanticVisior;

    bool has_class_main_{false};
    bool has_method_main_{false};

public:
    using Base::Base;

    void visit(program_class &node) override
    {
        for (int i = node.classes->first(); node.classes->more(i); i = node.classes->next(i))
        {
            if (has_class_main_)
            {
                break;
            }

            class__class *current_class = dynamic_cast<class__class *>(node.classes->nth(i));
            current_class->accept(*this);
        }

        if (!has_class_main_)
        {
            if (!has_method_main_)
            {
                context_.error("No class 'Main' with method 'main'");
            }
            else
            {
                context_.error("No class 'Main'");
            }

            has_class_main_ = false;
            has_method_main_ = false;
        }
    }

    void visit(class__class &node) override
    {
        auto class_name = GetNameVisitor::get(&node);

        if (class_name == "Main")
        {
            has_class_main_ = true;
            auto *features = node.features;

            for (int i = features->first(); features->more(i); i = features->next(i))
            {
                auto *feature = features->nth(i);
                if ((feature = dynamic_cast<method_class *>(feature)))
                {
                    feature->accept(*this);
                }
            }
        }
    }

    void visit(method_class &node) override
    {
        auto method_name = GetNameVisitor::get(&node);

        if (method_name == "main")
        {
            has_method_main_ = true;
            auto *formals = node.formals;

            if (formals->len() != 0)
            {
                context_.error("Method 'main' must have no parameters");
            }
        }
    }
};

class CheckUniqueFeaturesVisitor : public SemanticVisior
{
private:
    using Base = SemanticVisior;

    std::unordered_set<std::string> features_;

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
        features_.clear();

        auto *features = node.features;
        for (int i = features->first(); features->more(i); i = features->next(i))
        {
            auto *feature = features->nth(i);
            feature->accept(*this);
        }
    }

    void visit(method_class &node) override
    {
        auto method_name = GetNameVisitor::get(&node);
        auto result = features_.insert(method_name);

        if (!result.second)
        {
            context_.error("feature '" + method_name + "' already exist");
        }
    }

    void visit(attr_class &node) override
    {
        auto attr_name = GetNameVisitor::get(&node);
        auto result = features_.insert(attr_name);

        if (!result.second)
        {
            context_.error("feature '" + attr_name + "' already exist");
        }
    }
};

class CheckUniqueFormalsVisitor : public SemanticVisior
{
private:
    using Base = SemanticVisior;

    std::unordered_set<std::string> formals_;

public:
    using Base::Base;

    void visit(program_class &node) override
    {
        auto *classes = node.classes;
        for (int i = classes->first(); classes->more(i); i = classes->next(i))
        {
            class__class *current_class = dynamic_cast<class__class *>(node.classes->nth(i));
            current_class->accept(*this);
        }
    }

    void visit(class__class &node) override
    {
        auto *features = node.features;
        for (int i = features->first(); features->more(i); i = features->next(i))
        {
            auto *feature = features->nth(i);
            feature->accept(*this);
        }
    }

    void visit(method_class &node) override
    {
        formals_.clear();

        auto *formals = node.formals;
        for (int i = formals->first(); formals->more(i); i = formals->next(i))
        {
            auto *formal = formals->nth(i);
            formal->accept(*this);
        }
    }

    void visit(formal_class &node) override
    {
        auto name = GetNameVisitor::get(&node);
        auto result = formals_.insert(name);

        if (!result.second)
        {
            context_.error("formal '" + name + "' already exist");
        }
    }
};

class InheritanceCheckerVisitor : public SemanticVisior
{
private:
    using Base = SemanticVisior;

    void detect_cycle()
    {
        std::unordered_set<std::string> visited;
        std::unordered_set<std::string> currently_visited;

        std::function<bool(const std::string &)> dfs = [&](const std::string &class_name) {
            if (visited.find(class_name) != visited.end())
            {
                return false;
            }

            if (currently_visited.find(class_name) != currently_visited.end())
            {
                return true;
            }

            currently_visited.insert(class_name);

            auto it = context_.classes_hierarchy_.find(class_name);
            if (it != context_.classes_hierarchy_.end())
            {
                if (dfs(it->second))
                {
                    return true;
                }
            }

            visited.insert(class_name);
            currently_visited.erase(class_name);
            return false;
        };

        for (const auto &entry : context_.classes_hierarchy_)
        {
            if (dfs(entry.first))
            {
                context_.error("Find inheritance loop with 'class " + entry.first + " inherit " +
                               entry.second + "'");
            }
        }
    }

public:
    using Base::Base;

    void visit(program_class &node) override
    {
        auto *classes = node.classes;
        for (int i = classes->first(); classes->more(i); i = classes->next(i))
        {
            class__class *current_class = dynamic_cast<class__class *>(node.classes->nth(i));
            current_class->accept(*this);
        }

        detect_cycle();
    }

    void visit(class__class &node) override
    {
        auto class_name = GetNameVisitor::get(&node);
        auto parent_name = GetParentVisitor::get(&node);

        context_.classes_hierarchy_[class_name] = parent_name;

        if (context_.dont_inherit.find(parent_name) != context_.dont_inherit.end())
        {
            context_.error("class '" + class_name + "' inherit '" + parent_name +
                           "'. Cannot inherit from builin types");
        }
        else if (context_.classes_names.find(parent_name) == context_.classes_names.end())
        {
            context_.error("class '" + class_name + "' inherit '" + parent_name +
                           "'. Cannot inherit from '" + parent_name + "', class '" + parent_name +
                           "' dont defined");
        }
    }
};