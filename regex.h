//
// File:   regex.h
// Author: joris
//
// Created on December 5, 2012, 1:22 PM
//

#ifndef _REGEX_H
#define	_REGEX_H

#include <vector>
#include "ndfa.h"

class pattern
{
public:

    static pattern* compile(std::string s);

    bool matches(std::string s);
    int leftmost_match(std::string s);
    int leftmost_match(std::string s, int offset);
    int rightmost_match(std::string s);
    int rightmost_match(std::string s, int offset);

    ~pattern();

private:

    // methods
    bool match_brackets();
    void process_escaped();
    void pre_processing();
    void to_postfix();
    void build();

    bool is_operator(std::string s);
    bool is_literal(std::string s);
    int precedence(std::string s);

    //  fields
    std::string m_regex;
    std::vector<bool> m_escaped;
    std::vector<std::string> m_chunks;
    ndfa* m_nfa;
    dfa* m_dfa;

};
#endif	/* _REGEX_H */

