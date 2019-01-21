#ifndef SCRIPT_TEST_TOKENIZER_H
#define SCRIPT_TEST_TOKENIZER_H
#include <memory>
#include "config.h"

namespace script {
    class Tokenizer {
    public:
        class Token {
        public:
            const char* str = nullptr;
            const unsigned long len = 0UL;

            operator bool() const {
                return str != nullptr;
            }
        };

        Tokenizer(const char* start, const char* end);
        virtual ~Tokenizer() = default;

        Token getToken();

        const char* getCurrentStrPos() const {
            return start;
        }

        bool eof() const {
            return start >= end;
        }

        const char* getCurrentLineStart() const {
            return lastLine;
        }
    private:
        const char* start;
        const char* end;
        const char* lastLine;
    };
}

#endif