#ifndef INTEG_FUNCTORS
#define INTEG_FUNCTORS

#include "functor.h"
#include <vector>

/**
 * ROUNT TO INT
 **/
class round_int:public unary_functor
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
};

/**
 * ABSOLUTE VALUE
 **/
class absolute_int:public unary_functor
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
    // mathML
    std::string to_mathML();
private:
};

/**
 * PRIMALITY TEST
 **/
class is_prime:public unary_functor
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
};

#endif
