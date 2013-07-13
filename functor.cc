/*
 * File:   functor.cpp
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#include "functor.h"

/**
 * FUNCTOR
 **/

int functor::get_type()
{
    return atom::TYPE_OPERATOR;
}

/**
 * CONSTANT
 **/
int constant::count_keywords()
{
    return 1;
}
int constant::get_arity()
{
    return 0;
}
int constant::get_precedence()
{
    // TODO determine INTEGER_MAX_VALUE
    return 1024;
}
bool constant::is_pre_operand()
{
    return false;
}
bool constant::is_left_associative()
{
    return false;
}

/**
 * UNARY FUNCTOR
 **/

int unary_functor::count_keywords()
{
    return 1;
}
int unary_functor::get_arity()
{
    return 1;
}
int unary_functor::get_precedence()
{
    return 6;
}
bool unary_functor::is_left_associative()
{
    return false;
}
bool unary_functor::is_pre_operand()
{
    return false;
}
std::string unary_functor::to_mathML()
{
    return "<apply><csymbol>" + get_keyword(0) + "</csymbol>" + get_arg(0)->to_mathML() + "</apply>";
}

// binary infix functor
int binary_infix_functor::count_keywords()
{
    return 1;
}
int binary_infix_functor::get_arity()
{
    return 2;
}
int binary_infix_functor::get_precedence()
{
    return 6;
}
bool binary_infix_functor::is_left_associative()
{
    return true;
}
bool binary_infix_functor::is_pre_operand()
{
    return false;
}
std::string binary_infix_functor::to_mathML()
{
    return "<apply><csymbol>" + get_keyword(0) + "</csymbol>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() + "</apply>";
}

// binary prefix functor
int binary_prefix_functor::count_keywords()
{
    return 1;
}
int binary_prefix_functor::get_arity()
{
    return 2;
}
int binary_prefix_functor::get_precedence()
{
    return 6;
}
bool binary_prefix_functor::is_left_associative()
{
    return true;
}
bool binary_prefix_functor::is_pre_operand()
{
    return true;
}
std::string binary_prefix_functor::to_mathML()
{
    return "<apply><csymbol>" + get_keyword(0) + "</csymbol>" + get_arg(1)->to_mathML() + get_arg(0)->to_mathML() + "</apply>";
}
