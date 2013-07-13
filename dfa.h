/*
 * File:   dfa.h
 * Author: joris
 *
 */

#ifndef _DFA_H
#define	_DFA_H

#include <map>
#include <set>
#include <string>

class dfa
{
public:

    dfa();
    dfa(dfa* d);

    bool add_state(std::string s);
    bool rem_state(std::string s);
    bool has_state(std::string s);

    bool set_accepting(std::string s, bool aFlag);
    bool is_accepting(std::string s);
    bool set_start(std::string s);

    bool add_transition(std::string from, std::string to, char c);
    bool rem_transition(std::string from, std::string to, char c);

    bool has_transition(std::string from, std::string to);
    bool has_transition(std::string from, std::string to, char c);
    bool has_transition_from(std::string from, char c);
    bool has_transition_to(std::string to, char c);

    std::set<char> alphabet();
    std::set<std::string> states();

    bool accepts(std::string s);
    bool accepts(std::string s, int offset);
    int rightmost_accept(std::string s);
    int rightmost_accept(std::string s, int offset);
    int leftmost_accept(std::string s);
    int leftmost_accept(std::string s, int offset);

    virtual ~dfa();

private:
    // fields
    std::map<std::string,std::map<char, std::string>* > m_transitions;
    std::set<std::string> m_accepting;
    std::string m_start;
};

#endif	/* _DFA_H */

