/*
 * File:   error.cc
 * Author: joris
 *
 */

#include "error.h"

bool error::is_error(atom* a)
{
    return dynamic_cast<error*>(a)!=0;
}

/**
 * DIVIDE BY ZERO
 */

atom* divide_by_zero::shallow_copy()
{
    return new divide_by_zero();
}
bool divide_by_zero::same_class(atom* a)
{
    return dynamic_cast<divide_by_zero*>(a)!=0;
}
std::string divide_by_zero::get_keyword(int i)
{
    return "divide_by_zero";
}
atom* divide_by_zero::evaluate(context* c)
{
    return new divide_by_zero();
}
std::string divide_by_zero::to_mathML()
{
    return "<merror><mtext>divide by zero</mtext></merror>";
}

/**
 * INVALID ARGUMENT ERROR
 */
atom* invalid_argument::shallow_copy()
{
    return new invalid_argument();
}
bool invalid_argument::same_class(atom* a)
{
    return dynamic_cast<invalid_argument*>(a)!=0;
}
std::string invalid_argument::get_keyword(int i)
{
    return "invalid_argument";
}
atom* invalid_argument::evaluate(context* c)
{
    return new invalid_argument();
}
std::string invalid_argument::to_mathML()
{
    return "<merror><mtext>invalid argument</mtext></merror>";
}

/**
 * BAD PARSE ERROR
 */
atom* bad_parse::shallow_copy()
{
    return new bad_parse();
}
bool bad_parse::same_class(atom* a)
{
    return dynamic_cast<bad_parse*>(a)!=0;
}
std::string bad_parse::get_keyword(int i)
{
    return "bad_parse";
}
atom* bad_parse::evaluate(context* c)
{
    return new bad_parse();
}
std::string bad_parse::to_mathML()
{
    return "<merror><mtext>bad_parse</mtext></merror>";
}