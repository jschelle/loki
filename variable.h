#ifndef VARIABLE_H
#define VARIABLE_H

#include "atom.h"
#include <string>

/**
 * VARIABLE
 **/
class variable:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // constructor
    variable(std::string id);
    std::string get_identifier();
    // equality
    bool same_class(atom* a);
    bool equals(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
    std::string m_identifier;
};

#endif
