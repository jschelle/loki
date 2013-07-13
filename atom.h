/*
 * File:   atom.h
 * Author: joris
 *
 */

#ifndef ATOM_H
#define ATOM_H

#include <map>
#include <string>
#include <vector>

class context;

/**
 * ATOM
 **/

class atom
{
public:

// compile-time types
    static const int TYPE_WHITESPACE = 1;
    static const int TYPE_LEFT_ROUND_BRACKET = 2;
    static const int TYPE_RIGHT_ROUND_BRACKET = 3;
    static const int TYPE_LEFT_STRAIGHT_BRACKET = 32;
    static const int TYPE_RIGHT_STRAIGHT_BRACKET = 33;
    static const int TYPE_LEFT_CURLY_BRACKET = 35;
    static const int TYPE_RIGHT_CURLY_BRACKET = 37;
    static const int TYPE_INT_NUMBER = 5;
    static const int TYPE_DOUBLE_NUMBER = 7;
    static const int TYPE_VARIABLE = 13;
    static const int TYPE_FUNCTION_ARGUMENT_SEPARATOR = 17;
    static const int TYPE_OPERATOR = 19;
    static const int TYPE_TEXT = 23;

    // runtime types
    static const int TYPE_FRACTION = 29;
    static const int TYPE_ARRAY = 31;
    static const int TYPE_COMMANDS_SEQUENCE = 37;

    // constructor
    atom();
    // type
    virtual int get_type() = 0;
    // copy
    virtual atom* shallow_copy() = 0;
    virtual atom* deep_copy();
    // parent
    atom* get_parent();
    // arguments
    void set_arg(int index, atom* value);
    atom* get_arg(int index);
    void del_args();
    int count_args();
    // equality
    virtual bool same_class(atom* a) = 0;
    virtual bool equals(atom* a);
    // evaluate
    virtual atom* evaluate(context* c) = 0;
    // mathML
    virtual std::string to_mathML() = 0;
    // destructor
    virtual ~atom();
private:
    atom* m_parent;
    std::map<int,atom*> m_args;
};

#endif
