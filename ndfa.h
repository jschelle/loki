//
// File:   ndfa.h
// Author: joris
//
// Created on December 5, 2012, 1:22 PM
//

#ifndef _NDFA_H
#define	_NDFA_H

#include <map>
#include <set>
#include <string>

#include "dfa.h"

class ndfa
{
    friend class ndfa_concat;
    friend class ndfa_union;
    friend class ndfa_star;
public:

    ndfa();
    ndfa(ndfa* n);

    bool add_state(std::string s);
    bool rem_state(std::string s);
    bool has_state(std::string s);

    bool set_accepting(std::string s, bool aFlag);
    bool is_accepting(std::string s);
    bool set_start(std::string s);
    std::string start();

    bool add_transition(std::string from, std::string to, char c);
    bool add_epsilon_transition(std::string from, std::string to);
    bool rem_transition(std::string from, std::string to, char c);
    bool rem_epsilon_transition(std::string from, std::string to);

    bool has_transition(std::string from, std::string to);
    bool has_transition(std::string from, std::string to, char c);
    bool has_epsilon_transition(std::string from, std::string to);
    bool has_transition_from(std::string from, char c);
    bool has_epsilon_transition_from(std::string from);
    bool has_transition_to(std::string to, char c);
    bool has_epsilon_transition_to(std::string to);

    std::set<char> alphabet();
    std::set<std::string> states();

    bool accepts(std::string s);
    bool accepts(std::string s, int offset);
    int rightmost_accept(std::string s);
    int rightmost_accept(std::string s, int offset);
    int leftmost_accept(std::string s);
    int leftmost_accept(std::string s, int offset);

    dfa* convert_to_dfa();

    virtual ~ndfa();

private:
    // methods
    std::set<std::string> yields(std::set<std::string> states, char c);
    std::set<std::string> epsilon_closure(std::set<std::string> states);
    std::string name_multistate(std::set<std::string> multistate);
    std::string name_by_number(int i);
    bool contains_accepting(std::set<std::string> multistate);

protected:
    // fields
    std::map<std::string, std::map<int, std::set<std::string>* >* > m_transitions;
    std::set<std::string> m_accepting;
    std::string m_start;
};

class ndfa_concat: public ndfa
{
public:
    ndfa_concat(ndfa* n0, ndfa* n1);
private:
    std::string name_by_number(int i);
};

/**
 * NFA symbolizing the logical 'OR' of two other NFA
 * formally:    NDFA_OR is constructed given two NFA's,
 *              nfa0 and nfa1 respectively
 * NDFA_OR will accept a string s iff. (nfa0 accepts s) OR (nfa1 accepts s)
 */
class ndfa_union: public ndfa
{
public:
    ndfa_union(ndfa* n0, ndfa* n1);
private:
    std::string name_by_number(int i);
};

/**
 *
 * NFA symbolizing the kleene star of another given NFA
 * formally:    NFA_STAR is constructed given 1 argument
 *              nfa0
 * NDFA_STAR will accept a string s iff. s is a zero or more repeat of a string s0, and s0 is accepted by nfa0
 */
class ndfa_star: public ndfa
{
public:
    ndfa_star(ndfa* n0);
private:
    std::string name_by_number(int i);
};

class ndfa_string: public ndfa
{
public:
    ndfa_string(std::string s);
private:
    std::string name_by_number(int i);
};

class ndfa_char: public ndfa
{
public:
    ndfa_char(char c);
};
#endif	/* _NDFA_H */

