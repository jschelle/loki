/*
 * File:   core.cc
 * Author: joris
 *
 */

#include "array_functors.h"
#include "algbr_functors.h"
#include "type_functors.h"
#include "compr_functors.h"
#include "core.h"
#include "elemt_functors.h"
#include "gonio_functors.h"
#include "integ_functors.h"
#include "logic_functors.h"
#include "memry_functors.h"
#include "power_functors.h"
#include "progr_functors.h"
#include <iostream>

core::core()
{

    // context
    m_context = new context();

    // algebra
    m_context->add_functor(new derivative());
    m_context->add_functor(new nth_derivative());
    m_context->add_functor(new substitute());

    // memory
    m_context->add_functor(new function_assign());
    m_context->add_functor(new assign());
    m_context->add_functor(new unassign());
    m_context->add_functor(new is_assigned());

    // type
    m_context->add_functor(new evalf());
    m_context->add_functor(new is_number());
    m_context->add_functor(new is_double());
    m_context->add_functor(new is_integer());
    m_context->add_functor(new is_text());
    m_context->add_functor(new is_array());
    m_context->add_functor(new is_operator());

    // elementary
    m_context->add_functor(new plus());
    m_context->add_functor(new minus());
    m_context->add_functor(new unary_minus());
    m_context->add_functor(new multiply());
    m_context->add_functor(new divide());

    // logic
    m_context->add_functor(new logical_and());
    m_context->add_functor(new logical_or());
    m_context->add_functor(new logical_not());

    // comparison
    m_context->add_functor(new equivalence());
    m_context->add_functor(new inequivalence());
    m_context->add_functor(new less_than());
    m_context->add_functor(new less_than_or_equal());
    m_context->add_functor(new greater_than());
    m_context->add_functor(new greater_than_or_equal());

    // programming
    m_context->add_functor(new programming_if());
    m_context->add_functor(new programming_while());
    m_context->add_functor(new programming_proc());
    m_context->add_functor(new programming_load(this));

    // powers
    m_context->add_functor(new second_power());
    m_context->add_functor(new third_power());
    m_context->add_functor(new power());
    m_context->add_functor(new logarithm());

    // gonio
    m_context->add_functor(new sine());
    m_context->add_functor(new cosine());
    m_context->add_functor(new tangent());
    m_context->add_functor(new cosecant());
    m_context->add_functor(new secant());
    m_context->add_functor(new cotangent());
    m_context->add_functor(new pi_constant());
    m_context->add_functor(new to_rad());
    m_context->add_functor(new to_degrees());

    // array
    m_context->add_functor(new concat_array());
    m_context->add_functor(new concat_array_in_place());
    m_context->add_functor(new cartprod_array());
    m_context->add_functor(new array_length());
    m_context->add_functor(new array_length_in_place());
    m_context->add_functor(new select_from());
    m_context->add_functor(new select_from_in_place());
    m_context->add_functor(new set_element());

    // int
    m_context->add_functor(new round_int());
    m_context->add_functor(new absolute_int());
    m_context->add_functor(new is_prime());

    // tokenizer
    m_lexer = new hybrid_lexer();

    // binder
    m_binder = new binder();

    // shuntingyard
    m_shuntingyard = new shuntingyard();
}
atom* core::evaluate_to_atom(std::string s)
{

    std::vector<lexeme> tokens = m_lexer->tokenize(s,m_context->get_functors());
    std::vector<binding> interp = m_binder->bind(tokens,m_context->get_functors());
    tokens.clear();

    atom* retval = 0;

    for(int i=0; i<interp.size(); i++)
    {

        // convert to postfix
        binding postfix = m_shuntingyard->to_postfix(interp[i]);
        if(postfix.size()==0)
        {
            delete_binding(interp[i]);
            continue;
        }

        // build tree
        atom* root =  m_shuntingyard->build_tree(postfix);
        delete_binding(interp[i]);
        delete_binding(postfix);
        if(root==0)
        {
            continue;
        }

        // attempt to evaluate
        atom* evl = root->evaluate(m_context);
        delete root;
        if(evl==0)
        {
            continue;
        }
        retval=evl;

        // if we're here, evl!=0
        // => cleanup other interpretations
        for(int j=i+1; j<interp.size(); j++)
            delete_binding(interp[j]);
        break;

    }

    return retval;
}
std::string core::evaluate_to_string(std::string s)
{
    atom* evl = evaluate_to_atom(s);
    if(evl==0)
        return "";
    std::string retval = evl->to_mathML();
    delete evl;
    return "<math xmlns='http://www.w3.org/1998/Math/MathML'>"+retval+"</math>";
}
void core::delete_binding(binding b)
{
    for(int i=0; i<b.size(); i++)
        delete b[i];
}
core::~core()
{
    // context
    delete m_context;
    // tokenizer
    delete m_lexer;
    // binder
    // delete m_binder;
    delete m_binder;
    // shuntingyard
    delete m_shuntingyard;
}
