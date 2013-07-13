/*
 * File:   functor.h
 * Author: joris
 *
 * Created on March 14, 2012, 7:01 PM
 */

#ifndef FUNCTOR_H
#define	FUNCTOR_H

#include "atom.h"

/**
 * FUNCTOR
 **/

class functor:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    virtual atom* shallow_copy() = 0;
    // meta-information
    virtual std::string get_keyword(int i) = 0;
    virtual int count_keywords() = 0;
    // techical information
    virtual int get_arity() = 0;
    virtual int get_precedence() = 0;
    virtual bool is_pre_operand() = 0;
    virtual bool is_left_associative() = 0;
    // mathML
    virtual std::string to_mathML() = 0;
private:
};

/**
 * CONSTANT
 **/

class constant:public functor
{
public:
    virtual atom* shallow_copy() = 0;
    virtual atom* evaluate(context* c) = 0;
    virtual std::string to_mathML() = 0;
    int count_keywords();
    int get_arity();
    int get_precedence();
    bool is_pre_operand();
    bool is_left_associative();
};

/**
 * UNARY FUNCTOR
 **/
class unary_functor:public functor
{
public:
    // copy
    virtual atom* shallow_copy() = 0;
    // meta-information
    virtual std::string get_keyword(int i) = 0;
    virtual int count_keywords();
    // techical information
    virtual int get_arity();
    virtual int get_precedence();
    virtual bool is_left_associative();
    virtual bool is_pre_operand();
    // mathML
    virtual std::string to_mathML();
private:
};

/**
 * BINARY INFIX FUNCTOR
 **/
class binary_infix_functor:public functor
{
public:
    // copy
    virtual atom* shallow_copy() = 0;
    // meta-information
    virtual std::string get_keyword(int i) = 0;
    virtual int count_keywords();
    // techical information
    virtual int get_arity();
    virtual int get_precedence();
    virtual bool is_left_associative();
    virtual bool is_pre_operand();
    // mathML
    virtual std::string to_mathML();
private:
};

/**
 * BINARY PREFIX FUNCTOR
 **/
class binary_prefix_functor:public functor
{
public:
    // copy
    virtual atom* shallow_copy() = 0;
    // meta-information
    virtual std::string get_keyword(int i) = 0;
    virtual int count_keywords();
    // techical information
    virtual int get_arity();
    virtual int get_precedence();
    virtual bool is_left_associative();
    virtual bool is_pre_operand();
    // mathML
    virtual std::string to_mathML();
private:
};

#endif	/* FUNCTOR_H */

