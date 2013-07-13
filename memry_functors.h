
#ifndef MEMRY_FUNCTORS_H
#define MEMRY_FUNCTORS_H

#include "array_atom.h"
#include "context.h"
#include "functor.h"
#include "variable.h"
#include <vector>

class function_assign:public functor
{
public:
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
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class function_template:public functor
{
public:
    // constructor
    function_template();
    function_template(variable* id, variable* var, atom* exp);
    function_template(variable* id, std::vector<atom*> var, atom* exp);
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    int count_keywords();
    // technical information
    int get_arity();
    int get_precedence();
    bool is_left_associative();
    bool is_pre_operand();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
    // destructor
    ~function_template();
private:
    // methods
    atom* copy_and_modify(atom* root, std::map<std::string,atom*> vars);
    // fields
    variable* identifier;
    std::vector<variable*> variables;
    atom* expression;
};

class assign:public binary_infix_functor
{
public:
    // copy
    atom* shallow_copy();
    // meta-information
    std::string get_keyword(int i);
    // techical information
    int get_precedence();
    bool is_left_associative();
    // equality
    bool same_class(atom* a);
    // evaluation
    atom* evaluate(context* c);
    // mathML
    std::string to_mathML();
private:
};

class unassign:public unary_functor
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

class is_assigned:public unary_functor
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
