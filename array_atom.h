/*
 * File:   array_atom.h
 * Author: joris
 *
 */

#ifndef ARRAY_ATOM_H
#define ARRAY_ATOM_H

#include "atom.h"
#include <vector>

/**
 * ARRAY ATOM
 **/

class array_atom:public atom
{
public:
    // constructor
    array_atom(std::vector<atom*> elements, bool reverse);
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluate
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

#endif
