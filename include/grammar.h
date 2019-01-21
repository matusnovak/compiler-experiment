#ifndef SCRIPT_TEST_GRAMMAR_H
#define SCRIPT_TEST_GRAMMAR_H
#include "config.h"

namespace script {
    enum class Symbol {
        UNDEFINED = 0,
        ID,
        KEYWORD,
        OPERATOR,
        BRACKET_L,
        BRACKET_R,
        CURLY_BRACKET_L,
        CURLY_BRACKET_R,
        SEPARATOR,
        END_OF_STATEMENT,
    };

    enum class Keyword {
        UNDEFINED = 0,
        FUNCTION,
        RETURN,
        VAR,
        IF,
        WHILE
    };

    enum class Identifier {
        UNDEFINED = 0,
        KEY,
        FLOAT,
        INTEGER,
        CHAR,
        STRING
    };

    enum class Operator {
        UNDEFINED = 0,
        ADD,
        SUBSTRACT,
        DIVIDE,
        MULTIPLY,
        ASSIGNMENT,
        EQUALS,
        INCREMENT,
        DECREMENT,
    };

    extern Keyword strToKeyword(const char* str, unsigned len);
    extern Operator strToOperator(const char* str, unsigned len);
    extern const char* symbolToStr(Symbol symbol);
    extern const char* keywordToStr(Keyword kw);
    extern const char* operatorToStr(Operator op);
}

#endif