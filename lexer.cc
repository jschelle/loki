/*
 * File:   lexer.cpp
 * Author: joris
 *
 * Created on June 16, 2012, 4:02 AM
 */

#include "lexer.h"

lexer::lexer()
{
}
lexer::~lexer()
{
    std::map<int,pattern*>::iterator it0;
    for(it0=m_regexes.begin(); it0!=m_regexes.end(); it0++)
        delete it0->second;
    m_regexes.clear();
}

bool lexer::add_rule(std::string regexp, int tag)
{
    if(m_regexes.find(tag)!=m_regexes.end())
        return false;
    m_regexes.insert(std::make_pair<int,pattern*>(tag,pattern::compile(regexp)));
    return true;
}
lexeme lexer::next_lexeme(std::string s, int p)
{
// keep track of longest match
    int l_len = 0;
    int l_tag = -1;
    // go over candidate regexes
    std::map<int,pattern*>::iterator it0;
    for(it0=m_regexes.begin(); it0!=m_regexes.end(); it0++)
    {
        int len = it0->second->rightmost_match(s,p) - p +1;
        if(len>l_len)
        {
            l_len = len;
            l_tag = it0->first;
        }
    }
    // construct lexeme
    lexeme l;
    l.m_start = p;
    l.m_end = p+l_len;
    l.m_tag = l_tag;
    l.m_text = s.substr(p,l_len);
    return l;
}
std::vector<lexeme> lexer::to_lexemes(std::string s)
{
    // keep track of lexemes
    std::vector<lexeme> retval;
    // go over string
    int pos = 0;
    while(pos < s.size())
    {
        lexeme l = next_lexeme(s,pos);
        l.m_index = retval.size();
        retval.push_back(l);
        // continue next loop at last match
        pos = l.m_end;
    }
    return retval;
}

