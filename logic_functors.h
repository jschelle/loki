/*
 * File:   logic_functors.h
 * Author: joris
 *
 * Created on March 16, 2012, 10:49 PM
 */

#ifndef LOGIC_FUNCTORS_H
#define	LOGIC_FUNCTORS_H

#include "functor.h"

class logical_and:public binary_infix_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class logical_or:public binary_infix_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class logical_not:public unary_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    bool is_pre_operand();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

#endif	/* LOGIC_FUNCTORS_H */

