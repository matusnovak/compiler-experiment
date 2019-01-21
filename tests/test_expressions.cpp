#include <exception>
#include "catch.hpp"
#include "../include/tokenizer.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/exceptions.h"

using namespace script;

#define STRINGIFY(x) #x

#define GET_AND_ASSERT_TOKEN(p, expected) { auto t = p.getToken(); REQUIRE(t); REQUIRE(std::string(t.str, t.len) == expected); }
#define GET_AND_ASSERT_TOKEN_FAIL(p) { auto t = p.getToken(); REQUIRE(!(t)); REQUIRE(t.str == nullptr); REQUIRE(t.len == 0UL); }
#define CHECK_ERROR(p) { if (parser.hasError()) {throw std::runtime_error(parser.getError().msg + " at: " + std::string(parser.getCurrentStrPos())); } REQUIRE(parser.hasError() == false); }

TEST_CASE("Simple expression addition") {
    static const std::string source = "a + b";
    Parser parser(&source[0], &source[0] + source.size());

    parser.nextsym();
    parser.expression();
    REQUIRE(parser.eof() == true);
}

TEST_CASE("Simple expression addition with missing identifier") {
    static const std::string source = "a + b + ";
    Parser parser(&source[0], &source[0] + source.size());

    parser.nextsym();
    REQUIRE_THROWS_AS(parser.expression(), ParserException);
    REQUIRE(parser.getError().expected == Symbol::ID);
    REQUIRE(parser.getCurrentStrPos() == &source[0] + 7);
}

TEST_CASE("Simple expression addition with extra operator") {
    static const std::string source = "a + +";
    Parser parser(&source[0], &source[0] + source.size());

    parser.nextsym();
    REQUIRE_THROWS_AS(parser.expression(), ParserException);
    REQUIRE(parser.getError().expected == Symbol::ID);
    REQUIRE(parser.getCurrentStrPos() == &source[0] + 5);
}
