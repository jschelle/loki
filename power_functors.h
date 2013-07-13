
#ifndef POWER_FUNCTORS_H
#define POWER_FUNCTORS_H

#include "functor.h"
#include "number.h"

class power:public binary_infix_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    bool is_left_associative();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
    // exceptions
    atom* pow_int_frac(number* base, number* exp);
    atom* pow_frac_frac(number* base, number* exp);
    // computation
    atom* num_pow_nume(number* base, number* exp);
    atom* num_pow_symb(number* base, number* exp);
};

class second_power:public unary_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    bool is_left_associative();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class third_power:public unary_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    bool is_left_associative();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class logarithm:public unary_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    bool is_left_associative();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

#endif
