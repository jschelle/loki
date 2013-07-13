/*
 * File:   atom.cc
 * Author: joris
 *
 */

#include "atom.h"
#include "number.h"
#include <iostream>

/**
 * ATOM
 **/

atom::atom()
{
    m_parent = 0;
}
atom* atom::deep_copy()
{
    atom* atm = shallow_copy();
    std::map<int,atom*>::iterator it;
    for(it=m_args.begin(); it!=m_args.end(); it++)
    {
        atom* cpy = it->second->deep_copy();
        atm->m_args.insert(std::make_pair<int,atom*>(it->first,cpy));
    }
    return atm;
}
void atom::set_arg(int index, atom* value)
{
    std::map<int,atom*>::iterator it;
    it = m_args.find(index);
    if(it!=m_args.end())
    {
        it->second = value;
    }
    else
    {
        m_args.insert(std::make_pair<int,atom*>(index,value));
    }
    value->m_parent = this;
}
atom* atom::get_arg(int index)
{
    std::map<int,atom*>::iterator it = m_args.find(index);
    if(it==m_args.end())
        return 0;
    return it->second;
}
void atom::del_args()
{
    std::map<int,atom*>::iterator it;
    for(it=m_args.begin(); it!=m_args.end(); it++)
        delete it->second;
    m_args.clear();
}
atom* atom::get_parent()
{
    return m_parent;
}
int atom::count_args()
{
    return m_args.size();
}
bool atom::equals(atom* a)
{
    // must be of equal type
    if(a->get_type()!=get_type())
        return false;
    // must be equal class
    if(!a->same_class(this))
        return false;
    // number of arguments must be equal
    if(a->count_args()!=count_args())
        return false;
    // matching arguments must be equal
    for(int i=0; i<a->count_args(); i++)
    {
        atom* arg0 = a->get_arg(i);
        atom* arg1 = get_arg(i);
        if(!arg0->equals(arg1))
            return false;
    }
    // default
    return true;
}
atom::~atom()
{
    for(int i=0; i<count_args(); i++)
    {
        delete get_arg(i);
    }
}