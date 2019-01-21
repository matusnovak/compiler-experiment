#include "../include/grammar.h"
#include <cstring>

static bool isSame(const char* src, unsigned srcLen, const char* expected) {
    if (srcLen != strlen(expected))return false;
    for(unsigned int i = 0; i < srcLen; i++) {
        if (expected[i] == '\0')return false;
        if (src[i] != expected[i])return false;
    }
    return true;
}

script::Keyword script::strToKeyword(const char* str, unsigned len) {
    if(len == 0) {
        return Keyword::UNDEFINED;
    }

    if (isSame(str, len, "function"))return Keyword::FUNCTION;
    if (isSame(str, len, "return"))return Keyword::RETURN;
    if (isSame(str, len, "var"))return Keyword::VAR;
    if (isSame(str, len, "if"))return Keyword::IF;
    if (isSame(str, len, "while"))return Keyword::WHILE;
    
    return Keyword::UNDEFINED;
}

script::Operator script::strToOperator(const char* str, unsigned len) {
    if (len == 0) {
        return Operator::UNDEFINED;
    }

    if (len == 1) {
        switch (*str) {
            case '+': return Operator::ADD;
            case '-': return Operator::SUBSTRACT;
            case '/': return Operator::DIVIDE;
            case '*': return Operator::MULTIPLY;
            case '=': return Operator::ASSIGNMENT;
            default: return Operator::UNDEFINED;
        }
    } else if (len == 2) {
        switch (*str) {
            case '+': return *(str + 1) == '+' ? Operator::INCREMENT : Operator::UNDEFINED;
            case '-': return *(str + 1) == '-' ? Operator::DECREMENT : Operator::UNDEFINED;
            case '=': return *(str + 1) == '=' ? Operator::EQUALS : Operator::UNDEFINED;
            default: return Operator::UNDEFINED;
        }
    } else {
        return Operator::UNDEFINED;
    }
}

const char* script::symbolToStr(const Symbol symbol) {
    switch(symbol) {
        case Symbol::UNDEFINED: return "unknown";
        case Symbol::ID: return "identifier";
        case Symbol::KEYWORD: return "keyword";
        case Symbol::OPERATOR: return "operator";
        case Symbol::BRACKET_L: return "left bracket";
        case Symbol::BRACKET_R: return "right bracket";
        case Symbol::CURLY_BRACKET_L: return "left curly bracket";
        case Symbol::CURLY_BRACKET_R: return "right curly bracket";
        case Symbol::SEPARATOR: return "comma";
        case Symbol::END_OF_STATEMENT: return "semicolon";
        default: return "unknown";
    }
}

const char* script::keywordToStr(const Keyword keyword) {
    switch(keyword) {
        case Keyword::FUNCTION: return "function";
        case Keyword::RETURN: return "return";
        case Keyword::VAR: return "var";
        case Keyword::IF: return "if";
        case Keyword::WHILE: return "while";
        default: return "unknown";
    }
}

extern const char* script::operatorToStr(const Operator op) {
    switch (op) {
        case Operator::ADD: return "+";
        case Operator::SUBSTRACT: return "-";
        case Operator::DIVIDE: return "/";
        case Operator::MULTIPLY: return "*";
        case Operator::ASSIGNMENT: return "=";
        case Operator::EQUALS: return "==";
        case Operator::INCREMENT: return "++";
        case Operator::DECREMENT: return "--";
        default: return "unknown";
    }
}
