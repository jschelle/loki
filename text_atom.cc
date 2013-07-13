/*
 * File:   array_atom.cpp
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#include "text_atom.h"

/**
 * TEXT ATOM
 **/
text_atom::text_atom(std::string s, bool trim)
{
    m_text="";
    if(trim)
    {
        for(int i=1; i<s.length()-1; i++)
            m_text+=s[i];
    }
    else
    {
        for(int i=0; i<s.length(); i++)
            m_text+=s[i];

    }
}
int text_atom::get_type()
{
    return atom::TYPE_TEXT;
}
atom* text_atom::shallow_copy()
{
    return new text_atom(m_text,false);
}
atom* text_atom::deep_copy()
{
    return new text_atom(m_text,false);
}
bool text_atom::same_class(atom* a)
{
    return a->get_type()==atom::TYPE_TEXT;
}
bool text_atom::equals(atom* a)
{
    if(!same_class(a))
        return false;
    return static_cast<text_atom*>(a)->m_text.compare(m_text)==0;
}
atom* text_atom::evaluate(context* c)
{
    return deep_copy();
}
std::string text_atom::get_text()
{
    return m_text;
}
void text_atom::set_text(std::string s)
{
    m_text = s;
}
std::string text_atom::to_mathML()
{
    return "<csymbol>"+m_text+"</csymbol>";
}