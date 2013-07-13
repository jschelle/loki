/*
 * File:   array_functors.h
 * Author: joris
 *
 */

#ifndef ARRAY_FUNCTORS_H
#define ARRAY_FUNCTORS_H

#include "functor.h"

/**
 * CONCATENATE ARRAYS/STRINGS
 **/
class concat_array:public binary_prefix_functor
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
 * CONCATENATE ARRAYS/STRINGS (IN PLACE)
 **/
class concat_array_in_place:public binary_prefix_functor
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
 * CALCULATE CARTESIAN PRODUCT OF ARRAYS
 **/
class cartprod_array:public binary_prefix_functor
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
 * LENGTH OF ARRAY/STRING
 **/
class array_length:public unary_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    bool is_pre_operand();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
private:
};

/**
 * LENGTH OF ARRAY/STRING (IN PLACE)
 **/
class array_length_in_place:public unary_functor
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
 * SELECT ELEMENT/CHARACTER FROM ARRAY/STRING
 **/
class select_from: public binary_prefix_functor
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
    bool is_array_of_numbers(atom* a);
    atom* select_once(atom* a, int index, bool copy);
};

/**
 * SELECT ELEMENT/CHARACTER FROM ARRAY/STRING (IN PLACE)
 **/
class select_from_in_place: public binary_prefix_functor
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
    bool is_array_of_numbers(atom* a);
    atom* select_once(atom* a, int index, bool copy);
};

/**
 * SET ELEMENT/CHARACTER IN ARRAY/STRING
 */
class set_element: public functor
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
    bool is_array_of_numbers(atom* a);
};

/**
 * SET ELEMENT/CHARACTER IN ARRAY/STRING (IN PLACE)
 */
class set_element_in_place: public functor
{
public:
private:
};

#endif
