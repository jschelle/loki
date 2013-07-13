/*
 * File:   command_sequence_atom.cc
 * Author: joris
 *
 */

#include "command_sequence_atom.h"
#include <iostream>

/*
 * COMMAND SEQUENCE ATOM
 */

command_sequence_atom::command_sequence_atom(std::vector<atom*> elements, bool reverse)
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
int command_sequence_atom::get_type()
{
    return atom::TYPE_COMMANDS_SEQUENCE;
}
atom* command_sequence_atom::shallow_copy()
{
    return new command_sequence_atom(std::vector<atom*>(),true);
}
bool command_sequence_atom::same_class(atom* a)
{
    return a->get_type()==atom::TYPE_COMMANDS_SEQUENCE;
}
atom* command_sequence_atom::evaluate(context* c)
{
    for(int i=0; i<count_args(); i++)
    {
        atom* a = get_arg(i)->evaluate(c);
        delete a;
    }
    return 0;
}
std::string command_sequence_atom::to_mathML()
{
    std::string s = "<set>";
    for(int i=0; i<count_args(); i++)
    {
        if(get_arg(i)!=0)
            s+=get_arg(i)->to_mathML();
    }
    s+="</set>";
    return s;
}
