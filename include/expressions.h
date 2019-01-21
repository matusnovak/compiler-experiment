#ifndef SCRIPT_TEST_EXPRESSIONS_H
#define SCRIPT_TEST_EXPRESSIONS_H
#include "grammar.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>

namespace script {
    /*class Expression {
    public:
        enum class Type {
            FUNCTION_DECLARATION,
            RETURN_STATEMENT,
            BINARY_EXPRESSION,
            IDENTIFIER,
            VARIABLE_DECLARATION
        };

        Expression() = delete;
        Expression(const Expression& other) = delete;
        Expression(Expression&& other) = delete;
        Expression& operator = (const Expression& other) = delete;
        Expression& operator = (Expression&& other) = delete;

        explicit Expression(const Type type) :type(type) {
        }

        virtual ~Expression() = default;

        virtual bool isSame(const Expression& other) const = 0;
#ifdef _DEBUG
        virtual void print(std::ostream& out, int indent = 0) const = 0;
#endif

        bool operator == (const Expression& other) const  {
            return this->isSame(other);
        }

        const Type type;
    };

    class ReturnStatement : public Expression {
    public:
        explicit ReturnStatement(std::shared_ptr<Expression> right) 
        :Expression(Type::RETURN_STATEMENT), right(std::move(right)) {
        }

        virtual ~ReturnStatement() = default;

        bool isSame(const Expression& other) const override {
            if (other.type != Type::RETURN_STATEMENT)return false;
            const auto ptr = dynamic_cast<const ReturnStatement*>(&other);
            if (right) {
                if (ptr->right == nullptr)return false;
                return right->isSame(*ptr->right);
            }
            else {
                return ptr->right == nullptr;
            }
        }

#if _DEBUG
        void print(std::ostream& out, const int indent) const override {
            for (auto i = 0; i < indent; i++)out << " ";
            out << "Return:" << std::endl;
            for (auto i = 0; i < indent; i++)out << " ";
            if (right) {
                right->print(out, indent + 2);
            }
        }
#endif

        std::shared_ptr<Expression> right;
    };

    class VariableDeclaration : public Expression {
    public:
        explicit VariableDeclaration(std::string name) :Expression(Type::VARIABLE_DECLARATION), 
        name(std::move(name)) {
        }

        virtual ~VariableDeclaration() = default;

        bool isSame(const Expression& other) const override {
            if (other.type != Type::VARIABLE_DECLARATION)return false;
            const auto ptr = dynamic_cast<const VariableDeclaration*>(&other);
            return name == ptr->name;
        }

#if _DEBUG
        void print(std::ostream& out, const int indent) const override {
            for (auto i = 0; i < indent; i++)out << " ";
            out << "Var: " << name << std::endl;
        }
#endif
        
        std::string name;
    };

    class FunctionCall : public Expression {
    public:
        static std::list<std::shared_ptr<script::Expression>> emptyParamsList() {
            return std::list<std::shared_ptr<script::Expression>>();
        }

        explicit FunctionCall(std::string name, std::list<std::shared_ptr<Expression>> params) 
        :Expression(Type::VARIABLE_DECLARATION), name(std::move(name)), params(std::move(params)) {
        }

        virtual ~FunctionCall() = default;

        bool isSame(const Expression& other) const override {
            if (other.type != Type::VARIABLE_DECLARATION)return false;
            const auto ptr = dynamic_cast<const FunctionCall*>(&other);
            if (name != ptr->name)return false;
            if (params.size() != ptr->params.size())return false;
            for (auto it = params.begin(), ptrit = ptr->params.begin(); it != params.end(); ++it, ++ptrit) {
                if (!it->get()->isSame(*ptrit->get()))return false;
            }
            return true;
        }

#if _DEBUG
        void print(std::ostream& out, const int indent) const override {
            for (auto i = 0; i < indent; i++)out << " ";
            out << "Call: " << name << " with params: " << std::endl;
            if (params.empty()) {
                for (auto i = 0; i < indent; i++)out << " ";
                out << "    void";
            }
            for (const auto& param : params) {
                for (auto i = 0; i < indent; i++)out << " ";
                param->print(out, indent + 4);
            }
        }
#endif

        std::string name;
        std::list<std::shared_ptr<Expression>> params;
    };

    class BinaryExpression : public Expression {
    public:
        explicit BinaryExpression(const Operator op, std::shared_ptr<Expression> left, 
        std::shared_ptr<Expression> right) :Expression(Type::BINARY_EXPRESSION),
        op(op),
        left(std::move(left)),
        right(std::move(right)) {
            
        }

        virtual ~BinaryExpression() = default;

        bool isSame(const Expression& other) const override {
            if (other.type != Type::BINARY_EXPRESSION)return false;
            const auto ptr = dynamic_cast<const BinaryExpression*>(&other);
            if (ptr->op != op) return false;
            if (left) {
                if (ptr->left == nullptr)return false;
                if (!left->isSame(*ptr->left))return false;
            } 
            else {
                if (ptr->left != nullptr)return false;
            }
            if (right) {
                if (ptr->right == nullptr)return false;
                if (!right->isSame(*ptr->right))return false;
            }
            else {
                if (ptr->left != nullptr)return false;
            }
            return true;
        }

#if _DEBUG
        void print(std::ostream& out, const int indent) const override {
            for (auto i = 0; i < indent; i++)out << " ";
            out << "Operator: " << std::endl;
            if(left) {
                left->print(out, indent + 2);
            }
            if(right) {
                right->print(out, indent + 2);
            }
        }
#endif

        Operator op;
        std::shared_ptr<Expression> left;
        std::shared_ptr<Expression> right;
    };

    class FunctionDeclaration : public Expression {
    public:
        explicit FunctionDeclaration(std::string name) 
        :Expression(Type::FUNCTION_DECLARATION), name(std::move(name)) {
        }

        virtual ~FunctionDeclaration() = default;

        bool isSame(const Expression& other) const override {
            if (other.type != Type::FUNCTION_DECLARATION)return false;
            const auto ptr = dynamic_cast<const FunctionDeclaration*>(&other);
            if (name != ptr->name) return false;
            if (args.size() != ptr->args.size()) return false;
            for(size_t i = 0; i < args.size(); i++) {
                if (args[i] != ptr->args[i])return false;
            }
            if (body.size() != ptr->body.size()) return false;
            for (size_t i = 0; i < body.size(); i++) {
                if (!body[i]->isSame(*ptr->body[i]))return false;
            }
            return true;
        }

#if _DEBUG
        void print(std::ostream& out, const int indent) const override {
            for (auto i = 0; i < indent; i++)out << " ";
            out << "Function: " << name << std::endl;
        }
#endif

        const std::string name;
        std::vector<std::string> args;
        std::vector<std::shared_ptr<Expression>> body;
    };

    class Identifier: public Expression {
    public:
        explicit Identifier(std::string name):Expression(Type::IDENTIFIER),name(std::move(name)) {
            
        }

        virtual ~Identifier() = default;

        bool isSame(const Expression& other) const override {
            if (other.type != Type::IDENTIFIER)return false;
            const auto ptr = dynamic_cast<const Identifier*>(&other);
            return name == ptr->name;
        }

#if _DEBUG
        void print(std::ostream& out, const int indent) const override {
            for (auto i = 0; i < indent; i++)out << " ";
            out << "Identifier: " << name << std::endl;
        }
#endif

        const std::string name;
    };*/
}

#endif