/*
 * File:   lexer.h
 * Author: joris
 *
 * Created on June 16, 2012, 4:02 AM
 */

#ifndef LEXER_H
#define	LEXER_H

#include "regex.h"
#include <string>
#include <vector>

struct lexeme_t
{
    std::string m_text;
    int m_start;
    int m_end;
    int m_index;
    int m_tag;
};
typedef struct lexeme_t lexeme;

class lexer
{
public:
    lexer();

    // rules
    bool add_rule(std::string regex, int tag);

    // split to lexemes
    lexeme next_lexeme(std::string s, int p);
    std::vector<lexeme> to_lexemes(std::string s);

    virtual ~lexer();
private:
    std::map<int, pattern*> m_regexes;
};

#endif	/* LEXER_H */

