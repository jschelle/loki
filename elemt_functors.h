/*
 * File:   elemt_functors.h
 * Author: joris
 *
 */

#ifndef ELEMT_FUNCTORS_H
#define ELEMT_FUNCTORS_H

#include "functor.h"
#include "number.h"
#include <set>
#include <vector>

class atom_tuple
{
public:
    // represents term
    atom* m_first;
    // represents coefficient / power
    atom* m_second;

    // destructor
    ~atom_tuple();

    // comparison
    static bool compare(atom_tuple* t0, atom_tuple* t1);
};

/**
 * PLUS CHAIN
 * chain for handling plus, minus, unary minus etc on a tree-level
 **/
class plus_chain
{
public:
    plus_chain(atom* r);
    atom* to_atom(context* c);
    ~plus_chain();
private:
    // methods
    void scan_tree();
    void scan_tree(atom* a, atom* coeff);
    void add_term(atom* a, atom* coeff);
    void add_constant(number* c);
    bool is_zero(atom* a);
    atom* create_term(atom_tuple* el);
    // fields
    atom* m_root;
    std::vector<atom_tuple*> m_terms;
};

/**
 * PLUS
 **/
class plus:public binary_infix_functor
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
    // method
    bool is_chain_root();
};

/**
 * MINUS
 **/
class minus:public binary_infix_functor
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
    // method
    bool is_chain_root();
};

/**
 * UNARY MINUS
 **/
class unary_minus:public unary_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // technical information
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
 * MULTIPLICATION CHAIN
 * chain for handling multiplication, division, power etc on a tree-level
 **/
class mult_chain
{
public:
    mult_chain(atom* r);
    atom* to_atom(context* c);
    ~mult_chain();
private:
    // methods
    void scan_tree();
    void scan_tree(atom* a, atom* power);
    void mul_factor(atom* a, atom* power);
    void mul_constant(number* c);
    atom* create_factor(atom_tuple* el);
    bool is_zero(atom* a);
    // fields
    atom* m_root;
    std::vector<atom_tuple*> m_factors;
};

/**
 * MULTIPLY
 **/
class multiply:public binary_infix_functor
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
    bool is_chain_root();
};

/**
 * DIVIDE
 **/
class divide:public binary_infix_functor
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
    bool is_chain_root();
};

#endif
