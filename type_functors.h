/*
 * File:   basic_functors.h
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#ifndef TYPE_FUNCTORS_H
#define TYPE_FUNCTORS_H

#include "functor.h"

class null_value:public functor
{
public:
};

/**
 * EVALUATE TO FLOAT
 **/
class evalf:public unary_functor
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

/**
 * CHECK FOR NUMBERS
 **/
class is_number:public unary_functor
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

/**
 * CHECK FOR INTEGER
 **/
class is_integer:public unary_functor
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

/**
 * CHECK FOR DOUBLE
 **/
class is_double:public unary_functor
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

/**
 * CHECK FOR TEXT
 **/
class is_text:public unary_functor
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

/**
 * CHECK FOR ARRAY
 **/
class is_array:public unary_functor
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

/**
 * CHECK FOR OPERATOR
 **/
class is_operator:public unary_functor
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

/**
 * CHECK FOR INSTANCE OF OPERATOR
 **/
class is_instance_of:public binary_prefix_functor
{
public:
private:
};

#endif
