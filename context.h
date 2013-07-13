/*
 * File:   context.h
 * Author: joris
 *
 */

#ifndef CONTEXT_H
#define	CONTEXT_H

#include "functor.h"

/**
 * CONTEXT
 **/

class context
{
public:
    static const int NUMERICAL_MODE = 2;
    static const int SYMBOLIC_MODE = 3;
    // constructor
    context();
    context(context* c, bool includeFunctors, bool includeVars);
    // variables
    bool is_defined_var(std::string s);
    void set_var(std::string s, atom* v);
    atom* get_var(std::string s);
    void free_var(std::string s);
    // functors
    bool is_defined_functor(std::string s);
    void add_functor(functor* f);
    std::vector<functor*>* get_functors();
    void free_functor(std::string s);
    // mode
    int get_mode();
    void set_mode(int aFlag);
    // destructor
    ~context();
private:
    // mode
    int m_mode;
    // bound variables
    std::map<std::string,atom*> m_variables;
    // functors
    std::vector<functor*> m_functors;
};

#endif	/* CONTEXT_H */

