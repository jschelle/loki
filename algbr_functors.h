/*
 * File:   algbr_functors.h
 * Author: joris
 *
 */

#ifndef ALGBR_FUNCTORS_H
#define ALGBR_FUNCTORS_H

#include "functor.h"

/**
 * DERIVATIVE
 **/

class derivative : public binary_prefix_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
private:
    atom* deriv(atom* expr, atom* var);
};

/**
 * NTH DERIVATIVE
 **/
class nth_derivative : public functor
{
public:
    // equality
    bool same_class(atom* a);
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    int count_keywords();
    // techical information
    int get_arity();
    int get_precedence();
    bool is_left_associative();
    bool is_pre_operand();
    // evaluate
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

/**
 * SUBSTITUTE
 */
class substitute : public functor
{
public:
    atom* shallow_copy();
    bool same_class(atom* a);
    // meta-information
    std::string get_keyword(int i);
    int count_keywords();
    // techical information
    int get_arity();
    int get_precedence();
    bool is_pre_operand();
    bool is_left_associative();
    // evaluate
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
    atom* copy_and_replace(atom* f, std::vector<atom*> vars, std::vector<atom*> replacement);
};

#endif