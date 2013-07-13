/*
 * File:   error.h
 * Author: joris
 *
 */

#ifndef ERROR_H
#define ERROR_H

#include "functor.h"

/**
 * ERROR
 */
class error: public constant
{
public:
    virtual atom* shallow_copy() = 0;
    virtual bool same_class(atom* a) = 0;
    virtual atom* evaluate(context* c) = 0;
    virtual std::string to_mathML() = 0;
    virtual std::string get_keyword(int i) = 0;
    static bool is_error(atom* a);
private:
};

/**
 * DIVIDE BY ZERO ERROR
 **/
class divide_by_zero: public error
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
 * INVALID ARGUMENT ERROR
 **/
class invalid_argument: public error
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
 * BAD PARSE ERROR
 **/

class bad_parse: public error
{
public:
    atom* shallow_copy();
    bool same_class(atom* a);
    atom* evaluate(context* c);
    std::string to_mathML();
    std::string get_keyword(int i);
private:
};

#endif