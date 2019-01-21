#ifndef SCRIPT_TEST_LEXER_H
#define SCRIPT_TEST_LEXER_H
#include "tokenizer.h"
#include "grammar.h"
#include <string>

namespace script {
    class Lexer: public Tokenizer {
    public:
        class Token {
        public:
            Symbol symbol = Symbol::UNDEFINED;
            const char* str = nullptr;
            unsigned long len = 0UL;

            union Data {
                Keyword keyword;
                Operator op;
                Identifier id;
            } data;

            bool empty() const {
                return str == nullptr;
            }

            std::string asString() const {
                if (!str)return std::string("");
                return std::string(str, len);
            }
        };

        Lexer(const char* start, const char* end);
        virtual ~Lexer() = default;

        Token getLexerToken();

    private:
        static bool isNumber(const char* str, unsigned len, bool& decimal);
    };
}

#endif