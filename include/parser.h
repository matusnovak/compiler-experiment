#ifndef SCRIPT_TEST_PARSER_H
#define SCRIPT_TEST_PARSER_H
#include "lexer.h"
#include "expressions.h"
#include "exceptions.h"

namespace script {
    class Parser : public Lexer {
    public:
        Parser(const char* start, const char* end);
        virtual ~Parser() = default;

        void nextsym();
        bool accept(Symbol symbol);
        bool acceptOp(Operator op);
        bool acceptKeyword(Keyword keyword);
        void expect(Symbol symbol);
        void expectOp(Operator op);
        void expectKeyword(Keyword keyword);
        void factor();
        void term();
        void expression();
        void condition();
        void statement();
        void block();
        void program();
        const ParserException& getError() const {
            return err;
        }
    private:
        void raiseError(const char* msg);
        void raiseErrorSymbol(Symbol symbol);
        void raiseErrorKeyword(Keyword keyword);
        void raiseErrorOp(Operator op);
        Token tokenSym;
        ParserException err;

        /*std::unique_ptr<Expression> getExpression();
    private:
        void eat(Lexer::Symbol tokenType);
        std::unique_ptr<Expression> factor();
        std::unique_ptr<Expression> term();
        std::unique_ptr<Expression> expr();
        void expectEndOfStatement();
        //void parseFunctionArgs(FunctionDeclaration* func);
        //void parseFunctionBody(FunctionDeclaration* func);
        Lexer::Token currentToken;*/
    };
}

#endif