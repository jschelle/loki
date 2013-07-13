/*
 * File:   array_atom.cc
 * Author: joris
 *
 */

#include "array_atom.h"

/**
 * ARRAY ATOM
 **/

array_atom::array_atom(std::vector<atom*> elements, bool reverse)
{
    if(!reverse)
    {
        for(int i=0; i<elements.size(); i++)
            set_arg(i,elements[i]);
    }
    else
    {
        for(int i=0; i<elements.size(); i++)
            set_arg(elements.size()-i-1,elements[i]);
    }
}
int array_atom::get_type()
{
    return atom::TYPE_ARRAY;
}
atom* array_atom::shallow_copy()
{
    return new array_atom(std::vector<atom*>(),true);
}
bool array_atom::same_class(atom* a)
{
    return a->get_type()==atom::TYPE_ARRAY;
}
atom* array_atom::evaluate(context* c)
{
    std::vector<atom*> eval;
    for(int i=0; i<count_args(); i++)
        eval.push_back(get_arg(i)->evaluate(c));
    return new array_atom(eval,false);
}
std::string array_atom::to_mathML()
{
    std::string s = "<list>";
    for(int i=0; i<count_args(); i++)
    {
        s+=get_arg(i)->to_mathML();
    }
    s+="</list>";
    return s;
}
