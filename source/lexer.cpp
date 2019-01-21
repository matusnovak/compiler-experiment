#include "../include/lexer.h"
#include "../include/exceptions.h"
#include <iostream>
#include <string>

script::Lexer::Lexer(const char* start, const char* end):Tokenizer(start, end) {
}

script::Lexer::Token script::Lexer::getLexerToken() {
    const auto token = Tokenizer::getToken();

    if (!token) {
        return {};
    }

    Token::Data data{};
    data.id = Identifier::KEY;

    if (token.len == 1) {
        switch (*token.str) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                data.id = Identifier::INTEGER;
                return Lexer::Token{ Symbol::ID, token.str, token.len, data };
            case ';':
                return Lexer::Token{ Symbol::END_OF_STATEMENT, token.str, token.len, data };
            case ',':
                return Lexer::Token{ Symbol::SEPARATOR, token.str, token.len, data };
            case ':':
            case '+':
            case '-':
            case '/':
            case '*':
            case '>':
            case '<':
            case '!':
            case '|':
            case '%':
            case '=':
                data.op = strToOperator(token.str, token.len);
                return Lexer::Token{ Symbol::OPERATOR, token.str, token.len, data };
            case '(':
                return Lexer::Token{ Symbol::BRACKET_L, token.str, token.len, data };
            case ')':
                return Lexer::Token{ Symbol::BRACKET_R, token.str, token.len, data };
            case '{':
                return Lexer::Token{ Symbol::CURLY_BRACKET_L, token.str, token.len, data };
            case '}':
                return Lexer::Token{ Symbol::CURLY_BRACKET_R, token.str, token.len, data };
            default: 
                const auto keyword = strToKeyword(token.str, token.len);
                if(keyword != Keyword::UNDEFINED) {
                    data.keyword = keyword;
                    return Lexer::Token{ Symbol::KEYWORD, token.str, token.len, data };
                }
                return Lexer::Token{ Symbol::ID, token.str, token.len, data };
        }
    } else {
        if (token.len == 2) {
            /*if(*token.str == '=' && *(token.str + 1) == '=') {
                data.op = strToOperator(token.str, token.len);
                return Lexer::Token{ Symbol::OPERATOR, token.str, token.len, data };
            }*/
            const auto test = strToOperator(token.str, token.len);
            if (test != Operator::UNDEFINED) {
                data.op = test;
                return Lexer::Token{ Symbol::OPERATOR, token.str, token.len, data };
            }
        }

        auto decimalPoint = false;
        if(isNumber(token.str, token.len, decimalPoint)) {
            if(decimalPoint) {
                //data.decimal = stof(std::string(token.str, token.len));
                data.id = Identifier::FLOAT;
                return Lexer::Token{ Symbol::ID, token.str, token.len, data };
            } else {
                //data.integer = stoi(std::string(token.str, token.len));
                data.id = Identifier::INTEGER;
                return Lexer::Token{ Symbol::ID, token.str, token.len, data };
            }
        }

        const auto keyword = strToKeyword(token.str, token.len);
        if (keyword != Keyword::UNDEFINED) {
            data.keyword = keyword;
            return Lexer::Token{ Symbol::KEYWORD, token.str, token.len, data };
        }
        return Lexer::Token{ Symbol::ID, token.str, token.len, data };
    }
}

bool script::Lexer::isNumber(const char* str, unsigned len, bool& decimal) {
    for(unsigned i = 0; i < len; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            // All good
        } else if (str[i] == '.'){
            decimal = true;
        } else {
            return false;
        }
    }
    return true;
}