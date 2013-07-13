/*
 * File:   command_sequence_atom.h
 * Author: joris
 *
 */

#ifndef COMMAND_SEQUENCE_ATOM_H
#define	COMMAND_SEQUENCE_ATOM_H

#include "atom.h"

/**
 * COMMAND SEQUENCE ATOM
 **/

class command_sequence_atom:public atom
{
public:
    // constructor
    command_sequence_atom(std::vector<atom*> elements, bool reverse);
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

#endif	/* COMMAND_SEQUENCE_ATOM_H */

