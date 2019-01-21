#include "catch.hpp"
#include "../include/parser.h"

using namespace script;

#define STRINGIFY(x) #x

TEST_CASE("Simple assingment statement") {
    static const std::string source = "var a = 0;";
    Parser parser(&source[0], &source[0] + source.size());

    parser.nextsym();
    parser.statement();
}

/*TEST_CASE("Simple expression with function declaration") {
    static const std::string source = "function foo(a, b)\nreturn a + b;\n}";

    Parser parser(&source[0], &source[0] + source.size());
}*/