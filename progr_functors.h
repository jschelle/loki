#ifndef PROGR_FUNCTORS_H
#define PROGR_FUNCTORS_H

#include "functor.h"
#include "core.h"

/*
 * IF
 */

class programming_if:public functor
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

/*
 * WHILE
 */

class programming_while: public functor
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

/*
 * PROCEDURE
 */

class programming_proc: public functor
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
    bool is_array_of_variables(atom* a);
};

/*
 * PROCEDURE
 */

class procedure_atom: public functor
{
public:
    procedure_atom();
    procedure_atom(std::string n, std::vector<atom*> i, std::vector<atom*> o, atom* c);
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
    // delete
    ~procedure_atom();
private:
    // fields
    std::string m_name;
    std::vector<atom*> m_input;
    std::vector<atom*> m_output;
    atom* m_code;
    // methods
    atom* copy_and_modify(atom* a);
};

/*
 * EXTERNAL LOADING
 */

class programming_load:public unary_functor
{
public:
    // constructor
    programming_load(core* c);
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
    // methods
    std::vector<std::string> loadFile(std::string s);
    // fields
    core* m_core;
};

#endif