/*
 * File:   context.cc
 * Author: joris
 *
 */

#include "context.h"

context::context()
{
    m_mode = context::SYMBOLIC_MODE;
}
context::context(context* c, bool includeFunctors, bool includeVars)
{
    // copy variables
    if(includeVars)
    {
        std::map<std::string,atom*>::iterator it0;
        for(it0=c->m_variables.begin(); it0!=c->m_variables.end(); it0++)
            m_variables.insert(std::make_pair<std::string,atom*>(it0->first,it0->second->deep_copy()));
    }
    // copy functors
    if(includeFunctors)
    {
        for(int i=0; i<c->m_functors.size(); i++)
            m_functors.push_back(static_cast<functor*>(c->m_functors[i]->deep_copy()));
    }
    // set mode
    m_mode = context::SYMBOLIC_MODE;
}
bool context::is_defined_var(std::string s)
{
    return m_variables.find(s)!=m_variables.end();
}
void context::set_var(std::string s, atom* a)
{
    std::map<std::string,atom*>::iterator it = m_variables.find(s);
    if(it!=m_variables.end())
    {
        delete it->second;
        m_variables.erase(it);
    }
    m_variables.insert(std::make_pair<std::string,atom*>(s,a));
}
atom* context::get_var(std::string s)
{
    std::map<std::string,atom*>::iterator it = m_variables.find(s);
    if(it==m_variables.end())
        return 0;
    return it->second;
}
void context::free_var(std::string s)
{
    std::map<std::string,atom*>::iterator it = m_variables.find(s);
    if(it!=m_variables.end())
    {
        delete it->second;
        m_variables.erase(it);
    }
}
bool context::is_defined_functor(std::string s)
{
    for(int i=0; i<m_functors.size(); i++)
        for(int j=0; j<m_functors[i]->count_keywords(); j++)
            if(m_functors[i]->get_keyword(j).compare(s)==0)
                return true;
    return false;
}
void context::add_functor(functor* f)
{
    m_functors.push_back(f);
}
std::vector<functor*>* context::get_functors()
{
    return &m_functors;
}
void context::free_functor(std::string s)
{
    // TODO
}
int context::get_mode()
{
    return m_mode;
}
void context::set_mode(int aFlag)
{
    m_mode = aFlag;
}
context::~context()
{
    // functors
    for(int i=0; i<m_functors.size(); i++)
        delete m_functors[i];
    // variables
    for(std::map<std::string,atom*>::iterator it=m_variables.begin(); it!=m_variables.end(); it++)
        delete it->second;
}
