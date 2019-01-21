#define CATCH_CONFIG_MAIN
#include <exception>
#include "catch.hpp"
#include "../include/tokenizer.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/grammar.h"

using namespace script;

#define STRINGIFY(x) #x

#define GET_AND_ASSERT_TOKEN(p, expected) { auto t = p.getToken(); REQUIRE(t); REQUIRE(std::string(t.str, t.len) == expected); }
#define GET_AND_ASSERT_TOKEN_FAIL(p) { auto t = p.getToken(); REQUIRE(!(t)); REQUIRE(t.str == nullptr); REQUIRE(t.len == 0UL); }
#define CHECK_ERROR(p) { if (parser.hasError()) {throw std::runtime_error(parser.getError().msg + " at: " + std::string(parser.getCurrentStrPos())); } REQUIRE(parser.hasError() == false); }

TEST_CASE("Tokenize simple expression") {
    static const std::string source = "a + b + c";

    Tokenizer tokenizer(&source[0], &source[0] + source.size());
    GET_AND_ASSERT_TOKEN(tokenizer, "a");
    GET_AND_ASSERT_TOKEN(tokenizer, "+");
    GET_AND_ASSERT_TOKEN(tokenizer, "b");
    GET_AND_ASSERT_TOKEN(tokenizer, "+");
    GET_AND_ASSERT_TOKEN(tokenizer, "c");
    GET_AND_ASSERT_TOKEN_FAIL(tokenizer);
}

TEST_CASE("Open parser and get tokens") {
    static const std::string source = STRINGIFY(
        function foo(a, b) {
        return a + b;
    }
    );

    Tokenizer parser(&source[0], &source[0] + source.size());

    GET_AND_ASSERT_TOKEN(parser, "function");
    GET_AND_ASSERT_TOKEN(parser, "foo");
    GET_AND_ASSERT_TOKEN(parser, "(");
    GET_AND_ASSERT_TOKEN(parser, "a");
    GET_AND_ASSERT_TOKEN(parser, ",");
    GET_AND_ASSERT_TOKEN(parser, "b");
    GET_AND_ASSERT_TOKEN(parser, ")");
    GET_AND_ASSERT_TOKEN(parser, "{");
    GET_AND_ASSERT_TOKEN(parser, "return");
    GET_AND_ASSERT_TOKEN(parser, "a");
    GET_AND_ASSERT_TOKEN(parser, "+");
    GET_AND_ASSERT_TOKEN(parser, "b");
    GET_AND_ASSERT_TOKEN(parser, ";");
    GET_AND_ASSERT_TOKEN(parser, "}");
    GET_AND_ASSERT_TOKEN_FAIL(parser);
}

TEST_CASE("Simple expression tokens") {
    static const std::string source = "a + b + c";

    Tokenizer tokenizer(&source[0], &source[0] + source.size());
    GET_AND_ASSERT_TOKEN(tokenizer, "a");
    GET_AND_ASSERT_TOKEN(tokenizer, "+");
    GET_AND_ASSERT_TOKEN(tokenizer, "b");
    GET_AND_ASSERT_TOKEN(tokenizer, "+");
    GET_AND_ASSERT_TOKEN(tokenizer, "c");
    GET_AND_ASSERT_TOKEN_FAIL(tokenizer);
}

TEST_CASE("Simple expression tokens with extra symbol") {
    static const std::string source = "a + b + c +";

    Tokenizer tokenizer(&source[0], &source[0] + source.size());
    GET_AND_ASSERT_TOKEN(tokenizer, "a");
    GET_AND_ASSERT_TOKEN(tokenizer, "+");
    GET_AND_ASSERT_TOKEN(tokenizer, "b");
    GET_AND_ASSERT_TOKEN(tokenizer, "+");
    GET_AND_ASSERT_TOKEN(tokenizer, "c");
    GET_AND_ASSERT_TOKEN(tokenizer, "+");
    GET_AND_ASSERT_TOKEN_FAIL(tokenizer);
}

static void ASSERT_LEXER_TOKEN(Lexer& lexer, const std::string& expected, Symbol type) {
    auto t = lexer.getLexerToken();
    REQUIRE(!t.empty());
    REQUIRE(std::string(t.str, t.len) == expected);
    REQUIRE(t.symbol == type);
}

static void ASSERT_LEXER_KEYWORD(Lexer& lexer, const std::string& expected, Keyword keyword) {
    auto t = lexer.getLexerToken();
    REQUIRE(!t.empty());
    REQUIRE(std::string(t.str, t.len) == expected);
    REQUIRE(t.symbol == Symbol::KEYWORD);
    REQUIRE(t.data.keyword == keyword);
}

static void ASSERT_LEXER_OPERATOR(Lexer& lexer, const std::string& expected, Operator op) {
    auto t = lexer.getLexerToken();
    REQUIRE(!t.empty());
    REQUIRE(std::string(t.str, t.len) == expected);
    REQUIRE(t.symbol == Symbol::OPERATOR);
    REQUIRE(t.data.op == op);
}

static void ASSERT_LEXER_EMPTY(Lexer& lexer) {
    auto t = lexer.getLexerToken();
    REQUIRE(t.empty());
}

TEST_CASE("Open lexer and get tokens function") {
    static const std::string source = STRINGIFY(
        function foo(a, b) {
        return a + b;
    }
    );

    Lexer lexer(&source[0], &source[0] + source.size());

    ASSERT_LEXER_KEYWORD(lexer, "function", Keyword::FUNCTION);
    ASSERT_LEXER_TOKEN(lexer, "foo", Symbol::ID);
    ASSERT_LEXER_TOKEN(lexer, "(", Symbol::BRACKET_L);
    ASSERT_LEXER_TOKEN(lexer, "a", Symbol::ID);
    ASSERT_LEXER_TOKEN(lexer, ",", Symbol::SEPARATOR);
    ASSERT_LEXER_TOKEN(lexer, "b", Symbol::ID);
    ASSERT_LEXER_TOKEN(lexer, ")", Symbol::BRACKET_R);
    ASSERT_LEXER_TOKEN(lexer, "{", Symbol::CURLY_BRACKET_L);
    ASSERT_LEXER_KEYWORD(lexer, "return", Keyword::RETURN);
    ASSERT_LEXER_TOKEN(lexer, "a", Symbol::ID);
    ASSERT_LEXER_OPERATOR(lexer, "+", Operator::ADD);
    ASSERT_LEXER_TOKEN(lexer, "b", Symbol::ID);
    ASSERT_LEXER_TOKEN(lexer, ";", Symbol::END_OF_STATEMENT);
    ASSERT_LEXER_TOKEN(lexer, "}", Symbol::CURLY_BRACKET_R);
    ASSERT_LEXER_EMPTY(lexer);
}

TEST_CASE("Open lexer and get tokens assingment") {
    static const std::string source = "var a = b;";

    Lexer lexer(&source[0], &source[0] + source.size());

    ASSERT_LEXER_KEYWORD(lexer, "var", Keyword::VAR);
    ASSERT_LEXER_TOKEN(lexer, "a", Symbol::ID);
    ASSERT_LEXER_OPERATOR(lexer, "=", Operator::ASSIGNMENT);
    ASSERT_LEXER_TOKEN(lexer, "b", Symbol::ID);
    ASSERT_LEXER_TOKEN(lexer, ";", Symbol::END_OF_STATEMENT);
    ASSERT_LEXER_EMPTY(lexer);
}

TEST_CASE("Open lexer and get tokens equals") {
    static const std::string source = "var a == b;";

    Lexer lexer(&source[0], &source[0] + source.size());

    ASSERT_LEXER_KEYWORD(lexer, "var", Keyword::VAR);
    ASSERT_LEXER_TOKEN(lexer, "a", Symbol::ID);
    ASSERT_LEXER_OPERATOR(lexer, "==", Operator::EQUALS);
    ASSERT_LEXER_TOKEN(lexer, "b", Symbol::ID);
    ASSERT_LEXER_TOKEN(lexer, ";", Symbol::END_OF_STATEMENT);
    ASSERT_LEXER_EMPTY(lexer);
}

TEST_CASE("Open lexer and get tokens call") {
    static const std::string source = "a + b()";

    Lexer lexer(&source[0], &source[0] + source.size());

    ASSERT_LEXER_TOKEN(lexer, "a", Symbol::ID);
    ASSERT_LEXER_OPERATOR(lexer, "+", Operator::ADD);
    ASSERT_LEXER_TOKEN(lexer, "b", Symbol::ID);
    ASSERT_LEXER_TOKEN(lexer, "(", Symbol::BRACKET_L);
    ASSERT_LEXER_TOKEN(lexer, ")", Symbol::BRACKET_R);
    ASSERT_LEXER_EMPTY(lexer);
}

TEST_CASE("Open lexer and get tokens increment") {
    static const std::string source = "a++;";

    Lexer lexer(&source[0], &source[0] + source.size());

    ASSERT_LEXER_TOKEN(lexer, "a", Symbol::ID);
    ASSERT_LEXER_OPERATOR(lexer, "++", Operator::INCREMENT);
    ASSERT_LEXER_TOKEN(lexer, ";", Symbol::END_OF_STATEMENT);
    ASSERT_LEXER_EMPTY(lexer);
}

TEST_CASE("Open lexer and get tokens decrement") {
    static const std::string source = "a--;";

    Lexer lexer(&source[0], &source[0] + source.size());

    ASSERT_LEXER_TOKEN(lexer, "a", Symbol::ID);
    ASSERT_LEXER_OPERATOR(lexer, "--", Operator::DECREMENT);
    ASSERT_LEXER_TOKEN(lexer, ";", Symbol::END_OF_STATEMENT);
    ASSERT_LEXER_EMPTY(lexer);
}