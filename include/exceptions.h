#ifndef SCRIPT_TEST_EXCEPTIONS_H
#define SCRIPT_TEST_EXCEPTIONS_H
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "grammar.h"

namespace script {

    class Formatter {
    public:
        Formatter() = default;
        ~Formatter() = default;

        Formatter(const Formatter &) = delete;
        Formatter & operator = (Formatter &) = delete;

        template <typename Type>
        Formatter & operator << (const Type & value) {
            stream << value;
            return *this;
        }

        std::string str() const {
            return stream.str();
        }

        operator std::string() const {
            return stream.str();
        }

        enum ConvertToString
        {
            to_str
        };
        std::string operator >> (ConvertToString) const {
            return stream.str();
        }

    private:
        std::stringstream stream;
    };

    class ParserException: public std::runtime_error {
    public:
        ParserException():std::runtime_error("Unknow error"),expected(Symbol::UNDEFINED) {
        }

        explicit ParserException(const char* str):std::runtime_error(str),expected(Symbol::UNDEFINED) {
        }

        explicit ParserException(const Symbol symbol):std::runtime_error(Formatter() << "expected symbol " << symbolToStr(symbol)), expected(symbol) {
            this->expected = expected;
        }

        explicit ParserException(const Keyword keyword):std::runtime_error(Formatter() << "expected keyword " << keywordToStr(keyword)), expected(Symbol::KEYWORD) {
            data.keyword = keyword;
            expected = Symbol::KEYWORD;
        }

        explicit ParserException(const Operator op):std::runtime_error(Formatter() << "expected operator " << operatorToStr(op)), expected(Symbol::OPERATOR) {
            data.op = op;
            expected = Symbol::OPERATOR;
        }

        union Data {
            Keyword keyword;
            Operator op;
        } data;

        Symbol expected;
    };

    class EndOfFile: public std::runtime_error {
    public:
        EndOfFile(const std::string& msg) :std::runtime_error(msg) {

        }
    };
}

#endif
