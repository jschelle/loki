#ifndef TEXT_ATOM_H
#define TEXT_ATOM_H

#include "atom.h"
#include <string>

/**
 * TEXT ATOM
 **/
class text_atom: public atom
{
public:
    text_atom(std::string s, bool trim);
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    atom* deep_copy();
    // equality
    bool same_class(atom* a);
    bool equals(atom* a);
    // evaluate
    atom* evaluate(context* c);
    // member access
    std::string get_text();
    void set_text(std::string s);
    // mathML
    std::string to_mathML();
private:
    std::string m_text;
};

#endif