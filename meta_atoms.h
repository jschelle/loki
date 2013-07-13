/*
 * File:   bracket.h
 * Author: joris
 *
 * Created on March 16, 2012, 2:35 AM
 */

#ifndef META_ATOMS_H
#define META_ATOMS_H

#include "atom.h"

class left_round_bracket:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class right_round_bracket:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class right_straight_bracket:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class left_straight_bracket:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class left_curly_bracket:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class right_curly_bracket:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class separator:public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluate
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class whitespace: public atom
{
public:
    // meta-information
    int get_type();
    // copy
    atom* shallow_copy();
    // equality
    bool same_class(atom* a);
    // evaluate
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

#endif
