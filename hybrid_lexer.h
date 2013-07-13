#ifndef HYBRID_LEXER_H
#define HYBRID_LEXER_H

#include "functor.h"
#include "lexer.h"
#include <string>
#include <vector>

class hybrid_lexer
{
public:

    // numerical types
    static const int TYPE_INTEGER = 2;
    static const int TYPE_DOUBLE = 3;

    // meta types
    static const int TYPE_LEFT_ROUND_BRACKET = 7;
    static const int TYPE_RIGHT_ROUND_BRACKET = 11;
    static const int TYPE_LEFT_STRAIGHT_BRACKET = 13;
    static const int TYPE_RIGHT_STRAIGHT_BRACKET = 17;
    static const int TYPE_LEFT_CURLY_BRACKET = 19;
    static const int TYPE_RIGHT_CURLY_BRACKET = 23;

    static const int TYPE_VARIABLE = 29;
    static const int TYPE_FUNCTION_ARGUMENT_SEPARATOR = 31;
    static const int TYPE_SPACING = 37;
    static const int TYPE_OPERATOR = 41;
    static const int TYPE_STRING = 43;

    // constructor
    hybrid_lexer();

    // destructor
    virtual ~hybrid_lexer();

    // methods
    std::vector<lexeme> tokenize(std::string s, std::vector<functor*>* f);

private:
    // fields
    lexer* m_lex;

    // methods
    lexeme pick_longest(std::string s, int p, std::vector<std::string>* f);

    lexeme lex_by_regex(std::string s, int p);
    void make_lexer();

    lexeme lex_by_list(std::string s, int p, std::vector<std::string>* f);
    bool starts_with(std::string s, std::string prefix, int p);
};

#endif
