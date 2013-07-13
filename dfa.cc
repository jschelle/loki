/*
 * File:   dfa.cc
 * Author: joris
 *
 */

#include "dfa.h"
#define DEBUG_DFA_CPP 0

dfa::dfa()
{
}
dfa::dfa(dfa* d)
{
    // copy m_start
    m_start = d->m_start;

    // copy m_accepting
    std::set<std::string>::iterator it0;
    for(it0=d->m_accepting.begin(); it0!=d->m_accepting.end(); it0++)
        m_accepting.insert(*it0);

    // copy m_transitions
    std::map<std::string, std::map<char, std::string>* >::iterator it1;
    for(it1=d->m_transitions.begin(); it1!=d->m_transitions.end(); it1++)
    {
        std::string from = it1->first;

        // copy map of transitions from 'from'
        std::map<char,std::string>* m = new std::map<char,std::string>();
        std::map<char,std::string>::iterator it2;
        for(it2=it1->second->begin(); it2!=it1->second->end(); it2++)
            m->insert(std::make_pair<char,std::string>(it2->first,it2->second));

        m_transitions.insert(std::make_pair<std::string, std::map<char,std::string>* >(from,m));
    }
}

bool dfa::add_state(std::string s)
{
    // check if state exists
    if(has_state(s))
        return false;
    // add state
    std::map<char,std::string>* m = new std::map<char,std::string>();
    m_transitions.insert(std::make_pair<std::string, std::map<char,std::string>* >(s,m));
    // return
    return true;
}
bool dfa::rem_state(std::string s)
{
    // check if state exists
    if(!has_state(s))
        return false;
    // remove transitions from state
    std::map<std::string, std::map<char, std::string>* >::iterator it0;
    it0 = m_transitions.find(s);
    it0->second->clear();
    // remove transitions to state
    for(it0=m_transitions.begin(); it0!=m_transitions.end(); it0++)
    {
        std::map<char,std::string>* m = it0->second;
        std::map<char,std::string>::iterator it1;
        for(it1=m->begin(); it1!=m->end(); it1++)
        {
            if(it1->second.compare(s)==0)
                m->erase(it1);
        }
    }
    // remove from accepting states
    m_accepting.erase(m_accepting.find(s));
    // remove as start state
    if(m_start.compare(s)==0)
        m_start = "";
    // return
    return true;
}

bool dfa::has_state(std::string s)
{
    return m_transitions.find(s)!=m_transitions.end();
}

bool dfa::set_accepting(std::string s, bool aFlag)
{
    // check state exists
    if(!has_state(s))
        return false;
    std::set<std::string>::iterator it = m_accepting.find(s);
    if(aFlag)
    {
        // if state should become accepting, and it is already accepting return false
        if(it!=m_accepting.end())
            return false;
        // insert state into set of accepting states
        m_accepting.insert(s);
        // return
        return true;
    }
    else
    {
        // if state should become non-accepting, and it already is non-accepting return false
        if(it==m_accepting.end())
            return false;
        // erase state from set of accepting states
        m_accepting.erase(it);
        // return
        return true;
    }
}
bool dfa::is_accepting(std::string s)
{
    return m_accepting.find(s)!=m_accepting.end();
}
bool dfa::set_start(std::string s)
{
    // check if state exists
    if(!has_state(s))
        return false;
    // check if 's' isn't the start already
    if(m_start.compare(s)==0)
        return false;
    // make 's' the start state
    m_start = s;
    return true;
}

bool dfa::add_transition(std::string from, std::string to, char c)
{
    // check states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // check for duplicate transition
    if(has_transition_from(from,c))
        return false;
    // insert transition
    m_transitions.find(from)->second->insert(std::make_pair<char,std::string>(c,to));
    return true;
}
bool dfa::rem_transition(std::string from, std::string to, char c)
{
    // check states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // check whether transition exists
    if(!has_transition(from,to,c))
        return false;
    std::map<char,std::string>* m = m_transitions.find(from)->second;
    std::map<char,std::string>::iterator it = m->find(c);
    m->erase(it);
    return true;
}

bool dfa::has_transition(std::string from, std::string to)
{
    // check states exist
    if(!has_state(from) || !has_state(to))
        return false;

    std::map<char,std::string>* m = m_transitions.find(from)->second;
    std::map<char, std::string>::iterator it;
    for(it=m->begin(); it!=m->end(); it++)
    {
        if(it->second.compare(to)==0)
            return true;
    }

    // default
    return false;
}
bool dfa::has_transition(std::string from, std::string to, char c)
{
    // check states exist
    if(!has_state(from) || !has_state(to))
        return false;

    std::map<char, std::string>* m = m_transitions.find(from)->second;
    std::map<char, std::string>::iterator it = m->find(c);
    if(it==m->end())
        return false;
    return it->second.compare(to)==0;
}
bool dfa::has_transition_from(std::string from, char c)
{
    // check state exists
    if(!has_state(from))
        return false;
    std::map<char,std::string>* m = m_transitions.find(from)->second;
    return m->find(c)!=m->end();
}
bool dfa::has_transition_to(std::string to, char c)
{
    // check state exists
    if(!has_state(to))
        return false;

    std::set<std::string> m_states = states();
    std::set<std::string>::iterator it;
    for(it=m_states.begin(); it!=m_states.end(); it++)
        if(has_transition(*it,to,c))
            return true;

    // default
    return false;
}

std::set<char> dfa::alphabet()
{
    std::set<char> m_alphabet;
    std::map<std::string,std::map<char,std::string>* >::iterator it0;
    for(it0=m_transitions.begin(); it0!=m_transitions.end(); it0++)
    {
        std::map<char,std::string>* m = it0->second;
        std::map<char,std::string>::iterator it1;
        for(it1=m->begin(); it1!=m->end(); it1++)
        {
            m_alphabet.insert(it1->first);
        }
    }
    return m_alphabet;
}
std::set<std::string> dfa::states()
{
    std::set<std::string> m_states;
    std::map<std::string, std::map<char,std::string>* >::iterator it0;
    for(it0=m_transitions.begin(); it0!=m_transitions.end(); it0++)
    {
        m_states.insert(it0->first);
    }
    return m_states;
}
bool dfa::accepts(std::string s)
{
    return rightmost_accept(s)==s.length()-1;
}
bool dfa::accepts(std::string s, int offset)
{
    return rightmost_accept(s,offset)==s.length()-1;
}
int dfa::rightmost_accept(std::string s)
{
    return rightmost_accept(s,0);
}
int dfa::rightmost_accept(std::string s, int offset)
{
    std::string m_curr = m_start;
    int last_accept = offset - 1;
    for(int i=offset; i<s.length(); i++)
    {
        char c = s[i];
        // fetch all transitions from state m_curr
        std::map<char,std::string>* m = m_transitions.find(m_curr)->second;
        // check whether transition exists on character 'c'
        if(m->find(c)==m->end())
        {
            // if no transition exists, return m_last_accept
            return last_accept;
        }
        else
        {
            // if a transition exists, go to that state
            m_curr = m->find(c)->second;
            // check whether that state is an accepting state
            if(is_accepting(m_curr))
                last_accept = i;
        }
    }
    return last_accept;
}
int dfa::leftmost_accept(std::string s)
{

}
int dfa::leftmost_accept(std::string s, int offset)
{

}
dfa::~dfa()
{
    // clear accepting states
    m_accepting.clear();
    // clear start
    m_start = "";
    // clear transitions
    std::map<std::string, std::map<char,std::string>* >::iterator it;
    for(it=m_transitions.begin(); it!=m_transitions.end(); it++)
    {
        delete it->second;
    }
    m_transitions.clear();
}