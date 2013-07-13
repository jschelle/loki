/*
 * File:   core.h
 * Author: joris
 *
 */

#ifndef CORE_H
#define CORE_H

#include "atom.h"
#include "binder.h"
#include "context.h"
#include "hybrid_lexer.h"
#include "shuntingyard.h"
#include <string>

/**
 * CORE
 **/

class core
{
public:
    // constructor
    core();
    // evaluate
    atom* evaluate_to_atom(std::string s);
    std::string evaluate_to_string(std::string s);
    // destructor
    ~core();
private:
    // util
    void delete_binding(binding b);
    // context
    context* m_context;
    // tokenizer
    hybrid_lexer* m_lexer;
    // binder
    binder* m_binder;
    // shuntingyard
    shuntingyard* m_shuntingyard;
};

#endif
