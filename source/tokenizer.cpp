#include "../include/tokenizer.h"
#include <iostream>

enum CharType {
    NONE = 0,
    WHITESPACE,
    NEWLINE,
    SYMBOL,
    ALPHANUM
};

const char* charTypeStr(CharType type) {
    switch (type) {
        case CharType::NONE: return "NONE";
        case CharType::WHITESPACE: return "WHITESPACE";
        case CharType::NEWLINE: return "NEWLINE";
        case CharType::SYMBOL: return "SYMBOL";
        case CharType::ALPHANUM: return "ALPHANUM";
        default: return "UNKNOWN";
    }
}

script::Tokenizer::Tokenizer(const char* start, const char* end):start(start),end(end), lastLine(start) {
    
}

script::Tokenizer::Token script::Tokenizer::getToken() {
    auto current = start;
    auto type = CharType::NONE;
    auto firstType = CharType::NONE;
    const char* fistPtr = nullptr;

    while(current != end) {
        switch(*current) {
            case '\n':
                type = CharType::NEWLINE;
                lastLine = start + 1;
                break;
            case ' ': 
                type = CharType::WHITESPACE;
                break;
            case '(':
            case ')':
            case '{':
            case '}':
            case '[':
            case ']':
            case ',':
            case ';':
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '=':
                if (fistPtr == nullptr) {
                    firstType = CharType::SYMBOL;
                    fistPtr = current;
                }
                type = CharType::SYMBOL;
                break;
            default: 
                if (fistPtr == nullptr) {
                    firstType = CharType::ALPHANUM;
                    fistPtr = current;
                }
                type = CharType::ALPHANUM;
                break;
        }

        if (fistPtr != nullptr) {
            // We found a symbol, do not go further and return
            if(firstType == CharType::SYMBOL) {
                start = current + 1;
                // Check for symbols such as == or ++
                if (start != end && (*start == '=' || *start == '+' || *start == '-')) {
                    start = current + 2;
                    return Token{ fistPtr, unsigned long(current - fistPtr + 2) };
                }
                return Token{ fistPtr, unsigned long(current - fistPtr + 1) };
            }

            // We found a word
            if(firstType == CharType::ALPHANUM && type != CharType::ALPHANUM) {
                start = current;
                return Token{ fistPtr, unsigned long(current - fistPtr) };
            }
        }

        current++;

        if (current == end && fistPtr != nullptr) {
            if (firstType == CharType::ALPHANUM) {
                //std::cout << "return last token" << std::endl;
                start = current;
                return Token{ fistPtr, unsigned long(current - fistPtr) };
            }
        }
    }

    return Token{ nullptr, 0UL };
}
