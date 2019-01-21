#include <sstream>
#include <list>
#include "../include/parser.h"
#include "../include/exceptions.h"

// Based on https://en.wikipedia.org/wiki/Recursive_descent_parser

using namespace script;

Parser::Parser(const char* start, const char* end) :Lexer(start, end) {
    //currentToken = Lexer::getLexerToken();
    //std::cout << "getLexerToken: " << std::string(currentToken) << std::endl;
}

void Parser::nextsym() {
    tokenSym = Lexer::getLexerToken();
}

bool Parser::accept(const Symbol symbol) {
    if (tokenSym.symbol == symbol) {
        nextsym();
        return true;
    }
    return false;
}

bool Parser::acceptOp(const Operator op) {
    if (tokenSym.symbol == Symbol::OPERATOR && tokenSym.data.op == op) {
        nextsym();
        return true;
    }
    return false;
}

bool Parser::acceptKeyword(const Keyword keyword) {
    if (tokenSym.symbol == Symbol::KEYWORD && tokenSym.data.keyword == keyword) {
        nextsym();
        return true;
    }
    return false;
}

void Parser::expect(const Symbol symbol) {
    if (accept(symbol))return;
    raiseErrorSymbol(symbol);
}

void Parser::expectOp(const Operator op) {
    if (acceptOp(op))return;
    raiseErrorOp(op);
}

void Parser::expectKeyword(const Keyword keyword) {
    if (acceptKeyword(keyword))return;
    raiseErrorKeyword(keyword);
}

void Parser::factor() {
    if (accept(Symbol::ID)) {
        ;
    }
    else if (accept(Symbol::BRACKET_L)) {
        expression();
        expect(Symbol::BRACKET_R);
    }
    else {
        raiseErrorSymbol(Symbol::ID);
        nextsym();
    }
}

void Parser::term() {
    factor();
    while (tokenSym.symbol == Symbol::OPERATOR && (tokenSym.data.op == Operator::MULTIPLY || tokenSym.data.op == Operator::DIVIDE)) {
        nextsym();
        factor();
    }
}

void Parser::expression() {
    if (tokenSym.symbol == Symbol::OPERATOR && (tokenSym.data.op == Operator::ADD || tokenSym.data.op == Operator::SUBSTRACT)) {
        nextsym();
    }
    term();
    while (tokenSym.symbol == Symbol::OPERATOR && (tokenSym.data.op == Operator::ADD || tokenSym.data.op == Operator::SUBSTRACT)) {
        nextsym();
        term();
    }
}

void Parser::condition() {
    //if (accept(oddsym)) {
    //    expression();
    //}
    //else {
    expression();
    if (tokenSym.symbol == Symbol::OPERATOR) {
        switch(tokenSym.data.op) {
            case Operator::EQUALS: break;
            default: raiseError("condition invalid operator");
        }
        nextsym();
        expression();
    }
    else {
        raiseError("condition invalid operator");
        nextsym();
    }
    //}
}

void Parser::statement() {
    if (acceptKeyword(Keyword::VAR)) {
        expect(Symbol::ID);
        expectOp(Operator::ASSIGNMENT);
        expression();
        expect(Symbol::END_OF_STATEMENT);
    }
    /*else if (accept(callsym)) {
        expect(ident);
    }*/
    /*else if (accept(beginsym)) {
        do {
            statement();
        } while (accept(semicolon));
        expect(endsym);
    }*/
    else if (acceptKeyword(Keyword::IF)) {
        condition();
        expect(Symbol::CURLY_BRACKET_L);
        statement();
        expect(Symbol::CURLY_BRACKET_R); // ??
    }
    else if (acceptKeyword(Keyword::WHILE)) {
        condition();
        expect(Symbol::CURLY_BRACKET_L);
        statement();
        expect(Symbol::CURLY_BRACKET_R); // ??
    }
    else {
        raiseError("expected identifier or keyword");
        nextsym();
    }
}

void Parser::block() {
    
}

void Parser::program() {
    
}

void Parser::raiseError(const char* msg) {
    err = ParserException(msg);
    throw err;
}

void Parser::raiseErrorSymbol(const Symbol symbol) {
    err = ParserException(symbol);
    throw err;
}

void Parser::raiseErrorKeyword(const Keyword keyword) {
    err = ParserException(keyword);
    throw err;
}

void Parser::raiseErrorOp(const Operator op) {
    err = ParserException(op);
    throw err;
}

/*// The following parser was based on https://ruslanspivak.com/lsbasi-part7/ by https://github.com/rspivak/
void script::Parser::eat(const Lexer::Symbol tokenType) {
    if (currentToken.type == tokenType) {
        //std::cout << "eat(" << currentToken.type << ")" << std::endl;
        try {
            const auto got = Lexer::getLexerToken();
            //std::cout << "got token: " << std::string(currentToken) << std::endl;
            if (tokenType == Lexer::Symbol::ID && got.type == Lexer::Symbol::ID) {
                throw ParserException("Unexpected token");
            }
            currentToken = got;
        } catch (EndOfFile& e) {
            //std::cout << "tokenType: " << int(tokenType) << " ? ID: " << int(Lexer::Symbol::BRACKET_R) << std::endl;
            switch(tokenType) {
                case Lexer::Symbol::BRACKET_R:
                case Lexer::Symbol::ID:
                    return;
                default: 
                    break;
            }
            (void)e;
            //std::cout << "throwing unexpected end of file" << std::endl;
            throw ParserException("Unexpected end of file");
        }
    } else {
        throw ParserException("Unexpected token \'" + std::string(currentToken) + "\'");
    }
}

std::unique_ptr<script::Expression> script::Parser::factor() {
    const auto token = currentToken;
    if (token.type == Lexer::Symbol::ID) {
        eat(Lexer::Symbol::ID);
        const auto idName = std::string(token.str, token.len);
        //std::cout << "factor return Identifier " << std::string(token) << " next: " << std::string(currentToken) << std::endl;
        if(currentToken && currentToken.type == Lexer::Symbol::BRACKET_L) {
            //std::cout << "function call" << std::endl;
            eat(Lexer::Symbol::BRACKET_L);
            if(currentToken) {
                if(currentToken.type == Lexer::Symbol::BRACKET_R) {
                    // Function with no parameters
                    eat(Lexer::Symbol::BRACKET_R);
                    return std::make_unique<FunctionCall>(idName, std::list<std::shared_ptr<script::Expression>>());
                }
                else if(currentToken.type == Lexer::Symbol::ID) {
                    // Function with parameters
                    std::list<std::shared_ptr<Expression>> params;
                    //std::cout << "looking for parameter" << std::endl;
                    while(true) {
                        auto param = expr();
                        //std::cout << "got param: " << int(param->type) << std::endl;
                        params.push_back(std::move(param));
                        // End of params?
                        if (currentToken.type == Lexer::Symbol::BRACKET_R) {
                            eat(Lexer::Symbol::BRACKET_R);
                            break;
                        } else if(currentToken.type == Lexer::Symbol::SEPARATOR) {
                            eat(Lexer::Symbol::SEPARATOR);
                            continue;
                        } else {
                            throw ParserException("Expected ',', got instead TODO");
                        }
                    }
                    return std::make_unique<FunctionCall>(idName, params);
                }
                else {
                    //std::cout << "currentToken type: " << currentToken.type << std::endl;
                    throw ParserException("Expected function parameter or ')', got instead: " + std::string(currentToken));
                }
            } else {
                throw ParserException("Expected function parameter or ')', got instead end of file");
            }
        } else if(currentToken && currentToken.type == Lexer::Symbol::OPERATOR) {
            if(currentToken.data.op == Operator::INCREMENT) {
                eat(Lexer::Symbol::OPERATOR);
                eat(Lexer::Symbol::END_OF_STATEMENT);
                //std::cout << "factor return ++" << std::endl;
                return std::make_unique<BinaryExpression>(Operator::INCREMENT, std::make_unique<Identifier>(idName), nullptr);
            }
            if (currentToken.data.op == Operator::DECREMENT) {
                eat(Lexer::Symbol::OPERATOR);
                eat(Lexer::Symbol::END_OF_STATEMENT);
                //std::cout << "factor return --" << std::endl;
                return std::make_unique<BinaryExpression>(Operator::DECREMENT, std::make_unique<Identifier>(idName), nullptr);
            }
        }
        //std::cout << "factor return identifier" << std::endl;
        return std::make_unique<Identifier>(idName);
    } else if (token.type == Lexer::Symbol::BRACKET_L) {
        eat(Lexer::Symbol::BRACKET_L);
        auto node = expr();
        try {
            eat(Lexer::Symbol::BRACKET_R);
        } catch (std::exception_ptr& e) {
            std::rethrow_exception(e);
        }
        //std::cout << "factor return node" << std::endl;
        return node;
    } else if (token.type == Lexer::Symbol::KEYWORD) {
        //std::cout << "is keyword!" << std::endl;
        switch(currentToken.data.keyword) {
            case Keyword::VAR: {
                eat(Lexer::Symbol::KEYWORD);
                const auto varName = std::string(currentToken.str, currentToken.len);
                eat(Lexer::Symbol::ID);
                //std::cout << "factor return variable declaration" << std::endl;
                return std::make_unique<VariableDeclaration>(varName);
            }
            default: {
                throw ParserException("Unexpected keyword");
            }
        }
    }
    //std::cout << "returning nullptr" << std::endl;
    return nullptr;
}

void script::Parser::expectEndOfStatement() {
    if (!currentToken) {
        //std::cout << "expectEndOfStatement false" << std::endl;
        return;
    }

    //std::cout << "expectEndOfStatement true" << std::endl;

    if (currentToken.type == Lexer::Symbol::END_OF_STATEMENT) {
        try {
            const auto got = Lexer::getLexerToken();
            currentToken = got;
            //std::cout << "next token: " << std::string(currentToken) << std::endl;
        }
        catch (EndOfFile& e) {
            (void)e;
        }
    } else {
        throw ParserException("Expected ';', got instead: " + std::string(currentToken));
    }
}

std::unique_ptr<script::Expression> script::Parser::term() {
    auto node = factor();

    while(currentToken.type == Lexer::Symbol::OPERATOR && (currentToken.data.op == Operator::MULTIPLY || currentToken.data.op == Operator::DIVIDE)) {
        const auto token = currentToken;
        if (token.type == Lexer::Symbol::OPERATOR && token.data.op == Operator::MULTIPLY) {
            eat(Lexer::Symbol::OPERATOR);
        }
        else if (token.type == Lexer::Symbol::OPERATOR && token.data.op == Operator::DIVIDE) {
            eat(Lexer::Symbol::OPERATOR);
        }

        //std::cout << "term node = node" << std::endl;
        std::unique_ptr<Expression> newNode = std::make_unique<BinaryExpression>(token.data.op, std::move(node), factor());
        std::swap(newNode, node);
    }

    //std::cout << "currentToken: " << currentToken.type << " str: " << std::string(currentToken.str, currentToken.len) << std::endl;
    while (currentToken.type == Lexer::Symbol::OPERATOR && (currentToken.data.op == Operator::ASSIGNMENT)) {
        const auto token = currentToken;
        if (token.type == Lexer::Symbol::OPERATOR && token.data.op == Operator::ASSIGNMENT) {
            eat(Lexer::Symbol::OPERATOR);
        }
        //std::cout << "term node = node #2" << std::endl;
        std::unique_ptr<Expression> newNode = std::make_unique<BinaryExpression>(token.data.op, std::move(node), expr());
        std::swap(newNode, node);
        expectEndOfStatement();
    }

    return node;
}

std::unique_ptr<script::Expression> script::Parser::expr() {
    auto node = term();

    while (currentToken.type == Lexer::Symbol::OPERATOR && (currentToken.data.op == Operator::ADD || currentToken.data.op == Operator::SUBSTRACT)) {
        const auto token = currentToken;
        if (token.type == Lexer::Symbol::OPERATOR && token.data.op == Operator::ADD) {
            eat(Lexer::Symbol::OPERATOR);
        }
        else if (token.type == Lexer::Symbol::OPERATOR && token.data.op == Operator::SUBSTRACT) {
            eat(Lexer::Symbol::OPERATOR);
        }

        //std::cout << "expr node = node " << std::string(currentToken) << std::endl;
        std::unique_ptr<Expression> newNode = std::make_unique<BinaryExpression>(token.data.op, std::move(node), term());
        std::swap(newNode, node);
    }

    return node;
}

std::unique_ptr<script::Expression> script::Parser::getExpression() {
    return expr();
}
*/