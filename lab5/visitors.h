#pragma once

#include "cool-tree.h"
#include "semantic.h"

#include <functional>
#include <string>
#include <vector>

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

class GetExpressionTypeVisitor : public Visitor
{
public:
    std::string type;

    static std::string get(tree_node *node);

public:
    void visit(branch_class &node) override
    {
        type = "branch_class";
    }

    void visit(assign_class &node) override
    {
        type = "assign_class";
    }

    void visit(static_dispatch_class &node) override
    {
        type = "static_dispatch_class";
    }

    void visit(dispatch_class &node) override
    {
        type = "dispatch_class";
    }

    void visit(let_class &node) override
    {
        type = "let_class";
    }

    void visit(new__class &node) override
    {
        type = "new__class";
    }

    void visit(object_class &node) override
    {
        type = "object_class";
    }

    void visit(block_class &node) override
    {
        type = "block_class";
    }

    void visit(plus_class &node) override
    {
        type = "plus_class";
    }

    void visit(sub_class &node) override
    {
        type = "sub_class";
    }

    void visit(mul_class &node) override
    {
        type = "mul_class";
    }

    void visit(divide_class &node) override
    {
        type = "divide_class";
    }

    void visit(string_const_class &node) override
    {
        type = "string_const_class";
    }

    void visit(bool_const_class &node) override
    {
        type = "bool_const_class";
    }

    void visit(int_const_class &node) override
    {
        type = "int_const_class";
    }

    void visit(neg_class &node) override
    {
        type = "neg_class";
    }

    void visit(lt_class &node) override
    {
        type = "lt_class";
    }

    void visit(eq_class &node) override
    {
        type = "eq_class";
    }

    void visit(leq_class &node) override
    {
        type = "leq_class";
    }

    void visit(cond_class &node) override
    {
        type = "cond_class";
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

    std::string current_class_name_;

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
        current_class_name_ = GetNameVisitor::get(&node);

        context_.methods_[current_class_name_] = {};
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
        context_.methods_[current_class_name_][method_name] = node.return_type->get_string();

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

    using StringUSet = std::unordered_set<std::string>;

    bool dfs(const std::string &class_name, StringUSet visited, StringUSet currently_visited)
    {
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
            if (dfs(it->second, visited, currently_visited))
            {
                return true;
            }
        }

        visited.insert(class_name);
        currently_visited.erase(class_name);
        return false;
    }

    void detect_cycle()
    {
        StringUSet visited;
        StringUSet currently_visited;

        for (const auto &entry : context_.classes_hierarchy_)
        {
            if (dfs(entry.first, visited, currently_visited))
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

class TypeCheckerVisitor : public SemanticVisior
{
private:
    using Base = SemanticVisior;

    struct Scope
    {
        std::unordered_map<std::string, std::string> variables;
    };

    std::vector<Scope> scopes_;

private:
    void enter_scope()
    {
        scopes_.push_back(Scope{});
    }

    void exit_scope()
    {
        if (!scopes_.empty())
        {
            scopes_.pop_back();
        }
    }

    bool add_var(const std::string &name, const std::string &type)
    {
        if (scopes_.empty())
            return false;

        if (scopes_.back().variables.count(name))
            return false;

        scopes_.back().variables.insert({name, type});
        return true;
    }

    bool is_visible(const std::string &name)
    {
        for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it)
        {
            if (it->variables.count(name))
                return true;
        }
        return false;
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
    }

    void visit(class__class &node) override
    {
        enter_scope();

        auto *features = node.features;
        for (int i = features->first(); features->more(i); i = features->next(i))
        {
            auto *feature = features->nth(i);
            feature->accept(*this);
        }

        exit_scope();
    }

    void visit(method_class &node) override
    {
        enter_scope();

        auto *formals = node.formals;
        for (int i = formals->first(); formals->more(i); i = formals->next(i))
        {
            auto *formal = formals->nth(i);
            formal->accept(*this);
        }

        node.expr->accept(*this);

        exit_scope();
    }

    void visit(formal_class &node) override
    {
        add_var(GetNameVisitor::get(&node), node.type_decl->get_string());
    }

    void visit(attr_class &node) override
    {
    }

    void visit(plus_class &node) override
    {
        node.e1.
    }
};
