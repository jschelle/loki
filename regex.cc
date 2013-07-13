#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stack>

#include "regex.h"
#define DEBUG_REGEX_CPP 0

pattern* pattern::compile(std::string s)
{

    pattern* ret = new pattern();
    ret->m_regex = s;

    // process escaped characters
    ret->process_escaped();

    // test whether brackets  match
    if(!ret->match_brackets())
    {
        delete ret;
        return 0;
    }

    // convert to postfix
    ret->pre_processing();
    ret->to_postfix();

    // build to ndfa
    ret->build();

    return ret;
}
void pattern::process_escaped()
{

    for(int i=0; i<m_regex.length(); i++)
    {
        char c = m_regex[i];
        if(c=='\\')
        {
            m_escaped.push_back(true);
            c = m_regex[i+1];
            m_chunks.push_back(std::string(1,c));
            i=i+2;
        }
        else
        {
            m_escaped.push_back(false);
            m_chunks.push_back(std::string(1,c));
        }
    }

}
bool pattern::match_brackets()
{

    std::stack<char> stk;

    for(int i=0; i<m_chunks.size(); i++)
    {
        std::string c = m_chunks[i];
        if(c.compare("(")==0 && !m_escaped[i])
        {
            stk.push('(');
        }
        else if(c.compare(")")==0 && !m_escaped[i])
        {
            if(stk.empty())
                return false;
            stk.pop();
        }
    }

    if(!stk.empty())
        return false;

    // default
    return true;
}
void pattern::pre_processing()
{

    // mark "_" as escaped
    for(int i=0; i<m_chunks.size(); i++)
    {
        if(m_chunks[i].compare("_")==0)
            m_escaped[i] = true;
    }

    // insert implicit concatenation
    // this can be done locally, by considering the current and previous chunk
    // 5 categories exist:
    //                          1.      '('
    //                          2.      ')'
    //                          3.      '*' or '+'
    //                          4.      '|'
    //                          5.      <literal>
    std::vector<std::string> m_chunks2;
    std::vector<bool> m_escaped2;
    for(int i=1; i<m_chunks.size(); i++)
    {
        bool concat = false;
        std::string curr = m_chunks[i];
        std::string prev = m_chunks[i-1];
        // ')' followed by '('
        if(prev.compare(")")==0 && m_escaped[i-1]==false && curr.compare("(")==0 && m_escaped[i]==false)
        {
            concat = true;
        }
        // ')' followed by <literal>
        if(prev.compare(")")==0 && m_escaped[i-1]==false && (is_literal(curr) || m_escaped[i]==true))
        {
            concat = true;
        }
        // '*' followed by '('
        if(prev.compare("*")==0 && m_escaped[i-1]==false && curr.compare("(")==0 && m_escaped[i]==false)
        {
            concat = true;
        }
        // '*' followed by <literal>
        if(prev.compare("*")==0 && m_escaped[i-1]==false && (is_literal(curr) || m_escaped[i]==true))
        {
            concat = true;
        }
        // '+' followed by '('
        if(prev.compare("+")==0 && m_escaped[i-1]==false && curr.compare("(")==0 && m_escaped[i]==false)
        {
            concat = true;
        }
        // '+' followed by <literal>
        if(prev.compare("+")==0 && m_escaped[i-1]==false && (is_literal(curr) || m_escaped[i]==true))
        {
            concat = true;
        }
        // literal followed by '('
        if((is_literal(prev) || m_escaped[i-1]==true) && curr.compare("(")==0 && m_escaped[i]==false)
        {
            concat = true;
        }
        // literal followed by literal
        if((is_literal(prev) || m_escaped[i-1]==true) && (is_literal(curr) || m_escaped[i]==true))
        {
            concat = true;
        }

        // copy
        m_chunks2.push_back(prev);
        m_escaped2.push_back(m_escaped[i-1]);
        // add concat if needed
        if(concat)
        {
            m_chunks2.push_back("_");
            m_escaped2.push_back(false);
        }
    }
    m_chunks2.push_back(m_chunks[m_chunks.size()-1]);
    m_escaped2.push_back(m_escaped[m_escaped.size()-1]);

    m_chunks.clear();
    m_escaped.clear();
    for(int i=0; i<m_chunks2.size(); i++)
    {
        m_chunks.push_back(m_chunks2[i]);
        m_escaped.push_back(m_escaped2[i]);
    }

}
void pattern::to_postfix()
{

    std::vector<std::string> m_seq;     // output sequence
    std::vector<bool> m_isl;            // is literal?
    std::vector<std::string> m_stk;     // stack

    for(int i=0; i<m_chunks.size(); i++)
    {
        std::string c = m_chunks[i];

        // operator
        if((is_operator(c)) && !m_escaped[i])
        {
            // determine precedence
            int p0 = precedence(c);
            // while there is an operator on top of the stack
            while(m_stk.size()>0 && is_operator(m_stk[m_stk.size()-1]))
            {
                // determine precedence of operator on top of stack
                int p1 = precedence(m_stk[m_stk.size()-1]);
                if(p1>p0)
                {
                    m_seq.push_back(m_stk[m_stk.size()-1]);
                    m_isl.push_back(false);
                    m_stk.pop_back();
                }
                else
                {
                    break;
                }
            }
            m_stk.push_back(c);
        }
        // left parenthesis
        else if(c.compare("(")==0 && !m_escaped[i])
        {
            m_stk.push_back(c);
        }
        // right parenthesis
        else if(c.compare(")")==0 && !m_escaped[i])
        {
            // until top of stack is left parenthesis, pop operators from stack, onto output
            while(m_stk[m_stk.size()-1].compare("(")!=0)
            {
                m_seq.push_back(m_stk[m_stk.size()-1]);
                m_isl.push_back(false);
                m_stk.pop_back();
            }
            // pop left parenthesis
            m_stk.pop_back();
        }
        // literal
        else
        {
            m_seq.push_back(c);
            m_isl.push_back(true);
        }
    }

    while(m_stk.size()>0)
    {
        m_seq.push_back(m_stk[m_stk.size()-1]);
        m_isl.push_back(false);
        m_stk.pop_back();
    }

    m_chunks.clear();
    m_escaped.clear();
    for(int i=0; i<m_seq.size(); i++)
    {
        m_chunks.push_back(m_seq[i]);
        m_escaped.push_back(m_isl[i]);
    }

}
bool pattern::is_operator(std::string s)
{
    return s.compare("+")==0 || s.compare("*")==0 || s.compare("|")==0 || s.compare("_")==0;
}
bool pattern::is_literal(std::string s)
{
    return !is_operator(s) && s.compare("(")!=0 && s.compare(")")!=0;
}
int pattern::precedence(std::string s)
{
    if(!is_operator(s))
    {
        return -1;
    }
    // kleene plus, kleene star
    else if(s.compare("+")==0 || s.compare("*")==0)
    {
        return 5;
    }
    // concatenation
    else if(s.compare("_")==0)
    {
        return 4;
    }
    // union
    else
    {
        return 3;
    }
}
void pattern::build()
{
    std::vector<ndfa*> stk;
    for(int i=0; i<m_chunks.size(); i++)
    {
        std::string c = m_chunks[i];
        if(is_operator(c) && !m_escaped[i])
        {
            // kleene star
            if(c.compare("*")==0)
            {
                ndfa* arg = stk[stk.size()-1];
                stk.pop_back();

                ndfa* n = new ndfa_star(arg);
                delete arg;
                stk.push_back(n);
            }
            // kleene plus
            else if(c.compare("+")==0)
            {

                ndfa* arg = stk[stk.size()-1];
                stk.pop_back();

                ndfa* part0 = new ndfa_star(arg);
                ndfa* n = new ndfa_concat(arg,part0);

                delete arg;
                delete part0;
                stk.push_back(n);
            }
            // concatenation
            else if(c.compare("_")==0)
            {

                ndfa* arg0 = stk[stk.size()-1];
                ndfa* arg1 = stk[stk.size()-2];
                stk.pop_back();
                stk.pop_back();

                ndfa* n = new ndfa_concat(arg1,arg0);
                delete arg0;
                delete arg1;
                stk.push_back(n);

            }
            // union
            else if(c.compare("|")==0)
            {

                ndfa* arg0 = stk[stk.size()-1];
                ndfa* arg1 = stk[stk.size()-2];
                stk.pop_back();
                stk.pop_back();

                ndfa* n = new ndfa_union(arg0,arg1);
                delete arg0;
                delete arg1;
                stk.push_back(n);
            }
        }
        else
        {
            // build nfa for literal
            ndfa* n = new ndfa_string(c);
            // push on stack
            stk.push_back(n);
        }
    }

    // parsing error
    if(stk.size()!=1)
    {

    }

    m_nfa = stk[0];
    m_dfa = m_nfa->convert_to_dfa();

}
bool pattern::matches(std::string s)
{
    return m_dfa->accepts(s);
}
int pattern::leftmost_match(std::string s)
{
    return m_dfa->leftmost_accept(s);
}
int pattern::leftmost_match(std::string s, int offset)
{
    return m_dfa->leftmost_accept(s, offset);
}
int pattern::rightmost_match(std::string s)
{
    return m_dfa->rightmost_accept(s);
}
int pattern::rightmost_match(std::string s, int offset)
{
    return m_dfa->rightmost_accept(s,offset);
}
pattern::~pattern()
{
    delete m_dfa;
    delete m_nfa;
    m_chunks.clear();
    m_escaped.clear();
}