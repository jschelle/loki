#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

#include "ndfa.h"
#include "regex.h"
#define DEBUG_NDFA_CPP 0

ndfa::ndfa()
{

}

bool ndfa::add_state(std::string s)
{
    // check if state exists
    if(has_state(s))
        return false;
    // add state
    std::map<int,std::set<std::string>* >* m = new std::map<int,std::set<std::string>* >();
    m_transitions.insert(std::make_pair<std::string, std::map<int,std::set<std::string>* >* >(s,m));
    // return
    return true;
}
bool ndfa::rem_state(std::string s)
{
    if(!has_state(s))
        return false;
    // remove transitions from s
    std::map<int, std::set<std::string>* >* m = m_transitions.find(s)->second;
    std::map<int, std::set<std::string>* >::iterator it0;
    for(it0=m->begin(); it0!=m->end(); it0++)
    {
        delete it0->second;
    }
    delete m_transitions.find(s)->second;
    m_transitions.erase(m_transitions.find(s));
    // remove transitions to s
    std::map<std::string, std::map<int, std::set<std::string>* >* >::iterator it1;
    for(it1=m_transitions.begin(); it1!=m_transitions.end(); it1++)
    {
        m = it1->second;
        for(it0=m->begin(); it0!=m->end(); it0++)
        {
            std::set<std::string>* dest = it0->second;
            dest->erase(dest->find(s));
        }
    }
    // remove from accepting transitions
    m_accepting.erase(m_accepting.find(s));
    // return
    return true;
}
bool ndfa::has_state(std::string s)
{
    return m_transitions.find(s)!=m_transitions.end();
}
bool ndfa::set_accepting(std::string s, bool aFlag)
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
bool ndfa::is_accepting(std::string s)
{
    return m_accepting.find(s)!=m_accepting.end();
}
bool ndfa::set_start(std::string s)
{
    if(m_start.compare(s)==0)
        return false;
    m_start = s;
    return true;
}
std::string ndfa::start()
{
    return m_start;
}

bool ndfa::add_transition(std::string from, std::string to, char c)
{
    // check whether states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // fetch std::map containing transitions from 'from'
    std::map<int, std::set<std::string>* >* m = m_transitions.find(from)->second;
    int ci = (int) c;
    if(m->find(ci)==m->end())
    {
        // if no mapping exist on 'c', create a new std::set to hold all future destinations
        std::set<std::string>* s = new std::set<std::string>();
        // insert 'to' in this set
        s->insert(to);
        // add mapping 'c' => 's'
        m->insert(std::make_pair<int,std::set<std::string>* >(ci,s));
        // return
        return true;
    }
    else
    {
        std::set<std::string>* s = m->find(ci)->second;
        if(s->find(to)!=s->end())
            return false;
        s->insert(to);
        return true;
    }
}
bool ndfa::add_epsilon_transition(std::string from, std::string to)
{
    // check whether states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // fetch std::map containing transitions from 'from'
    std::map<int, std::set<std::string>* >* m = m_transitions.find(from)->second;
    int ci = -1;
    if(m->find(ci)==m->end())
    {
        // if no mapping exist on 'c', create a new std::set to hold all future destinations
        std::set<std::string>* s = new std::set<std::string>();
        // insert 'to' in this set
        s->insert(to);
        // add mapping 'c' => 's'
        m->insert(std::make_pair<int,std::set<std::string>* >(ci,s));
        // return
        return true;
    }
    else
    {
        std::set<std::string>* s = m->find(ci)->second;
        if(s->find(to)!=s->end())
            return false;
        s->insert(to);
        return true;
    }
}
bool ndfa::rem_transition(std::string from, std::string to, char c)
{
    // check whether states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // TODO
    std::map<int,std::set<std::string>*> * m = m_transitions.find(from)->second;
    if(m->find(c)==m->end())
        return false;
}
bool ndfa::rem_epsilon_transition(std::string from, std::string to)
{
    // check whether states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // TODO
}

bool ndfa::has_transition(std::string from, std::string to)
{
    // check whether states exist
    if(!has_state(from) || !has_state(to))
        return false;
    std::map<int, std::set<std::string>* >* m = m_transitions.find(from)->second;
    std::map<int, std::set<std::string>* >::iterator it0;
    for(it0=m->begin(); it0!=m->end(); it0++)
    {
        std::set<std::string>* dest = it0->second;
        if(dest->find(to)!=dest->end())
            return true;
    }
    // default
    return false;
}
bool ndfa::has_transition(std::string from, std::string to, char c)
{
    // check whether states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // fetch std::map holding all transitions starting at 'from'
    std::map<int, std::set<std::string>* >* m = m_transitions.find(from)->second;
    std::map<int, std::set<std::string>* >::iterator it0 = m->find((int) c);
    if(it0==m->end())
        return false;
    std::set<std::string>* dest = it0->second;
    return dest->find(to)!=dest->end();
}
bool ndfa::has_epsilon_transition(std::string from, std::string to)
{
    // check whether states exist
    if(!has_state(from) || !has_state(to))
        return false;
    // fetch std::map holding all transitions starting at 'from'
    std::map<int, std::set<std::string>* >* m = m_transitions.find(from)->second;
    std::map<int, std::set<std::string>* >::iterator it0 = m->find(-1);
    if(it0==m->end())
        return false;
    std::set<std::string>* dest = it0->second;
    return dest->find(to)!=dest->end();
}
bool ndfa::has_transition_from(std::string from, char c)
{
    // check whether state exists
    if(!has_state(from))
        return false;
    // fetch std::map holding all transitions starting at 'from'
    std::map<int, std::set<std::string>* >* m = m_transitions.find(from)->second;
    std::map<int, std::set<std::string>* >::iterator it0 = m->find((int)c);
    if(it0==m->end())
        return false;
    std::set<std::string>* dest = it0->second;
    return dest->size()>0;
}
bool ndfa::has_epsilon_transition_from(std::string from)
{
    // check whether state exists
    if(!has_state(from))
        return false;
    // fetch std::map holding all transitions starting at 'from'
    std::map<int, std::set<std::string>* >* m = m_transitions.find(from)->second;
    std::map<int, std::set<std::string>* >::iterator it0 = m->find(-1);
    if(it0==m->end())
        return false;
    std::set<std::string>* dest = it0->second;
    return dest->size()>0;
}
bool ndfa::has_transition_to(std::string to, char c)
{
    // check whether state exists
    if(!has_state(to))
        return false;
    std::set<std::string> m_states = states();
    std::set<std::string>::iterator it;
    for(it=m_states.begin(); it!=m_states.end(); it++)
    {
        if(has_transition(*it,to,c))
            return true;
    }
    // default
    return false;
}
bool ndfa::has_epsilon_transition_to(std::string to)
{
    // check whether state exists
    if(!has_state(to))
        return false;
    std::set<std::string> m_states = states();
    std::set<std::string>::iterator it;
    for(it=m_states.begin(); it!=m_states.end(); it++)
    {
        if(has_epsilon_transition(*it,to))
            return true;
    }
    // default
    return false;
}

std::set<char> ndfa::alphabet()
{
    std::set<char> m_alpha;
    std::map<std::string, std::map<int, std::set<std::string>* >* >::iterator it0;
    for(it0=m_transitions.begin(); it0!=m_transitions.end(); it0++)
    {
        std::map<int, std::set<std::string>* >* m = it0->second;
        std::map<int, std::set<std::string>* >::iterator it1;
        for(it1=m->begin(); it1!=m->end(); it1++)
        {
            std::set<std::string>* s = it1->second;
            if(s->size()>0 && it1->first>0)
                m_alpha.insert((char)it1->first);
        }
    }
    return m_alpha;
}
std::set<std::string> ndfa::states()
{
    std::set<std::string> m_states;
    std::map<std::string, std::map<int, std::set<std::string>* >* >::iterator it;
    for(it=m_transitions.begin(); it!=m_transitions.end(); it++)
    {
        m_states.insert(it->first);
    }
    return m_states;
}
dfa* ndfa::convert_to_dfa()
{

    if(DEBUG_NDFA_CPP)
        std::cout << "<TO_DFA>" << std::endl;

    // determine alphabet
    std::set<char> alpha = alphabet();

    // keep track of states already processed
    std::set<std::string> processed;

    // target dfa
    dfa* ret = new dfa();

    // mapping of multistates to new name
    std::map<std::string,std::string> name_mapping;

    // determine epsilon closure of start
    std::set<std::string> curr_states;
    curr_states.insert(m_start);
    curr_states = epsilon_closure(curr_states);

    name_mapping.insert(std::make_pair<std::string,std::string>(name_multistate(curr_states),name_by_number(0)));
    ret->add_state(name_by_number(0));
    ret->set_accepting(name_by_number(0),contains_accepting(curr_states));
    ret->set_start(name_by_number(0));
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state [s0], previously " << name_multistate(curr_states) << std::endl;
        std::cout << "marking [s0] as start" << std::endl;
        if(contains_accepting(curr_states))
            std::cout << "marking [s0] as accepting" << std::endl;
    }

    // push multistate on stack
    std::vector<std::set<std::string> > m_stack;
    m_stack.push_back(curr_states);

    // until no more states need to be visited
    while(m_stack.size()>0)
    {

        // pop multistate from stack
        std::set<std::string> source = m_stack[m_stack.size()-1];
        std::string source_canonical = name_mapping.find(name_multistate(source))->second;
        m_stack.pop_back();

        // skips states that have already been visited
        if(processed.find(name_multistate(source))!=processed.end())
            continue;

        // determine yield for each character of alphabet
        std::set<char>::iterator alpha_it;
        for(alpha_it=alpha.begin(); alpha_it!=alpha.end(); alpha_it++)
        {
            std::set<std::string> dest = epsilon_closure(yields(source,*alpha_it));
            std::string dest_name = name_multistate(dest);

            // if destination wasn't processed yet
            if(processed.find(dest_name)==processed.end())
            {
                // check if we've encountered destination multistate before
                std::string dest_canonical;
                if(name_mapping.find(dest_name)==name_mapping.end())
                {
                    // create new name for multistate
                    dest_canonical = name_by_number(name_mapping.size());
                    // insert name into name_mapping
                    name_mapping.insert(std::make_pair<std::string,std::string>(dest_name,dest_canonical));
                    // add new state to dfa
                    ret->add_state(dest_canonical);
                    ret->set_accepting(dest_canonical,contains_accepting(dest));
                    // add it to stack
                    m_stack.push_back(dest);

                }
                else
                {
                    // lookup canonical name
                    dest_canonical = name_mapping.find(dest_name)->second;
                }
                // add mapping to dfa
                ret->add_transition(source_canonical,dest_canonical,*alpha_it);
                if(DEBUG_NDFA_CPP)
                {
                    std::cout << "adding transition from " << source_canonical << " to " << dest_canonical << " on '" << *alpha_it << "'" << std::endl;
                    if(ret->is_accepting(dest_canonical))
                        std::cout << "marking " << dest_canonical << " as accepting" << std::endl;
                }
            }
            else
            {
                // find canonical name of destination
                std::string dest_canonical = name_mapping.find(dest_name)->second;
                // add transition
                ret->add_transition(source_canonical,dest_canonical,*alpha_it);
                if(DEBUG_NDFA_CPP)
                    std::cout << "adding transition from " << source_canonical << " to " << dest_canonical << " on '" << *alpha_it << "'" << std::endl;
            }
        }

        // add multistate to list of processed states
        processed.insert(name_multistate(source));

    }

    if(DEBUG_NDFA_CPP)
        std::cout << "</TO_DFA>" << std::endl;

    // return
    return ret;

}
std::set<std::string> ndfa::yields(std::set<std::string> states, char c)
{
    std::set<std::string> m_yield;
    std::set<std::string>::iterator it;
    for(it=states.begin(); it!=states.end(); it++)
    {
        std::map<int, std::set<std::string>* >* m = m_transitions.find(*it)->second;
        if(m->find((int)c)!=m->end())
        {
            std::set<std::string>* s = m->find(c)->second;
            std::set<std::string>::iterator it1;
            for(it1=s->begin(); it1!=s->end(); it1++)
                m_yield.insert(*it1);
        }
    }
    return m_yield;
}
std::set<std::string> ndfa::epsilon_closure(std::set<std::string> states)
{
    std::set<std::string> m_states;
    std::set<std::string>::iterator it;

    std::stack<std::string> to_inspect;
    for(it=states.begin(); it!=states.end(); it++)
        to_inspect.push(*it);

    while(to_inspect.size()>0)
    {

        std::string state = to_inspect.top();
        to_inspect.pop();

        if(has_state(state))
        {
            std::map<int, std::set<std::string>* >* m = m_transitions.find(state)->second;
            if(m->find((int)-1)!=m->end())
            {
                std::set<std::string>* s = m->find(-1)->second;
                std::set<std::string>::iterator it1;
                for(it1=s->begin(); it1!=s->end(); it1++)
                {
                    to_inspect.push(*it1);
                }
            }
        }

        m_states.insert(state);

    }

    return m_states;
}
std::string ndfa::name_multistate(std::set<std::string> multistate)
{
    std::vector<std::string> m_states;
    std::set<std::string>::iterator it;
    for(it=multistate.begin(); it!=multistate.end(); it++)
        m_states.push_back(*it);
    std::sort(m_states.begin(),m_states.end());
    std::string s = "[";
    for(int i=0; i<m_states.size(); i++)
    {
        if(i==m_states.size()-1)
            s+=m_states[i];
        else
            s+=m_states[i]+",";
    }
    return s+"]";
}
std::string ndfa::name_by_number(int i)
{
    std::stringstream ss;
    ss << i;
    return "[s"+ss.str()+"]";
}
bool ndfa::contains_accepting(std::set<std::string> multistate)
{

    std::set<std::string>::iterator it;
    for(it=multistate.begin(); it!=multistate.end(); it++)
        if(is_accepting(*it))
            return true;

    // default
    return false;
}
bool ndfa::accepts(std::string s)
{
    return rightmost_accept(s) == s.length()-1;
}
bool ndfa::accepts(std::string s, int offset)
{
    return rightmost_accept(s,offset)==s.length()-1;
}
int ndfa::leftmost_accept(std::string s)
{

}
int ndfa::rightmost_accept(std::string s)
{
    return rightmost_accept(s,0);
}
int ndfa::rightmost_accept(std::string s, int offset)
{

    int last_accept = offset - 1;

    std::set<std::string> m_curr;
    m_curr.insert(m_start);
    m_curr = epsilon_closure(m_curr);

    for(int i=offset; i<s.length(); i++)
    {
        char c = s[i];

        if(contains_accepting(m_curr))
            last_accept = i;

        m_curr = epsilon_closure(yields(m_curr,c));

        if(m_curr.size()==0)
            break;

    }

    return last_accept;
}
ndfa::~ndfa()
{

    // delete m_start
    m_start = "";

    // delete m_accepting
    m_accepting.clear();

    // delete m_transitions
    std::map<std::string, std::map<int, std::set<std::string>* >* >::iterator it0;
    for(it0=m_transitions.begin(); it0!=m_transitions.end(); it0++)
    {

        std::map<int, std::set<std::string>* >::iterator it1;
        for(it1=it0->second->begin(); it1!=it0->second->end(); it1++)
        {
            it1->second->clear();
            delete it1->second;
        }

        it0->second->clear();
        delete it0->second;
    }

    m_transitions.clear();
}

ndfa_concat::ndfa_concat(ndfa* n0, ndfa* n1)
{

    if(DEBUG_NDFA_CPP)
        std::cout << "<NDFA_CONCAT>" << std::endl;

    // add start
    std::map<std::string,std::string> state_renamer;
    state_renamer.insert(std::make_pair<std::string,std::string>("__start__","[s0]"));
    add_state("[s0]");
    set_start("[s0]");
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state [s0]" << std::endl;
        std::cout << "marking state [s0] as start" << std::endl;
    }

    // copy states from n0
    std::set<std::string> st0 = n0->states();
    std::set<std::string>::iterator it0;
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        std::string name = name_by_number(state_renamer.size());
        state_renamer.insert(std::make_pair<std::string,std::string>(*it0+"_0",name));
        add_state(name);
        if(DEBUG_NDFA_CPP)
        {
            std::cout << "adding state " << name << std::endl;
        }
    }

    // add transition from start to start of n0
    add_epsilon_transition("[s0]",state_renamer.find(n0->start()+"_0")->second);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from [s0] to " << state_renamer.find(n0->start()+"_0")->second << " on epsilon" << std::endl;
    }

    // add transitions from n0
    std::map<std::string, std::map<int,std::set<std::string>* >* >::iterator it1;
    for(it1=n0->m_transitions.begin(); it1!=n0->m_transitions.end(); it1++)
    {
        std::string from = it1->first;
        std::string from_can = state_renamer.find(from+"_0")->second;
        std::map<int,std::set<std::string>*>::iterator it2;
        for(it2=it1->second->begin(); it2!=it1->second->end(); it2++)
        {
            int ch = it2->first;
            std::set<std::string>::iterator it3;
            for(it3=it2->second->begin(); it3!=it2->second->end(); it3++)
            {
                std::string to = *it3;
                std::string to_can = state_renamer.find(to+"_0")->second;
                if(ch!=-1)
                {
                    add_transition(from_can,to_can,(char)ch);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on '" << (char)ch << "'" << std::endl;
                    }
                }
                else
                {
                    add_epsilon_transition(from_can,to_can);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on epsilon" << std::endl;
                    }
                }
            }
        }
    }

    // create join state
    std::string join = name_by_number(state_renamer.size());
    state_renamer.insert(std::make_pair<std::string,std::string>("__join__",join));
    add_state(join);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state " << join << std::endl;
    }

    // add transition from accepting states of n0 to end state
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        if(n0->is_accepting(*it0))
        {
            add_epsilon_transition(state_renamer.find(*it0 + "_0")->second,join);
            if(DEBUG_NDFA_CPP)
                std::cout << "adding transition from " << state_renamer.find(*it0 + "_0")->second << " to " << join << " on epsilon" << std::endl;
        }
    }

    // add states from n1
    st0 = n1->states();
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        std::string name = name_by_number(state_renamer.size());
        state_renamer.insert(std::make_pair<std::string,std::string>(*it0+"_1",name));
        add_state(name);
        if(DEBUG_NDFA_CPP)
            std::cout << "adding state " << name << std::endl;
    }

    // add transitions from n1
    for(it1=n1->m_transitions.begin(); it1!=n1->m_transitions.end(); it1++)
    {
        std::string from = it1->first;
        std::string from_can = state_renamer.find(from+"_1")->second;
        std::map<int,std::set<std::string>*>::iterator it2;
        for(it2=it1->second->begin(); it2!=it1->second->end(); it2++)
        {
            int ch = it2->first;
            std::set<std::string>::iterator it3;
            for(it3=it2->second->begin(); it3!=it2->second->end(); it3++)
            {
                std::string to = *it3;
                std::string to_can = state_renamer.find(to+"_1")->second;
                if(ch!=-1)
                {
                    add_transition(from_can,to_can,(char)ch);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on '" << (char)ch << "'" << std::endl;
                    }
                }
                else
                {
                    add_epsilon_transition(from_can,to_can);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on epsilon" << std::endl;
                    }
                }
            }
        }
    }

    // add transition from join to start of n1
    add_epsilon_transition(join,state_renamer.find(n1->start()+"_1")->second);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from " << join << " to " << state_renamer.find(n1->start()+"_1")->second << " on epsilon" << std::endl;
    }

    // create end state
    std::string end = name_by_number(state_renamer.size());
    state_renamer.insert(std::make_pair<std::string,std::string>("__end__",end));
    add_state(end);
    set_accepting(end,true);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state " << end << std::endl;
        std::cout << "marking state " << end << " as accepting" << std::endl;
    }

    // add transition from accepting states of n0 to end state
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        if(n1->is_accepting(*it0))
        {
            add_epsilon_transition(state_renamer.find(*it0 + "_1")->second,end);
            if(DEBUG_NDFA_CPP)
                std::cout << "adding transition from " << state_renamer.find(*it0 + "_1")->second << " to " << end << " on epsilon" << std::endl;
        }
    }

}
std::string ndfa_concat::name_by_number(int i)
{
    std::stringstream ss;
    ss << i;
    return "[s"+ss.str()+"]";
}

ndfa_union::ndfa_union(ndfa* n0, ndfa* n1)
{

    if(DEBUG_NDFA_CPP)
        std::cout << "<NDFA_UNION>" << std::endl;

    // add start
    std::map<std::string,std::string> state_renamer;
    state_renamer.insert(std::make_pair<std::string,std::string>("__start__","[s0]"));
    add_state("[s0]");
    set_start("[s0]");
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state [s0]" << std::endl;
        std::cout << "marking state [s0] as start" << std::endl;
    }

    // copy states from n0
    std::set<std::string> st0 = n0->states();
    std::set<std::string>::iterator it0;
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        std::string name = name_by_number(state_renamer.size());
        state_renamer.insert(std::make_pair<std::string,std::string>(*it0+"_0",name));
        add_state(name);
        if(DEBUG_NDFA_CPP)
        {
            std::cout << "adding state " << name << std::endl;
        }
    }

    // add transition from start to start of n0
    add_epsilon_transition("[s0]",state_renamer.find(n0->start()+"_0")->second);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from [s0] to " << state_renamer.find(n0->start()+"_0")->second << " on epsilon" << std::endl;
    }

    // add transitions from n0
    std::map<std::string, std::map<int,std::set<std::string>* >* >::iterator it1;
    for(it1=n0->m_transitions.begin(); it1!=n0->m_transitions.end(); it1++)
    {
        std::string from = it1->first;
        std::string from_can = state_renamer.find(from+"_0")->second;
        std::map<int,std::set<std::string>*>::iterator it2;
        for(it2=it1->second->begin(); it2!=it1->second->end(); it2++)
        {
            int ch = it2->first;
            std::set<std::string>::iterator it3;
            for(it3=it2->second->begin(); it3!=it2->second->end(); it3++)
            {
                std::string to = *it3;
                std::string to_can = state_renamer.find(to+"_0")->second;
                if(ch!=-1)
                {
                    add_transition(from_can,to_can,(char)ch);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on '" << (char)ch << "'" << std::endl;
                    }
                }
                else
                {
                    add_epsilon_transition(from_can,to_can);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on epsilon" << std::endl;
                    }
                }
            }
        }
    }

    // create end state
    std::string end = name_by_number(state_renamer.size());
    state_renamer.insert(std::make_pair<std::string,std::string>("__end__",end));
    add_state(end);
    set_accepting(end,true);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state " << end << std::endl;
        std::cout << "marking state " << end << " as accepting" << std::endl;
    }

    // add transition from accepting states of n0 to end state
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        if(n0->is_accepting(*it0))
        {
            add_epsilon_transition(state_renamer.find(*it0 + "_0")->second,end);
            if(DEBUG_NDFA_CPP)
                std::cout << "adding transition from " << state_renamer.find(*it0 + "_0")->second << " to " << end << " on epsilon" << std::endl;
        }
    }

    // add states from n1
    st0 = n1->states();
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        std::string name = name_by_number(state_renamer.size());
        state_renamer.insert(std::make_pair<std::string,std::string>(*it0+"_1",name));
        add_state(name);
        if(DEBUG_NDFA_CPP)
            std::cout << "adding state " << name << std::endl;
    }

    // add transition from start to start of n1
    add_epsilon_transition("[s0]",state_renamer.find(n1->start()+"_1")->second);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from [s0] to " << state_renamer.find(n1->start()+"_1")->second << " on epsilon" << std::endl;
    }

    // add transitions from n1
    for(it1=n1->m_transitions.begin(); it1!=n1->m_transitions.end(); it1++)
    {
        std::string from = it1->first;
        std::string from_can = state_renamer.find(from+"_1")->second;
        std::map<int,std::set<std::string>*>::iterator it2;
        for(it2=it1->second->begin(); it2!=it1->second->end(); it2++)
        {
            int ch = it2->first;
            std::set<std::string>::iterator it3;
            for(it3=it2->second->begin(); it3!=it2->second->end(); it3++)
            {
                std::string to = *it3;
                std::string to_can = state_renamer.find(to+"_1")->second;
                if(ch!=-1)
                {
                    add_transition(from_can,to_can,(char)ch);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on '" << (char)ch << "'" << std::endl;
                    }
                }
                else
                {
                    add_epsilon_transition(from_can,to_can);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on epsilon" << std::endl;
                    }
                }
            }
        }
    }

    // add transition from accepting states of n1 to end state
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        if(n1->is_accepting(*it0))
        {
            add_epsilon_transition(state_renamer.find(*it0 + "_1")->second,end);
            if(DEBUG_NDFA_CPP)
                std::cout << "adding transition from " << state_renamer.find(*it0 + "_1")->second << " to " << end << " on epsilon" << std::endl;
        }
    }

    if(DEBUG_NDFA_CPP)
        std::cout << "</NDFA_UNION>" << std::endl;
}
std::string ndfa_union::name_by_number(int i)
{
    std::stringstream ss;
    ss << i;
    return "[s"+ss.str()+"]";
}

ndfa_star::ndfa_star(ndfa* n0)
{

    if(DEBUG_NDFA_CPP)
        std::cout << "<NDFA_STAR>" << std::endl;

    std::map<std::string,std::string> state_renamer;

    // add starting state
    state_renamer.insert(std::make_pair<std::string,std::string>("__start__","[s0]"));
    add_state("[s0]");
    set_start("[s0]");
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state [s0]" << std::endl;
        std::cout << "marking state [s0] as start" << std::endl;
    }

    // add states from n0
    std::set<std::string> st0 = n0->states();
    std::set<std::string>::iterator it0;
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        std::string name = name_by_number(state_renamer.size());
        state_renamer.insert(std::make_pair<std::string,std::string>(*it0+"_0",name));
        add_state(name);
        if(DEBUG_NDFA_CPP)
        {
            std::cout << "adding state " << name << std::endl;
        }
    }

    // add transition from start to start of n0
    add_epsilon_transition("[s0]",state_renamer.find(n0->start()+"_0")->second);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from [s0] to " << state_renamer.find(n0->start()+"_0")->second << " on epsilon" << std::endl;
    }

    // add transitions of n0
    std::map<std::string, std::map<int,std::set<std::string>* >* >::iterator it1;
    for(it1=n0->m_transitions.begin(); it1!=n0->m_transitions.end(); it1++)
    {
        std::string from = it1->first;
        std::string from_can = state_renamer.find(from+"_0")->second;
        std::map<int,std::set<std::string>*>::iterator it2;
        for(it2=it1->second->begin(); it2!=it1->second->end(); it2++)
        {
            int ch = it2->first;
            std::set<std::string>::iterator it3;
            for(it3=it2->second->begin(); it3!=it2->second->end(); it3++)
            {
                std::string to = *it3;
                std::string to_can = state_renamer.find(to+"_0")->second;
                if(ch!=-1)
                {
                    add_transition(from_can,to_can,(char)ch);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on '" << (char)ch << "'" << std::endl;
                    }
                }
                else
                {
                    add_epsilon_transition(from_can,to_can);
                    if(DEBUG_NDFA_CPP)
                    {
                        std::cout << "adding transition from " << from_can << " to " << to_can << " on epsilon" << std::endl;
                    }
                }
            }
        }
    }

    // add cumulative accept state
    std::string accept = name_by_number(state_renamer.size());
    state_renamer.insert(std::make_pair<std::string,std::string>("__accept__",accept));
    add_state(accept);
    set_accepting(accept,true);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state " << accept << std::endl;
        std::cout << "marking state " << accept << " as accepting" << std::endl;
    }

    // add transitions from accepting state(s) of n0 to accept state
    for(it0=st0.begin(); it0!=st0.end(); it0++)
    {
        if(n0->is_accepting(*it0))
        {
            add_epsilon_transition(state_renamer.find(*it0 + "_0")->second,accept);
            if(DEBUG_NDFA_CPP)
                std::cout << "adding transition from " << state_renamer.find(*it0 + "_0")->second << " to " << accept << " on epsilon" << std::endl;
        }
    }

    // add transition from accept state to start
    add_epsilon_transition(accept,state_renamer.find(n0->start()+"_0")->second);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from " << accept << " to " << state_renamer.find(n0->start()+"_0")->second << " on epsilon" << std::endl;
    }

    // add end state
    std::string end = name_by_number(state_renamer.size());
    state_renamer.insert(std::make_pair<std::string,std::string>("__end__",end));
    add_state(end);
    set_accepting(end,true);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state " << end << std::endl;
        std::cout << "marking state " << end << " as accepting" << std::endl;
    }

    // add transition from accept to end state
    add_epsilon_transition(accept,end);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from " << accept << " to " << end << " on epsilon" << std::endl;
    }

    // add transition from start to end state
    add_epsilon_transition("[s0]",end);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from [s0] to " << end << " on epsilon" << std::endl;
        std::cout << "</NDFA_STAR>" << std::endl;
    }

}
std::string ndfa_star::name_by_number(int i)
{
    std::stringstream ss;
    ss << i;
    return "[s"+ss.str()+"]";
}

ndfa_string::ndfa_string(std::string s)
{

    if(DEBUG_NDFA_CPP)
        std::cout << "<NDFA_STRING>" << std::endl;

    add_state("[s0]");
    set_start("[s0]");
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state [s0]" << std::endl;
        std::cout << "marking [s0] as start" << std::endl;
    }

    for(int i=0; i<s.size(); i++)
    {
        add_state(name_by_number(i+1));
        add_transition(name_by_number(i),name_by_number(i+1),s[i]);
        if(DEBUG_NDFA_CPP)
        {
            std::cout << "adding state " << name_by_number(i+1) << std::endl;
            std::cout << "adding transition from " << name_by_number(i) << " to " << name_by_number(i+1) << " on '" << s[i] << "'" << std::endl;
        }
    }

    set_accepting(name_by_number(s.size()),true);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "marking " << name_by_number(s.size()) << " as accepting" << std::endl;
        std::cout << "</NDFA_STRING>" << std::endl;
    }

}
std::string ndfa_string::name_by_number(int i)
{
    std::stringstream ss;
    ss << i;
    return "[s"+ss.str()+"]";
}

ndfa_char::ndfa_char(char c)
{

    // add state s0
    add_state("[s0]");
    set_start("[s0]");
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "<NDFA_SINGLE_CHAR>" << std::endl;
        std::cout << "adding state [s0]" << std::endl;
        std::cout << "marking [s0] as start" << std::endl;
    }

    // add state s1
    add_state("[s1]");
    set_accepting("[s1]",true);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding state [s1]" << std::endl;
        std::cout << "marking [s1] as accepting" << std::endl;
    }

    // add transition from s0 to s1, on c
    add_transition("[s0]","[s1]",c);
    if(DEBUG_NDFA_CPP)
    {
        std::cout << "adding transition from [s0] to [s1] on epsilon" << std::endl;
        std::cout << "</NDFA_SINGLE_CHAR>" << std::endl;
    }

}