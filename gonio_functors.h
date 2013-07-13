#ifndef GONIO_FUNCTORS
#define GONIO_FUNCTORS

#include "functor.h"

/**
 * PI
 **/

class pi_constant: public constant
{
public:
    atom* shallow_copy();
    bool same_class(atom* a);
    atom* evaluate(context* c);
    std::string to_mathML();
    std::string get_keyword(int i);
private:
};

/**
 * TO DEGREES
 **/
class to_degrees:public unary_functor
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
 * TO RADIANS
 **/
class to_rad:public unary_functor
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
 * SINE
 **/
class sine:public unary_functor
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
    atom* translate_special(double d);
};

/**
 * COSINE
 **/
class cosine:public unary_functor
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
 * TANGENT
 **/
class tangent:public unary_functor
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
 * COTANGENT
 **/
class cotangent:public unary_functor
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
 * COSECANT
 **/
class cosecant:public unary_functor
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
 * SECANT
 **/
class secant:public unary_functor
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
